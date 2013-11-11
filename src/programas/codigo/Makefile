
all: clean test cleanObj

INVOICECFLAGS=`pkg-config --cflags glib-2.0`
INVOICECFLAGS += -g 
INVOICELIBS=`pkg-config --libs glib-2.0` -lyaml -pthread

test: test.o
	$(CC) -g -o $@ $< $(INVOICELIBS)
	echo "ejecutable creado"

test.o: test.c
	$(CC) $(INVOICECFLAGS) -c $< 
	echo "archivo object creado"

clean:
	rm  -f *.o test
	echo "archivos antiguos eliminados"

cleanObj:
	rm -f *.o
	echo "archivo object eliminado"