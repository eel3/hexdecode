# @brief   hexdecode Makefile for Unix like environment.
# @author  eel3
# @date    2013/11/07
#
# @note
# - GCC 4.2.1 (Xcode 4.6.1) on Mac OS X 10.8.4
# - GCC 4.6.3 on Ubuntu 12.04 LTS 32bit

app        := hexdecode
CFLAGS     += -Wall -ansi -pedantic

.PHONY: all
all: $(app)

.PHONY: clean
clean:
	$(RM) $(app)
