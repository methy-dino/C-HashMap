libmap.a: map.o
	ar rsv libhmap.a hmap.o

map.o:
	gcc -c hashMap.c -o hmap.o

