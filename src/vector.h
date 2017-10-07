/*
 * File:   vector.h
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


#ifndef VECTOR_H
#define VECTOR_H
#include<stdbool.h>

#define MALLOC_CHECK_ 2

/**
 * @brief     A generic and dynamic container that expands automatically
 *              as elements are added.
 *
 *              The library supports amortized constant time insertion and
 *              removal of elements , as well as constant time access.
 * *
 *              The object provided by the Vector library is an array type
 *              container that:
 *
 *              1. It only must contain addresses of HEAP.
 *                  The insertion must be done  by calling the Memory
 *                  Manager, that is: any new insertion will have
 *                  to be done with pointers to HEAP.
 *
 *               2. Resides in contiguous memory, accessible by
 *                   arithmetic of pointers
 *
 *               3. It is generic: it supports any types: we identifie the
 *                   information not by its type, but by its size
 *
 *               4. The object resides in HEAP
 *
 *               5. The object is self-expanding: doubles its capacity
 *                   automatically if the  insertion of a new element
 *                   requires it
 *
 *               6. The library provides insertion of items from file.
 *
 *
 *               The insertion of elements is always by reference. If the item
 *               were created in main, (reside in STACK) the object pointed by
 *               our stored direction, is released when the application finishes.
 *               There is nothing to free.
 *
 *               But if you create the element by calling the Memory Manager
 *               (calloc,  * malloc, ...) then the pointer resides in the HEAP, and
 *               must be released before application has finished.
 *
 *              This means that either we create data that resides in STACK or
 *              we create it by calling the memory manager. If both forms
 *              are combined  then we release a STACK direction with the call
 *              to Destroy, and this will cause a segment violation, or any other
 *              undefined behavior. Or, if we do not call Destroy, we will leave
 *              pointers without releasing when our application has returned
 *              (memory - leak).
 *
 *              If the insertion is made from function calls, you must only insert
 *              elements whose addresses reside in HEAP, since the space in
 *              the stack corresponding to the function is released once it
 *              returns.
 *
 *              Well, our object only must contain addresses of HEAP.
 *              The insertion must be done  by calling the Memory Manager
 *              before. That is: any new insertion will have to be done with
 *              pointers to HEAP.
 *
 *              The contract is:
 *
 *              1. Only insert references to HEAP
 *              2. Who calls the Memory Manager, free the direction stored in
 *                  the pointer.
 *
 *              If we follow the rules, there will be no problem: neither
 *              memory leak, neither double free.
 *
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

        void(*Destroy_slice)(struct Vector *v);/**<Destructor*/


} vector_t;


typedef enum V_stat {
    V_OK = 0,
    V_ERR_IS_EMPTY = 1,
    V_ERR_VALUE_NOT_FOUND = 2,
    V_ERR_OUT_OF_RANGE = 3,
    V_ERR_FILE = 4,
    V_ERR_STACK = 5,
    V_ERR_ALLOCATE_MEMORY = 6,
    V_ERR_MEMCPY = 7,
    V_ERR_MEMMOVE = 8,
    V_ERR_INVALID_ARGUMENT = 9,
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

#define _v_assert(X, Y, F, LINE)                                                                              \
{                                                                                                             \
    if(!X){                                                                                                   \
        vector_Abort(                                                                                         \
                Y,                                                                                            \
                F,                                                                                            \
                LINE);                                                                                        \
    }                                                                                                         \
}

#define v_assert(X, Y) _v_assert(X, Y, __FILE__, __LINE__)


/***************************************************************
 *
 *                                  Constructor  -  Destructor
 *
 * *************************************************************/
vector_t * vector_Init(int  capacity, size_t ele_size,
                                  int (*compar)(const void *, const void *));
static void vector_Destroy(vector_t *v);
static void vector_Destroy_pointer(vector_t *v);
static void vector_Destroy_slice(vector_t *v);



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
static bool vector_stack(vector_t *v, void *item);

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
void vector_Insert(vector_t *v, void *item);
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
static void vector_Abort(v_stat status,
                            const char* file, int line_number);
static void printBinary(long long n);

#endif /* VECTOR_H */
