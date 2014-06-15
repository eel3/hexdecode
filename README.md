hexdecode
=========

Command line tool to decode hex-dumped text.

License
-------

zlib License.

Target environments
-------------------

Windows, Linux, Mac OS X.

hexdecode is written in ANSI C89, and so probably work fine on other OS.

Set up
------

All you have to do is compile hexdecode.c by ANSI C compiler.

You can use Makefiles if you have GNU make and clang/gcc (include MinGW).
You can also use Makefile_vc_nmake if you have Microsoft(R) C/C++ Compiler and NMAKE (experimental).

Usage
-----

Please check help message `hexdecode -h`

Example
-------

    $ echo 68656C6C6F20776F726C640D0A | hexdecode
    hello world
    $ _
