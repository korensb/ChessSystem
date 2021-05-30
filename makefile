CC=gcc
OBJS= ChessSystem.o game.o tournament.o player.o
EXEC=prog
DEBUG_FLAG=# now empty, assign -g for debug
COMP_FLAG= -std=c99 -Wall -Werror -pedantic 
 
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -L. -lmap -o $@
ChessSystem.o : ./mtm_map/map.h ChessSystem.c chessSystemTestsExampleSegel.c chessSystem.h game.h tournament.h player.h  test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o : ./mtm_map/map.h game.c game.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o : ./mtm_map/map.h tournament.c tournament.h game.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : ./mtm_map/map.h player.c game.h player.h 
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)
