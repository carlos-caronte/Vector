/*
 * File:   check_vector.c
 * Author: jcr
 *
 * Created on 29 September, 2017
 */
 /*
 * Copyright (C) 2017 Juan Carlos Rey <carlos.caronte@gmail.com>
 *
 * This file is part of Vector.
 *
 * Vector is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vector is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vector.  If not, see <http://www.gnu.org/licenses/>.
 */


#include<stdio.h>
#include<stdlib.h>
#include<check.h>
#include"../src/vector.h"

#define CAPACITY 90000
#define MOD 17
#define RANDOM 1300000
#define N 10000
#define DEN 30000.0

START_TEST(int_vector)
{
        vector_t *v = vector_Init(N, sizeof (int), int_cmp);
        int n = random() % CAPACITY;

        int mod = random() % MOD;    // for proportional remove
        if (mod < 3)
            mod = 3;
        int *a = calloc(n, sizeof (int));
        int i;

        for (i = 0; i < n; i++) {
                a[i] = random() % RANDOM;
        }

        for (i = 1; i < (n - 1); i++) {

                // Proportional remove items
                if (i % mod == 0) {
                    int value = 0;
                    vector_Item(v, &value, -1);
                    v_stat x = vector_Remove(v, -1);
                    ck_assert(!x);
                    ck_assert_int_eq(value, a[i - 2]);
                }

                vector_Insert(v, &a[i - 1]);
                int x = vector_Len(v);

                ck_assert_int_eq(x, i - (i / mod));

        }
        vector_t *slice = vector_Init(10, sizeof(int), int_cmp);
        // Slice's length is To - From + 1
        vector_Slice(v, slice, 10, 19);

        for (i = 1; i <= 10; i++) {

                int value_slice = 0;
                int value_v = 0;
                vector_Item(slice, &value_slice, i - 1);
                vector_Item(v, &value_v, (i - 1) + 10);
                ck_assert_int_eq(value_slice, value_v);
        }

        vector_Destroy(v);
        vector_Destroy(slice);
}

END_TEST

START_TEST(float_vector)
{
        vector_t *v = vector_Init(N, sizeof (float), float_cmp);
        int n = random() % CAPACITY;
        float *a = calloc(n, sizeof(float));
        int i;
        for (i = 0; i < n; i++) {
                a[i] = (random() % RANDOM) / DEN;
        }

        int v_len;
        v_stat x_stat;
        for (i = 1; i < (n - 1); i++) {

                if (i % 11 == 0) {
                    x_stat = vector_Remove(v, 0);
                    ck_assert(!x_stat);
                }
                vector_Insert(v, &a[i - 1]);
                v_len = vector_Len(v);

                ck_assert_int_eq(v_len, i - (i / 11));
        }
        vector_Sort(v);
        float v_item1 = 0.0f, v_item2 = 0.0f;
        x_stat = vector_Begin(v, &v_item1);
        ck_assert(!x_stat);

        for (i = 0; i < 50; i++) {
            x_stat = vector_Iter_next(v, &v_item2, i);
            ck_assert(!x_stat);
            ck_assert(v_item2 >= v_item1);
            v_item1 = v_item2;
        }

        vector_Destroy(v);
}

END_TEST

START_TEST(string_vector)
{
        vector_t *v = vector_Init(N, sizeof (char *), string_cmp);

        int n = random() % CAPACITY;
        int mod = random() % MOD;
        if (mod < 3)
            mod = 3;

        char **a = calloc(n, sizeof (char *));  /* n pointers to char *  */
        int i;
        for (i = 0; i < n; i++) {
                int n1 = random() % 13 + 4;
                a[i] = calloc(n1 + 1, sizeof (char));
                int j;
                for (j = 0; j < n1; j++) {
                        a[i][j] = random() % 26 + 97;
                }
                a[i][n1] = 0;
        }

        char *value;
        for (i = 1; i < (n - 1); i++) {

                if (i % mod == 0) {
                    vector_Item(v, &value, -1);
                    v_stat x = vector_Remove(v, 0);
                    ck_assert(!x);
                    ck_assert_str_eq(value, a[i - 2]);
                }
                vector_Insert(v, &a[i - 1]);
                int x = vector_Len(v);

                ck_assert_int_eq(x, i - (i / mod));
        }

        vector_Destroy(v);

        for (i = 0; i < n; i++) {
                free(a[i]);
        }
        free(a);
}

END_TEST


Suite* vector_suite()
{
        Suite *v = suite_create("Vector");

        TCase *tc_int = tcase_create("int");

        TCase *tc_float = tcase_create("float");

        TCase *tc_string = tcase_create("string");


        tcase_add_test(tc_int, int_vector);
        tcase_add_test(tc_float, float_vector);
        tcase_add_test(tc_string, string_vector);

        suite_add_tcase(v, tc_int);
        suite_add_tcase(v, tc_float);
        suite_add_tcase(v, tc_string);

        return v;
}

int main()
{
        int number_failed;
        Suite *v = vector_suite();
        SRunner *vr = srunner_create(v);
        srunner_run_all(vr, CK_NORMAL);
        number_failed = srunner_ntests_failed(vr);
        srunner_free(vr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
