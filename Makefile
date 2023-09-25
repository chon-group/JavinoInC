
SRC = javino.c
OBJ = $(SRC:.c=.o)

ALL_CFLAGS = $(CFLAGS) -Wall -Wextra -Iinclude

CC = gcc
LD = gcc
AR = ar

VPATH = src

%.o: %.c 
	$(CC) $(ALL_CFLAGS) -c $< -o $@

.PHONY: all clean

all: $(OBJ)
	$(AR) cru lib/libjavino.a $(OBJ)

clean:
	rm -f lib/libjavino.a
	rm -f *.o

