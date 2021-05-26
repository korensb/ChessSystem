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
Map createTournamentsMap();
Map createIntsMap();

/* functions that will be used by the Maps */
//COPY
MapDataElement tournamentCopy(MapDataElement element);
MapKeyElement intCopyKey(MapKeyElement i);
MapDataElement intCopyData(MapDataElement i);
//Destroy:
void tournamentDestroy(MapDataElement tournament);
void intKeyDestroy(MapKeyElement id);
void intDataDestroy(MapDataElement id);

//compare
int intCompare(MapKeyElement num1, MapKeyElement num2);


/* functions to update data of standings and tournaments after adding a game */
bool is_tournament_active (Tournament tournament);

/* aux functions*/
MapResult add_game_to_tournament_map(Tournament tournament, Game game);
MapResult add_game_to_tournament(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);

bool printTournamentStatistics (Tournament tournament, char* path_file);
