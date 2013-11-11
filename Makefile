
all: clean sisctrl cleanObj

PROCESOSAUTCFLAGS=`pkg-config --cflags glib-2.0`
PROCESOSAUTCFLAGS += -g 
PROCESOSAUTLIBS=`pkg-config --libs glib-2.0` -lyaml -pthread

sisctrl: sisctrl.o
	$(CC) -g -o src/$@ $< $(PROCESOSAUTLIBS)
	echo "ejecutable creado"

sisctrl.o: src/sisctrl.c
	$(CC) $(PROCESOSAUTCFLAGS) -c $< 
	echo "archivo object creado"

clean:
	rm  -f *.o src/sisctrl
	echo "archivos antiguos eliminados"

cleanObj:
	rm -f *.o
	echo "archivo object eliminado"