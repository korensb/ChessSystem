#ifndef _PLAYER_H
#define _PLAYER_H

/* the pointer to use of the player struct */

typedef struct player *Player;

#include "tournament.h"
#include "game.h"
#include "./mtm_map/map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* functions to create players, playersTournaments and gamePointersMap */

Player playerCreate();
Map createPlayersMap();
Map createPlayerTournamentsMap(); // key: tournament ID, data: Map of pointers to games
Map createDoublesMap();

/* functions that will be used by the Maps */

//copy
MapDataElement doubleCopy(MapDataElement i);
MapDataElement playerCopy(MapDataElement element);
MapDataElement mapDataCopy (MapDataElement element);

//destroy
void playerDestroy(MapDataElement player);
void doubleDestroy(MapDataElement id);
void mapDataDestroy(MapDataElement map);


/* aux functions*/
MapResult player_add_player_to_tournament_if_not_exist(Player player, int tournament_id);
int player_games_in_tournament_num (Player player, int tournament_id);
// void player_wins_losses_in_tournament_calculate(Player player, int player_id, int tournament_id, int* wins, int* losses);
bool is_game_existed (Player player1, int tournament_id, int player2);
MapResult player_add_game_to_players (Player player1, Player player2, Game game, int first_player, int second_player, int tournament_id, int play_time, Winner winner);
MapResult player_remove_tournament (Player player, int tournament_id, int player_id);
void player_update_opponent_stats_after_remove(Player opponent, int points);
ChessResult playerLevelCalculate (Player player, int player_id, Map levels, double array[], int array_index);
double calculatePlayerAveragePlayTime(Player player);
ChessResult player_remove_from_system(ChessSystem chess, Player player, int player_id);
int* playerFirstTournament(Player player);
int* playerNextTournament(Player player, int* tournament_id);
Game playerFirstGameInTournament(Player player ,int* tournament_id);
Game playerNextGameInTournament(Player player,int* tournament_id, int opponent_id);

#endif