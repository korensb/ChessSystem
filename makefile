CC=gcc
OBJS= ChessSystem.o game.o tournament.o player.o
EXEC=prog
DEBUG_FLAG=# now empty, assign -g for debug
COMP_FLAG= -std=c99 -Wall -Werror 
 
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
ChessSystem.o : ./mtm_map/map.h libmap.a ChessSystem.c chessSystem.h game.h tournament.h player.h chessSystemTestsExample.c 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o : ./mtm_map/map.h libmap.a game.c game.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o : ./mtm_map/map.h libmap.a tournament.c tournament.h game.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : ./mtm_map/map.h libmap.a player.c game.h player.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)
