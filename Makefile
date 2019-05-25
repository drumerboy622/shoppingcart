make: shopping.c
	gcc -g -Wall -std=c99 -o shopping shopping.c
	./shopping

clean: 
	rm shopping results.txt
