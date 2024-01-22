##########################################################
#
# $Id:$
#
# $Log:$
#
#

CXX = g++
CXXSTANDARD = -std=c++20
OPTFLAGS = -O3 -flto
MFLAGS = -mcpu=native
WFLAGS = -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3
EXTRA_CXXFLAGS = -DNORMALUNIX -DLINUX

LDFLAGS =
LIBS = -lm

CXXFLAGS = $(CXXSTANDARD) $(OPTFLAGS) $(MFLAGS) $(WFLAGS) $(EXTRA_CXXFLAGS) $(INCLUDE)

O = linux

all:	 $(O)/sndserver

clean:
	rm -f *.o *~ *.flc
	rm -f linux/*

# Target
$(O)/sndserver: \
	$(O)/soundsrv.o \
	$(O)/sounds.o \
	$(O)/wadread.o \
	$(O)/linux.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) \
	$(O)/soundsrv.o \
	$(O)/sounds.o \
	$(O)/wadread.o \
	$(O)/linux.o -o $(O)/sndserver $(LIBS)
	echo make complete.

# Rule
$(O)/%.o: %.C
	$(CC) $(CFLAGS) -c $< -o $@