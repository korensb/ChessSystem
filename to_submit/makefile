CC=gcc
OBJS= Chess.o game.o tournament.o player.o chessSystemTestsExample.o
EXEC=chess
DEBUG_FLAG=# now empty, assign -g for debug
COMP_FLAG= -std=c99 -Wall -Werror -pedantic 
 
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -L. -lmap -o $@
chessSystemTestsExample.o : chessSystemTestsExample.c test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
Chess.o : Chess.c chessSystem.h game.h tournament.h player.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o :  game.c game.h chessSystem.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o :  tournament.c tournament.h game.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : player.c player.h game.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(EXEC)
