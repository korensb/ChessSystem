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

Tournament tournamentCopy(Tournament element) 
{
	if (element == NULL) {
		return NULL;
	}
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament == NULL)
    return NULL;

	*tournament = *element;
	return tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if(tournament != NULL)
    {
        mapDestroy(tournament->gamesMap);
        free(tournament);
    }

    return 0;
}

void losses_and_wins_in_tournament_calculator(Map player_games, int player_id, int* wins, int* losses)
{
    Game game;
    *wins = 0;
    *losses = 0;
    int game_id = mapGetFirst(player_games);
    while (game_id != NULL)
    {
        game = mapGet(player_games, game_id);
        if (game->first_player == player_id)
        {
            if (game->winner == FIRST_PLAYER)
                *wins++;
            if (game->winner == SECOND_PLAYER)
                *losses++;
        }   
        if (game->second_player == player_id)
        {
            if (game->winner == SECOND_PLAYER)
                *wins++;
            if (game->winner == FIRST_PLAYER)
                *losses++;
        }   
        game_id = mapGetNext(player_games);
    }
    return;
}

static bool location_validation(const char* tournament_location)
{ 
    if (tournament_location[0] < 'A' || tournament_location[0] > 'Z' )
        return false;
    int i = 1;
    while (tournament_location[i] != NULL){
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