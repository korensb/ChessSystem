#include "chessSystem.h"
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

static void losses_and_wins_in_tournament_calculator(Map player_games, int player_id, int* wins, int* losses)
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
                *wins = *wins +1;
            if (game->winner == SECOND_PLAYER)
                *losses = *losses +1;
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

static bool is_tournament_existed (ChessSystem chess, int tournament_id)
{
    if(mapContains(chess, tournament_id))
        return true;
    return false;

}

    int bubble(double a[], int n) {
        int i, swapped = 0;
        for (i = 1; i < n; ++i)
            if (a[i - 1] > a[i])
            {
                swap(&a[i], &a[i - 1]);
                swapped = 1;
            }
        return swapped;
    }

    void bubble_sort(double a[], int n)
    {
        int not_sorted = 1;
        while ((n > 1) && not_sorted)
            not_sorted = bubble(a, n--);
    }

/*==============================================================*/
MapResult add_player_to_system_if_not_exist(ChessSystem chess, int player_id)
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

bool verify_id (int num)
{
    if (num < 1)
        return false;
    return true;
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

                            if (add_player_to_system_if_not_exist(chess, first_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (add_player_to_system_if_not_exist(chess, second_player) == MAP_OUT_OF_MEMORY)
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
                             
                            if (!players_games_num_in_tournament_validation (player1, player2, tournament))
                                return CHESS_EXCEEDED_GAMES;


                            Game game = gameCreate(first_player,second_player, winner, play_time);

                            if (add_game_to_tournament_map(tournament, game) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (add_game_to_players(player1, player2, game, first_player, second_player, tournament_id, play_time, winner) == MAP_OUT_OF_MEMORY)
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




ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (player_id < 1)
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

    while (tournament_id != NULL)
    {
        tournament = mapGet(chess->tournaments_map, &tournament_id);        
        // Map attempt = mapGet(chess->tournaments_map, *tournament_id)->standing; will work?
        if (is_tournament_active (tournament))
            {
            tournament_remove_player(tournament, player_id);
            
            tournament_map = mapGet(player->PlayerTournaments, &tournament_id);
            game = mapGet(tournament_map, mapGetFirst(tournament_map));

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
            }
                game = mapGet(tournament_map, mapGetNext(tournament_map));
            }
        }
        tournament_id = mapGetNext(player->PlayerTournaments);
    }

    mapRemove(chess->players_map, player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (tournament_id < 1)
    {
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->tournaments_map, tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);

    if (!tournament->is_active)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (mapGetSize(tournament) == 0){
        return CHESS_NO_GAMES;
    }

    tournament->is_active = false;

    int winner_ID;
    int winner_score = 0;
    int winner_losses = 0;
    int winner_wins = 0;
    int current_id = mapGetFirst(tournament->standing);
    int check_wins;
    int check_losses;
    Player player;

    while (current_id != NULL)
    {
        if (*(int *)mapGet(tournament->standing, current_id) >= winner_score)
        {
            if (*(int *)mapGet(tournament->standing, current_id) > winner_score)
            {
                winner_ID = current_id;
                winner_score = *(int *)mapGet(tournament->standing, current_id);
                losses_and_wins_in_tournament_calculator(mapGet(player->PlayerTournaments, tournament_id), current_id, &winner_wins, &winner_losses);
            }
            else 
            {
                player = mapGet(chess->players_map, current_id);
                losses_and_wins_in_tournament_calculator(mapGet(player->PlayerTournaments, tournament_id), current_id, &check_wins, &check_losses);
                if (check_losses < winner_losses || check_losses == winner_losses && check_wins > winner_wins)
                    {
                        winner_ID = current_id;
                        winner_wins = check_wins;
                        winner_losses = check_losses;
                    }
            }
        }
        current_id = mapGetNext(tournament->standing);
    }
    tournament->tournament_winner = winner_ID;
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
        if (printTournamentStatistics(tournament, path_file) == true){
            no_tournament_ended = false;
        }
        tournament = mapGet(chess->tournaments_map, mapGetNext(chess->tournaments_map));
    }
    fclose(path_file);
    if (no_tournament_ended)
        return CHESS_NO_TOURNAMENTS_ENDED;
    return CHESS_SUCCESS;
};