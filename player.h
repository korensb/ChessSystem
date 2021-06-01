#ifndef _PLAYER_H
#define _PLAYER_H

#include "game.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Type for representing a player that organizes the data about that player */

typedef struct player *Player;

/* functions to create players, maps with player as data and maps as playersTournaments as data */

//creates an empty player
Player playerCreate();

//creats an empty map with player as the data
Map createPlayersMap();

//creates an emptymap with the tournamnets a player has played as the data
Map createPlayerTournamentsMap();

//creates an empty map with doubles as the data
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

//adds a player to a tournmanet if he is not there yet
MapResult playerAddPlayerToTournamentIfNotExist(Player player, int tournament_id);

//calculates the number of games of a player in a tournament
int playerGamesInTournamentNum (Player player, int tournament_id);

//checks if a game has existed is a tournament
bool isGameExisted (Player player1, int tournament_id, int player2);

//adds a game data to players
MapResult playerAddGameToPlayers (Player player1, Player player2, Game game, int first_player, int second_player, int tournament_id, int play_time, Winner winner);

//removes a players data from a tournamnet
MapResult playerRemoveTournament (Player player, int tournament_id, int player_id);

//updates a players data after his opponent has been removed
void playerUpdateOpponentStatsAfterRemove(Player opponent, int points);

//calculates a players level
MapResult playerLevelCalculate (Player player, int player_id, Map levels, double array[], int array_index);

//calculates a players average play time
double calculatePlayerAveragePlayTime(Player player);

//return the first tournament the player has played in by id
int* playerFirstTournament(Player player);

//return the next tournament the player has played in by id
int* playerNextTournament(Player player, int* tournament_id);

//return the first game a player has played in tournament
Game playerFirstGameInTournament(Player player ,int* tournament_id);

//return the game a player has played in a tournament against an opponent
Game playerNextGameInTournament(Player player,int* tournament_id, int opponent_id);

#endif