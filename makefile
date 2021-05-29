CC=gcc
OBJS= chessSystem.o map.o 
EXEC=prog
DEBUG=# now empty, assign -g for debug
CFLAGS=-std=c99 -Wall -Werror 
 
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
chessSystem.o : chessSystem.c chessSystem.h game.h tournament.h player.h chessSystemTestsExample.c map.c
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o : game.c game.h ./mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o : tournament.c tournament.h game.h ./mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : player.c game.h player.h ./mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)
