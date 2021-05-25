#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct player
{
    Map PlayerTournaments;
    int games;
    int losses;
    int wins;
    int points;
    int total_time;
};

Player playerCreate()
{
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    return NULL;
    player->PlayerTournaments = createPlayerTournamentsMap();
	if (player->PlayerTournaments == NULL)
    {
		return NULL;
	}
    player->games = 0;
    player->wins = 0;
    player->points = 0;
    player->losses = 0;
    player->total_time = 0;

    return player;
}

Map createPlayersMap()
{
    Map newMap = mapCreate(playerCopy,
              intCopyKey,
              playerDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}

Map createPlayerTournamentsMap() // key: tournament ID, data: GamesPointersMap
{
    Map newMap = mapCreate(
              mapDataCopy, // used my func
              intCopyKey,
              mapDataDestroy, // used the map.h func
              intKeyDestroy,
              intCompare);

    return newMap;
}

Map createDoublesMap()
{
    Map newMap = mapCreate(doubleCopy,
              intCopyKey,
              doubleDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}

void playerAddGame (...) // need to make the function

MapDataElement doubleCopy(MapDataElement i)
{
    double* ip = malloc(sizeof(double));
    if (ip == NULL){
        return NULL;
    }
    *ip = *(double*)i;
    return ip;
}

MapDataElement playerCopy(MapDataElement element) // hold the tourments object
{
	if (element == NULL) {
		return NULL;
	}
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    return NULL;

	player = (Player) element;
	return player;
}


void playerDestroy(MapDataElement player)
{
    if(player != NULL)
    {
        Player player_to_destroy = (Player) player;
        mapDestroy(player_to_destroy->PlayerTournaments);
        free(player);
    }

    return;
}

MapResult player_add_player_to_tournament_if_not_exist(Player player, Tournament tournament, int tournament_id, int player_id)
{
    if (!mapContains(player->PlayerTournaments, tournament_id))
        {
            int zero = 0;
            if(mapPut(player->PlayerTournaments, tournament_id, createGamesMap()) != MAP_SUCCESS)
                return MAP_OUT_OF_MEMORY;

            if (tournament_add_player_to_tournament(tournament, player_id) != MAP_SUCCESS)
                return MAP_OUT_OF_MEMORY;

        }
        return MAP_SUCCESS;
}

int player_games_in_tournament_num (Player player, int tournament_id)
{
    Map player_games = mapGet(player->PlayerTournaments, tournament_id);
    return mapGetSize(player_games);
}


bool is_game_existed (Player player1, int tournament_id, int player2)
{
    Map player1_games = mapGet(player1->PlayerTournaments, tournament_id);
    if (mapContains(player1_games, second_player)) //the keyElement is the opponent ID
        return true;
}


mapPut(player1_games, second_player, mapGet(tournament->gamesMap, tournament->num_games));

add_game_to_players (Player player1, Player player2, Game game, int first_player, int second_player, int tournament_id, int play_time, Winner winner)
{
    Map player1_games = mapGet(player1->PlayerTournaments, &tournament_id);
    Map player1_games = mapGet(player2->PlayerTournaments, &tournament_id);

    mapPut(player1_games, &second_player, game);
    mapPut(player2_games, &second_player, game);

    player1->total_time = (player->total_time) + play_time;
    player2->total_time = (player->total_time) + play_time;

    if (winner == FIRST_PLAYER)
        {
            player1->points = player1->points + 2;
            player1->wins++;
            player2->losses++;
        }
        else if (winner == SECOND_PLAYER)
        {
            player2->points = player2->points + 2;
            player2->wins++;
            player1->losses++;
        }            
        else
        {
        player1->points++;
        player2->points++;
        }

        player1->games++;
        player2->games++;

}