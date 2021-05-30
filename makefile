CHESS=chess
CC=gcc
OBJS= chess.o game.o tournament.o player.o chessSystemTestsExample.o
COMP_FLAG= -std=c99 -Wall -pedantic -errors -Werror 
DEBUG_FLAG= -DNDEBUG -g
 
$(CHESS) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -L. -lmap -o $@
chessSystemTestsExample.o : tests/chessSystemTestsExample.c test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
chess.o : chess.c chessSystem.h game.h tournament.h player.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o :  game.c game.h chessSystem.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o :  tournament.c tournament.h game.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : player.c player.h game.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(CHESS)
