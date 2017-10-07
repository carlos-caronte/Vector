# Vector

**A contiguous growable generic type array in C programming language.**

Vector supports amortized constant time insertion and removal of elements , as well as constant time access.

C doesn’t allow data types to be passed as parameters. That means a generic container needs to manually manage memory in terms of the client element size, not client data type. You can do generic prommaming in c, without to use macros.

Vector is a small standard library and [an easy-to-use C API][embedding]

The object provided by the Vector library is an array type container that:


1. It only must contain addresses of HEAP. The insertion must be done  by calling the Memory Manager, that is: any new insertion will have to be done with pointers to HEAP.
2. Resides in contiguous memory, accessible by arithmetic of pointers
3. It is generic: it supports any types: we identifie the information not by its type, but by its size
4. The object resides in HEAP
5. The object is self-expanding: doubles its capacity automatically if the  insertion of a new element requires it
6. The library provides insertion of items from file.

The contract is:

1. Insert only references to HEAP
2. Who calls the Memory Manager, free the direction stored in the pointer.

If we follow the rules, there will be no problem: neither memory leak, neither corruption memory.

**Example**

```dart
        int structs_len = 10;

        // The client calls Memory Manager
        struct st_ex *structs = calloc(structs_len, sizeof(struct st_ex));
        structs[0] = (struct st_ex) {"mp3 player", 299.0f};
        structs[1] = (struct st_ex) {"plasma tv", 2200.0f};
        structs[2] = (struct st_ex) {"notebook", 1300.0f};
        structs[3] = (struct st_ex) {"smartphone", 499.99f};
        structs[4] = (struct st_ex) {"mp3 player", 600.0f};
        structs[5] = (struct st_ex) {"mp3 player", 451.0f};
        structs[6] = (struct st_ex) {"mp4 player", 7211.0f};
        structs[7] = (struct st_ex) {"dvd player", 150.0f};
        structs[8] = (struct st_ex) {"mp3 ply v", 631.0f};
        structs[9] = (struct st_ex) {"matches", 0.2f };

        //Constructor
        vector_t *struc = vector_Init(structs_len,
                                    sizeof(struct st_ex),
                                    struct_cmp_by_product);

        size_t i;
        v_stat status;

        for(i = 0; i < structs_len; i++)
            vector_Insert(struc, &structs[i]);

/*
 *         struct st_ex buy = {"matches", 32.0f}
 *         vector_Insert(struc, &pepito);
 *         It will generate a warning. 'buy' is in the stack
 */


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

        // Binary Search of an item. It copies in 'searched' all the items that
        // match with 'found'
        status = vector_Filter(struc, found, searched);

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
            case V_ERR_INVALID_ARGUMENT:
                                printf("Invalid arguments\n");
                                break;
        }

        // The client frees its pointers to HEAP
        free(structs);

        // The library frees its pointers to HEAP
        struc->Destroy(struc);
        searched->Destroy_slice(searched);

        return 0;

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
