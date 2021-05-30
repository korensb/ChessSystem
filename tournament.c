#include "./mtm_map/map.h"
#include "tournament.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct tournament
{
    int max_games_per_player;
    const char* tournament_location;
    Map gamesMap;
    Map standing;
    int total_time;
    int longest_time;
    int tournament_winner;
    int num_games;
    bool is_active;
};

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location)
{
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament == NULL)
    return NULL;
	tournament->gamesMap = createGamesMap();
	if (tournament->gamesMap == NULL)
    {
		return NULL;
	}

    tournament->standing = createIntsMap();
	if (tournament->standing == NULL)
    {
		return NULL;
	}

    tournament->max_games_per_player = max_games_per_player;
    tournament->tournament_location = tournament_location;
    tournament->is_active = true;
    tournament->longest_time = 0;
    tournament->total_time = 0;
    tournament->num_games = 0;

    return tournament;
}

Map createTournamentsMap()
{
    Map newMap = mapCreate(tournamentCopy,
              intCopyKey,
              tournamentDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}

Map createIntsMap()
{
    Map newMap = mapCreate(intCopyData,
              intCopyKey,
              intDataDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}



MapDataElement tournamentCopy(MapDataElement element) // hold the tourments object
{
	if (element == NULL) {
		return NULL;
	}
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament == NULL)
    return NULL;

	*tournament = *(Tournament)element;
	return tournament;
}

void tournamentDestroy(MapDataElement tournament)
{
    if(tournament != NULL)
    {
        Tournament tournament_to_destroy = (Tournament) tournament;
        mapDestroy(tournament_to_destroy->gamesMap);
        mapDestroy(tournament_to_destroy->standing);
        free(tournament_to_destroy);
    }

    return;
}




/* if int compare doesnt work
static int intCompare(MapKeyElement num1, MapKeyElement num2) {
    int a,b;
	int* num1_ptr = &a;
	int* num2_ptr = &b;
	*num2_ptr = *(int*)num2;
	*num1_ptr = *(int*)num1;
    if (*num1_ptr < *num2_ptr) return -1;
    if (*num1_ptr > *num2_ptr) return +1;
    return 0;
}
*/

bool locationValidation(const char* tournament_location)
{ 
    if (tournament_location[0] < 'A' || tournament_location[0] > 'Z' )
        return false;
    int i = 1;
    while (tournament_location[i] != 0){
        if ((tournament_location[i] >= 'a' && tournament_location[i] <= 'z') || (tournament_location[i] == ' ')){
            i++;
        }
        else{
            return false;
        } 
    }
    return true;
}

bool playersGamesNumInTournamentValidation (int player1_amount, int player2_amount, Tournament tournament, int tournament_id)
{
    if (player1_amount ==  tournament->max_games_per_player || player2_amount == tournament->max_games_per_player)
        return false;
    return true;    
}

bool isTournamentActive (Tournament tournament)
{
    if (tournament->is_active == true)
        return true;
    return false;
}

MapResult addGameToTournamentMap(Tournament tournament, Game game)
 {
     assert(game != NULL);
     if (mapPut(tournament->gamesMap, &(tournament->num_games), game) != MAP_SUCCESS)
        {
            return MAP_OUT_OF_MEMORY;
        }
     tournament->num_games = tournament->num_games + 1;
     return MAP_SUCCESS;
 }

MapResult addGameToTournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
    int* points;
    tournament->total_time = (tournament->total_time) + play_time;
    if ((tournament->longest_time) < play_time)
        tournament->longest_time = play_time;

    if (winner == DRAW)
        {
            points = mapGet(tournament->standing, &first_player);
            *points = *points + 1;
            points = mapGet(tournament->standing, &second_player);
            *points = *points + 1;
        }
    else if (winner == FIRST_PLAYER)
    {
        points = mapGet(tournament->standing, &first_player);
        *points = *points + 2;
    }
    else
    {
        points = mapGet(tournament->standing, &second_player);
        *points = *points + 2; 
    }
    return MAP_SUCCESS;
}

MapResult tournamentAddPlayerToTournament(Tournament tournament, int player_id)
{
    if (!mapContains(tournament->standing, &player_id))
    {
        int zero = 0;
        if (mapPut(tournament->standing, &player_id, &zero) != MAP_SUCCESS)
            return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS;
}

void tournamentUpdateOpponentScoreAfterRemovePlayer(Tournament tournament, int player_id, int points)
{
    int* player_score = mapGet(tournament->standing, &player_id);
    assert(player_score != NULL);
    *player_score = *player_score + points;
    return;
}

MapResult tournamentRemovePlayer(Tournament tournament, int player_id)
{
    return mapRemove(tournament->standing, &player_id);
}

bool printTournamentStatistics (Tournament tournament, FILE* stream)
{
    bool tournament_ended = false;
    if (!tournament->is_active)
        {
            tournament_ended = true;
            float average_time = (float)tournament->total_time/mapGetSize(tournament->gamesMap);
            fprintf(stream,  "%d\n" , tournament->tournament_winner);
            fprintf(stream,  "%d\n" , tournament->longest_time);
            fprintf(stream, "%0.2f\n", average_time);
            fprintf(stream, "%s\n" , tournament->tournament_location);
            fprintf(stream,  "%d\n" , mapGetSize(tournament->gamesMap));
            fprintf(stream,  "%d\n" , mapGetSize(tournament->standing));

                /*
                Print:
                winner = tournament->tournament_winner
                longest game time = tournament->longest_time
                average game time = (double)tournament->total_time/mapGetSize(tournament->gamesMap)
                location = tournament->tournament_location
                number of games = mapGetSize(tournament->gamesMap)
                number of players = mapGetSize(tournament->standing)
                */
        }
    return tournament_ended;
}

int tournamentNumOfGames(Tournament tournament)
{
    return tournament->num_games;
}


MapResult endTournament(Tournament tournament, int tournament_id)
{
    tournament->is_active = false;

    int winner_ID;
    int winner_score = 0;
    int winner_losses = 0;
    int winner_wins = 0;
    int* current_id = mapGetFirst(tournament->standing);
    int check_wins;
    int check_losses;

    while (current_id != NULL)
    {
        if (*(int *)mapGet(tournament->standing, current_id) >= winner_score)
        {
            if (*(int *)mapGet(tournament->standing, current_id) > winner_score) //if current player has higher score update the winner stats to his stats
            {
                winner_ID = *current_id;
                winner_score = *(int *)mapGet(tournament->standing, current_id);
                gameLossesAndWinsInTournamentCalculate(tournament->gamesMap, *current_id, &winner_wins, &winner_losses);
            }
            else 
            {
                gameLossesAndWinsInTournamentCalculate(tournament->gamesMap, *current_id, &check_wins, &check_losses);
                if (check_losses < winner_losses || (check_losses == winner_losses && check_wins > winner_wins))
                    {
                        winner_ID = *current_id;
                        winner_wins = check_wins;
                        winner_losses = check_losses;
                    }
            }
        }
        free(current_id);
        current_id = mapGetNext(tournament->standing);
    }
    tournament->tournament_winner = winner_ID;
    return MAP_SUCCESS;
}
