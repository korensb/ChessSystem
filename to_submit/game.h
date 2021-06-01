#ifndef _GAME_H
#define _GAME_H


#include "map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Type for representing a game that stores the data of the player, and metadata about the game */

typedef struct game *Game;


Game gameCreate(int first_player, int second_player, Winner winner, int play_time);
Map createGamesMap();
// Map createGamesPointersMap(); // key: opponent ID, data: pointer to the game
Map createGamesMapForPlayerTournamentsMap(); // key: opponent ID, data: pointer to the game

/* functions that will be used by the Maps */
//COPY
MapKeyElement intCopyKey(MapKeyElement i);
MapDataElement intCopyData(MapDataElement i);
MapDataElement gameCopy(MapDataElement element);
// MapDataElement gamePointerCopy(MapDataElement element);
MapDataElement gameCopyForPlayerTournamentsMap(MapDataElement element);


//destroy
// void gamePointerDestroy(MapDataElement game);
void gameDestroy(MapDataElement game);
void gameDestroyForPlayerTournamentsMap(MapDataElement game);
void intKeyDestroy(MapKeyElement id);
void intDataDestroy(MapDataElement id);

//compare
int intCompare(MapKeyElement num1, MapKeyElement num2);

/* aux functions*/
void gameLossesAndWinsInTournamentCalculate(Map games, int player_id, int* wins, int* losses);
int gamePointsAchieved(Game game, int player_id);
int gameReturnOpponentId(Game game, int player_id);
void gameRemovePlayer(Game game, int player_id);
void addSerialToGame(Game game, int serial);
int getGameSerial(Game game);

void gamePrint(Game game); //remove after checks


#endif