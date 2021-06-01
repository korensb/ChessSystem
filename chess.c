#include "map.h"
#include "game.h"
#include "tournament.h"
#include "player.h"
#include "chessSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define EMPTY 0
#define EPSILON 0.001

/*
Maps list:
Tournaments_map- key=tournament ID, data=Tournament
standing- key= player ID, data = int player score
gamesMap- key = opponent_id, data = Game

Players_map- key =player ID, data=Player
PlayerTournaments- key = tournament ID, data = GamesMap
GamesMap- key = opponent ID, data = Game

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
static bool checkIfTournamentExisted (ChessSystem chess, int tournament_id)
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
//checks if 2 doubles are the same number
static bool AreSame(double a, double b)
{
    return (fabs(a-b) < EPSILON);
}

//adds player to player_map if he deosnt exist there
static MapResult systemAddPlayerIfNotExist(ChessSystem chess, int player_id)
{
    if (!mapContains(chess->players_map, &player_id))
        {
            Player player = playerCreate();
            if(mapPut(chess->players_map, &player_id, player) != MAP_SUCCESS)
            {
                chessDestroy(chess);
                return MAP_OUT_OF_MEMORY;
            }
            free(player);
        }
    return MAP_SUCCESS;
}

// checks the ID is valid
static bool verifyId (int num)
{
    if (num < 1)
        return false;
    return true;
}

// removes the data from playerTournaments map
static MapResult systemRemovePlayerFromTournament(ChessSystem chess, int tournament_id, int player_id)
{
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    return tournamentRemovePlayer(tournament, player_id);
}

// updates a player stats after his opponent has been removed
static void systemUpdatePlayerStatsAfterRemoveOpponent(ChessSystem chess, int player_id, int points_to_add, int tournament_id)
    {
        Player player = mapGet(chess->players_map, &player_id);
        playerUpdateOpponentStatsAfterRemove(player, points_to_add);
        Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
        tournamentUpdateOpponentScoreAfterRemovePlayer(tournament, player_id, points_to_add);
        return;
    }

/*=============MAIN FUNCTIONS===========================================*/

ChessSystem chessCreate()
{
	ChessSystem newSystem = malloc(sizeof(*newSystem));
    if (newSystem == NULL)
    {
		return NULL;
	}
    newSystem->tournaments_map = createTournamentsMap();
    if (newSystem->tournaments_map == NULL)
    {
        free(newSystem);
        return NULL;
    }
    newSystem->players_map = createPlayersMap();
    if (newSystem->players_map == NULL)
    {
        mapDestroy(newSystem->tournaments_map);
        free(newSystem);
        return NULL;
    }
	return newSystem;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }
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
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id < 1)
    {
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->tournaments_map,&tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!locationValidation (tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player < 1)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament tournament = tournamentCreate(max_games_per_player, tournament_location);
    if (mapPut(chess->tournaments_map, &tournament_id, tournament) == MAP_SUCCESS)
    {
        free(tournament);
        return CHESS_SUCCESS;
    }
    else
    {
        free(tournament);
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
                        {
                            if (chess == NULL)
                            {
                                return CHESS_NULL_ARGUMENT;   
                            }
                            if (!verifyId(tournament_id) || !verifyId(first_player) || !verifyId(second_player) || first_player == second_player)
                            {
                                return CHESS_INVALID_ID;
                            }
                            if (play_time < 0)
                            {
                                return CHESS_INVALID_PLAY_TIME;
                            }
                            if (!mapContains(chess->tournaments_map, &tournament_id))
                            {
                                return CHESS_TOURNAMENT_NOT_EXIST;
                            }
                            Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
                            if (!(isTournamentActive(tournament)))
                            {
                                return CHESS_TOURNAMENT_ENDED;
                            }
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
                            {
                                return CHESS_GAME_ALREADY_EXISTS;
                            }
                            if (!playersGamesNumInTournamentValidation (playerGamesInTournamentNum (player1, tournament_id), playerGamesInTournamentNum (player2, tournament_id), tournament, tournament_id))
                            {
                                return CHESS_EXCEEDED_GAMES;
                            }
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
                            gameDestroy(game);
                            return CHESS_SUCCESS;
                        }

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
        return CHESS_NULL_ARGUMENT;
    if (!verifyId(tournament_id))
        return CHESS_INVALID_ID;
    if (!checkIfTournamentExisted (chess, tournament_id))
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
        Tournament tournament = mapGet(chess->tournaments_map, tournament_id);
        if (isTournamentActive(tournament))
        {
            Game game = playerFirstGameInTournament(player,tournament_id);
            int points_to_opponent;
            int opponent_id;
            while (game != NULL)
            {
                points_to_opponent = gamePointsAchieved(game, player_id);
                opponent_id = gameReturnOpponentId(game, player_id);
                if (opponent_id != EMPTY)
                {
                    Player opponent = mapGet(chess->players_map, &opponent_id);
                    int game_serial = getGameSerial(game);
                    tournamentUpdateGameAfterRemovePlayer(tournament, player_id, game_serial);
                    opponentUpdateGameAfterRemovePlayer(opponent, *tournament_id ,player_id);
                    systemUpdatePlayerStatsAfterRemoveOpponent(chess, opponent_id, points_to_opponent, *tournament_id);
                }
                game = playerNextGameInTournament(player,tournament_id,opponent_id);     
            }
            systemRemovePlayerFromTournament(chess, *tournament_id, player_id);
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
    if (!checkIfTournamentExisted (chess, tournament_id))
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
        if (playerLevelCalculate (player, *player_id, levels, array, j) == MAP_OUT_OF_MEMORY)
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
        fprintf(file,  "%d " , *player_id);
        fprintf(file, "%0.2f\n", array[j]);
        mapRemove(levels, player_id);
        free(player_id);
        j--;
    }
    free(array);
    mapDestroy(levels);
    return CHESS_SUCCESS;
}



ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (chess == NULL || path_file == NULL)
        return CHESS_NULL_ARGUMENT;
    bool no_tournament_ended = true;
    int* tournament_id = mapGetFirst(chess->tournaments_map);
    Tournament tournament = mapGet(chess->tournaments_map, tournament_id);
    FILE* stream = fopen(path_file, "w");
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