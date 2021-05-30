#include "tournament.h"
#include "game.h"
#include "player.h"
#include "./mtm_map/map.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EMPTY 0

struct game
{
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
    int tournament_id;
};

Game gameCreate(int first_player, int second_player, Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    return NULL;
    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
    return game;
}

Map createGamesMap()
{
    Map newMap = mapCreate(gameCopy,
              intCopyKey,
              gameDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}

Map createGamesMapForPlayerTournamentsMap()
{
    Map newMap = mapCreate(gameCopyForPlayerTournamentsMap,
              intCopyKey,
              gameDestroyForPlayerTournamentsMap,
              intKeyDestroy,
              intCompare);

    return newMap;
}
/*
Map createGamesPointersMap() // key: opponent ID, data: pointer to the game
{
    Map newMap = mapCreate(
              gamePointerCopy,
              intCopyKey,
              gamePointerDestroy,
              intKeyDestroy,
              intCompare);

    return newMap;
}
*/

MapDataElement gameCopy(MapDataElement element) // hold the game object
{
	if (element == NULL)
    {
		return NULL;
	}
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    return NULL;

	game = (Game)element;
	return game;
}

MapDataElement gameCopyForPlayerTournamentsMap(MapDataElement element) 
{
	if (element == NULL) {
		return NULL;
	}
    Game game = (Game) element;
    return game;
}
/*
MapDataElement gamePointerCopy(MapDataElement element) // hold pointers to games object
{
	if (element == NULL) {
		return NULL;
	}
    Game* game = malloc(sizeof(game));
    if (game == NULL)
    return NULL;

	*game = *(Game*) element;
	return game;
}
*/

 /* 
 void gamePointerDestroy(MapDataElement game)
{
    if(game != NULL)
    {
        free(game);
    }

    return;
}
*/

void gameDestroy(MapDataElement game)
{
    if(game != NULL)
    {
        Game game_to_destroy = (Game) game;
        free(game_to_destroy);
    }
    return;
}

void gameDestroyForPlayerTournamentsMap(MapDataElement game)
{
    return;
}

void gameLossesAndWinsInTournamentCalculate(Map games, int player_id, int* wins, int* losses)
{
    Game game;
    *wins = 0;
    *losses = 0;
    int* game_id = mapGetFirst(games);
    while (game_id != NULL)
    {
        game = mapGet(games, game_id);
        if (game->first_player == player_id)
        {
            if (game->winner == FIRST_PLAYER)
                *wins = *wins +1;
            if (game->winner == SECOND_PLAYER)
                *losses = *losses +1;
        }   
        if (game->second_player == player_id)
        {
            if (game->winner == SECOND_PLAYER)
                *wins = *wins + 1;
            if (game->winner == FIRST_PLAYER)
                *losses = *losses +1;
        }
        free(game_id);   
        game_id = mapGetNext(games);
    }
    return;
}

int gamePointsAchieved(Game game, int player_id)
{
    if(game->winner == DRAW)
        return 1;
    if ((game->winner == FIRST_PLAYER && player_id == game->first_player) || (game->winner == SECOND_PLAYER && player_id == game->second_player))
        return 2;
    return 0;
}

int gameReturnOpponentId(Game game, int player_id)
{
    if(player_id == game->first_player)
        return game->second_player;
    return game->first_player;
}

void gameRemovePlayer(Game game, int player_id)
{
    if(player_id == game->first_player)
        {
            game->first_player = EMPTY;
            if (game->second_player != EMPTY)
                game->winner = SECOND_PLAYER;
        }
    else
    {
        game->second_player = EMPTY;
            if (game->first_player != EMPTY)
                game->winner = FIRST_PLAYER;
    }
    return;
}