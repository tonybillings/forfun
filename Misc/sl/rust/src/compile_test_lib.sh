#!/bin/bash
gcc -fPIC -c -o build/sl-test.o sl-test.c
ar rcs ../deps/libsl.a build/sl-test.o

