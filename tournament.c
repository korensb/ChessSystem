#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tournament
{
    int max_games_per_player;
    const char* tournament_location;
    Map gamesMap;
    Map standing;
    int total_time;
    int longest_time;
    int tournament_winner;
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

void tournamentAddGame (....) //need to make the functiong

Map createGamesMap()
{
    Map newMap = mapCreate(gameCopy,
              intCopy,
              gameDestroy,
              intDestroy,
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

MapKeyElement intCopyKey(MapKeyElement i)
{
    int *ip = malloc(sizeof(int));
    if (ip == NULL){
        return NULL;
    }
    *ip = *(int*)i;
    return ip;
}

MapDataElement intCopyData(MapDataElement i)
{
    int *ip = malloc(sizeof(int));
    if (ip == NULL){
        return NULL;
    }
    *ip = *(int*)i;
    return ip;
}

MapDataElement tournamentCopy(MapDataElement element) // hold the tourments object
{
	if (element == NULL) {
		return NULL;
	}
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament == NULL)
    return NULL;

	tournament = (Tournament)element;
	return tournament;
}

void tournamentDestroy(MapDataElement tournament)
{
    if(tournament != NULL)
    {
        Tournament tournament_to_destroy = (Tournament) tournament;
        mapDestroy(tournament_to_destroy->gamesMap);
        free(tournament);
    }

    return;
}

void intKeyDestroy(MapKeyElement id) {
    free(id);
}

void intDataDestroy(MapDataElement id) {
    free(id);
}

int intCompare(MapKeyElement num1, MapKeyElement num2) {
	int b = *(int*)num2;
	int a = *(int*)num1;
    if (a < b) return -1;
    if (a > b) return +1;
    return 0;
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

static bool location_validation(const char* tournament_location)
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

bool players_games_num_in_tournament_validation (Player player1, Player player2, Tournament tournament)
{
    if (player_games_in_tournament_num(player1, tournament_id) ==  tournament->max_games_per_player || player_games_in_tournament_num (player2, tournament_id) == tournament->max_games_per_player))
        return false;
    return true;    
}

bool is_tournament_active (Tournament tournament)
{
    if (tournament->is_active == TRUE)
        return true;
    return false;
}

 MapResult add_game_to_tournament_map(Tournament tournament, Game game)
 {
     assert(game != NULL);
     if (mapPut(tournament->gamesMap, &(tournament->num_games), game) != MAP_SUCCESS)
        {
            return MAP_OUT_OF_MEMORY;
        }
     tournament->num_games++;
     return MAP_SUCCESS;
 }

MapResult add_game_to_tournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
    int points;
    tournament->total_time = (tournament->total_time) + play_time;
    if (tournament->longest_time < play_time)
        tournament->longest_time = play_time;

    if (winner == DRAW)
        {
            points = mapGet(tournament->standing, &first_player) + 1;
            if (mapPut(tournament->standing, &first_player, &points) != MAP_SUCCESS)
            {
                return MAP_OUT_OF_MEMORY;
            }
            points = mapGet(tournament->standing, &second_player) + 1;
            if (mapPut(tournament->standing, &second_player, &points) != MAP_SUCCESS)
            {
                return MAP_OUT_OF_MEMORY;
            }
        }
    else if (winner == FIRST_PLAYER)
    {
        points = mapGet(tournament->standing, &first_player) + 2;
        if (mapPut(tournament->standing, &first_player, points) != MAP_SUCCESS)
            {
                return MAP_OUT_OF_MEMORY;
            }
    }
    else
    {
        points = mapGet(tournament->standing, &second_player) + 2;
        if (mapPut(tournament->standing, &second_player, points) != MAP_SUCCESS)
            {
                return MAP_OUT_OF_MEMORY;
            }   
    }
}

MapResult tournament_add_player_to_tournament(Tournament tournament, int player_id)
{
    int zero = 0;
    if (mapPut(tournament->standing, &player_id, &zero) != MAP_SUCCESS)
                return MAP_OUT_OF_MEMORY;
    return MAP_SUCCESS;
}

update_opponent_score_in_tournament_after_remove_player(Tournament tournament, int player_id, int points)
{
    int* player_score = mapGet(tournament->standing, game->player_id);
    *player_score = *player_score + points;
}

tournament_remove_player(Tournament tournament, int player_id)
{
    mapRemove(tournament->standing, player_id);
}

bool printTournamentStatistics (Tournament tournament, char* path_file){
    bool tournament_ended = false;
    if (!tournament->is_active)
        {
            tournament_ended = true;
            float average_time = (float)tournament->total_time/mapGetSize(tournament->gamesMap);
            fprintf(stream,  "%d\n" , tournament->tournament_winner);
            fprintf(stream,  "%d\n" , tournament->longest_time);
            fprintf(stream, "%f\n", average_time);
            fprintf(stream, "%s\n" , tournament->tournament_location);
            fprintf(stream,  "%d\n" , mapGetSize(tournament->gamesMap));
            fprintf(stream,  "%d\n" , mapGetSize(tournament->standing));
            fclose(stream);

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
