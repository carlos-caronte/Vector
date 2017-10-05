# Vector
A generic and dynamic container that expands automatically as elements are added, in C programming language. Vector supports amortized constant time insertion and removal of elements , as well as constant time access. 

C doesn’t allow data types to be passed as parameters. That means a generic container needs to manually manage memory in terms of the client element size, not client data type.

How to Install
Prerequisites for installation:

    gcc
    make

Prerequisites for Running Tests:

    Check

Steps to run tests:

    Download a release from github.

    Extract the archive and cd into the extracted directory.

    Run this command

./configure

    If the above command runs successfully, run this

make

    Again, if the above command runs successfully, run this

make install

    If you want to run tests(Optional) to make sure everything works fine, run this

make check

Now, you can include heade file vector.h

For linking the vector library, gcc command would look like,

gcc -L/usr/local/lib/vector your_cprog.c -lvector

where 'your_cprog.c' is your c program using ds library.

By default vector library is installed into /usr/local/lib/vector unless you have changed the prefix while configuring.

If you find any bugs or have any feature requests, please create an Issue on Github.
