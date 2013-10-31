
all: sisctrl

INVOICECFLAGS=`pkg-config --cflags glib-2.0`
INVOICECFLAGS += -g 
INVOICELIBS=`pkg-config --libs glib-2.0` -lyaml -pthread

sisctrl: sisctrl.o
	$(CC) -g -o src/$@ $< $(INVOICELIBS)


sisctrl.o: src/sisctrl.c
	echo "Hello"
	$(CC) $(INVOICECFLAGS) -c $< 

clean:
	rm  -f sisctrl.o sisctrl



