hexdecode
=========

Command line tool to decode hex-dumped text.

License
-------

zlib License.

Target environments
-------------------

Windows, Linux, macOS.

hexdecode is written in ANSI C89, and so probably works fine on other OS.

Set up
------

1. Compile hexdecode.c. Use make and Makefile.
2. Put hexdecode in a directory registered in PATH.

| toolset                            | Makefile                 |
|:-----------------------------------|:-------------------------|
| Linux                              | Makefile                 |
| Mac OS X                           | Makefile\_mac            |
| MinGW/TDM-GCC (with GNU make)      | Makefile\_mingw          |
| MinGW-w64/TDM64-GCC (32bit binary) | Makefile\_mingw64\_32bit |
| Microsoft Visual C++ (with NMAKE)  | Makefile\_vc\_nmake      |

Usage
-----

Please check help message `hexdecode -h`

Example
-------

    $ echo 68656C6C6F20776F726C640D0A | hexdecode
    hello world
    $ _
