
all: test

INVOICECFLAGS=`pkg-config --cflags glib-2.0`
INVOICELIBS=`pkg-config --libs glib-2.0` -lyaml

test: test.o
	$(CC)  -o $@ $< $(INVOICELIBS)


test.o: test.c
	echo "Hello"
	$(CC) $(INVOICECFLAGS) -c $< 

clean:
	rm -f test.o test


