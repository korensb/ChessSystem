#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the pointer to use of the player struct */

typedef struct player *Player;

/* functions to create players, playersTournaments and gamePointersMap */

Player playerCreate();
Map createPlayerTournamentsMap(); // key: tournament ID, data: Map of pointers to games
Map createPlayersMap();
Map createDoublesMap();

/* functions to update data of player after adding a game */
bool is_game_existed (Player player1, int tournament_id, int player2);
void playerAddGame (...) // need to make the function
ChessResult add_player_to_tournament_if_not_exist(Player player, int tournament_id);
/* functions that will be used by the Maps */
//copy

MapDataElement doubleCopy(MapDataElement i);
MapDataElement playerCopy(MapDataElement element);
MapDataElement mapDataCopy (MapDataElement element);
//destroy

void playerDestroy(MapDataElement player);
void doubleDestroy(MapDataElement id);
void mapDataDestroy(MapDataElement map);

