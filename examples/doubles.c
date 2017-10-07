/*
 * File:   doubles.c
 * Author: jcr
 *
 * Created on 6 October, 2017
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
#include<assert.h>
#include"vector.h"

#define CAPACITY 90000
#define MOD 17
#define RANDOM 1300000
#define N 10000
#define DEN 30000.0



int main()
{
    /***************************************************************
     *
     *                                  Test Double Sort
     *
     * *************************************************************/

        // We create new Vector Object. The function int_cmp is a function
        // pointer that is used to order the vector. The library has function
        // pointer to order int [int_cmp], string[string_comp], float[float_cmp]
        // and double[double_cmp].

        vector_t *v = vector_Init(N, sizeof (double), double_cmp);
        int n = random() % CAPACITY;
        double *f = calloc(n, sizeof(double));
        for (int i = 0; i < n; i++) {
                f[i] = (random() % RANDOM) / DEN;
        }

        int v_len;
        v_stat x_stat;
        for (int i = 1; i < (n - 1); i++) {

                if (i % 11 == 0) {
                    // we remove the data, but not free the pointer. The client
                    // will must do it before the program has finished
                    x_stat = vector_Remove(v, 0);
                    assert(!x_stat);
                }
                 vector_Insert(v, &f[i - 1]);

                v_len = vector_Len(v);
                assert(v_len == i - (i / 11));
        }

        vector_Sort(v);
        double v_item1 = 0.0f, v_item2 = 0.0f;
        x_stat = vector_Begin(v, &v_item1);
        assert(!x_stat);

        for (int i = 0; i < 50; i++) {
            x_stat = vector_Iter_next(v, &v_item2, i);
            assert(!x_stat);
            assert(v_item2 >= v_item1);
            v_item1 = v_item2;
        }
        // The client free its pointers to HEAP
        free(f);

        // The library free its pointers to HEAP
        v->Destroy(v);

        return 0;
}
