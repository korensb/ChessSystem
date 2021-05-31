CHESS = chess
GCC = gcc
OBJS = chess.o game.o tournament.o player.o chessSystemTestsExample.o
COMP_FLAG = -std=c99 -Wall -pedantic -errors -Werror 
DEBUG_FLAG =  -g
 
$(CHESS) :$(OBJS)
	$(GCC) $(DEBUG_FLAG) $(OBJS)  -o $@ -L. -lmap
chessSystemTestsExample.o : tests/chessSystemTestsExample.c chessSystem.h map.h test_utilities.h
	$(GCC) -c $(DEBUG_FLAG) $(COMP_FLAG) $<
chess.o : chess.c chessSystem.h game.h tournament.h player.h map.h
	$(GCC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
game.o :  game.c game.h chessSystem.h map.h
	$(GCC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o :  tournament.c tournament.h game.h map.h
	$(GCC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o : player.c player.h game.h map.h
	$(GCC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean:
	rm -f $(OBJS) $(CHESS)
