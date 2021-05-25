#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void gameAddGame (...) // need to make the function

MapDataElement gameCopy(MapDataElement element) // hold the game object
{
	if (element == NULL) {
		return NULL;
	}
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    return NULL;

	game = (Game)element;
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

MapDataElement mapDataCopy (MapDataElement element){
    Map map = mapCopy ((Map)element);
    if (map == NULL){
        return NULL;
    }
    return map;
}

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
    free(game);
    return;
}

int points_achieved_in_game(Game game, int player_id)
{
    if(game->winner == DRAW)
        return 1;
    if (game->winner == FIRST_PLAYER && player_id == game->first_player || game->winner == SECOND_PLAYER && player_id == game->second_player)
        return 2;
    return 0;
}

