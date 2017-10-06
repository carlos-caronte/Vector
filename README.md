# Vector

**A contiguous growable generic type array in C programming language.**

Vector supports amortized constant time insertion and removal of elements , as well as constant time access.

C doesn’t allow data types to be passed as parameters. That means a generic container needs to manually manage memory in terms of the client element size, not client data type. You can do generic prommaming in c, without to use macros.

Vector is a small standard library and [an easy-to-use C API][embedding]

               The object provided by the Vector library is an array type
               container that:

               1. It only must contain addresses of HEAP.
                   The insertion must be done  by calling the Memory
                   Manager, that is: any new insertion will have
                   to be done with pointers to HEAP.

                2. Resides in contiguous memory, accessible by
                    arithmetic of pointers

                3. It is generic: it supports any types: we identifie the
                    information not by its type, but by its size

                4. The object resides in HEAP

                5. The object is self-expanding: doubles its capacity
                    automatically if the  insertion of a new element
                    requires it

                6. The library provides insertion of items from file.


                The insertion of elements is always by reference. If the item
                were created in main, (reside in STACK) the object pointed by
                our stored direction, is released when the application finishes.
                There is nothing to free.

                But if you create the element by calling the Memory Manager
                (calloc,  * malloc, ...) then the pointer resides in the HEAP, and
                must be released before application has finished.

               This means that either we create data that resides in STACK or
               we create it by calling the memory manager. If both forms
               are combined  then we release a STACK direction with the call
               to Destroy, and this will cause a segment violation, or any other
               undefined behavior. Or, if we do not call Destroy, we will leave
               pointers without releasing when our application has returned
               (memory - leak).

               If the insertion is made from function calls, you must only insert
               elements whose addresses reside in HEAP, since the space in
               the stack corresponding to the function is released once it
               returns.

               Well, our object only must contain addresses of HEAP.
               The insertion must be done  by calling the Memory Manager
               before. That is: any new insertion will have to be done with
               pointers to HEAP.

               The contract is:

               1.Insert only references to HEAP
               2. Who calls the Memory Manager, free the direction stored in
                   the pointer.

               If we follow the rules, there will be no problem: neither
               memory leak, neither double free.



**Example**

```dart

                             Remove records with patterns


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

        // Constructor: required size of elements (sizeof(struct st_ex)
        // and order criteria (pointer function "struct_cmp_by_product"
        vector_t *struc = vector_Init(structs_len,
                                    sizeof(struct st_ex),
                                    struct_cmp_by_product);

        size_t i;
        for(i = 0; i < structs_len; i++)
            vector_Insert(struc, &structs[i]);

        // Use qsort to order the Vector object
        vector_Sort(struc);

        puts("*** Struct (product)... remove pattern");

        // Use pattern (fnmatch.h library)
        pattern ="mp*";

        status = vector_Remove_Pattern(struc, pattern);

        switch(status) {

            case V_OK :  print_struct_Vector(struc, vector_Len(struc));
                                 break;
            case V_ERR_IS_EMPTY :
                                printf("Vector is Empty\n");
                                break;
            case V_ERR_VALUE_NOT_FOUND :
                                printf("Pattern %s has not been found\n", pattern);
                                break;
        }


        struc->Destroy(struc);
```
**Output**
```
*** Struct (product)... remove pattern
[ product: dvd player    price: $195.00 ]
[ product: matches   price: $0.26 ]
[ product: notebook      price: $1690.00 ]
[ product: plasma tv     price: $2860.00 ]
[ product: smartphone    price: $649.99 ]
```


**How to Install**
Prerequisites for installation:

    gcc
    make

**Prerequisites for Running Tests:**

    Check

Steps to run tests:

    Download a release from github.

    Extract the archive and cd into the extracted directory.

    Run this command

`./configure`

    If the above command runs successfully, run this

`make`

    Again, if the above command runs successfully, run this

`make install`

    If you want to run tests(Optional) to make sure everything works fine, run this

`make check`

Now, you can include heade file **vector.h**

For linking the vector library, gcc command would look like, `gcc -L/usr/local/lib/vector your_cprog.c -lvector`

where `your_cprog.c` is your c program using Vector library.

By default vector library is installed into `/usr/local/lib/vector` unless you have changed the prefix while configuring.

If you find any bugs or have any feature requests, please create an Issue on Github.

[embedding]: https://carlos-caronte.github.io/Vector/
