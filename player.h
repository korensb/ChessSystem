#ifndef _PLAYER_H
#define _PLAYER_H

#include "tournament.h"
#include "game.h"
#include "./mtm_map/map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the pointer to use of the player struct */

typedef struct player *Player;

/* functions to create players, playersTournaments and gamePointersMap */

Player playerCreate();
Map createPlayerTournamentsMap(); // key: tournament ID, data: Map of pointers to games
Map createPlayersMap();
Map createDoublesMap();

/* functions to update data of player after adding a game */
bool is_game_existed (Player player1, int tournament_id, int player2);
void playerAddGame (...) // need to make the function
ChessResult add_player_to_tournament_if_not_exist(Player player, int tournament_id);
MapResult player_remove_tournament (Player player, int tournament_id, int player_id);
add_game_to_players (Player player1, Player player2, Game game, int first_player, int second_player, int tournament_id, int play_time, Winner winner);
/* functions that will be used by the Maps */
//copy

MapDataElement doubleCopy(MapDataElement i);
MapDataElement playerCopy(MapDataElement element);
MapDataElement mapDataCopy (MapDataElement element);
//destroy

void playerDestroy(MapDataElement player);
void doubleDestroy(MapDataElement id);
void mapDataDestroy(MapDataElement map);

//functions to receive the data needed for the chessSystem functions
ChessResult playerLevelCalculate (Player player, int player_id, Map levels, double* array, int array_index);
double calculatePlayerAveragePlayTime(Player player);

#endif