#include "tournament.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the pointer to use of the tournament struct */
typedef struct tournament *Tournament;

/* functions to create tournaments and games map */ 
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);
createTournamentsMap()

/* functions that will be used by the Maps */
//COPY
Tournament tournamentCopy(Tournament element);

//Destroy:
void tournamentDestroy(Tournament tournament);




/* functions to update data of standings and tournaments after adding a game */
bool is_tournament_active (Tournament tournament);

void tournamentAddGame (....) //need to make the functiong

