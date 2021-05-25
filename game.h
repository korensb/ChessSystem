#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the pointer to use of the game struct */

typedef struct game *Game;

/* functions to create games */

Game gameCreate(int first_player, int second_player, Winner winner, int play_time);
Map createGamesMap();
Map createGamesPointersMap(); // key: opponent ID, data: pointer to the game
Map createPlayerTournamentsMap(); // key: tournament ID, data: Map of pointers to games

/* functions that will be used by the Maps */
//COPY

MapDataElement gameCopy(MapDataElement element);
MapDataElement gamePointerCopy(MapDataElement element);

//destroy

void gamePointerDestroy(MapDataElement game);
void gameDestroy(MapDataElement game);

/* functions to update data of game after adding a game */

void gameAddGame (...) // need to make the function