#include "tournament.h"
#include "player.h"
#include "chessSystem.h"
#include "game.h"
#include "./mtm_map/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EMPTY 0
// #define NDEBUG

/*
Maps list:
Tournaments_map- key=tournament ID, data=Tournament
Players_map- key=player ID, data=Player
gamesMap- key= game_id (counter of each tournament), data = Game
standing- key= player ID, data = int player score
PlayerTournaments- key= tournament ID, data = GamesPointersMap
GamesPointersMap- key= opponent ID, data- pointer to Game
*/

/*
==========================================================================================
Declarations:
==========================================================================================
*/

struct chess_system_t
{
    Map tournaments_map;
    Map players_map;
};


/*==============================================================*/

static bool is_tournament_existed (ChessSystem chess, int tournament_id)
{
    if(mapContains(chess, tournament_id))
    {
        return true;
    }
    return false;

}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int bubble(double a[], int n)
{
        int i, swapped = 0;
        for (i = 1; i < n; ++i){
            if (a[i - 1] > a[i])
            {
                swap(&a[i], &a[i - 1]);
                swapped = 1;
            }
        }
        return swapped;
}

void bubble_sort(double a[], int n)
{
        int not_sorted = 1;
        while ((n > 1) && not_sorted)
        {
            not_sorted = bubble(a, n--);
        }
}

/*===============Aux Functions===============================================*/
MapResult system_add_player_if_not_exist(ChessSystem chess, int player_id)
{
    if (!mapContains(chess->players_map, &player_id))
        {
            if(mapPut(chess->players_map, &player_id, playerCreate()) != MAP_SUCCESS)
            {
                chessDestroy(chess);
                return MAP_OUT_OF_MEMORY;
            }
            return MAP_SUCCESS;
        }
}

static bool verify_id (int num)
{
    if (num < 1)
        return false;
    return true;
}

bool is_tournament_active_by_id (ChessSystem chess, int tournament_id)
{
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    if (tournament->is_active == true)
        return true;
    return false;
}

MapResult system_remove_player_from_tournament(ChessSystem chess, int tournament_id, int player_id)
{
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    return tournament_remove_player(tournament, player_id);
}

void system_update_player_stats_after_remove_opponent(int player_id, int points_to_add, int tournament_id)
    {
        Player player = mapGet(chess->players_map, &player_id);
        player_update_opponent_stats_after_remove(player, points_to_add);
        Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
        tournament_update_opponent_score_after_remove_player(tournament, player_id, points_to_add);
        return;
    }

void system_calculate_player_wins_losses_in_tournament(ChessSystem chess, int tournament_id, int player_id, int* wins, int* losses)
{
    Player player = mapGet(chess->players_map, &player_id);
    player_wins_losses_in_tournament_calculate(player, player_id, tournament_id, wins, losses);
    return;
}

/*==============================================================*/

ChessSystem chessCreate()
{
	ChessSystem newSystem = malloc(sizeof(*newSystem));
    	if (newSystem == NULL)
    {
		return NULL;
	}
    newSystem->tournaments_map = createTournamentsMap();
    if (newSystem->tournaments_map == NULL){
        free(newSystem);
        return NULL;
    }
    newSystem->players_map = createPlayersMap();
    if (newSystem->players_map == NULL){
        free(newSystem);
        mapDestroy(newSystem->tournaments_map);
        return NULL;
    }
	return newSystem;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
        return;
    mapDestroy(chess->tournaments_map);
    mapDestroy(chess->players_map);
    free(chess);
    return;
}

ChessResult chessAddTournament (ChessSystem chess, int
tournament_id, int max_games_per_player, const char*
tournament_location)
{
if (chess == NULL || tournament_location == NULL)
    return CHESS_NULL_ARGUMENT;   
if (tournament_id < 1)
    return CHESS_INVALID_ID;
if (mapContains(chess->tournaments_map,tournament_id))
    return CHESS_TOURNAMENT_ALREADY_EXISTS;
if (!location_validation)
    return CHESS_INVALID_LOCATION;
if(max_games_per_player < 1){
    return CHESS_INVALID_MAX_GAMES;
}
if (mapPut(chess->tournaments_map, tournament_id, tournamentCreate(max_games_per_player, tournament_location)) == MAP_SUCCESS)
    return CHESS_SUCCESS;
else
    chessDestroy(chess);
    return CHESS_OUT_OF_MEMORY;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
                         {

                            if (chess == NULL)
                                return CHESS_NULL_ARGUMENT;   

                            if (!verify_id(tournament_id) || !verify_id(first_player) || !verify_id(second_player) || first_player == second_player)
                                return CHESS_INVALID_ID;

                            if (play_time < 0)
                                return CHESS_INVALID_PLAY_TIME;

                            if (!mapContains(chess->tournaments_map, tournament_id))
                                return CHESS_TOURNAMENT_NOT_EXIST;

                            Tournament tournament = mapGet(chess->tournaments_map, tournament_id);

                            if (!(is_tournament_active(tournament)))
                                return CHESS_TOURNAMENT_ENDED;

                            if (system_add_player_if_not_exist(chess, first_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (system_add_player_if_not_exist(chess, second_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }

                            Player player1 = mapGet(chess->players_map, first_player);
                            Player player2 = mapGet(chess->players_map, second_player);
                            
                            if (player_add_player_to_tournament_if_not_exist(player1, tournament,tournament_id, first_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (player_add_player_to_tournament_if_not_exist(player2, tournament,tournament_id, second_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }

                            if (!is_game_existed(player1, tournament_id, second_player))
                                return CHESS_GAME_ALREADY_EXISTS;
                             
                            if (!players_games_num_in_tournament_validation (player1, player2, tournament, tournament_id))
                                return CHESS_EXCEEDED_GAMES;


                            Game game = gameCreate(first_player,second_player, winner, play_time);

                            if (add_game_to_tournament_map(tournament, game) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (player_add_game_to_players(player1, player2, game, first_player, second_player, tournament_id, play_time, winner) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (add_game_to_tournament(tournament, first_player, second_player, winner, play_time) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }

                            return CHESS_SUCCESS;

                        }

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (!verify_id(tournament_id))
        return CHESS_INVALID_ID;
    if (!is_tournament_existed (chess, tournament_id))
        return CHESS_TOURNAMENT_NOT_EXIST;

    int player_id = mapGetFirst(chess->players_map);
    Player player;

    while (player_id != NULL)
    {
        player = mapGet(chess->players_map, player_id);
        player_remove_tournament(player, tournament_id, player_id);
        player_id = mapGetNext(chess->players_map);
    }
    mapRemove(chess->tournaments_map, tournament_id); // verify we delete the games as well
    return CHESS_SUCCESS;
}

/* chessRemovePlayer: VERSION 1- using iterators
VERSION 1- using iterators
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (!verify_id(player_id))
        return CHESS_INVALID_ID;
    if (!mapContains(chess->players_map, player_id))
        return CHESS_PLAYER_NOT_EXIST;


    Player player = mapGet(chess->players_map, player_id);
    int tournament_id = mapGetFirst(player->PlayerTournaments);
    Map tournament_map;
    Game game;
    Player opponent;
    Tournament tournament;
    int points_to_opponent;
    int opponent_id;

    // Tachles- needs iterator that gives the tournament_id of all the tournaments of this player.
    while (tournament_id != NULL)
    {
        tournament = mapGet(chess->tournaments_map, &tournament_id);

        if (is_tournament_active (tournament))
            {
                tournament_remove_player(tournament, player_id);
                tournament_map = mapGet(player->PlayerTournaments, &tournament_id);
                game = mapGet(tournament_map, mapGetFirst(tournament_map));
                // needs iterator that gives all the games in the PlayerTournamentGamesMap (2 lines above)
                while (game != NULL)
                    {
                        points_to_opponent = points_achieved_in_game(game, player_id);
                        game_remove_player(game, player_id);
                        opponent_id = return_opponent_id(game, player_id);
                        if (opponent_id != EMPTY)
                            {
                                opponent = mapGet(chess->players_map, opponent_id);
                                update_opponent_stats_after_remove_player(opponent, points_to_opponent);
                                update_opponent_score_in_tournament_after_remove_player(tournament, opponent_id, points_to_opponent);
                            }     
                        game = mapGet(tournament_map, mapGetNext(tournament_map));
                    }
            }
            tournament_id = mapGetNext(player->PlayerTournaments);
    }

    mapRemove(chess->players_map, player_id);
    return CHESS_SUCCESS;
}
*/

/* VERSION 2- without iterators (more functions) */
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (!verify_id(player_id))
        return CHESS_INVALID_ID;
    if (!mapContains(chess->players_map, player_id))
        return CHESS_PLAYER_NOT_EXIST;

    Player player = mapGet(chess->players_map, player_id);
    player_remove_from_system(chess, player, player_id);

    mapRemove(chess->players_map, player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }        
    if (tournament_id < 1)
    {
        return CHESS_INVALID_ID;
    }
    if (!is_tournament_existed (chess, tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);

    if (!is_tournament_active(tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (tournament_num_of_games(tournament) == 0)
    {
        return CHESS_NO_GAMES;
    }

    end_tournament(chess, tournament, tournament_id);

    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL || chess_result == NULL)
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    if (player_id < 1)
    {
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    if (!mapContains(chess->players_map, &player_id))
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }

    Player player = mapGet(chess->players_map, &player_id);
    *chess_result = MAP_SUCCESS;
    return calculatePlayerAveragePlayTime(player);
}

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL || file == NULL)
        return CHESS_NULL_ARGUMENT;
    if (mapGetSize(chess->players_map) == 0){
        return MAP_SUCCESS;
    }
    FILE* stream = fopen(file, "a");
    if (stream == NULL){
        return CHESS_SAVE_FAILURE;
    }
    Map levels = createDoublesMap(); //key= ID ,data = level
    int* player_id = (int*)mapGetFirst(chess->players_map);
    Player player;
    int remain_players = mapGetSize(chess->players_map);
    double *array = malloc(sizeof(double)*remain_players));
    if (array == NULL){
        fclose(file);
        return MAP_OUT_OF_MEMORY;
    }
    for (int i = 0; i<remain_players; i++){
        array[i] = 0;
    }
    int j = 0;
    while (player_id != NULL)
    {
        player = mapGet(chess->players_map, player_id);
        if (playerLevelCalculate (player, *player_id, levels, array, j) == MAP_OUT_OF_MEMORY){
            fclose(file);
            return MAP_OUT_OF_MEMORY;
        }
        j++;
        player_id = (int*)mapGetNext(chess->players_map);
    }
    j--;
    bubble_sort(array, remain_players);
    while (j >= 0)
    {
        player_id = (int*)mapGetFirst(chess->players_map);
        while (!mapContains(levels, player_id) || *(double*)mapGet(levels, player_id) != array[j])
        {
            player_id = (int*)mapGetNext(chess->players_map);
        }
        fprintf(stream,  "%d\n" , player_id);
        fprintf(stream, "%f\n", array[j]);
        mapRemove(levels, player_id);
        j--;
    }
    fclose(stream);
    return MAP_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (chess == NULL || path_file == NULL)
        return CHESS_NULL_ARGUMENT;
    bool no_tournament_ended = true;
    Tournament tournament = mapGet(chess->tournaments_map, mapGetFirst(chess->tournaments_map));
    FILE* stream = fopen(path_file, "a");
            if (stream == NULL)
            {
                return CHESS_SAVE_FAILURE;
            }
    while (tournament != NULL)
    {
        if (printTournamentStatistics(tournament, stream) == true)
        {
            no_tournament_ended = false;
        }
        tournament = mapGet(chess->tournaments_map, mapGetNext(chess->tournaments_map));
    }
    fclose(path_file);
    if (no_tournament_ended)
        return CHESS_NO_TOURNAMENTS_ENDED;
    return CHESS_SUCCESS;
}