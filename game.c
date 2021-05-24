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

void gameAddGame (...) // need to make the function

Game gameCopy(Game element) // hold the game object
{
	if (element == NULL) {
		return NULL;
	}
    Game game = malloc(sizeof(game));
    if (game == NULL)
    return NULL;

	*game = *element;
	return game;
}

void gameDestroy(Game game)
{
    if(game != NULL)
    free(game);
    return 0;
}