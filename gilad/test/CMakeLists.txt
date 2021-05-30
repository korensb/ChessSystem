cmake_minimum_required(VERSION 3.16)
project(ex1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include(CTest)

add_custom_target(
        test_memcheck
        COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
        COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

set(USE_OFFICIAL_LIBMAP true)

set(MTM_FLAGS_DEBUG "-std=c99 -g -Wall -Wextra -Werror --pedantic-errors")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

set(MAP_SOURCE src/chess/mtm_map/map.c)
set(CHESS_SOURCE src/chess/chessSystem.c)

add_library(
        own_map
        ${MAP_SOURCE}
        # add more src if we have
)

set(CHESS_SOURCES
        ${CHESS_SOURCE}
        # Add more sources here
	src/chess/player.c
	src/chess/tournament.c
	src/chess/game.c
        )

set(TEST_SOURCES
        src/tests/map_test.cc
        src/tests/chess_test.cc
        src/tests/mtm_official_chess_test.cc
        src/tests/test_chess_behaviour.cc
        src/tests/test_map_get_next_allocation_failure.cc
        # Add more sources here
        )

add_library(
        own_chess
        ${CHESS_SOURCES}
)

target_link_libraries(
        own_chess
        own_map
)

if (EXISTS src/2-dry/dry.md AND EXISTS /usr/bin/pandoc)
        set(DRY_SOURCE src/2-dry/dry.md)
        add_custom_target(
                dry-pdf ALL
                COMMAND pandoc -o dry.pdf
                ../${DRY_SOURCE}
        )
endif()

add_executable(
        map_example_test
        src/chess/tests/map_example_test.c
)

target_link_libraries(
        map_example_test
        own_map
)

add_executable(
        chess_example_test
        src/chess/tests/chessSystemTestsExample.c
)

target_link_libraries(
        chess_example_test
        own_chess
)

# GTest instructions here https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        own_tests
        ${TEST_SOURCES}
)

target_link_libraries(
        own_tests
        gtest_main
        own_chess
)

include(GoogleTest)
gtest_discover_tests(own_tests)

if (USE_OFFICIAL_LIBMAP AND EXISTS /etc/redhat-release)
    link_directories(./libmap_for_csl3)

    add_library(
            chess_libmap
            ${CHESS_SOURCES}
    )

    target_link_libraries(
            chess_libmap
            map
    )

    add_executable(
            libmap_tests
            ${TEST_SOURCES}
    )

    target_link_libraries(
            libmap_tests
            gtest_main
            chess_libmap
    )

    gtest_discover_tests(libmap_tests)

    add_executable(
            map_example_test_libmap
            src/chess/tests/map_example_test.c
    )

    target_link_libraries(
            map_example_test_libmap
            map
    )

    add_executable(
            chess_example_test_libmap
            src/chess/tests/chessSystemTestsExample.c
    )

    target_link_libraries(
            chess_example_test_libmap
            chess_libmap
    )

endif ()
