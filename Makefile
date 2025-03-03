dungeon: RecursiveShadowcasting.c
	gcc -g RecursiveShadowcasting.c -o dungeon -Wall -Werror

clean: 
	rm -f *.o dungeon *~