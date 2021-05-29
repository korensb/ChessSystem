#ifndef _GAME_H
#define _GAME_H

/* the pointer to use of the game struct */

typedef struct game *Game;

#include "tournament.h"
#include "player.h"
#include "./mtm_map/map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* functions to create games */

Game gameCreate(int first_player, int second_player, Winner winner, int play_time);
Map createGamesMap();
// Map createGamesPointersMap(); // key: opponent ID, data: pointer to the game
Map createGamesMapForPlayerTournamentsMap(); // key: opponent ID, data: pointer to the game

/* functions that will be used by the Maps */
//COPY
MapDataElement gameCopy(MapDataElement element);
// MapDataElement gamePointerCopy(MapDataElement element);
MapDataElement gameCopyForPlayerTournamentsMap(MapDataElement element);


//destroy
// void gamePointerDestroy(MapDataElement game);
void gameDestroy(MapDataElement game);
void gameDestroyForPlayerTournamentsMap(MapDataElement game);



/* aux functions*/
void gameLossesAndWinsInTournamentCalculate(Map games, int player_id, int* wins, int* losses);
int gamePointsAchieved(Game game, int player_id);
int gameReturnOpponentId(Game game, int player_id);
void gameRemovePlayer(Game game, int player_id);



#endif