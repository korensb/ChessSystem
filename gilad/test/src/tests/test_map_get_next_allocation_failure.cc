#include <gtest/gtest.h>

extern "C" {
    #include <stdlib.h>
    #include <stdio.h>

    #include "../chess//mtm_map/map.h"
}

static bool fail_copy = false;

static int* copy(int const * const n) {
    if (fail_copy) {
        return NULL;
    };
    int * const copy = (int *) malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *n;
    return copy;
}

static int compare(int const * const a, int const * const b) {
    return *a - *b;
}

static void destroy(void* const p) {
    free(p);
}

TEST(Map, GetNextAllocationFailure) {
    Map const map = mapCreate((copyMapDataElements) copy,
                              (copyMapKeyElements) copy, destroy, destroy,
                              (compareMapKeyElements) compare);
    ASSERT_TRUE(map);
    int key1 = 1;
    int data1 = 2;
    int key2 = 3;
    int data2 = 4;
    int key3 = 5;
    int data3 = 6;
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, &key1, &data1));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, &key2, &data2));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, &key3, &data3));
    int * const first = (int *) mapGetFirst(map);
    ASSERT_TRUE(first);
    free(first);

    fail_copy = true;
    int const * const second_fail = (int *) mapGetNext(map);
    ASSERT_FALSE(second_fail);
    fail_copy = false;
    int * const second_success = (int *) mapGetNext(map);
    // TODO the official libmap continues as usual here(???)
    ASSERT_TRUE(second_success);
    free(second_success);

    mapDestroy(map);
}
