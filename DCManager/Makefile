PSPSDK=$(shell psp-config --pspsdk-path)


TARGET = dcman
OBJS = main.o LflashFdisk.o exports.o imports.o

INCDIR = ../include $(PSPSDK)/include
CFLAGS = -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

LDFLAGS = -nostdlib -nodefaultlibs
LIBDIR = $(PSPSDK)/lib
LIBS = -lpspnand_driver -lpspsystemctrl_kernel -lpsplflash_fatfmt -lpspsemaphore_660


include $(PSPSDK)/lib/build.mak
