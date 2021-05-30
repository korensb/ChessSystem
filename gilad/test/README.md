# How to use locally:
These tests can either be run from this project, by placing your sources where they're expected to be, or by downloading tests and adding them to your own pr 

For CLion users, you can also run tests using the gui (probably other IDEs can do this too, but I'm only familiar with CLion).


## The recommended method: clone (requires git)
0. Clone:
```bash
$ git clone https://gitlab.com/intro-to-inline-and-void-pointers/ex1-public.git
```
1. Copy your sources to the relevant source directories ([mtm_map](https://gitlab.com/intro-to-inline-and-void-pointers/ex1-public/-/tree/master/src/chess/mtm_map) or [chess](https://gitlab.com/intro-to-inline-and-void-pointers/ex1-public/-/tree/master/src/chess))
2. Delete / comment out `./PreLoad.cmake` (unless you prefer ninja, then install ninja-build).
3. Run `./build.sh`. The build may fail if you are not using a Fedora-based environment:
```bash
$ ./build.sh
-- The C compiler identification is GNU 11.1.1
-- The CXX compiler identification is GNU 11.1.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found Python: /usr/bin/python3.9 (found version "3.9.5") found components: Interpreter
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
CMake Error at CMakeLists.txt:111 (add_library):
  Cannot find source file:

      src/chess/chessSystem.c

        Tried extensions .c .C .c++ .cc .cpp .cxx .cu .m .M .mm .h .hh .h++ .hm
          .hpp .hxx .in .txx .f .F .for .f77 .f90 .f95 .f03 .ispc


CMake Error at CMakeLists.txt:25 (add_library):
  Cannot find source file:

      src/chess/mtm_map/map.c

        Tried extensions .c .C .c++ .cc .cpp .cxx .cu .m .M .mm .h .hh .h++ .hm
          .hpp .hxx .in .txx .f .F .for .f77 .f90 .f95 .f03 .ispc


CMake Error at CMakeLists.txt:111 (add_library):
  No SOURCES given to target: chess_libmap


CMake Error at CMakeLists.txt:45 (add_library):
  No SOURCES given to target: own_chess


CMake Error at CMakeLists.txt:25 (add_library):
  No SOURCES given to target: own_map


CMake Generate step failed.  Build files cannot be regenerated correctly.
ninja: error: loading 'build.ninja': No such file or directory
```
4. Add your chess and map sources, i.e.
```diff
$ git diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 184b33f..e7f13f5 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -26,11 +26,14 @@ add_library(
add_library(
        own_map
        ${MAP_SOURCE}
        # Add more sources here
+       src/chess/mtm_map/node.c
        )

set(CHESS_SOURCES
        ${CHESS_SOURCE}
        # Add more sources here
+       src/chess/tournament.c
+       src/chess/match.c
        )

set(TEST_SOURCES
        src/tests/map_test.cc
        src/tests/chess_test.cc
        # Add more sources here
+       src/tests/my_new_test.cc
        )
```
And you're done :)

You may use
* `./build.sh` to build
* `./test.sh` to run all tests
(These are useful in the shell.
If you use an IDE, after it reloads the cmake project it should let you run tests with a click of a button.
If it doesn't, you probably need to learn it better or switch to a different IDE.)


## Example use:
```bash
$ ./build.sh
-- The C compiler identification is GNU 11.1.1
-- The CXX compiler identification is GNU 11.1.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found Python: /usr/bin/python3.9 (found version "3.9.5") found components: Interpreter
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: ./build
[43/43] Linking CXX executable libmap_tests
$ ./test.sh
-- Configuring done
-- Generating done
-- Build files have been written to: ./build
[1/1] cd ./build && pandoc -o dry.pdf ../src/2-dry/dry.md
Test project ./build
      Start  1: Map.TestCreateDestroy
 1/68 Test  #1: Map.TestCreateDestroy ...............................   Passed    0.00 sec
      Start  2: Map.TestCopy
 2/68 Test  #2: Map.TestCopy ........................................   Passed    0.00 sec
      Start  3: Map.TestGetSize
 3/68 Test  #3: Map.TestGetSize .....................................   Passed    0.00 sec
      Start  4: Map.TestContains
 4/68 Test  #4: Map.TestContains ....................................   Passed    0.00 sec
      Start  5: Map.TestPutOutOfMemory
 5/68 Test  #5: Map.TestPutOutOfMemory ..............................   Passed    0.00 sec
      Start  6: Map.TestNullArgument
 6/68 Test  #6: Map.TestNullArgument ................................   Passed    0.00 sec
      Start  7: Map.TestPutDifferentKeys
 7/68 Test  #7: Map.TestPutDifferentKeys ............................   Passed    0.00 sec
      Start  8: Map.TestRemove
 8/68 Test  #8: Map.TestRemove ......................................   Passed    0.00 sec
      Start  9: Map.TestClear
 9/68 Test  #9: Map.TestClear .......................................   Passed    0.00 sec
      Start 10: Map.TestGetFirst
10/68 Test #10: Map.TestGetFirst ....................................   Passed    0.00 sec
      Start 11: Map.TestGetNext
11/68 Test #11: Map.TestGetNext .....................................   Passed    0.00 sec
      Start 12: Map.TestMapPutMemLeak
12/68 Test #12: Map.TestMapPutMemLeak ...............................   Passed    0.00 sec
      Start 13: Map.GetNextAllocationFailure
13/68 Test #13: Map.GetNextAllocationFailure ........................   Passed    0.00 sec
      Start 14: Chess.CreateDestroy
14/68 Test #14: Chess.CreateDestroy .................................   Passed    0.00 sec
      Start 15: Chess.TestAddTournament
15/68 Test #15: Chess.TestAddTournament .............................   Passed    0.00 sec
      Start 16: Chess.TestAddGame
16/68 Test #16: Chess.TestAddGame ...................................   Passed    0.00 sec
      Start 17: Chess.TestExceededGames
17/68 Test #17: Chess.TestExceededGames .............................   Passed    0.00 sec
      Start 18: Chess.TestRemoveTournament
18/68 Test #18: Chess.TestRemoveTournament ..........................   Passed    0.00 sec
      Start 19: Chess.TestRemovePlayer
19/68 Test #19: Chess.TestRemovePlayer ..............................   Passed    0.00 sec
      Start 20: Chess.TestEndTournament
20/68 Test #20: Chess.TestEndTournament .............................   Passed    0.00 sec
      Start 21: Chess.TestCalcAvgPlayTime
21/68 Test #21: Chess.TestCalcAvgPlayTime ...........................   Passed    0.00 sec
      Start 22: Chess.TestSaveTournamentStatistics
22/68 Test #22: Chess.TestSaveTournamentStatistics ..................   Passed    0.00 sec
      Start 23: Chess.TestMultipleGamesSamePlayers
23/68 Test #23: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.00 sec
      Start 24: Chess.addTournament
24/68 Test #24: Chess.addTournament .................................   Passed    0.00 sec
      Start 25: Chess.testChessRemoveTournament
25/68 Test #25: Chess.testChessRemoveTournament .....................   Passed    0.00 sec
      Start 26: Chess.testChessAddGame
26/68 Test #26: Chess.testChessAddGame ..............................   Passed    0.00 sec
      Start 27: Chess.testChessPrintLevelsAndTournamentStatistics
27/68 Test #27: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.00 sec
      Start 28: Chess.TestTournamentInvalidParameters
28/68 Test #28: Chess.TestTournamentInvalidParameters ...............   Passed    0.00 sec
      Start 29: Chess.TestMaxGames
29/68 Test #29: Chess.TestMaxGames ..................................   Passed    0.00 sec
      Start 30: Chess.TestGameAlreadyExists
30/68 Test #30: Chess.TestGameAlreadyExists .........................   Passed    0.00 sec
      Start 31: Chess.TestEndRemovedTournament
31/68 Test #31: Chess.TestEndRemovedTournament ......................   Passed    0.00 sec
      Start 32: Chess.TestEndedTournament
32/68 Test #32: Chess.TestEndedTournament ...........................   Passed    0.00 sec
      Start 33: Chess.TestAvgGameTime
33/68 Test #33: Chess.TestAvgGameTime ...............................   Passed    0.00 sec
      Start 34: ChessBehaviour.Flow1
34/68 Test #34: ChessBehaviour.Flow1 ................................   Passed    0.00 sec
      Start 35: Map.TestCreateDestroy
35/68 Test #35: Map.TestCreateDestroy ...............................   Passed    0.00 sec
      Start 36: Map.TestCopy
36/68 Test #36: Map.TestCopy ........................................   Passed    0.00 sec
      Start 37: Map.TestGetSize
37/68 Test #37: Map.TestGetSize .....................................   Passed    0.00 sec
      Start 38: Map.TestContains
38/68 Test #38: Map.TestContains ....................................   Passed    0.00 sec
      Start 39: Map.TestPutOutOfMemory
39/68 Test #39: Map.TestPutOutOfMemory ..............................   Passed    0.00 sec
      Start 40: Map.TestNullArgument
40/68 Test #40: Map.TestNullArgument ................................   Passed    0.00 sec
      Start 41: Map.TestPutDifferentKeys
41/68 Test #41: Map.TestPutDifferentKeys ............................   Passed    0.00 sec
      Start 42: Map.TestRemove
42/68 Test #42: Map.TestRemove ......................................   Passed    0.00 sec
      Start 43: Map.TestClear
43/68 Test #43: Map.TestClear .......................................   Passed    0.00 sec
      Start 44: Map.TestGetFirst
44/68 Test #44: Map.TestGetFirst ....................................   Passed    0.00 sec
      Start 45: Map.TestGetNext
45/68 Test #45: Map.TestGetNext .....................................   Passed    0.00 sec
      Start 46: Map.TestMapPutMemLeak
46/68 Test #46: Map.TestMapPutMemLeak ...............................   Passed    0.00 sec
      Start 47: Map.GetNextAllocationFailure
47/68 Test #47: Map.GetNextAllocationFailure ........................   Passed    0.00 sec
      Start 48: Chess.CreateDestroy
48/68 Test #48: Chess.CreateDestroy .................................   Passed    0.00 sec
      Start 49: Chess.TestAddTournament
49/68 Test #49: Chess.TestAddTournament .............................   Passed    0.00 sec
      Start 50: Chess.TestAddGame
50/68 Test #50: Chess.TestAddGame ...................................   Passed    0.00 sec
      Start 51: Chess.TestExceededGames
51/68 Test #51: Chess.TestExceededGames .............................   Passed    0.00 sec
      Start 52: Chess.TestRemoveTournament
52/68 Test #52: Chess.TestRemoveTournament ..........................   Passed    0.00 sec
      Start 53: Chess.TestRemovePlayer
53/68 Test #53: Chess.TestRemovePlayer ..............................   Passed    0.00 sec
      Start 54: Chess.TestEndTournament
54/68 Test #54: Chess.TestEndTournament .............................   Passed    0.00 sec
      Start 55: Chess.TestCalcAvgPlayTime
55/68 Test #55: Chess.TestCalcAvgPlayTime ...........................   Passed    0.00 sec
      Start 56: Chess.TestSaveTournamentStatistics
56/68 Test #56: Chess.TestSaveTournamentStatistics ..................   Passed    0.00 sec
      Start 57: Chess.TestMultipleGamesSamePlayers
57/68 Test #57: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.00 sec
      Start 58: Chess.addTournament
58/68 Test #58: Chess.addTournament .................................   Passed    0.00 sec
      Start 59: Chess.testChessRemoveTournament
59/68 Test #59: Chess.testChessRemoveTournament .....................   Passed    0.00 sec
      Start 60: Chess.testChessAddGame
60/68 Test #60: Chess.testChessAddGame ..............................   Passed    0.00 sec
      Start 61: Chess.testChessPrintLevelsAndTournamentStatistics
61/68 Test #61: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.00 sec
      Start 62: Chess.TestTournamentInvalidParameters
62/68 Test #62: Chess.TestTournamentInvalidParameters ...............   Passed    0.00 sec
      Start 63: Chess.TestMaxGames
63/68 Test #63: Chess.TestMaxGames ..................................   Passed    0.00 sec
      Start 64: Chess.TestGameAlreadyExists
64/68 Test #64: Chess.TestGameAlreadyExists .........................   Passed    0.00 sec
      Start 65: Chess.TestEndRemovedTournament
65/68 Test #65: Chess.TestEndRemovedTournament ......................   Passed    0.00 sec
      Start 66: Chess.TestEndedTournament
66/68 Test #66: Chess.TestEndedTournament ...........................   Passed    0.00 sec
      Start 67: Chess.TestAvgGameTime
67/68 Test #67: Chess.TestAvgGameTime ...............................   Passed    0.00 sec
      Start 68: ChessBehaviour.Flow1
68/68 Test #68: ChessBehaviour.Flow1 ................................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 68

Total Test time (real) =   0.12 sec

The following tests did not run:
         23 - Chess.TestMultipleGamesSamePlayers (Skipped)
         57 - Chess.TestMultipleGamesSamePlayers (Skipped)
$ ./memtest.sh
-- Configuring done
-- Generating done
-- Build files have been written to: ./build
[1/1] cd ./build && pandoc -o dry.pdf ../src/2-dry/dry.md
Test project ./build
      Start  1: Map.TestCreateDestroy
 1/68 Test  #1: Map.TestCreateDestroy ...............................   Passed    0.00 sec
      Start  2: Map.TestCopy
 2/68 Test  #2: Map.TestCopy ........................................   Passed    0.00 sec
      Start  3: Map.TestGetSize
 3/68 Test  #3: Map.TestGetSize .....................................   Passed    0.00 sec
      Start  4: Map.TestContains
 4/68 Test  #4: Map.TestContains ....................................   Passed    0.00 sec
      Start  5: Map.TestPutOutOfMemory
 5/68 Test  #5: Map.TestPutOutOfMemory ..............................   Passed    0.00 sec
      Start  6: Map.TestNullArgument
 6/68 Test  #6: Map.TestNullArgument ................................   Passed    0.00 sec
      Start  7: Map.TestPutDifferentKeys
 7/68 Test  #7: Map.TestPutDifferentKeys ............................   Passed    0.00 sec
      Start  8: Map.TestRemove
 8/68 Test  #8: Map.TestRemove ......................................   Passed    0.00 sec
      Start  9: Map.TestClear
 9/68 Test  #9: Map.TestClear .......................................   Passed    0.00 sec
      Start 10: Map.TestGetFirst
10/68 Test #10: Map.TestGetFirst ....................................   Passed    0.00 sec
      Start 11: Map.TestGetNext
11/68 Test #11: Map.TestGetNext .....................................   Passed    0.00 sec
      Start 12: Map.TestMapPutMemLeak
12/68 Test #12: Map.TestMapPutMemLeak ...............................   Passed    0.00 sec
      Start 13: Map.GetNextAllocationFailure
13/68 Test #13: Map.GetNextAllocationFailure ........................   Passed    0.00 sec
      Start 14: Chess.CreateDestroy
14/68 Test #14: Chess.CreateDestroy .................................   Passed    0.00 sec
      Start 15: Chess.TestAddTournament
15/68 Test #15: Chess.TestAddTournament .............................   Passed    0.00 sec
      Start 16: Chess.TestAddGame
16/68 Test #16: Chess.TestAddGame ...................................   Passed    0.00 sec
      Start 17: Chess.TestExceededGames
17/68 Test #17: Chess.TestExceededGames .............................   Passed    0.00 sec
      Start 18: Chess.TestRemoveTournament
18/68 Test #18: Chess.TestRemoveTournament ..........................   Passed    0.00 sec
      Start 19: Chess.TestRemovePlayer
19/68 Test #19: Chess.TestRemovePlayer ..............................   Passed    0.00 sec
      Start 20: Chess.TestEndTournament
20/68 Test #20: Chess.TestEndTournament .............................   Passed    0.00 sec
      Start 21: Chess.TestCalcAvgPlayTime
21/68 Test #21: Chess.TestCalcAvgPlayTime ...........................   Passed    0.00 sec
      Start 22: Chess.TestSaveTournamentStatistics
22/68 Test #22: Chess.TestSaveTournamentStatistics ..................   Passed    0.00 sec
      Start 23: Chess.TestMultipleGamesSamePlayers
23/68 Test #23: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.00 sec
      Start 24: Chess.addTournament
24/68 Test #24: Chess.addTournament .................................   Passed    0.00 sec
      Start 25: Chess.testChessRemoveTournament
25/68 Test #25: Chess.testChessRemoveTournament .....................   Passed    0.00 sec
      Start 26: Chess.testChessAddGame
26/68 Test #26: Chess.testChessAddGame ..............................   Passed    0.00 sec
      Start 27: Chess.testChessPrintLevelsAndTournamentStatistics
27/68 Test #27: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.00 sec
      Start 28: Chess.TestTournamentInvalidParameters
28/68 Test #28: Chess.TestTournamentInvalidParameters ...............   Passed    0.00 sec
      Start 29: Chess.TestMaxGames
29/68 Test #29: Chess.TestMaxGames ..................................   Passed    0.00 sec
      Start 30: Chess.TestGameAlreadyExists
30/68 Test #30: Chess.TestGameAlreadyExists .........................   Passed    0.00 sec
      Start 31: Chess.TestEndRemovedTournament
31/68 Test #31: Chess.TestEndRemovedTournament ......................   Passed    0.00 sec
      Start 32: Chess.TestEndedTournament
32/68 Test #32: Chess.TestEndedTournament ...........................   Passed    0.00 sec
      Start 33: Chess.TestAvgGameTime
33/68 Test #33: Chess.TestAvgGameTime ...............................   Passed    0.00 sec
      Start 34: ChessBehaviour.Flow1
34/68 Test #34: ChessBehaviour.Flow1 ................................   Passed    0.00 sec
      Start 35: Map.TestCreateDestroy
35/68 Test #35: Map.TestCreateDestroy ...............................   Passed    0.00 sec
      Start 36: Map.TestCopy
36/68 Test #36: Map.TestCopy ........................................   Passed    0.00 sec
      Start 37: Map.TestGetSize
37/68 Test #37: Map.TestGetSize .....................................   Passed    0.00 sec
      Start 38: Map.TestContains
38/68 Test #38: Map.TestContains ....................................   Passed    0.00 sec
      Start 39: Map.TestPutOutOfMemory
39/68 Test #39: Map.TestPutOutOfMemory ..............................   Passed    0.00 sec
      Start 40: Map.TestNullArgument
40/68 Test #40: Map.TestNullArgument ................................   Passed    0.00 sec
      Start 41: Map.TestPutDifferentKeys
41/68 Test #41: Map.TestPutDifferentKeys ............................   Passed    0.00 sec
      Start 42: Map.TestRemove
42/68 Test #42: Map.TestRemove ......................................   Passed    0.00 sec
      Start 43: Map.TestClear
43/68 Test #43: Map.TestClear .......................................   Passed    0.00 sec
      Start 44: Map.TestGetFirst
44/68 Test #44: Map.TestGetFirst ....................................   Passed    0.00 sec
      Start 45: Map.TestGetNext
45/68 Test #45: Map.TestGetNext .....................................   Passed    0.00 sec
      Start 46: Map.TestMapPutMemLeak
46/68 Test #46: Map.TestMapPutMemLeak ...............................   Passed    0.00 sec
      Start 47: Map.GetNextAllocationFailure
47/68 Test #47: Map.GetNextAllocationFailure ........................   Passed    0.00 sec
      Start 48: Chess.CreateDestroy
48/68 Test #48: Chess.CreateDestroy .................................   Passed    0.00 sec
      Start 49: Chess.TestAddTournament
49/68 Test #49: Chess.TestAddTournament .............................   Passed    0.00 sec
      Start 50: Chess.TestAddGame
50/68 Test #50: Chess.TestAddGame ...................................   Passed    0.00 sec
      Start 51: Chess.TestExceededGames
51/68 Test #51: Chess.TestExceededGames .............................   Passed    0.00 sec
      Start 52: Chess.TestRemoveTournament
52/68 Test #52: Chess.TestRemoveTournament ..........................   Passed    0.00 sec
      Start 53: Chess.TestRemovePlayer
53/68 Test #53: Chess.TestRemovePlayer ..............................   Passed    0.00 sec
      Start 54: Chess.TestEndTournament
54/68 Test #54: Chess.TestEndTournament .............................   Passed    0.00 sec
      Start 55: Chess.TestCalcAvgPlayTime
55/68 Test #55: Chess.TestCalcAvgPlayTime ...........................   Passed    0.00 sec
      Start 56: Chess.TestSaveTournamentStatistics
56/68 Test #56: Chess.TestSaveTournamentStatistics ..................   Passed    0.00 sec
      Start 57: Chess.TestMultipleGamesSamePlayers
57/68 Test #57: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.00 sec
      Start 58: Chess.addTournament
58/68 Test #58: Chess.addTournament .................................   Passed    0.00 sec
      Start 59: Chess.testChessRemoveTournament
59/68 Test #59: Chess.testChessRemoveTournament .....................   Passed    0.00 sec
      Start 60: Chess.testChessAddGame
60/68 Test #60: Chess.testChessAddGame ..............................   Passed    0.00 sec
      Start 61: Chess.testChessPrintLevelsAndTournamentStatistics
61/68 Test #61: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.00 sec
      Start 62: Chess.TestTournamentInvalidParameters
62/68 Test #62: Chess.TestTournamentInvalidParameters ...............   Passed    0.00 sec
      Start 63: Chess.TestMaxGames
63/68 Test #63: Chess.TestMaxGames ..................................   Passed    0.00 sec
      Start 64: Chess.TestGameAlreadyExists
64/68 Test #64: Chess.TestGameAlreadyExists .........................   Passed    0.00 sec
      Start 65: Chess.TestEndRemovedTournament
65/68 Test #65: Chess.TestEndRemovedTournament ......................   Passed    0.00 sec
      Start 66: Chess.TestEndedTournament
66/68 Test #66: Chess.TestEndedTournament ...........................   Passed    0.00 sec
      Start 67: Chess.TestAvgGameTime
67/68 Test #67: Chess.TestAvgGameTime ...............................   Passed    0.00 sec
      Start 68: ChessBehaviour.Flow1
68/68 Test #68: ChessBehaviour.Flow1 ................................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 68

Total Test time (real) =   0.13 sec

The following tests did not run:
         23 - Chess.TestMultipleGamesSamePlayers (Skipped)
         57 - Chess.TestMultipleGamesSamePlayers (Skipped)
[1/1] cd ./build && ./build/Testing/Temporary/MemoryChecker.*.log
   Site: etherless
   Build name: Linux-c++
Create new tag: 20210530-1326 - Experimental
Memory check project ./build
      Start  1: Map.TestCreateDestroy
 1/68 MemCheck  #1: Map.TestCreateDestroy ...............................   Passed    0.66 sec
      Start  2: Map.TestCopy
 2/68 MemCheck  #2: Map.TestCopy ........................................   Passed    0.54 sec
      Start  3: Map.TestGetSize
 3/68 MemCheck  #3: Map.TestGetSize .....................................   Passed    0.54 sec
      Start  4: Map.TestContains
 4/68 MemCheck  #4: Map.TestContains ....................................   Passed    0.54 sec
      Start  5: Map.TestPutOutOfMemory
 5/68 MemCheck  #5: Map.TestPutOutOfMemory ..............................   Passed    0.55 sec
      Start  6: Map.TestNullArgument
 6/68 MemCheck  #6: Map.TestNullArgument ................................   Passed    0.55 sec
      Start  7: Map.TestPutDifferentKeys
 7/68 MemCheck  #7: Map.TestPutDifferentKeys ............................   Passed    0.56 sec
      Start  8: Map.TestRemove
 8/68 MemCheck  #8: Map.TestRemove ......................................   Passed    0.58 sec
      Start  9: Map.TestClear
 9/68 MemCheck  #9: Map.TestClear .......................................   Passed    0.57 sec
      Start 10: Map.TestGetFirst
10/68 MemCheck #10: Map.TestGetFirst ....................................   Passed    0.57 sec
      Start 11: Map.TestGetNext
11/68 MemCheck #11: Map.TestGetNext .....................................   Passed    0.56 sec
      Start 12: Map.TestMapPutMemLeak
12/68 MemCheck #12: Map.TestMapPutMemLeak ...............................   Passed    0.57 sec
      Start 13: Map.GetNextAllocationFailure
13/68 MemCheck #13: Map.GetNextAllocationFailure ........................   Passed    0.54 sec
      Start 14: Chess.CreateDestroy
14/68 MemCheck #14: Chess.CreateDestroy .................................   Passed    0.55 sec
      Start 15: Chess.TestAddTournament
15/68 MemCheck #15: Chess.TestAddTournament .............................   Passed    0.56 sec
      Start 16: Chess.TestAddGame
16/68 MemCheck #16: Chess.TestAddGame ...................................   Passed    0.56 sec
      Start 17: Chess.TestExceededGames
17/68 MemCheck #17: Chess.TestExceededGames .............................   Passed    0.56 sec
      Start 18: Chess.TestRemoveTournament
18/68 MemCheck #18: Chess.TestRemoveTournament ..........................   Passed    0.56 sec
      Start 19: Chess.TestRemovePlayer
19/68 MemCheck #19: Chess.TestRemovePlayer ..............................   Passed    0.56 sec
      Start 20: Chess.TestEndTournament
20/68 MemCheck #20: Chess.TestEndTournament .............................   Passed    0.57 sec
      Start 21: Chess.TestCalcAvgPlayTime
21/68 MemCheck #21: Chess.TestCalcAvgPlayTime ...........................   Passed    0.57 sec
      Start 22: Chess.TestSaveTournamentStatistics
22/68 MemCheck #22: Chess.TestSaveTournamentStatistics ..................   Passed    0.60 sec
      Start 23: Chess.TestMultipleGamesSamePlayers
23/68 MemCheck #23: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.57 sec
      Start 24: Chess.addTournament
24/68 MemCheck #24: Chess.addTournament .................................   Passed    0.55 sec
      Start 25: Chess.testChessRemoveTournament
25/68 MemCheck #25: Chess.testChessRemoveTournament .....................   Passed    0.56 sec
      Start 26: Chess.testChessAddGame
26/68 MemCheck #26: Chess.testChessAddGame ..............................   Passed    0.54 sec
      Start 27: Chess.testChessPrintLevelsAndTournamentStatistics
27/68 MemCheck #27: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.61 sec
      Start 28: Chess.TestTournamentInvalidParameters
28/68 MemCheck #28: Chess.TestTournamentInvalidParameters ...............   Passed    0.55 sec
      Start 29: Chess.TestMaxGames
29/68 MemCheck #29: Chess.TestMaxGames ..................................   Passed    0.56 sec
      Start 30: Chess.TestGameAlreadyExists
30/68 MemCheck #30: Chess.TestGameAlreadyExists .........................   Passed    0.56 sec
      Start 31: Chess.TestEndRemovedTournament
31/68 MemCheck #31: Chess.TestEndRemovedTournament ......................   Passed    0.54 sec
      Start 32: Chess.TestEndedTournament
32/68 MemCheck #32: Chess.TestEndedTournament ...........................   Passed    0.56 sec
      Start 33: Chess.TestAvgGameTime
33/68 MemCheck #33: Chess.TestAvgGameTime ...............................   Passed    0.55 sec
      Start 34: ChessBehaviour.Flow1
34/68 MemCheck #34: ChessBehaviour.Flow1 ................................   Passed    0.60 sec
      Start 35: Map.TestCreateDestroy
35/68 MemCheck #35: Map.TestCreateDestroy ...............................   Passed    0.54 sec
      Start 36: Map.TestCopy
36/68 MemCheck #36: Map.TestCopy ........................................   Passed    0.56 sec
      Start 37: Map.TestGetSize
37/68 MemCheck #37: Map.TestGetSize .....................................   Passed    0.54 sec
      Start 38: Map.TestContains
38/68 MemCheck #38: Map.TestContains ....................................   Passed    0.60 sec
      Start 39: Map.TestPutOutOfMemory
39/68 MemCheck #39: Map.TestPutOutOfMemory ..............................   Passed    0.55 sec
      Start 40: Map.TestNullArgument
40/68 MemCheck #40: Map.TestNullArgument ................................   Passed    0.57 sec
      Start 41: Map.TestPutDifferentKeys
41/68 MemCheck #41: Map.TestPutDifferentKeys ............................   Passed    0.56 sec
      Start 42: Map.TestRemove
42/68 MemCheck #42: Map.TestRemove ......................................   Passed    0.54 sec
      Start 43: Map.TestClear
43/68 MemCheck #43: Map.TestClear .......................................   Passed    0.55 sec
      Start 44: Map.TestGetFirst
44/68 MemCheck #44: Map.TestGetFirst ....................................   Passed    0.54 sec
      Start 45: Map.TestGetNext
45/68 MemCheck #45: Map.TestGetNext .....................................   Passed    0.56 sec
      Start 46: Map.TestMapPutMemLeak
46/68 MemCheck #46: Map.TestMapPutMemLeak ...............................   Passed    0.55 sec
      Start 47: Map.GetNextAllocationFailure
47/68 MemCheck #47: Map.GetNextAllocationFailure ........................   Passed    0.61 sec
      Start 48: Chess.CreateDestroy
48/68 MemCheck #48: Chess.CreateDestroy .................................   Passed    0.54 sec
      Start 49: Chess.TestAddTournament
49/68 MemCheck #49: Chess.TestAddTournament .............................   Passed    0.55 sec
      Start 50: Chess.TestAddGame
50/68 MemCheck #50: Chess.TestAddGame ...................................   Passed    0.59 sec
      Start 51: Chess.TestExceededGames
51/68 MemCheck #51: Chess.TestExceededGames .............................   Passed    0.56 sec
      Start 52: Chess.TestRemoveTournament
52/68 MemCheck #52: Chess.TestRemoveTournament ..........................   Passed    0.56 sec
      Start 53: Chess.TestRemovePlayer
53/68 MemCheck #53: Chess.TestRemovePlayer ..............................   Passed    0.57 sec
      Start 54: Chess.TestEndTournament
54/68 MemCheck #54: Chess.TestEndTournament .............................   Passed    0.57 sec
      Start 55: Chess.TestCalcAvgPlayTime
55/68 MemCheck #55: Chess.TestCalcAvgPlayTime ...........................   Passed    0.57 sec
      Start 56: Chess.TestSaveTournamentStatistics
56/68 MemCheck #56: Chess.TestSaveTournamentStatistics ..................   Passed    0.65 sec
      Start 57: Chess.TestMultipleGamesSamePlayers
57/68 MemCheck #57: Chess.TestMultipleGamesSamePlayers ..................***Skipped   0.57 sec
      Start 58: Chess.addTournament
58/68 MemCheck #58: Chess.addTournament .................................   Passed    0.55 sec
      Start 59: Chess.testChessRemoveTournament
59/68 MemCheck #59: Chess.testChessRemoveTournament .....................   Passed    0.59 sec
      Start 60: Chess.testChessAddGame
60/68 MemCheck #60: Chess.testChessAddGame ..............................   Passed    0.57 sec
      Start 61: Chess.testChessPrintLevelsAndTournamentStatistics
61/68 MemCheck #61: Chess.testChessPrintLevelsAndTournamentStatistics ...   Passed    0.63 sec
      Start 62: Chess.TestTournamentInvalidParameters
62/68 MemCheck #62: Chess.TestTournamentInvalidParameters ...............   Passed    0.56 sec
      Start 63: Chess.TestMaxGames
63/68 MemCheck #63: Chess.TestMaxGames ..................................   Passed    0.57 sec
      Start 64: Chess.TestGameAlreadyExists
64/68 MemCheck #64: Chess.TestGameAlreadyExists .........................   Passed    0.56 sec
      Start 65: Chess.TestEndRemovedTournament
65/68 MemCheck #65: Chess.TestEndRemovedTournament ......................   Passed    0.65 sec
      Start 66: Chess.TestEndedTournament
66/68 MemCheck #66: Chess.TestEndedTournament ...........................   Passed    0.57 sec
      Start 67: Chess.TestAvgGameTime
67/68 MemCheck #67: Chess.TestAvgGameTime ...............................   Passed    0.57 sec
      Start 68: ChessBehaviour.Flow1
68/68 MemCheck #68: ChessBehaviour.Flow1 ................................   Passed    0.61 sec

100% tests passed, 0 tests failed out of 68

Total Test time (real) =  39.08 sec

The following tests did not run:
         23 - Chess.TestMultipleGamesSamePlayers (Skipped)
         57 - Chess.TestMultipleGamesSamePlayers (Skipped)
-- Processing memory checking output:
MemCheck log files can be found here: (<#> corresponds to test number)
./build/Testing/Temporary/MemoryChecker.<#>.log
Memory checking results:
<nothing here means there are no memory errors>
```

# Motivation:
1. [Learn in public](https://twitter.com/swyx/status/1009174159690264579?s=19) (the entire thread is highly recommended)
2. [Inspire others to join](https://www.youtube.com/watch?v=fW8amMCVAJQ)


# Requirements:
1. gcc
2. valgrind
3. ninja (optional, disable PreLoad.cmake to use normal "Unix Makefile")
4. pandoc (optional, for markdown-to-pdf conversion)


See previous [Merge Requests](https://gitlab.com/intro-to-inline-and-void-pointers/ex1-public/-/merge_requests?scope=all&utf8=%E2%9C%93&state=all&label_name[]=example) to see how to add your own tests for everyone else's benefit.
