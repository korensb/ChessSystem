#ifndef _GAME_H
#define _GAME_H


#include "map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Type for representing a game that organizes the data about that game */

typedef struct game *Game;

/* functions to create games, and maps with games as data */

//creates a game with the data given
Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

//creats an empty map with games as data
Map createGamesMap();

//creats an empty map with games as data
Map createGamesMapForPlayerTournamentsMap(); 

/* functions that will be used by the Maps */
//COPY
MapKeyElement intCopyKey(MapKeyElement i);
MapDataElement intCopyData(MapDataElement i);
MapDataElement gameCopy(MapDataElement element);
MapDataElement gameCopyForPlayerTournamentsMap(MapDataElement element);


//destroy
void gameDestroy(MapDataElement game);
void gameDestroyForPlayerTournamentsMap(MapDataElement game);
void intKeyDestroy(MapKeyElement id);
void intDataDestroy(MapDataElement id);

//compare
int intCompare(MapKeyElement num1, MapKeyElement num2);

/* aux functions*/

// calculates hoe many losses and wins were in a game 
void gameLossesAndWinsInTournamentCalculate(Map games, int player_id, int* wins, int* losses);

//calculates how many pointes a player acheived in a game
int gamePointsAchieved(Game game, int player_id);

//returns a players opponent id in a game
int gameReturnOpponentId(Game game, int player_id);
<<<<<<< HEAD
=======
void gameRemovePlayer(Game game, int player_id);
void addSerialToGame(Game game, int serial);
int getGameSerial(Game game);
>>>>>>> af85a0e55687e1bfaf7f1ca28861b814108ec613

//removes a player from a game
void gameRemovePlayer(Game game, int player_id);


#endif