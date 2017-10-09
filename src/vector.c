/*
 * File:   vector.c
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
#include<string.h>
#include<fnmatch.h>
#include"vector.h"

/***************************************************************
 *
 *                                  Constructor  -  Destructor
 *
 * *************************************************************/

/**
 * @brief           Destructor. Management of memory: free the data array
 *                      in the Vector object.
 * @param v     Pointer to vector_t type variable
 *
 */
void vector_Destroy(vector_t *v) {

    int len = vector_Len(v);
    vowner_t owner = vector_Owner(v);

    switch (owner){
        case V_OWNER:
            for (int i = 0; i < len; i++) {
                void *tmp;
                vector_Item(v, &tmp, i);
                free(tmp);
                tmp = NULL;
            }
            free(v->data);
            v->data = NULL;
            free(v);
            break;
        case V_BORROWER:
            free(v->data);
            v->data = NULL;
            free(v);
            break;
        case V_SLICE:
            free(v);
    }
 }


/**
 * @brief                   Constructor. Initializes Vector object with the given
 *                              capacity
 * @param capacity  Capacity of the Vector object to be created
 * @param ele_size   To identifie Vector's data type.  The constructor
 *                              requires that the client element size be identified
 *                              up front. Since it doesn’t (and will never) know
 *                              the true data type, it needs the size so it at least
 *                              knows how many bytes to replicate behind the
 *                              scenes with every call to manage data.
 * @param compar    Comparing function pointer
 * @returns               Pointer to vector_t type
 *
 */
vector_t * vector_Init(int capacity, size_t ele_size,
                int (*compar)(const void * a, const void * b)) {

     v_assert(capacity > 0 && ele_size > 0,
                                            V_ERR_INVALID_ARGUMENT);

     vector_t *v = malloc(sizeof(vector_t));
     v_assert(v, V_ERR_ALLOCATE_MEMORY);
     v->len = 0;
     v->capacity = capacity;
     v->ele_size = ele_size;
     v->safe = V_SAFE;
     v->owner = V_BORROWER;
     v->compar = compar;
     v->data = calloc(v->capacity, ele_size);
     // If memory error, exit,,,
     v_assert(v->data, V_ERR_ALLOCATE_MEMORY);

    return v;
}


/***************************************************************
 *
 *                                  Informative Methods
 *
 * *************************************************************/

/**
 * @brief           Returns the capacity of the specified Vector. The capacity
 *                      is the maximum number of elements a Vector can hold
 *                      before it has to be resized. Capacity has to be greater
 *                      than Zero
 *
 * @param v     Vector object whose capacity is being returned
 * @returns       The capacity of the Vector object
 *
 *
 */
int vector_Capacity(const vector_t *v)
{
    return v->capacity;
}


/**
 * @brief           It Compares the size of elements of two Vector objects
 *                      (v1, v2) and if are equals, it compares the first n bytes
 *                      of memory area of v1->data and v2->data
 * @param v1    A vector object
 * @param v2    A vector object
 * @returns       True, if v1 and v2 are equals, otherwise False
 *
 *
 */
bool vector_isEquals(const vector_t *v1, const vector_t *v2) {
  if (v1->ele_size != v2->ele_size) {
    return false;
  }
  return memcmp(v1->data, v2->data, v1->ele_size) == 0;
}


/**
 * @brief           Returns True if Vector is Empty, that is when its length
 *                      is equal to Zero
 * @param v     Pointer to vector_t type
 * @returns       Bool
 *
 */
bool vector_isEmpty(const vector_t  *v) {
    return v->len == 0;
}


/**
 * @brief
 * @param v
 * @param item
 * @returns
 *
 *
 */
static bool vector_Heap(const vector_t *v, void *item) {

    long long t_v = (long long) v;
    long long t_item = (long long) item;
    size_t i;
    size_t tam = sizeof(long long);
    unsigned long long k = 1 << (tam * 8 - 1);

    for (i = 0; i < 24; i++) {

        if (((t_v & (k >> i)) == (k >> i)) !=
            ((t_item & (k >> i)) == (k >> i)))

            return false;
    }

    return true;

}


/**
 * @brief           Returns the number of elements in the Vector,
 *                      the Vector's length
 * @param v     Pointer to vector_t type
 * @returns       v->len, the number of elements in Vector
 *
 */
int vector_Len(const vector_t *v) {

    return v->len;
}


vowner_t vector_Owner(const vector_t *v) {
    return v->owner;
}


/**
 * @brief                    Control if supplied position is out of range or Vector
 *                               is empty.
 * @param v              Pointer to vector_t type variable
 * @param position    Position to calculate
 * @returns                V_OK if position is correct and Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *                               V_ERR_OUT_OF_RANGE if position is not ok
*/
v_stat vector_Pos_Err(const vector_t *v, int position) {
    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {
        if (position < 0) {
            if(v->len + position >= 0)
                return V_OK;
            else
                return V_ERR_OUT_OF_RANGE;

        } else if(v->len - position >= 0) {
                return V_OK;
            }
                else {
                    return V_ERR_OUT_OF_RANGE;
                }
    }
}


/**
 * @brief
 * @param v
 * @returns
 *
 *
 */
int vector_Max_capacity(const vector_t *v) {
    unsigned k = 1 << (sizeof(int) * 8 - 1);
    int m = v->ele_size * 8;
    return (k - 1) / m;
}

/***************************************************************
 *
 *                                  Element Access Methods
 *
 * *************************************************************/

/**
 * @brief                   Item at Vector's beginning.
 * @param v             Pointer to vector_t type variable
 * @param item        To copy the value found at searched position
 * @returns                V_OK if Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *
 */
v_stat vector_Begin(const vector_t *v, void *item) {
    return vector_Item(v, item, 0);
}


/**
 * @brief                   Item at Vector's ending.
 * @param v             Pointer to vector_t type variable
 * @param item        To copy the value found at searched position
 * @returns                V_OK if Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *
 */
v_stat vector_End(const vector_t *v, void *item) {
    return vector_Item(v, item, -1);
}


/**
 * @brief                   Item at position in the Vector object.
 * @param v             Pointer to vector_t type variable
 * @param item        To copy the value found at searched position
 * @param position   Position to search
 * @returns                V_OK if position is correct and Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *                               V_ERR_OUT_OF_RANGE if position is not ok
 *
 */
v_stat vector_Item(const vector_t *v, void *item, int position) {

    v_stat status = vector_Pos_Err(v, position);
    switch (status) {
        case V_OK:
                        position = vector_Pos(v, position);
                        void *sourceAddr;
                        sourceAddr = (char *)v->data + v->ele_size * position;
                        v_assert(memcpy(item, sourceAddr, v->ele_size),
                                                    V_ERR_MEMCPY);
                        return V_OK;
        case V_ERR_IS_EMPTY:
                        return V_ERR_IS_EMPTY;
        case V_ERR_OUT_OF_RANGE:
                        return V_ERR_OUT_OF_RANGE;
    }
}


/**
 * @brief                   Itertools, returns item at position in the Vector
 *                              object. If index==0, begin; if index==-1, end
 * @param v             Pointer to vector_t type variable
 * @param item        To copy the value found at searched position
 * @param index       Position to search
 * @returns                V_OK if position is correct and Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *                               V_ERR_OUT_OF_RANGE if position is not ok
 *
 */
v_stat vector_Iter(vector_t *v, void *item, int index) {
    return vector_Item(v,item, index);
}


/**
 * @brief                   Itertools, returns Item at index+1 in the Vector
 *                              object.
 * @param v             Pointer to vector_t type variable
 * @param item        To copy the value found at searched position
 * @param index       Position to search
 * @returns                V_OK if position is correct and Vector is not empty
 *                               V_ERR_IS_EMPTY if the Vector Object is empty
 *                               V_ERR_OUT_OF_RANGE if position is not ok
 *
 */
v_stat vector_Iter_next(vector_t *v, void *item, int index) {
  return vector_Item(v, item, index + 1);
}


/**
 * @brief               Binary Search of an item. It copies in 'vector_t *slice'
 *                          all the items that match with the supplied value by
 *                          the client.
 * @param v         A pointer to vector_t objet
 * @param value   Item searched
 * @param slice    Pointer to vector_t object which will collect all the
 *                          items that match with the supplied value by the
 *                          client.
 * @returns           V_OK if the element was found and  at most
 *                          slice->capacity items, or
 *
 *                          V_IS_EMPTY if the Vector Object is empty or
 *                          V_ERR_VALUE_NOT_FOUND
 *                          V_ERR_INVALID_ARGUMENT
 */
v_stat vector_Filter(vector_t *v, void *value, vector_t *slice) {

    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;

    else if (v->ele_size != slice->ele_size || slice->capacity < 1) {
        return V_ERR_INVALID_ARGUMENT;
    } else {

            vector_Sort(v);
            int left = 0;
            int right = v->len - 1;

            while (left <= right) {
                int middle = (left + right) / 2;
                void *item = (char *)v->data + v->ele_size * middle;
                int compar = v->compar( item, value);

                if (compar == 0) {
                    int reverse = middle;
                    do {
                        if (vector_Capacity(slice) >vector_Len(slice)) {
                            void *destAddr = (char *)slice->data +
                                                                     slice->ele_size *
                                                                     vector_Len(slice);

                            v_assert(memmove(destAddr, item,
                                                slice->ele_size),
                                                V_ERR_MEMMOVE);
                            slice->len++;


                        } else {
                                break;
                            }

                        middle++;
                        item = (char *)v->data + v->ele_size * middle;
                        compar = v->compar(value, item);
                    } while (compar == 0 && (middle < (v->len - 1)));

                    if (reverse > 0) {
                        middle = reverse - 1;
                        item = (char *)v->data + v->ele_size * middle;
                        compar = v->compar(value, item);

                        while (compar == 0 && (middle > 0)) {
                            if (vector_Capacity(slice) >vector_Len(slice)) {
                                void *destAddr = (char *)slice->data +
                                                                         slice->ele_size *
                                                                         vector_Len(slice);

                                v_assert(memmove(destAddr, item,
                                                    slice->ele_size),
                                                    V_ERR_MEMMOVE);
                                slice->len++;


                            } else {
                                    break;
                                }

                            middle--;
                            item = (char *)v->data + v->ele_size * middle;
                            compar = v->compar(value, item);
                        }
                    }
                    vector_Sort(slice);
                    slice->owner = V_SLICE;

                    return V_OK;

                }  else if (compar > 0)
                        right = middle - 1;

                    else
                        left = middle + 1;
            }
    }
    return V_ERR_VALUE_NOT_FOUND;
}


/**
 * @brief               A fold function that collects all of the elements in
 *                          the Vector object together.
 *                          For example, if we have an object of [a,b,c...] the
 *                          end result will be (...((a+b)+c)+...).
 * @param v         The Vector on which this operation is performed
 * @param fn        The operation function that is to be invoked on
 *                          each element
 * @param result  The pointer which will collect the end result
 * @returns           V_OK
 *                          V_IS_EMPTY if the Vector Object is empty
 *
 */
v_stat vector_Fold(vector_t *v, void (*fn) (void*, void*, void*),
                            void *result) {

    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {
        void *item;
        int len = vector_Len(v);

        if (len == 1) {
            fn(v->data, NULL, result);
            return V_OK;
        }

        fn(v->data, v->data + v->ele_size, result);
        for (size_t index = 2; index < len; index++) {
                item = (char *)v->data + v->ele_size * index;
                fn(result, item, result);
        }
        return V_OK;
    }

}


/**
 * @brief          Returns the strings that matches the pattern in one slice.
 *                     In this case, the search is, at least, Big-O lineal time of
 *                     complexity.
 *                     We use the C's library 'fnmatch' to pattern matching.
 *                     The pattern may include the following special characters:
 *
 *                    *  Matches zero of more characters.
 *                    ?  Matches exactly one character.

 *                [...]  Matches one character if it's in a range of characters.
 *                        If the first character is `!', matches if the character is
 *                        not in the range. Between the brackets, the range is
 *                        specified by listing the characters that are in the range,
 *                        or two characters separated by `-' to indicate all
 *                        characters in that range.
 *                        For example, `[a-d]' matches `a', `b', `c', or `d'.
 *                        If you want to include the literal `-' in the range, make
 *                        it the first character, like in `[-afz]'.

 *                    \   Causes the next character to not be treated as a
 *                         wildcard. For example, `\*' matches an asterisk.
 *
 * @param v             A pointer to vector_t objet
 * @param pattern    Pattern searched. It is a string.
 * @param slice        Vector object pointer to return the slice
 * @returns               V_OK if the element was found, or
 *                              V_IS_EMPTY if the Vector Object is empty or
 *                              V_ERR_VALUE_NOT_FOUND
 *                              V_ERR_INVALID ARGUMENT
 *                              It returns items until it is reached the capacity
 *                              of slice
 */
v_stat vector_Pattern(vector_t *v, const char *pattern,
                                                        vector_t *slice) {


    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;

    else if (v->ele_size != slice->ele_size || slice->capacity < 1) {
        return V_ERR_INVALID_ARGUMENT;
    } else {

            size_t index;
            void *item;
            void *destAddr;

            for (index = 0; index < v->len; index++) {
                if (v->capacity > v->len) {
                    item = (char *)v->data + v->ele_size * index;
                    if (!fnmatch(pattern, item, 0)) {
                        destAddr = (char *)slice->data + slice->ele_size * index;
                        v_assert(memmove(destAddr, item,
                                            slice->ele_size),
                                            V_ERR_MEMMOVE);
                        slice->len++;
                    }

                } else {
                        break;
                    }
            }
            if (slice->len > 0) {
                vector_Sort(slice);
                slice->owner = V_SLICE;

                return V_OK;
            } else {
                    return V_ERR_VALUE_NOT_FOUND;
                }
        }
}


/**
 * @brief               Slice of items from the Vector object.
 * @param v         Pointer to vector_t type variable
 * @param from    Initial position. From >= 0;
 * @param to        Final position. To > From;
 *  @param slice    Pointer to new vector_t type variable
 * @returns           V_OK if slice is correct and Vector is not empty
 *                          V_ERR_IS_EMPTY if the Vector Object is empty
 *                          V_ERR_OUT_OF_RANGE if slice is not ok
 *                          V_ERR_INVALID_ARGUMENT
 */
v_stat
vector_Slice(const vector_t *v, vector_t *slice, int from, int to ) {

    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;

    else if (v->ele_size != slice->ele_size || slice->capacity < 1) {
        return V_ERR_INVALID_ARGUMENT;
    }

    else {

        // Both, From and To are included
        int len_slice = to - from + 1;
        if(v->len < len_slice)
            return V_ERR_OUT_OF_RANGE;

        void *sourceAddr;
        sourceAddr = (char *)v->data + v->ele_size * from;

        v_assert(memmove(slice->data, sourceAddr,
                                    v->ele_size * len_slice),
                                    V_ERR_MEMMOVE);

        slice->len = len_slice;
        slice->owner = V_SLICE;

        return V_OK;
    }
}



/***************************************************************
 *
 *                                  Modifiers Methods
 *
 * *************************************************************/

/**
 * @brief               Binary Search of an item. It uses qsort.
 * @param v         A pointer to vector_t objet
 * @param value   Item searched
 * @returns           V_OK if the element was found, or
 *                          V_IS_EMPTY if the Vector Object is empty or
 *                          V_ERR_VALUE_NOT_FOUND.
 */
 v_stat vector_Has(vector_t *v, void *value){

    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {

        vector_Sort(v);
        int left = 0;
        int right = v->len - 1;

        while (left <= right) {
            int middle = (left + right) / 2;
            void *item = (char *)v->data + v->ele_size * middle;
            int compar = v->compar( item, value);

            if (compar == 0) {
                return V_OK;
            }

            else if (compar > 0)
                right = middle - 1;

            else
                left = middle + 1;
        }

        return V_ERR_VALUE_NOT_FOUND;
    }
}


/**
 * @brief               Insert (an item of data) at the end of Vector object.
 * @param v         Pointer to vector_t type variable
 * @param item    Value to insert in Vector object
 */
void vector_Insert(vector_t*v, void *item) {

        v_assert(vector_Heap(v, item), V_ERR_STACK);
        void *destAddr;
        if (v->len == v->capacity) {

                v->capacity *= 2; // Capacity is duplicated
                int max_cty = vector_Max_capacity(v);


                v_assert((v->capacity < max_cty),
                                            V_ERR_MAX_CAPACITY);

                v->data = realloc(v->data, v->capacity * v->ele_size);
                // If memory error, exit,,,
                v_assert(v->data, V_ERR_ALLOCATE_MEMORY);
        }

        /* v->len is Base One Index.
         * First we calculate his direction in memory; then we copy
         * item in it,  and then we do v->len++
         */
        destAddr = (char *)v->data + v->ele_size * v->len;
        v_assert(memcpy(destAddr, item, v->ele_size),
                                    V_ERR_MEMCPY);
        v->len++;

}


/**
 * @brief
 * @param v
 * @param filename
 * @param delimiter
 * @returns
 *
 *
 */
v_stat vector_Insert_from_file(vector_t *v, const char *filename,
                                                                const char delimiter)
{

        FILE *p_file = fopen( filename, "r" );
        char buffer[BUFFER_SIZE];
        char *token;

        if( p_file == NULL ){

            return V_ERR_FILE;

        } else{

            v->owner = V_OWNER;

           size_t pos, length, len;

            // Read each line into the buffer
            while( fgets(buffer, BUFFER_SIZE, p_file) != NULL ){

                pos = 0;
                length =  BUFFER_SIZE;
                while (length) {

                    length = split_String(buffer, pos, BUFFER_SIZE, delimiter);
                    if (length == 0)
                        break;
                    token = substring(buffer, pos, length);

                    // Capacity maximum storage
                    // (2^(sizeof(int) * 8 - 1))  / v->size_ele * 8
                    vector_Insert(v, &token);

                    if (buffer[pos + length] == '\n')
                        break;
                    pos += (length + 1);

                }
           }
        }

        fclose( p_file );

        return V_OK;
}


/**
 * @brief           Applies the function fn to each element of the Vector.
 * @param v     Vector object on which this operation is performed
 * @param fn    Operation function that is to be invoked on each Vector
 *                      element
 * @returns       V_OK or
 *                      V_IS_EMPTY if the Vector Object is empty
 */
v_stat vector_Map(vector_t *v, void (*fn) (void *))
{
    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {
        size_t index;
        void *item;

        for (index = 0; index < v->len; index++) {
            item = (char *)v->data + v->ele_size * index;
            fn(item);
        }
        return V_OK;
    }
 }


/**
 * @brief                   Remove (an item of data ) from the Vector object.
 *                              We move contigous memory from (position + 1)
 *                              until (v->len -1) to position.
 *                              There is not empty spaces inter records, and v->len
 *                              now is len - 1
 * @param v             Pointer to vector_t type variable
 * @param position   Position to remove in Vector object
 * @returns               V_OK if position is correct and Vector is not empty
 *                              V_ERR_IS_EMPTY if the Vector Object is empty
 *                              V_ERR_OUT_OF_RANGE if position is not ok
 *
 */
v_stat vector_Remove(vector_t *v, int position){

    v_stat status = vector_Pos_Err(v, position);
    switch (status) {
        case V_OK:
            position = vector_Pos(v, position);
            void *sourceAddr;
            void *destAddr;

            /*
             *  int quantum = v->len - (position + 1);
             * Quantum is the number of elements to move in memory
             * when we remove the value in the supplied positon.
             * If quantum == 0, there is nothing to move in memory
            */
            int quantum = v->len - (position + 1);
            if (quantum > 0) {
                sourceAddr = (char *)v->data + v->ele_size * (position + 1);
                destAddr = (char *)v->data + v->ele_size * position;
                v_assert(
                memcpy(destAddr, sourceAddr, v->ele_size * quantum),
                V_ERR_MEMCPY);
            }
            v->len--;
            return V_OK;

        case V_ERR_IS_EMPTY:
            return V_ERR_IS_EMPTY;

        case V_ERR_OUT_OF_RANGE:
            return V_ERR_OUT_OF_RANGE;
    }
}


/**
 * @brief          Remove all the strings that matches the pattern from the
 *                     Vector object. We call vector_Remove() to remove each
 *                     record.
 *                     We use the C's library 'fnmatch' to pattern matching.
 *                     The pattern may include the following special characters:
 *
 *                    *  Matches zero of more characters.
 *                    ?  Matches exactly one character.
 *
 *                [...]  Matches one character if it's in a range of characters.
 *                        If the first character is `!', matches if the character is
 *                        not in the range. Between the brackets, the range is
 *                        specified by listing the characters that are in the range,
 *                        or two characters separated by `-' to indicate all
 *                        characters in that range.
 *                        For example, `[a-d]' matches `a', `b', `c', or `d'.
 *                        If you want to include the literal `-' in the range, make
 *                        it the first character, like in `[-afz]'.

 *                    \   Causes the next character to not be treated as a
 *                         wildcard. For example, `\*' matches an asterisk.
 *
 * @param v             A pointer to vector_t objet
 * @param pattern    Pattern searched. It is a string.
 * @returns               V_OK if the element was found, or
 *                              V_IS_EMPTY if the Vector Object is empty or
 *                              V_ERR_VALUE_NOT_FOUND.
 */
v_stat vector_Remove_Pattern(vector_t *v, const char *pattern) {

    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {
        size_t index;
        const void *item;
        bool found = false;
        for (index = 0; index < v->len; index++) {
            item = (char *)v->data + v->ele_size * index;
            if (!fnmatch(pattern, item, 0)) {
                vector_Remove(v, index);
                index--;
                found = true;
            }
        }
        if (found)
            return V_OK;
    }
    return V_ERR_VALUE_NOT_FOUND;
}


/**
 * @brief                   Remove one item of data from the Vector object.
 *                              We call vector_Remove() function to remove the
 *                              item.
 * @param v             Pointer to vector_t type variable
 * @param value       Value searched to remove
 * @returns               V_OK if position is correct and Vector is not empty
 *                              V_ERR_IS_EMPTY if the Vector Object is empty
 *                              V_ERR_OUT_OF_RANGE if position is not ok
 *                              V_ERR_VALUE_NOT_FOUND if value has not been
 *                              found
 */
v_stat vector_Remove_value(vector_t *v, void *value){

    int position = vector_Value(v, value);
    if (position > 0) {
        v_stat status = vector_Remove(v, position);
        switch (status) {
            case V_OK:
                return V_OK;

            case V_ERR_IS_EMPTY:
                return V_ERR_IS_EMPTY;

            case V_ERR_OUT_OF_RANGE:
                return V_ERR_OUT_OF_RANGE;
        }
    }
    return V_ERR_VALUE_NOT_FOUND;
}


/**
 * @brief           Qsort ordering function
 * @returns       V_OK or
 *                      V_IS_EMPTY if the Vector Object is empty
 */
 v_stat vector_Sort(vector_t *v) {
    if (vector_isEmpty(v))
        return V_ERR_IS_EMPTY;
    else {
        qsort(v->data, v->len, v->ele_size, v->compar);
        return V_OK;
    }
}


/**
 * @brief               Binary Search of an item. It uses qsort.
 * @param v         A pointer to vector_t objet
 * @param value   Item searched
 * @returns           Position if the element was found, or
 *                          -1 if  the Vector Object is empty or the value was not
 *                          found.
 */
int vector_Value(vector_t *v, void *value){

    if (vector_isEmpty(v))
        return -1;
    else {

        vector_Sort(v);
        int left = 0;
        int right = v->len - 1;

        while (left <= right) {
            int middle = (left + right) / 2;
            void *item = (char *)v->data + v->ele_size * middle;
            int compar = v->compar( item, value);

            if (compar == 0) {
                return middle;
            }

            else if (compar > 0)
                right = middle - 1;

            else
                left = middle + 1;
        }

        return -1;
    }
}


/*******************************************************************
 *                          AUXILIARY FUNCTIONS
 *
 * *****************************************************************/


/**
 * @brief                    Calculate new position if it is a negative number
 * @param v              Pointer to vector_t type variable
 * @param position    Position to calculate
 * @returns                New position, if it is a negative number
 *
 */
static int vector_Pos(const vector_t *v, int position) {

    if (position < 0) {
    /* -1 = (v->len - 1)
     * -2 = (v->len - 1) - 1
     * -3 = (v->len - 1) - 2
     */
        return (v->len + position);

    } else {
        return position;
    }
}


/**
 * @brief           qsort int comparision function
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       Returns an integer less than, equal to, or greater
 *                      than zero
 */
int int_cmp(const void *a, const void *b)
{
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return *ia  - *ib;
}


/**
 * @brief           qsort strings comparision function
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       The strcmp () function compares the two character strings
 *                       s1 and s2. Returns an integer less than, equal to, or
 *                       greater than zero if s1 is, respectively, less than, equal
 *                       to (concordant), or greater than s2.
 */
int string_cmp(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}


/**
 * @brief           qsort float comparision function
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       Returns negative if b > a and positive if a > b.
 *                      We multiplied result by 1.0 * ACCURACY
 *
 */
int float_cmp(const void *a, const void *b)
{
    const float *ia = (const float *)a;
    const float *ib = (const float *)b;

    return (int)(1.f * ACCURACY * (*ia - *ib));
}


/**
 * @brief           qsort double comparision function
 * @param a     Comparision item
 * @param b     Comparision item
 * @returns       Returns negative if b > a and positive if a > b.
 *                      We multiplied result by 1.0 * ACCURACY
 *
 */
int double_cmp(const void *a, const void *b)
{
    const double *ia = (const double *)a;
    const double *ib = (const double *)b;

    return (int)(1.f * ACCURACY * (*ia - *ib));
}


/**
 * @brief                   To calculate the lenght of substring in the buffer,
 *                              from position, to position of the delimiter.
 * @param src           The buffer with the information
 * @param position    Position from that we search the delimiter
 * @param length      Length of the buffer
 * @param delimiter  Delimiter character
 * @returns               Two control characters: new line and delimiter. If
 *                              we find '\n', it returns Zero. If we find 'delimiter
 *                              char' it returns (index - position). Otherwise returns
 *                              length Zero of substring.
 *
 *
 */
static size_t  split_String( char* src, int position, int length,
                                                            char delimiter )
{
    char control;
   for( int idx=position; idx< length; idx++ )
   {
        control = src[idx];
        if(control == '\n' || control == delimiter)
            if (control == 'n')
                    return 0;
            else
                return (idx - position);
   } // end for
   return 0;
}  //end splitString()


/**
 * @brief                  To identifie substrings in another string.
 * @param string     The buffer to divide from position to (position +
 *                             length)
 * @param position  Substring from position.
 * @param length    Length of desired substring.
 * @returns             This function returns char pointers to Heap. That
 *                            implies the library is responsible to free them.  It does
 *                            whit the function Destroy_pointer.
 *
 *
 */
static char *substring(char *string, int position, int length) {

    char *pointer;
    int c;

   pointer = malloc(sizeof(char)*(length+1));
   v_assert(pointer, V_ERR_ALLOCATE_MEMORY);

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *(string+position);
      string++;
   }

    *(pointer+c) = '\0';

    return pointer;

}

/**
 * @brief
 * @param file
 * @param line_number
 *
 *
 */
static void vector_Abort(v_stat status, const char* file, int line_number) {

    switch(status) {
        case V_ERR_ALLOCATE_MEMORY :
            fprintf(stderr, "\nVirtual memory exhausted. File: %s Line:% d\n",
            file,
            line_number);
            exit(EXIT_FAILURE);
            break;

        case V_ERR_MEMCPY :
            fprintf(stderr,
            "\nCopy memory error [memcpy]. File: %s Line:% d\n",
            file,
            line_number);
            exit(EXIT_FAILURE);
            break;

        case V_ERR_MEMMOVE :
            fprintf(stderr,
            "\nMove memory error [memmove]. File: %s Line:% d\n",
            file,
            line_number);
            exit(EXIT_FAILURE);
            break;

        case V_ERR_INVALID_ARGUMENT :
            fprintf(stderr,
            "\nInvalid argument in Constructor. File: %s Line:% d\n",
            file,
            line_number);
            exit(EXIT_FAILURE);
            break;
        case V_ERR_STACK:
            fprintf(stderr,
            "\nDirection pointer to STACK. Warning!!!. File: %s : % d\n",
            file,
            line_number);
            break;
        case V_ERR_MAX_CAPACITY:
            // (2^(sizeof(int) * 8 - 1))  / v->size_ele * 8
            fprintf(stderr,
            "\nMax capacity. Memory exhausted. File: %s Line:% d\n",
            file,
            line_number);
            exit(EXIT_FAILURE);
            break;


    }
}


/**
 * @brief
 * @param n
 *
 *
 */
static void printBinary(long long n) {

    int i;
    size_t tam = sizeof(long long);
    unsigned k = 1 << (tam * 8 - 1);

    for (i = 0; i < tam * 8; i++) {
        if ((n & (k >> i)) == (k >> i))
            printf("1");
        else
            printf("0");
        if ( (i + 1) % 8 == 0)
            printf(" ");
    }
    putchar('\n');
}

