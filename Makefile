list_robots: getchar.o list_robots.o
	cc -o list_robots getchar.o list_robots.o -lcurses
getchar.o: getchar.c
	cc -c getchar.c
list_robots.o: list_robots.c
	cc -c list_robots.c
clean:
	rm -f *.o