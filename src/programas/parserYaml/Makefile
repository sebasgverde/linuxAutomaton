
all: test

INVOICECFLAGS=`pkg-config --cflags glib-2.0`
INVOICECFLAGS += -g 
INVOICELIBS=`pkg-config --libs glib-2.0` -lyaml -pthread

test: test.o
	$(CC) -g -o $@ $< $(INVOICELIBS)


test.o: test.c
	echo "Hello"
	$(CC) $(INVOICECFLAGS) -c $< 

clean:
	rm  -f test.o test


