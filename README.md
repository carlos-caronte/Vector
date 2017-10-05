# Vector

**A generic and dynamic container that expands automatically as elements are added, in C programming language.** 
Vector supports amortized constant time insertion and removal of elements , as well as constant time access. 

C doesn’t allow data types to be passed as parameters. That means a generic container needs to manually manage memory in terms of the client element size, not client data type.

Vector is a small standard library and [an easy-to-use C API][embedding]

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
[ product: dvd player 	 price: $195.00 ]
[ product: matches 	 price: $0.26 ]
[ product: notebook 	 price: $1690.00 ]
[ product: plasma tv 	 price: $2860.00 ]
[ product: smartphone 	 price: $649.99 ]
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
