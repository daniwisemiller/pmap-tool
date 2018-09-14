all: pp_pmap.c
	gcc -o pmap_new pp_pmap.c
clean:
	rm pmap_new *.o
