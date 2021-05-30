#include <gtest/gtest.h>

extern "C" {
    #include "../chess/chessSystem.h"

    #include "../chess/mtm_map/map.h"
}
TEST(Chess, CreateDestroy) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    chessDestroy(chess);
}

TEST(Chess, TestAddTournament) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 1, "A") );
    ASSERT_EQ(CHESS_INVALID_ID, chessAddTournament(chess, -1, -1, "a!"));
    ASSERT_EQ(CHESS_TOURNAMENT_ALREADY_EXISTS, chessAddTournament(chess, 1, -1, "a !"));
    ASSERT_EQ(CHESS_INVALID_LOCATION, chessAddTournament(chess, 2, -1, "a !"));
    ASSERT_EQ(CHESS_INVALID_MAX_GAMES, chessAddTournament(chess, 2, -1, "Abc"));
    chessDestroy(chess);
}

TEST(Chess, TestAddGame) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_INVALID_ID, chessAddGame(chess, -1, 1, 2, FIRST_PLAYER, -7));
    ASSERT_EQ(CHESS_INVALID_ID, chessAddGame(chess, 3, 1, 1, FIRST_PLAYER, -7));//This one is for the same id for both players
    ASSERT_EQ(CHESS_TOURNAMENT_NOT_EXIST, chessAddGame(chess, 2, 1, 2, FIRST_PLAYER, -7));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_GAME_ALREADY_EXISTS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, -7));
    ASSERT_EQ(CHESS_INVALID_PLAY_TIME, chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, -7));
    ASSERT_EQ(CHESS_SUCCESS, chessRemovePlayer(chess, 1));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 2, 3, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_EXCEEDED_GAMES, chessAddGame(chess, 1, 2, 4, FIRST_PLAYER, -7));
    chessDestroy(chess);
}

TEST(Chess, TestExceededGames) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_GAME_ALREADY_EXISTS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 3, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_EXCEEDED_GAMES, chessAddGame(chess, 1, 1, 4, FIRST_PLAYER, 7));
    chessDestroy(chess);
}

TEST(Chess, TestRemoveTournament) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_INVALID_ID, chessRemoveTournament(chess, -2));
    ASSERT_EQ(CHESS_TOURNAMENT_NOT_EXIST, chessRemoveTournament(chess, 1));
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_SUCCESS, chessRemoveTournament(chess, 1));
    chessDestroy(chess);
}

TEST(Chess, TestRemovePlayer) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_INVALID_ID, chessRemovePlayer(chess, -3) );
    ASSERT_EQ(CHESS_PLAYER_NOT_EXIST, chessRemovePlayer(chess, 1) );
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A") );
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7) );
    ASSERT_EQ(CHESS_SUCCESS, chessRemovePlayer(chess, 1));
    chessDestroy(chess);
}

TEST(Chess, TestEndTournament) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_INVALID_ID, chessEndTournament(chess, 0));
    ASSERT_EQ(CHESS_TOURNAMENT_NOT_EXIST, chessEndTournament(chess, 1));
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_NO_GAMES, chessEndTournament(chess, 1));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_SUCCESS, chessEndTournament(chess, 1));
    ASSERT_EQ(CHESS_TOURNAMENT_ENDED, chessEndTournament(chess, 1));
    chessDestroy(chess);
}

TEST(Chess, TestCalcAvgPlayTime) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ChessResult error = CHESS_SUCCESS;
    chessCalculateAveragePlayTime(chess, 0, &error);
    ASSERT_EQ(CHESS_INVALID_ID, error);
    chessCalculateAveragePlayTime(chess, 1, &error);
    ASSERT_EQ(CHESS_PLAYER_NOT_EXIST, error);
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    chessCalculateAveragePlayTime(chess, 1, &error);
    ASSERT_EQ(CHESS_SUCCESS, error);
    chessDestroy(chess);
}

TEST(Chess, TestSaveTournamentStatistics) {
    ChessSystem const chess = chessCreate();
    char path[] = "/dev/null";
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_NO_TOURNAMENTS_ENDED, chessSaveTournamentStatistics(chess, path));
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 2, "A"));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 7));
    ASSERT_EQ(CHESS_SUCCESS, chessEndTournament(chess, 1));
    ASSERT_EQ(CHESS_SUCCESS, chessSaveTournamentStatistics(chess, path));
    chessDestroy(chess);
}

TEST(Chess, TestMultipleGamesSamePlayers) {
    GTEST_SKIP_("Not implemented yet");
}
