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


int main()
{

    /***************************************************************
     *
     *                                  Insert from FILE
     *
     * *************************************************************/

        vector_t *v_file = vector_Init(10, sizeof(char *), string_cmp);

        const char *filename =" HERE THE FILE  ";
        char delim = 32; // SPACE
        size_t len, i;
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
        vector_Destroy(v_file);

        return 0;
}
