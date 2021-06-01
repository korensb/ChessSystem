#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "game.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* the pointer to use of the tournament struct */

typedef struct tournament *Tournament;

/* functions to create tournaments and games map */ 
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);
Map createTournamentsMap();
Map createIntsMap();

/* functions that will be used by the Maps */
//COPY
MapDataElement tournamentCopy(MapDataElement element);

//Destroy:
void tournamentDestroy(MapDataElement tournament);




/* aux functions*/
bool locationValidation(const char* tournament_location);
bool playersGamesNumInTournamentValidation (int player1_amount, int player2_amount, Tournament tournament, int tournament_id);
MapResult addGameToTournamentMap(Tournament tournament, Game game);
MapResult addGameToTournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);
MapResult tournamentAddPlayerToTournament(Tournament tournament, int player_id);
void tournamentUpdateOpponentScoreAfterRemovePlayer(Tournament tournament, int player_id, int points);
MapResult tournamentRemovePlayer(Tournament tournament, int player_id);
bool printTournamentStatistics (Tournament tournament, FILE* stream);
int tournamentNumOfGames(Tournament tournament);
MapResult endTournament(Tournament tournament, int tournament_id);
void tournamentUpdateGameAfterRemovePlayer(Tournament tournament, int removed_player_id, int game_serial);
bool isTournamentActive (Tournament tournament);

#endif