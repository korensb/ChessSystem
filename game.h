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

/* functions that will be used by the Maps */
//COPY
MapDataElement gameCopy(MapDataElement element);
// MapDataElement gamePointerCopy(MapDataElement element);

//destroy
// void gamePointerDestroy(MapDataElement game);
void gameDestroy(MapDataElement game);


/* aux functions*/
void game_losses_and_wins_in_tournament_calculate(Map player_games, int player_id, int* wins, int* losses);
int game_points_achieved(Game game, int player_id);
int game_return_opponent_id(Game game, int player_id);
void game_remove_player(Game game, int player_id);



#endif