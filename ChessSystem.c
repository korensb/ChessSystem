#include "./mtm_map/map.h"
#include "tournament.h"
#include "player.h"
#include "chessSystem.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define EMPTY 0
#define EPSILON 0.001
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


/*===============Aux Functions===============================================*/
static bool isTournamentExisted (ChessSystem chess, int tournament_id)
{
    if(mapContains(chess->tournaments_map, &tournament_id))
    {
        return true;
    }
    return false;

}

static void swap(double *xp, double *yp)
{
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}

static int bubble(double a[], int n)
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

static void bubbleSort(double a[], int n)
{
        int not_sorted = 1;
        while ((n > 1) && not_sorted)
        {
            not_sorted = bubble(a, n--);
        }
}

static bool AreSame(double a, double b)
{
    return (fabs(a-b) < EPSILON);
}

static MapResult systemAddPlayerIfNotExist(ChessSystem chess, int player_id)
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
    return MAP_SUCCESS;
}

static bool verifyId (int num)
{
    if (num < 1)
        return false;
    return true;
}

static bool isTournamentActiveById (ChessSystem chess, int tournament_id)
{
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    if (isTournamentActive (tournament))
    {
        return true;
    }
    return false;
}

static MapResult systemRemovePlayerFromTournament(ChessSystem chess, int tournament_id, int player_id)
{
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    return tournamentRemovePlayer(tournament, player_id);
}

static void systemUpdatePlayerStatsAfterRemoveOpponent(ChessSystem chess, int player_id, int points_to_add, int tournament_id)
    {
        Player player = mapGet(chess->players_map, &player_id);
        playerUpdateOpponentStatsAfterRemove(player, points_to_add);
        Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
        tournamentUpdateOpponentScoreAfterRemovePlayer(tournament, player_id, points_to_add);
        return;
    }

/*
void system_calculate_player_wins_losses_in_tournament(ChessSystem chess, int tournament_id, int player_id, int* wins, int* losses)
{
    Player player = mapGet(chess->players_map, &player_id);
    player_wins_losses_in_tournament_calculate(player, player_id, tournament_id, wins, losses);
    return;
}
*/

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
    if (tournament_id < 1){
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->tournaments_map,&tournament_id)){
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!locationValidation (tournament_location)){
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player < 1){
        return CHESS_INVALID_MAX_GAMES;
    }
    if (mapPut(chess->tournaments_map, &tournament_id, tournamentCreate(max_games_per_player, tournament_location)) == MAP_SUCCESS){
        return CHESS_SUCCESS;
    }
    else
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
                         {

                            if (chess == NULL)
                                return CHESS_NULL_ARGUMENT;   

                            if (!verifyId(tournament_id) || !verifyId(first_player) || !verifyId(second_player) || first_player == second_player)
                                return CHESS_INVALID_ID;

                            if (play_time < 0)
                                return CHESS_INVALID_PLAY_TIME;

                            if (!mapContains(chess->tournaments_map, &tournament_id))
                                return CHESS_TOURNAMENT_NOT_EXIST;

                            Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);

                            if (!(isTournamentActive(tournament)))
                                return CHESS_TOURNAMENT_ENDED;

                            if (systemAddPlayerIfNotExist(chess, first_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (systemAddPlayerIfNotExist(chess, second_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }

                            Player player1 = mapGet(chess->players_map, &first_player);
                            Player player2 = mapGet(chess->players_map, &second_player);
                            
                            if (playerAddPlayerToTournamentIfNotExist(player1, tournament_id) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (playerAddPlayerToTournamentIfNotExist(player2, tournament_id) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (tournamentAddPlayerToTournament(tournament, first_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (tournamentAddPlayerToTournament(tournament, second_player) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (isGameExisted(player1, tournament_id, second_player))
                                return CHESS_GAME_ALREADY_EXISTS;
                             
                            if (!playersGamesNumInTournamentValidation (playerGamesInTournamentNum (player1, tournament_id), playerGamesInTournamentNum (player2, tournament_id), tournament, tournament_id))
                                return CHESS_EXCEEDED_GAMES;

                            Game game = gameCreate(first_player,second_player, winner, play_time);

                            if (addGameToTournamentMap(tournament, game) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (playerAddGameToPlayers(player1, player2, game, first_player, second_player, tournament_id, play_time, winner) == MAP_OUT_OF_MEMORY)
                            {
                                chessDestroy(chess);
                                return CHESS_OUT_OF_MEMORY;
                            }
                            if (addGameToTournament(tournament, first_player, second_player, winner, play_time) == MAP_OUT_OF_MEMORY)
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
    if (!verifyId(tournament_id))
        return CHESS_INVALID_ID;
    if (!isTournamentExisted (chess, tournament_id))
        return CHESS_TOURNAMENT_NOT_EXIST;

    int* player_id = mapGetFirst(chess->players_map);
    Player player;

    while (player_id != NULL)
    {
        player = mapGet(chess->players_map, player_id);
        playerRemoveTournament(player, tournament_id, *player_id);
        free(player_id);
        player_id = mapGetNext(chess->players_map);
    }
    mapRemove(chess->tournaments_map, &tournament_id); // verify we delete the games as well
    return CHESS_SUCCESS;
}

/* chessRemovePlayer: VERSION 1- using iterators
VERSION 1- using iterators
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (!verifyId(player_id))
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

        if (isTournamentActive (tournament))
            {
                tournamentRemovePlayer(tournament, player_id);
                tournament_map = mapGet(player->PlayerTournaments, &tournament_id);
                game = mapGet(tournament_map, mapGetFirst(tournament_map));
                // needs iterator that gives all the games in the PlayerTournamentGamesMap (2 lines above)
                while (game != NULL)
                    {
                        points_to_opponent = points_achieved_in_game(game, player_id);
                        gameRemovePlayer(game, player_id);
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
    if (!verifyId(player_id))
        return CHESS_INVALID_ID;
    if (!mapContains(chess->players_map, &player_id))
        return CHESS_PLAYER_NOT_EXIST;

    Player player = mapGet(chess->players_map, &player_id);

    //player_remove_from_system(chess, player, player_id); moved here

    int* tournament_id = playerFirstTournament(player);
    while (tournament_id != NULL)
    {
        if (isTournamentActiveById(chess, *tournament_id))
        {
            systemRemovePlayerFromTournament(chess, *tournament_id, player_id);
            Game game = playerFirstGameInTournament(player,tournament_id);
            int points_to_opponent;
            int opponent_id;
            while (game != NULL)
            {
                points_to_opponent = gamePointsAchieved(game, player_id);
                gameRemovePlayer(game, player_id);
                opponent_id = gameReturnOpponentId(game, player_id);
                if (opponent_id != EMPTY)
                {
                   systemUpdatePlayerStatsAfterRemoveOpponent(chess, opponent_id, points_to_opponent, *tournament_id);
                }
                game = playerNextGameInTournament(player,tournament_id,opponent_id);     
            }
        }
        free(tournament_id);
        tournament_id = playerNextTournament(player, tournament_id);
    }
    mapRemove(chess->players_map, &player_id);
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
    if (!isTournamentExisted (chess, tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);

    if (!isTournamentActive(tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (tournamentNumOfGames(tournament) == 0)
    {
        return CHESS_NO_GAMES;
    }

    endTournament(tournament, tournament_id);

    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL || chess_result == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }

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
    *chess_result = CHESS_SUCCESS;
    return calculatePlayerAveragePlayTime(player);
}



ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL || file == NULL)
        return CHESS_NULL_ARGUMENT;
    if (mapGetSize(chess->players_map) == 0)
    {
        return CHESS_SUCCESS;
    }

    Map levels = createDoublesMap(); //key= ID ,data = level
    int* player_id = mapGetFirst(chess->players_map);
    Player player;
    int remain_players = mapGetSize(chess->players_map);
    double* array = malloc(sizeof(double)*remain_players);
    if (array == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    for (int i = 0; i < remain_players; i++)
    {
        array[i] = 0;
    }
    
    int j = 0;
    while (player_id != NULL)
    {
        player = mapGet(chess->players_map, player_id);
        if (playerLevelCalculate (player, *player_id, levels, array, j) == CHESS_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }
        j++;
        free(player_id);
        player_id = (int*)mapGetNext(chess->players_map);

    }
    bubbleSort(array, remain_players);
    j--;

    while (j >= 0)
    {
        player_id = (int*)mapGetFirst(levels);
        double player_level = *(double*)mapGet(levels, player_id);
        while (!AreSame(player_level,array[j]))
        {
            
            free(player_id);
            player_id = (int*)mapGetNext(levels);
            player_level = *(double*)mapGet(levels, player_id);
        }
      /*  if (player_id == NULL)
        {
            printf("player_id is NULL");
        }
        printf("levels size is %d, player_id is %d, j is %d  " , mapGetSize(levels), *player_id, j);
        */

        fprintf(file,  "%d " , *player_id);
        fprintf(file, "%0.2f\n", array[j]);

        mapRemove(levels, player_id);
        free(player_id);
        j--;
    }
    free(array);
    return CHESS_SUCCESS;
}



ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (chess == NULL || path_file == NULL)
        return CHESS_NULL_ARGUMENT;
    bool no_tournament_ended = true;
    int* tournament_id = mapGetFirst(chess->tournaments_map);
    Tournament tournament = mapGet(chess->tournaments_map, tournament_id);
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
        free (tournament_id);
        tournament_id = mapGetNext(chess->tournaments_map);
        tournament = mapGet(chess->tournaments_map, tournament_id);
    }
    fclose(stream);
    if (no_tournament_ended)
        return CHESS_NO_TOURNAMENTS_ENDED;
    return CHESS_SUCCESS;
}