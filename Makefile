TARGET	= proc_affinity

CC	= gcc
CFLAGS	= -g -Wall -D_GNU_SOURCE
OBJS	= proc_affinity.o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) -o $@ $? $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) core.*
