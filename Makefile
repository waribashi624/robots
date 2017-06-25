list_robots: getchar.o list_robots.o
	gcc -o list_robots getchar.o list_robots.o
getchar.o: getchar.c
	gcc -c getchar.c
list_robots.o: list_robots.c
	gcc -c list_robots.c
clean:
	rm -f *.o