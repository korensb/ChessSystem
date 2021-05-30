#include <gtest/gtest.h>

extern "C" {
    #include <stdio.h>

    #include "../chess/chessSystem.h"
}

TEST(ChessBehaviour, Flow1) {
    ChessSystem const chess = chessCreate();
    ASSERT_TRUE(chess);
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 1, 3, "Location a"));
    ASSERT_EQ(CHESS_TOURNAMENT_ALREADY_EXISTS, chessAddTournament(chess, 1, 3, "Location b"));
    ASSERT_EQ(CHESS_SUCCESS, chessAddTournament(chess, 2, 3, "Location c"));
    ASSERT_EQ(CHESS_SUCCESS, chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 3));
    ChessResult error = CHESS_SUCCESS;
    ASSERT_GT(0, chessCalculateAveragePlayTime(chess, 5, &error));
    ASSERT_EQ(CHESS_PLAYER_NOT_EXIST, error);
    ASSERT_EQ(CHESS_SUCCESS, chessSavePlayersLevels(chess, stdout));
    ASSERT_EQ(CHESS_SUCCESS, chessEndTournament(chess, 1));
    char path[] = "./tournament-statistics-out.txt";
    ASSERT_EQ(CHESS_SUCCESS, chessSaveTournamentStatistics(chess, path));

    chessDestroy(chess);
}
