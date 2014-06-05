hexdecode
=========

command line tool to decode hex-dumped text.

License
-------

zlib License.

Set up
------

All you have to do is compile hexdecode.c by ANSI C compiler.
You can use make(1) if you have gcc (include MinGW) or clang.

Usage
-----

Please check help message `hexdecode -h`

Example
-------

    $ echo 68656C6C6F20776F726C640D0A | hexdecode
    hello world
    $ _
