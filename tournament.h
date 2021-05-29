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
bool locationValidation(const char* tournament_location);
bool playersGamesNumInTournamentValidation (int player1_amount, int player2_amount, Tournament tournament, int tournament_id);
bool isTournamentActive (Tournament tournament);
MapResult addGameToTournamentMap(Tournament tournament, Game game);
MapResult addGameToTournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);
MapResult tournamentAddPlayerToTournament(Tournament tournament, int player_id);
void tournamentUpdateOpponentScoreAfterRemovePlayer(Tournament tournament, int player_id, int points);
MapResult tournamentRemovePlayer(Tournament tournament, int player_id);
bool printTournamentStatistics (Tournament tournament, FILE* stream);
int tournamentNumOfGames(Tournament tournament);
MapResult endTournament(Tournament tournament, int tournament_id);

#endif