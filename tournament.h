#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

/* the pointer to use of the tournament struct */

typedef struct tournament *Tournament;

#include "game.h"
#include "player.h"
#include "./mtm_map/map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* functions to create tournaments and games map */ 
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);
Map createTournamentsMap();
Map createIntsMap();

/* functions that will be used by the Maps */
//COPY
MapKeyElement intCopyKey(MapKeyElement i);
MapDataElement intCopyData(MapDataElement i);
MapDataElement tournamentCopy(MapDataElement element);

//Destroy:
void tournamentDestroy(MapDataElement tournament);
void intKeyDestroy(MapKeyElement id);
void intDataDestroy(MapDataElement id);

//compare
int intCompare(MapKeyElement num1, MapKeyElement num2);

/* aux functions*/
bool location_validation(const char* tournament_location);
bool players_games_num_in_tournament_validation (int player1_amount, int player2_amount, Tournament tournament, int tournament_id);
bool is_tournament_active (Tournament tournament);
MapResult add_game_to_tournament_map(Tournament tournament, Game game);
MapResult add_game_to_tournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);
MapResult tournament_add_player_to_tournament(Tournament tournament, int player_id);
void tournament_update_opponent_score_after_remove_player(Tournament tournament, int player_id, int points);
MapResult tournament_remove_player(Tournament tournament, int player_id);
bool printTournamentStatistics (Tournament tournament, FILE* stream);
int tournament_num_of_games(Tournament tournament);
MapResult end_tournament(Tournament tournament, int tournament_id);

#endif