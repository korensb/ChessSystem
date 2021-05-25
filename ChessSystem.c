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
                *wins++;
            if (game->winner == SECOND_PLAYER)
                *losses++;
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

//checks the location is valid
static bool location_validation(const char* tournament_location)
{ 
    if (tournament_location[0] < 'A' || tournament_location[0] > 'Z' )
        return false;
    int i = 1;
    while (tournament_location[i] != NULL){
        if ((tournament_location[i] >= 'a' && tournament_location[i] <= 'z') || (tournament_location[i] == ' ')){
            i++;
        }
        else{
            return false;
        } 
    }
    return true;
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
bool add_player_to_system_if_not_exist(ChessSystem chess, int player_id) // return true if already exist
{
    if (!mapContains(chess->players_map, first_player))
        {
            if(mapPut(chess->players_map, first_player, playerCreate()) != MAP_SUCCESS)
            {
                chessDestroy(chess);
                return MAP_OUT_OF_MEMORY;
            }
            return false;
        }
        return true;
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

                            if (!(is_tournament_active(tournament))
                                return CHESS_TOURNAMENT_ENDED;

                            add_player_to_system_if_not_exist(chess, first_player);
                            add_player_to_system_if_not_exist(chess, second_player);

                            Player player1 = mapGet(chess->players_map, first_player);
                            Player player2 = mapGet(chess->players_map, second_player);
                            
                            add_player_to_tournament_if_not_exist(player1, tournament_id);
                            add_player_to_tournament_if_not_exist(player2, tournament_id);

                            if (!is_game_existed)
                                return CHESS_GAME_ALREADY_EXISTS;
                             
                            if (!players_games_num_in_tournament_validation (player1, player2, tournament)
                                return CHESS_EXCEEDED_GAMES;


                            Game game = gameCreate(first_player,second_player, winner, play_time);

                            if (add_game_to_tournament_map(Tournament tournament, Game game) == MAP_OUT_OF_MEMORY)
                                return CHESS_OUT_OF_MEMORY;


                            mapPut(player1_games, second_player, mapGet(tournament->gamesMap, tournament->num_games));
                            mapPut(player2_games, first_player, mapGet(tournament->gamesMap, tournament->num_games));
                            player1->total_time = (player1->total_time) + play_time;
                            player2->total_time = (player2->total_time) + play_time;
                            tournament->total_time = (tournament->total_time) + play_time;
                            if (tournament->longest_time < play_time)
                                tournament->longest_time = play_time;
                            if (winner == FIRST_PLAYER)
                            {
                                player1->points = player1->points + 2;
                                player1->wins++;
                                player2->losses++;
                                int points = mapGet(tournament->standing, first_player)+2;
                                mapPut(tournament->standing, first_player, points);
                            }
                            else if (winner == SECOND_PLAYER)
                            {
                                 player2->points = player2->points + 2;
                                 player2->wins++;
                                 player1->losses++;
                                 int points = mapGet(tournament->standing, second_player)+2;
                                mapPut(tournament->standing, second_player, points);
                            }            
                            else
                            {
                                player1->points++;
                                player2->points++;
                            }

                            player1->games++;
                            player2->games++;
                            return CHESS_SUCCESS;
                         }

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (tournament_id < 1)
        return CHESS_INVALID_ID;
    if (!mapContains(chess, tournament_id))
        return CHESS_TOURNAMENT_NOT_EXIST;
    int player_id = mapGetFirst(chess->players_map);
    Player player = mapGet(chess->players_map, player_id);
    while (player != NULL)
    {
        if (mapContains(player->PlayerTournaments, tournament_id))
        {
            Map games = mapGet(player->PlayerTournaments, tournament_id);
            Game game = mapGet(games, mapGetFirst(games));
            while (game != NULL)
            {
                player->games--;
                if(game->winner == DRAW)
                player->points--;
                if (game->winner == FIRST_PLAYER && player_id == game->first_player || game->winner == SECOND_PLAYER && player_id == game->second_player)
            {
                player->wins--;
                player->points = player->points-2;
            }
                game = mapGet(games, mapGetNext(games));
            }
        }
        player = mapGet(chess->players_map, mapGetNext(chess->players_map));
        mapRemove(player->PlayerTournaments, tournament_id); //!! verify we did the correct destroy function
        mapRemove(chess->tournaments_map, tournament_id);
    }
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
    int* tournament_id = mapGetFirst(player->PlayerTournaments);
    Map tournament_map;
    Game game;
    Player opponent;
    Tournament tour_temp;

    while (*tournament_id != NULL)
    {
        tour_temp = mapGet(chess->tournaments_map, *tournament_id);        
        // Map attempt = mapGet(chess->tournaments_map, *tournament_id)->standing; will work?
        if (tour_temp->is_active)
        {
        mapRemove(tour_temp->standing, player_id);

        tournament_map = mapGet(player->PlayerTournaments, *tournament_id);
        game = mapGet(tournament_map, mapGetFirst(tournament_map));

        while (game != NULL)
        {
            if(player_id == game->first_player)
            {
                game->first_player = EMPTY;
                if (game->second_player != EMPTY)
                {
                     int* opponent_score = mapGet(tour_temp->standing, game->second_player);
                if(game->winner == FIRST_PLAYER)
                {
                    game->winner = SECOND_PLAYER;
                    opponent = mapGet(chess->players_map, game->second_player);
                    opponent->points = opponent->points + 2;
                    opponent->wins++;
                    opponent->losses--;
                    *opponent_score = *opponent_score + 2;
                    //mapPut(chess->players_map, game->second_player, temp);mapGet returns pointers so it should modify the data itself.
                }                  
                if(game->winner == DRAW)
                {
                    game->winner = SECOND_PLAYER;
                    opponent = mapGet(chess->players_map, game->second_player);
                    opponent->points = opponent->points + 1;
                    opponent->wins++;
                    *opponent_score = *opponent_score + 1;
                    //mapPut(chess->players_map, game->first_player, temp); mapGet returns pointers so it should modify the data itself.
                }
                } 
            }
            else if (player_id == game->second_player)
            {
                game->second_player = EMPTY;
                if (game->first_player != EMPTY)
                {
                     int* opponent_score = mapGet(tour_temp->standing, game->first_player);
                if(game->winner == SECOND_PLAYER)
                {
                    game->winner = FIRST_PLAYER;
                    opponent = mapGet(chess->players_map, game->first_player);
                    opponent->points = opponent->points + 2;
                    opponent->wins++;
                    opponent->losses--;
                    *opponent_score = *opponent_score + 2;
                    // mapPut(chess->players_map, game->first_player, temp); mapGet returns pointers so it should modify the data itself.
                }                  
                if(game->winner == DRAW)
                {
                    game->winner = FIRST_PLAYER;
                    opponent = mapGet(chess->players_map, game->first_player);
                    opponent->points = opponent->points + 1;
                    opponent->wins++;
                    *opponent_score = *opponent_score + 1;
                    //mapPut(chess->players_map, game->first_player, temp);mapGet returns pointers so it should modify the data itself.
                }
                }
               
            }
            game = mapGet(tournament_map, mapGetNext(tournament_map));
        }
        }
        *tournament_id = mapGetNext(player->PlayerTournaments);
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
        return CHESS_NULL_ARGUMENT;
    if (player_id < 1)
    {
        *chess_result = CHESS_INVALID_ID;
        return;
    }
    if (!mapContains(chess->players_map, player_id))
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return;
    }

    Player player = mapGet(chess->players_map, player_id);
    *chess_result = MAP_SUCCESS;
    return (double)player->total_time/player->games;
}

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL || file == NULL)
        return CHESS_NULL_ARGUMENT;

    FILE* stream = fopen(file, "a");
            if (stream == NULL)
            {
                return CHESS_NULL_ARGUMENT;
            }
    Map levels = createDoublesMap(); //key= ID ,data = level
    int player_id = mapGetFirst(chess->players_map);
    Player player;
    double player_level;
    int draw;
    int remain_players = mapGetSize(chess->players_map);
    double array [remain_players] = { 0 };
    int i = 0;

    while (player_id != NULL)
    {
        player = mapGet(chess->players_map, player_id);
        draw = player->points - (2*player->wins);
        player_level = (double)((6*player->wins) - (10*player->losses) + (2*draw))/player->games;
        mapPut(levels, player_id, player_level);
        array[i] = player_level;
        i++;
        player_id = mapGetNext(chess->players_map);
    }

    bubble_sort(array, remain_players);

    while (i >= 0)
    {
        player_id = mapGetFirst(chess->players_map);
        while (mapGet(levels, player_id) != array[i])
        {
            player_id = mapGetNext(chess->players_map);
        }
        fprintf(stream,  "%d\n" , player_id);
        fprintf(stream, "%f\n", array[i]);
        mapRemove(levels, player_id);
        i--;
    }
    fclose(stream);
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (chess == NULL || path_file == NULL)
        return CHESS_NULL_ARGUMENT;
    bool no_tournament_ended = true;
    Tournament tournament = mapGet(chess->tournaments_map, mapGetFirst(chess->tournaments_map));
    while (tournament != NULL)
    {
        if (!tournament->is_active)
        {
            no_tournament_ended = false;
            float average_time = (float)tournament->total_time/mapGetSize(tournament->gamesMap);
            FILE* stream = fopen(path_file, "a");
            if (stream == NULL)
            {
                return CHESS_NULL_ARGUMENT;
            }
            fprintf(stream,  "%d\n" , tournament->tournament_winner);
            fprintf(stream,  "%d\n" , tournament->longest_time);
            fprintf(stream, "%f\n", average_time);
            fprintf(stream, "%s\n" , tournament->tournament_location);
            fprintf(stream,  "%d\n" , mapGetSize(tournament->gamesMap));
            fprintf(stream,  "%d\n" , mapGetSize(tournament->standing));
            fclose(stream);

                /*
                Print:
                winner = tournament->tournament_winner
                longest game time = tournament->longest_time
                average game time = (double)tournament->total_time/mapGetSize(tournament->gamesMap)
                location = tournament->tournament_location
                number of games = mapGetSize(tournament->gamesMap)
                number of players = mapGetSize(tournament->standing)
                */
        }
        tournament = mapGet(chess->tournaments_map, mapGetNext(chess->tournaments_map));
    }
    if (no_tournament_ended)
        return CHESS_NO_TOURNAMENTS_ENDED;
    return CHESS_SUCCESS;
};