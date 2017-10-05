/*
 * File:   vector.h
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


#ifndef VECTOR_H
#define VECTOR_H
#include<stdbool.h>

/**
 * @brief                   A generic and dynamic container that expands
 *                              automatically as elements are added.

 *                              The library supports amortized constant time
 *                              insertion and removal of elements , as well as
 *                              constant time access.
 *
  *                              Memory leaks free. If one function in the library
 *                              creates a pointer to Heap, then the library supplies
 *                              a function to free it.
 *
 *                              The restriction in C to use generic container is
 *                              based on this language does not allow data types
 *                              to be passed as parameters. That means a generic
 *                              container needs to manually manage memory in
 *                              terms of the client element size, not client
 *                              data type.
 */
typedef struct Vector{
        int len; /**<  Number of elements in Vector object*/

        void *data; /**< Array to hold the information  */

        int capacity; /**<  Capacity, in number of elements. Capacity has
                                 to be greater than Zero*/

        size_t ele_size; /**<  Vector's data type.
                                 It has to be the returned value of 'sizeof(Vector's
                                 data type)'

                                 The constructor requires that the client element
                                 size be identified up front.

                                 Since it doesn’t (and will never) know the true data
                                 type, it needs the size so it at least knows how
                                 many bytes to replicate behind the scenes with
                                 every call to manage data.*/

        int (*compar)(const void *, const void *); /**<
                                Comparing Function pointer*/
        void(*Destroy)(struct Vector *v);/**<Destructor*/
        void(*Destroy_pointer)(struct Vector *v);/**<Destructor to use
                                with the function vector_Insert_from_file. It frees
                                the pointers created and then it calls Destroy**/

} vector_t;


typedef enum V_stat {
    V_OK = 0,
    V_ERR_IS_EMPTY = 1,
    V_ERR_VALUE_NOT_FOUND = 2,
    V_ERR_OUT_OF_RANGE = 3,
    V_ERR_FILE = 4,
}v_stat;

/**
 * @brief   Accuracy of 6 decimals for sort function double and float.
 *
 *
 */
#define ACCURACY 1000000

/**
 * @brief     Length of strings that we want to split in substrings.
 *                This data is used in the feature Insert_from_file.
 *
 */
#define BUFFER_SIZE 1024


/***************************************************************
 *
 *                                  Constructor  -  Destructor
 *
 * *************************************************************/
vector_t * vector_Init(int capacity, size_t ele_size,
                                  int (*compar)(const void *, const void *));
static void vector_Destroy(vector_t *v);
static void vector_Destroy_pointer(vector_t *v);



/***************************************************************
 *
 *                                  Informative Methods
 *
 * *************************************************************/
int vector_Capacity(vector_t *v);
bool vector_isEquals(const vector_t *v1, const vector_t *v2);
bool vector_isEmpty(const vector_t  *v);
int vector_Len(const vector_t *v);
v_stat vector_Pos_Err(const vector_t *v, int position);


/***************************************************************
 *
 *                                  Element Access Methods
 *
 * *************************************************************/
v_stat vector_Begin(const vector_t *v, void *item);
v_stat vector_End(const vector_t *v, void *item);
v_stat vector_Filter(vector_t *v, void *value, vector_t *slice);
v_stat vector_Fold(vector_t *v, void (*fn) (void*, void*, void*),
                                  void *result);
v_stat vector_Item(const vector_t *v, void *item, int position);
v_stat vector_Iter(vector_t *v, void *item, int index);
v_stat vector_Iter_next(vector_t *v, void *item, int index);
v_stat vector_Pattern(vector_t *v, const char *pattern,
                                                        vector_t *slice);
v_stat vector_Slice(const vector_t *v, vector_t *slice,
                            int position, int lenght );



/***************************************************************
 *
 *                                  Modifiers Methods
 *
 * *************************************************************/
v_stat vector_Has(vector_t *v, void *value);
void vector_Insert(vector_t*v, void const *item);
v_stat vector_Insert_from_file(vector_t *v, const char *filename,
                                                                const char delimiter);
v_stat vector_Map(vector_t *v, void (*fn) (void *));
v_stat vector_Remove(vector_t *v, int position);
v_stat vector_Remove_value(vector_t *v, void *value);
v_stat vector_Remove_Pattern(vector_t *v, const char *pattern);
v_stat vector_Sort(vector_t *v);
int vector_Value(vector_t *v, void *value);



// Auxiliary Functions
int float_cmp(const void *a, const void *b);
int int_cmp(const void *a, const void *b);
int string_cmp(const void *a, const void *b);
int double_cmp(const void *a, const void *b);
static int vector_Pos(const vector_t *v, int position);
static char * substring(char *string, int position, int length);
static size_t  split_String( char* src, int position, int length,
                                                                char delimiter );


#endif /* VECTOR_H */
