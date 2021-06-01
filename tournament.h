#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "game.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** Type for representing a tournament that organizes tournaments games and data */

typedef struct tournament *Tournament; 

/* functions to create empty tournaments and empty maps with tournaments as their data */ 

//creats a tournamnet with no games and data about the tournamnet itself
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

//creates an empty map with tournament as the data
Map createTournamentsMap();

//creates an empty map eith ints as the data
Map createIntsMap();

/* functions that will be used by the Maps */
//copy
MapDataElement tournamentCopy(MapDataElement element);

//destroy
void tournamentDestroy(MapDataElement tournament);




/* aux functions*/
//checks that the location given to tournamnet is valid
bool locationValidation(const char* tournament_location);

//checks the player did not exceed the number of games primitted in a tournament
bool playersGamesNumInTournamentValidation (int player1_amount, int player2_amount, Tournament tournament, int tournament_id);
MapResult addGameToTournamentMap(Tournament tournament, Game game);

//adds a game to a tournament
MapResult addGameToTournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);

//updates a player score after his opponet has been removed
void tournamentUpdateOpponentScoreAfterRemovePlayer(Tournament tournament, int player_id, int points);

//removes a players data from a tournament 
void tournamentRemovePlayer(Tournament tournament, int player_id);

//prints tournamnets statistics into a file
bool printTournamentStatistics (Tournament tournament, FILE* stream);

//calculates the number of games in a tournamnet
int tournamentNumOfGames(Tournament tournament);

MapResult tournamentAddPlayerToTournament(Tournament tournament, int player_id);

//updates a tournaments data after it has been ended
MapResult endTournament(Tournament tournament, int tournament_id);

//updates the tournament data after removing a player
void tournamentUpdateGameAfterRemovePlayer(Tournament tournament, int removed_player_id, int game_serial);

//checks if the tournament has been ended
bool isTournamentActive (Tournament tournament);

#endif