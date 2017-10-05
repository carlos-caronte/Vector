/*
 * File:   slice.c
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
#include<assert.h>
#include<string.h>
#include"vector.h"

#define CAPACITY 90000
#define MOD 17
#define RANDOM 1300000
#define N 10000
#define DEN 30000.0

    /***************************************************************
     *
     *                                  AUXILIARY FUNCTIONS
     *
     * *************************************************************/


struct st_ex {
    char product[16]; /**<  */
    float price; /**<  */
};


void struct_map_price_inc(void *o_struct)
{
        struct st_ex *io_struct = (struct st_ex *)o_struct;
        io_struct ->price *= 1.3f;
}


void fold_add(void *e1, void *e2, void *result)
{
    int el1 = e1 ? *((int*)e1) : 0;
    int el2 = e2 ? *((int*)e2) : 0;
    *((int*)result) = el1 + el2;
}


/**
 * @brief           qsort struct comparision function (price float field)
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       Returns negative if b > a and positive if a > b.
 *                      We multiplied result by 100.0 to preserve decimal
 *                      fraction
 *
 */
int struct_cmp_by_price(const void *a, const void *b)
{
    struct st_ex *ia = (struct st_ex *)a;
    struct st_ex *ib = (struct st_ex *)b;
    return (int)(100.f*ia->price - 100.f*ib->price);

}

/**
 * @brief           qsort struct comparision function (product C-string field)
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       The strcmp () function compares the two character strings
 *                       s1 and s2. Returns an integer less than, equal to, or
 *                       greater than zero if s1 is, respectively, less than, equal
 *                       to (concordant), or greater than s2.
 */
int struct_cmp_by_product(const void *a, const void *b)
{
    struct st_ex *ia = (struct st_ex *)a;
    struct st_ex *ib = (struct st_ex *)b;
    return strcmp(ia->product, ib->product);

}


/**
 * @brief               Integer Vector printing function
 * @param v         Pointer to vector_t Vector object
 * @param len       Number of items to print
 *
 *
 */
void print_int_Vector(vector_t *v, size_t len) {

    size_t i;
    int temp;
    v_stat status = vector_Pos_Err(v, len - 1);

    switch (status) {
        case V_OK:
                        for(i=0; i<len; i++) {
                            vector_Item(v, &temp, i);
                            printf("%d | ", temp);
                        }
                        putchar('\n');
                        puts("--");
                        break;
        case V_ERR_IS_EMPTY:
                        printf("Vector is Empty\n");
                        break;
        case V_ERR_OUT_OF_RANGE:
                        printf("Error out of range\n");
    }
}


/**
 * @brief               Struct Vector printing function
 * @param v         Pointer to vector_t Vector object
 * @param len      Number of items to print.
 *
 */
void print_struct_Vector(vector_t *v,  size_t len)
{
    struct st_ex temp;
    size_t i;

    v_stat status = vector_Pos_Err(v, len - 1);
    switch (status) {
        case V_OK:
                        for(i=0; i < len; i++) {
                            vector_Item(v, &temp, i);
                            printf("[ product: %s \t price: $%.2f ]\n",
                                        temp.product, temp.price);
                        }
                        puts("--");
                        break;
        case V_ERR_IS_EMPTY:
                        printf("Vector is Empty\n");
                        break;
        case V_ERR_OUT_OF_RANGE:
                        printf("Error out of range\n");
    }

}


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

        free(f);
        v->Destroy(v);


    /***************************************************************
     *
     *                                  STRUCTS
     *
     * *************************************************************/


        struct st_ex structs[] =
                                        {{"mp3 player", 299.0f},
                                            {"plasma tv", 2200.0f},
                                            {"notebook", 1300.0f},
                                            {"mp3 player", 132.0f},
                                            {"smartphone", 499.99f},
                                            {"mp3 player", 600.0f},
                                            {"dvd player", 150.0f},
                                            {"mp3 player", 451.0f},
                                            {"mp4 player", 7211.0f},
                                            {"mp3 ply v", 631.0f},
                                            {"matches", 0.2f }};

        size_t structs_len = sizeof(structs) / sizeof(struct st_ex);

        vector_t *struc = vector_Init(structs_len,
                                    sizeof(struct st_ex),
                                    struct_cmp_by_product);

        size_t i;
        for(i = 0; i < structs_len; i++)
            vector_Insert(struc, &structs[i]);

        puts("*** Struct ...");
        print_struct_Vector(struc, structs_len);

        vector_Sort(struc);
        puts("*** Struct sorting (product)...");
        print_struct_Vector(struc, structs_len);

    /***************************************************************
     *
     *                          STRUCT SEARCHING: Filter, Has, Pattern
     *
     * *************************************************************/


        puts("*** Struct searching (product)... ordered (by price)");
        vector_t *searched = vector_Init(structs_len,
                                            sizeof(struct st_ex),
                                            struct_cmp_by_price);
        char *found = "mp3 player";
        v_stat status = vector_Filter(struc, found, searched);
        switch(status) {
            case V_OK :  print_struct_Vector(searched,
                                                                vector_Len(searched));
                                 break;
            case V_ERR_IS_EMPTY :
                                printf("Vector is Empty\n");
                                break;
            case V_ERR_VALUE_NOT_FOUND :
                                printf("Item %s has not been found\n", found);
                                break;
        }

        found = "plasma";
        status = vector_Has(struc, found);
        switch(status) {
            case V_OK :  printf("Item %s is in the Vector\n", found);
                                 break;
            case V_ERR_IS_EMPTY :
                                printf("Vector is Empty\n");
                                break;
            case V_ERR_VALUE_NOT_FOUND :
                                printf("Item %s has not been found\n", found);
                                break;
        }
        puts("*** Struct searching Pattern string (product)");
        vector_t *s_pattern = vector_Init(structs_len,
                                            sizeof(struct st_ex),
                                            struct_cmp_by_product);
        const char *pattern ="[!m]*";
        status = vector_Pattern(struc, pattern, s_pattern );
        switch(status) {
            case V_OK :  print_struct_Vector(s_pattern,
                                                                vector_Len(s_pattern));
                                 break;
            case V_ERR_IS_EMPTY :
                                printf("Vector is Empty\n");
                                break;
            case V_ERR_VALUE_NOT_FOUND :
                                printf("Item %s has not been found\n", found);
                                break;
        }

        searched->Destroy(searched);
        s_pattern->Destroy(s_pattern);

    /***************************************************************
     *
     *                                  FOLD, MAP
     *
     * *************************************************************/


        /*
         *                                  FOLD
         */
        vector_t *fold = vector_Init(10, sizeof(int), int_cmp);

        int a = 1;
        int b = 78;
        int c = 12;
        int d = 2;
        int e = 8;

        vector_Insert(fold, &a);
        int result;
        vector_Fold(fold, fold_add, &result);
        putchar('\n');
        puts("--");
        printf("Fold sums %d and returned %d\n", a, result);

        vector_Insert(fold, &b);
        vector_Fold(fold, fold_add, &result);
        printf("Fold sums %d and returned %d\n", b, result);

        vector_Insert(fold, &c);
        vector_Insert(fold, &d);
        vector_Insert(fold, &e);
        vector_Fold(fold, fold_add, &result);
        printf("Now Fold returns %d\n", result);

        fold->Destroy(fold);

        /*
         *                                  MAP
         */

        puts("*** Struct ...");
        int len = vector_Len(struc);
        vector_Sort(struc);
        print_struct_Vector(struc, len);
        vector_Map(struc, struct_map_price_inc);
        puts("*** Struct (product)... editing price");
        print_struct_Vector(struc, len);

        /*
         *                    Remove records with patterns
         */

        puts("*** Struct (product)... remove pattern");
        pattern ="mp*";
        status = vector_Remove_Pattern(struc, pattern);
        switch(status) {
            case V_OK :  print_struct_Vector(struc,
                                                                vector_Len(struc));
                                 break;
            case V_ERR_IS_EMPTY :
                                printf("Vector is Empty\n");
                                break;
            case V_ERR_VALUE_NOT_FOUND :
                                printf("Pattern %s has not been found\n", pattern);
                                break;
        }


        struc->Destroy(struc);


    /***************************************************************
     *
     *                                  Insert from FILE
     *
     * *************************************************************/

        vector_t *v_file = vector_Init(10, sizeof(char *), string_cmp);
        const char *filename =" HERE THE FILE  ";
        char delim = 32; // SPACE
        v_stat stat_file = vector_Insert_from_file(v_file,
                                                                        filename, delim);

        char *prev, *next;
        int count = 0;

        switch (stat_file) {
                case V_OK:

                        vector_Sort(v_file);
                        len = vector_Len(v_file);
                        vector_Begin(v_file, &prev);
                        count++;

                        for (i = 1; i < len; i++) {
                            vector_Item(v_file, &next, i);
                            if (!strcmp(next, prev))
                                count++;
                            else {
                                printf("%s=%d \n", prev, count);
                                prev = next;
                                count = 1;
                            }
                        }

                        // Printing the last element
                        printf("%s=%d \n", prev, count);
                        break;

                case  V_ERR_FILE:
                        fprintf( stderr, "Unable to open file %s\n", filename );
                        perror( "The following error occurred " );
        }
        // Each record read from the file is a pointer to Heap. This function
        // clean up the memory.
        v_file->Destroy_pointer(v_file);

        return 0;
}
