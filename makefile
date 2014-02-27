#Makefile for Producer-Consumer program
#
CC=gcc
CFLAGS=-Wall -pthread
#
# Compiler Flags:
#
#	-Wall -- show all warnings
#	-pthread -- use POSIX threads library
prod_cons_main: producer.o consumer.o linked_list.o prod_cons_main.o mt19937-64.o
	$(CC) $(CFLAGS) -o prod_cons_main producer.o consumer.o linked_list.o mt19937-64.o prod_cons_main.o

prod_cons_main.o: linked_list.h prod_cons.h mt64.h prod_cons_main.c
	$(CC) $(CFLAGS) -c prod_cons_main.c

producer.o: linked_list.h prod_cons.h mt64.h producer.c
	$(CC) $(CFLAGS) -c producer.c

consumer.o: linked_list.h prod_cons.h consumer.c
	$(CC) $(CFLAGS) -c consumer.c

linked_list.o: linked_list.h linked_list.c
	$(CC) $(CFLAGS) -c linked_list.c

clean:
	rm -f *~ core *.core
	rm -f *.o
#end of makefile
