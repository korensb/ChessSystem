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
              mapCopy, // used my func
              intCopy,
              mapDestroy, // used the map.h func
              intDestroy,
              intCompare);

    return newMap;
}

Map createGamesPointersMap() // key: opponent ID, data: pointer to the game
{
    Map newMap = mapCreate(
              gamePointerCopy,
              intCopy,
              gamePointerDestroy,
              intDestroy,
              intCompare);

    return newMap;
}

void playerAddGame (...) // need to make the function

Player playerCopy(Player element) // hold the tourments object
{
	if (element == NULL) {
		return NULL;
	}
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    return NULL;

	*player = *element;
	return player;
}

Game* gamePointerCopy(Game* element) // hold pointers to games object
{
	if (element == NULL) {
		return NULL;
	}
    Game* game = malloc(sizeof(game));
    if (game == NULL)
    return NULL;

	*game = *element;
	return game;
}

void playerDestroy(Player player)
{
    if(player != NULL)
    {
        mapDestroy(player->PlayerTournaments);
        free(player);
    }

    return;
}

ChessResult add_player_to_tournament_if_not_exist(Player player, int tournament_id)
{
    if (!mapContains(player->PlayerTournaments, tournament_id))
        {
            if(mapPut(player->PlayerTournaments, tournament_id, createGamesPointersMap()) != MAP_SUCCESS)
        {
                chessDestroy(chess);
                return MAP_OUT_OF_MEMORY;
        }
                mapPut(tournament->standing,first_player,0);
        }
        return CHESS_SUCCESS;
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