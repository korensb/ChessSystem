#include <gtest/gtest.h>

extern "C" {
    #include <string.h>

    #include "../chess/chessSystem.h"
}

// from chessSystemExampleTests

TEST(Chess, addTournament)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddTournament(chess, 2, 5, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddTournament(chess, 1, 10, "Paris") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    chessDestroy(chess);
}

TEST(Chess, testChessRemoveTournament)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TRUE(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TRUE(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
}

TEST(Chess, testChessAddGame)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TRUE(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);

    chessDestroy(chess);
}


TEST(Chess, testChessPrintLevelsAndTournamentStatistics)
{
    // NOTE: make sure you have this file in this directory
    char path[] = "./player_levels.txt";
    FILE* file_levels = fopen(path, "w");

    ASSERT_TRUE(file_levels != NULL);
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 3, 2, SECOND_PLAYER, 3000) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 4, 1, SECOND_PLAYER, 1000) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, 3500) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 3, 4, DRAW, 400) == CHESS_SUCCESS);
    ASSERT_TRUE(chessEndTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TRUE(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    ASSERT_TRUE(chessSaveTournamentStatistics(chess, path) == CHESS_SUCCESS);
    chessDestroy(chess);
    fclose(file_levels);
}

TEST(Chess, TestTournamentInvalidParameters)
{
    int valid_id = 1, invalid_id = 0, valid_max_games = 1, invalid_max_games = 0;
    char invalid_location[] = "pAris";
    char valid_location[] = "Paris";

    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(NULL, valid_id, valid_max_games, valid_location) == CHESS_NULL_ARGUMENT);
    ASSERT_TRUE(chessAddTournament(chess, invalid_id, valid_max_games, valid_location) == CHESS_INVALID_ID);
    ASSERT_TRUE(chessAddTournament(chess, valid_id, invalid_max_games, valid_location) == CHESS_INVALID_MAX_GAMES);
    ASSERT_TRUE(chessAddTournament(chess, valid_id, valid_max_games, invalid_location) == CHESS_INVALID_LOCATION);

    chessDestroy(chess);
}

TEST(Chess, TestMaxGames)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 60) == CHESS_EXCEEDED_GAMES);
    chessDestroy(chess);
}



TEST(Chess, TestGameAlreadyExists)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TRUE(chessAddGame(chess, 1, 2, 1, FIRST_PLAYER, 60) == CHESS_GAME_ALREADY_EXISTS);
    chessDestroy(chess);
}

TEST(Chess, TestEndRemovedTournament)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TRUE(chessEndTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    chessDestroy(chess);
}

TEST(Chess, TestEndedTournament)
{
    char path[] = "player_levels.txt";
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 1, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_SUCCESS);
    ASSERT_TRUE(chessSaveTournamentStatistics(chess, path) == CHESS_NO_TOURNAMENTS_ENDED);
    ASSERT_TRUE(chessEndTournament(chess, 1) == CHESS_SUCCESS);

    // end non existing tournament
    ASSERT_TRUE(chessEndTournament(chess, 2)== CHESS_TOURNAMENT_NOT_EXIST);

    // add to ended tournament
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_TOURNAMENT_ENDED);

    // end invalid tournament
    ASSERT_TRUE(chessEndTournament(chess, 0) == CHESS_INVALID_ID);

    // end ended tournament
    ASSERT_TRUE(chessEndTournament(chess, 1) == CHESS_TOURNAMENT_ENDED);

    // end empty tournament
    ASSERT_TRUE(chessAddTournament(chess, 2, 1, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessEndTournament(chess, 2) == CHESS_NO_GAMES);

    chessDestroy(chess);
}

TEST(Chess, TestAvgGameTime)
{
    ChessSystem chess = chessCreate();
    ASSERT_TRUE(chessAddTournament(chess, 1, 5, "London") == CHESS_SUCCESS);
    ASSERT_TRUE(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 60) == CHESS_SUCCESS);

    ChessResult result;
    double first_avg = chessCalculateAveragePlayTime(chess, 1, &result);
    ASSERT_TRUE(result == CHESS_SUCCESS);
    double second_avg = chessCalculateAveragePlayTime(chess, 2, &result);
    ASSERT_TRUE(result == CHESS_SUCCESS);
    ASSERT_TRUE(first_avg == second_avg && first_avg == 60);

    ASSERT_TRUE(chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 40) == CHESS_SUCCESS);
    first_avg = chessCalculateAveragePlayTime(chess, 1, &result);
    ASSERT_TRUE(result == CHESS_SUCCESS);
    ASSERT_TRUE(first_avg != second_avg && first_avg == 50);

    chessDestroy(chess);
}
