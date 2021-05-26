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

void player_wins_losses_in_tournament_calculate(Player player, int tournament_id, int* wins, int* losses)
{
    Map games_in_tournament = mapGet(player->PlayerTournaments, tournament_id);
    losses_and_wins_in_tournament_calculator(games_in_tournament, player_id, wins, losses);
    return;
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

MapResult player_remove_tournament (Player player, int tournament_id, int player_id)
{
    if (mapContains(player->PlayerTournaments, tournament_id))
        {
            Map games = mapGet(player->PlayerTournaments, tournament_id);
            Game game = mapGet(games, mapGetFirst(games));
            while (game != NULL)
            {
                player->games--;
                int points_to_remove = points_achieved_in_game(game, player_id);
                player->points = player->points - points_to_remove;
                if (points_to_remove == 2)
                    player->wins--;
                if (points_to_remove == 0)
                    player->losses--;

                game = mapGet(games, mapGetNext(games));
            }
            mapRemove(player->PlayerTournaments, tournament_id); //!! verify we did the correct destroy function- need to destroy the map but not the games in the map. (createPlayerTournamentsMap)
        }        
    return MAP_SUCCESS;    
}

update_opponent_stats_after_remove_player(player opponent, int points)
{
    if (points == 2)
    {
        opponent->points = opponent->points + 2;
        opponent->wins++;
        opponent->losses--;
    }
    if (points == 1)
    {
        opponent->points = opponent->points + 1;
        opponent->wins++;
    }
}

ChessResult playerLevelCalculate (Player player, int player_id, Map levels, double* array, int array_index){
    int draw = player->points - (2*(player->wins));
    double player_level = (double)((6*player->wins) - (10*player->losses) + (2*draw))/(player->games);
    if (mapPut(levels, &player_id, &player_level) != MAP_SUCCESS){
        return MAP_OUT_OF_MEMORY;
    }
    array[array_index] = player_level;
    return MAP_SUCCESS;
}

double calculatePlayerAveragePlayTime(Player player)
{
    return (double) (player->total_time)/(player->games);
}

player_remove_from_system(ChessSystem chess, Player player)
{
    int tournament_id = mapGetFirst(player->PlayerTournaments);
    while (tournament_id != NULL)
    {
        if (is_tournament_active_by_id(chess, tournament_id))
            {
                system_remove_player_from_tournament(chess, tournament_id, player_id);

                tournament_map = mapGet(player->PlayerTournaments, &tournament_id);
                game = mapGet(tournament_map, mapGetFirst(tournament_map));

                while (game != NULL)
                    {
                        points_to_opponent = points_achieved_in_game(game, player_id);
                        game_remove_player(game, player_id);
                        opponent_id = return_opponent_id(game, player_id);
                        if (opponent_id != EMPTY)
                            {
                                system_update_player_stats_after_remove_opponent(opponent_id, points_to_opponent, tournament_id);
                            }     
                        game = mapGet(tournament_map, mapGetNext(tournament_map));
                    }
            }
            tournament_id = mapGetNext(player->PlayerTournaments);
    }
}

