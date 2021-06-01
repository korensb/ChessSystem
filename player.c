#include "map.h"
#include "game.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY 0

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
    {
        return NULL;
    }
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
              mapDataCopy,
              intCopyKey,
              mapDataDestroy,
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

MapDataElement doubleCopy(MapDataElement i)
{
    if (i == NULL)
    {
        return NULL;
    }
    double* ip = malloc(sizeof(*ip));
    if (ip == NULL)
    {
        return NULL;
    }
    *ip = *(double*)i;
    return ip;
}

MapDataElement playerCopy(MapDataElement element)
{
	if (element == NULL)
    {
		return NULL;
	}
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    {
        return NULL;
    }
	*player = *(Player) element;
	return player;
}

MapDataElement mapDataCopy (MapDataElement element)
{
    if (element == NULL)
    {
        return NULL;
    }
    Map map = mapCopy ((Map)element);
    if (map == NULL)
    {
        return NULL;
    }
    return map;
}

void playerDestroy(MapDataElement player)
{
    if(player != NULL)
    {
        Player player_to_destroy = (Player) player;
        mapDestroy(player_to_destroy->PlayerTournaments);
        free(player_to_destroy);
    }
}

void doubleDestroy(MapDataElement d)
{
    if( d != NULL)
    {
        double* double_to_destroy = (double*) d;
        free(double_to_destroy);
    }
}

void mapDataDestroy(MapDataElement map)
{
    if(map != NULL)
    {
        Map map_to_destroy = (Map) map;
        mapDestroy(map_to_destroy);
    }
    return;
}


MapResult playerAddPlayerToTournamentIfNotExist(Player player, int tournament_id)
{
    if (!mapContains(player->PlayerTournaments, &tournament_id))
    {
        Map games_in_tournament = createGamesMapForPlayerTournamentsMap();
        if(mapPut(player->PlayerTournaments, &tournament_id, games_in_tournament) != MAP_SUCCESS)
        {
            return MAP_OUT_OF_MEMORY;
        }
        mapDestroy(games_in_tournament);
    }
    return MAP_SUCCESS;
}

int playerGamesInTournamentNum (Player player, int tournament_id)
{
    Map player_games = mapGet(player->PlayerTournaments, &tournament_id);
    return mapGetSize(player_games);
}

bool isGameExisted (Player player1, int tournament_id, int player2)
{
    Map player1_games = mapGet(player1->PlayerTournaments, &tournament_id);
    if (mapContains(player1_games, &player2))
    {
        return true;
    }
    return false;
}

MapResult playerAddGameToPlayers (Player player1, Player player2, Game game, int first_player, int second_player, int tournament_id, int play_time, Winner winner)
{

    Map player1_games = mapGet(player1->PlayerTournaments, &tournament_id);
    Map player2_games = mapGet(player2->PlayerTournaments, &tournament_id);

    mapPut(player1_games, &second_player, game);
    mapPut(player2_games, &first_player, game);

    player1->total_time = (player1->total_time) + play_time;
    player2->total_time = (player2->total_time) + play_time;

    if (winner == FIRST_PLAYER)
    {
        player1->points = player1->points + 2;
        player1->wins = player1->wins + 1;
        player2->losses = player2->losses + 1;
    }
    else if (winner == SECOND_PLAYER)
    {
        player2->points = player2->points + 2;
        player2->wins = player2->wins + 1;
        player1->losses = player1->losses + 1;
    }            
    else
    {
        player1->points = player1->points + 1;
        player2->points = player2->points + 1;
    }

    player1->games = player1->games + 1;
    player2->games = player2->games + 1;

    return MAP_SUCCESS;
}

MapResult playerRemoveTournament (Player player, int tournament_id, int player_id)
{
    if (mapContains(player->PlayerTournaments, &tournament_id))
    {
        Map games = mapGet(player->PlayerTournaments, &tournament_id);
        int* game_id = mapGetFirst(games);
        Game game = mapGet(games, game_id);
        while (game != NULL)
        {
            player->games = player->games - 1;
            int points_to_remove = gamePointsAchieved(game, player_id);
            player->points = player->points - points_to_remove;
            if (points_to_remove == 2)
            {
                player->wins = player->wins - 1;
            }
            if (points_to_remove == 0)
            {
                player->losses = player->losses - 1;
            }
            free (game_id);
            game_id = mapGetNext(games);
            game = mapGet(games, game_id);
        }
        mapRemove(player->PlayerTournaments, &tournament_id);
        free(game_id);
    }        
    return MAP_SUCCESS;    
}

void playerUpdateOpponentStatsAfterRemove(Player opponent, int points)
{
    if (points == 2)
    {
        opponent->points = opponent->points + 2;
        opponent->wins = opponent->wins + 1;
        opponent->losses = opponent->losses - 1;
    }
    if (points == 1)
    {
        opponent->points = opponent->points + 1;
        opponent->wins = opponent->wins + 1;
    }
}

MapResult playerLevelCalculate (Player player, int player_id, Map levels, double* array, int array_index)
{
    int draw = player->points - (2*(player->wins));
    double player_level = (double)((6*player->wins) - (10*player->losses) + (2*draw))/(player->games);
    if (mapPut(levels, &player_id, &player_level) != MAP_SUCCESS)
    {
        return MAP_OUT_OF_MEMORY;
    }
    array[array_index] = player_level;
    return MAP_SUCCESS;
}

double calculatePlayerAveragePlayTime(Player player)
{
    return (double) (player->total_time)/(player->games);
}

int* playerFirstTournament(Player player)
{
    return (int*)mapGetFirst(player->PlayerTournaments);
}

int* playerNextTournament(Player player, int* tournament_id)
{
    if (!mapContains(player->PlayerTournaments, tournament_id))
    {
        return (int*)mapGetFirst(player->PlayerTournaments);
    }
    int* current_tournament = (int*)mapGetFirst(player->PlayerTournaments);
    while (current_tournament != NULL && *current_tournament != *tournament_id)
    {
        free(current_tournament);
        current_tournament = (int*)mapGetNext(player->PlayerTournaments);
    }
    current_tournament = (int*)mapGetNext(player->PlayerTournaments);
    return current_tournament;
}

Game playerFirstGameInTournament(Player player ,int* tournament_id)
{
    Map game_map = mapGet(player->PlayerTournaments, tournament_id);
    int* opponent_id = mapGetFirst(game_map);
    Game game = mapGet(game_map, opponent_id);
    free(opponent_id);
    return game;
}

Game playerNextGameInTournament(Player player,int* tournament_id, int opponent_id)
{
    Map tournament_map = mapGet(player->PlayerTournaments, tournament_id);
    int *current_opponent_id = mapGetFirst(tournament_map);
    while (current_opponent_id != NULL && *current_opponent_id != opponent_id)
    {
        free (current_opponent_id);
        current_opponent_id = mapGetNext(tournament_map);
    }
    if (current_opponent_id == NULL)
    {
        return NULL;
    }
    free (current_opponent_id);
    current_opponent_id = mapGetNext(tournament_map);
    if (current_opponent_id == NULL)
    {
        return NULL;
    }
    Game game = mapGet(tournament_map, current_opponent_id);
    free (current_opponent_id);
    return game;
}

void opponentUpdateGameAfterRemovePlayer(Player opponent, int tournament_id, int oponnent_id)
{
    Map games_in_tournaments = mapGet(opponent->PlayerTournaments, &tournament_id);
    Game game = mapGet(games_in_tournaments, &oponnent_id);
    gameRemovePlayer(game, oponnent_id);
}