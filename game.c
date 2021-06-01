#include "map.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct game
{
    int game_serial;
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
    int tournament_id;
};

Game gameCreate(int first_player, int second_player, Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL){
        return NULL;
    }
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
    Map newMap = mapCreate(gameCopy,
              intCopyKey,
              gameDestroy,
              intKeyDestroy,
              intCompare);
    return newMap;
}

MapKeyElement intCopyKey(MapKeyElement i)
{
    if (i == NULL)
    {
        return NULL;
    }
    int *ip = malloc(sizeof(*ip));
    if (ip == NULL)
    {
        return NULL;
    }
    *ip = *(int*)i;
    return ip;
}

MapDataElement intCopyData(MapDataElement i)
{
    if (i == NULL)
    {
        return NULL;
    }
    int *ip = malloc(sizeof(*ip));
    if (ip == NULL)
    {
        return NULL;
    }
    *ip = *(int*)i;
    return ip;
}

MapDataElement gameCopy(MapDataElement element) // hold the game object
{
	if (element == NULL)
    {
		return NULL;
	}
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        return NULL;
    }
    *game = *(Game)element;
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

void gameDestroy(MapDataElement game)
{
    if(game != NULL)
    {
        Game game_to_destroy = (Game) game;
        free(game_to_destroy);
    }
}

void gameDestroyForPlayerTournamentsMap(MapDataElement game)
{
    return;
}

void intKeyDestroy(MapKeyElement id)
{
    if (id != NULL)
    {
        int* int_to_destroy = (int*)id;
        free(int_to_destroy);
    }
}

void intDataDestroy(MapDataElement id)
{
     if (id != NULL)
     {
        int* int_to_destroy = (int*)id;
        free(int_to_destroy);
    }
}

int intCompare(MapKeyElement num1, MapKeyElement num2)
{
	int b = *(int*)num2;
	int a = *(int*)num1;
    if (a < b) return -1;
    if (a > b) return +1;
    return 0;
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
        if ((game->first_player) == player_id)
        {
            if (game->winner == FIRST_PLAYER)
            {
                *wins = *wins +1;
            }
            if (game->winner == SECOND_PLAYER)
            {
                *losses = *losses +1;
            }
        }   
        if (game->second_player == player_id)
        {
            if (game->winner == SECOND_PLAYER)
            {
                *wins = *wins + 1;
            }
            if (game->winner == FIRST_PLAYER)
            {
                *losses = *losses +1;
            } 
        }
        free(game_id);   
        game_id = mapGetNext(games);
    }
}

int gamePointsAchieved(Game game, int player_id)
{
    if(game->winner == DRAW)
    {
        return 1;
    }
    if ((game->winner == FIRST_PLAYER && player_id == (game->first_player)) || (game->winner == SECOND_PLAYER && player_id == game->second_player))
    {
        return 2;
    }
    return 0;
}

int gameReturnOpponentId(Game game, int player_id)
{
    if(player_id == (game->first_player))
    {
        return (game->second_player);
    }
    return (game->first_player);
}

void gameRemovePlayer(Game game, int player_id)
{
    if(player_id == (game->first_player))
    {
        (game->first_player) = 0;
        if (game->second_player != 0)
        {
            game->winner = SECOND_PLAYER;
        }
    }
    else
    {
        game->second_player = 0;
        if ((game->first_player) != 0)
        {
            game->winner = FIRST_PLAYER;
        }
    }
    return;
}

void addSerialToGame(Game game, int serial)
{
    game->game_serial = serial;
}

int getGameSerial(Game game)
{
    return game->game_serial;
}

