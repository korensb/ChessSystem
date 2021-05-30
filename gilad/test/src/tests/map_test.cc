#include <gtest/gtest.h>

extern "C" {
    #include <string.h>

    #include "../chess/mtm_map/map.h"
}

static Map createEmptyMap() {
    Map const map = mapCreate(
        (copyMapDataElements) strdup,
        (copyMapKeyElements) strdup,
        (freeMapDataElements) free,
        (freeMapKeyElements) free,
        (compareMapKeyElements) strcmp
    );
    assert(map);
    return map;
}

static bool fail_allocation = false;

static char* duplicate(char *string) {
    return fail_allocation ? NULL : strdup(string);
}

static Map createFallibleEmptyMap() {
    Map const map = mapCreate(
            (copyMapDataElements) duplicate,
            (copyMapKeyElements) duplicate,
            (freeMapDataElements) free,
            (freeMapKeyElements) free,
            (compareMapKeyElements) strcmp
    );
    assert(map);
    return map;
}

TEST(Map, TestCreateDestroy) {
    Map const map = createEmptyMap();
    ASSERT_TRUE(map);
    mapDestroy(map);
}

TEST(Map, TestCopy) {
    Map const map = createEmptyMap();
    ASSERT_TRUE(map);

    char const key1[] = "key1";
    char const value1[] = "value1";
    char const key2[] = "key2";
    char const value2[] = "value2";
    MapResult const error1 = mapPut(map, (MapKeyElement) key1, (MapDataElement) value1);
    ASSERT_EQ(MAP_SUCCESS, error1);
    MapResult const error2 = mapPut(map, (MapKeyElement) key2, (MapDataElement) value2);
    ASSERT_EQ(MAP_SUCCESS, error2);

    Map const copy = mapCopy(map);
    ASSERT_TRUE(copy);
    ASSERT_TRUE(mapGetSize(copy) == mapGetSize(map));
    MAP_FOREACH(MapKeyElement, key, map) {
        MapDataElement const value1 = mapGet(map, key);
        MapDataElement const value2 = mapGet(copy, key);
        ASSERT_FALSE(strcmp((char *)value1, (char *)value2));
        free(key);
    }

    mapDestroy(copy);
    mapDestroy(map);
}

TEST(Map, TestGetSize) {
    Map const map = createEmptyMap();
    ASSERT_EQ(0, mapGetSize(map));
    char key[] = "key";
    char data[] = "data";
    MapResult const error1 = mapPut(map, key, data);
    ASSERT_EQ(MAP_SUCCESS, error1);
    ASSERT_EQ(1, mapGetSize(map));
    mapDestroy(map);
}

TEST(Map, TestContains) {
    Map const map = createEmptyMap();
    char key[] = "missing";
    ASSERT_FALSE(mapContains(map, key));
    MapResult const put_error = mapPut(map, key, key);
    ASSERT_EQ(MAP_SUCCESS, put_error);
    ASSERT_TRUE(mapContains(map, key));
    MapResult const remove_error = mapRemove(map, key);
    ASSERT_EQ(MAP_SUCCESS, remove_error);
    ASSERT_FALSE(mapContains(map, key));
    mapDestroy(map);
}

TEST(Map, TestPutOutOfMemory) {
    Map const map = createFallibleEmptyMap();
    char key[] = "key";
    char value[] = "value";

    fail_allocation = true;
    ASSERT_EQ(MAP_OUT_OF_MEMORY, mapPut(map, key, value));
    fail_allocation = false;

    mapDestroy(map);
}

TEST(Map, TestNullArgument) {
    ASSERT_FALSE(mapCreate(NULL, NULL, NULL, NULL, NULL));

    Map const map = createEmptyMap();
    char key[] = "key";
    char value[] = "value";

    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(NULL, key, value));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(map, NULL, value));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(map, key, NULL));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(NULL, NULL, value));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(NULL, key, NULL));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(map, NULL, NULL));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapPut(NULL, NULL, NULL));

    ASSERT_EQ(NULL, mapGet(NULL, NULL));
    ASSERT_EQ(NULL, mapGet(map, NULL));
    ASSERT_EQ(NULL, mapGet(NULL, key));

    ASSERT_FALSE(mapCopy(NULL));

    ASSERT_FALSE(mapGetFirst(NULL));

    ASSERT_FALSE(mapGetNext(NULL));

    ASSERT_TRUE(-1 == mapGetSize(NULL));

    ASSERT_EQ(MAP_NULL_ARGUMENT, mapRemove(NULL, NULL));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapRemove(map, NULL));
    ASSERT_EQ(MAP_NULL_ARGUMENT, mapRemove(NULL, key));

    ASSERT_FALSE(mapContains(NULL, NULL));
    ASSERT_FALSE(mapContains(map, NULL));
    ASSERT_FALSE(mapContains(NULL, key));

    ASSERT_EQ(MAP_NULL_ARGUMENT, mapClear(NULL));

    mapDestroy(map);
}

TEST(Map, TestPutDifferentKeys) {
    Map const map = createEmptyMap();
    char key1[] = "key1";
    char key2[] = "key2";
    char value1[] = "value1";
    char value2[] = "value2";

    MapResult const error1 = mapPut(map, key1, value1);
    ASSERT_EQ(MAP_SUCCESS, error1);
    ASSERT_TRUE(mapGet(map, key1));
    ASSERT_FALSE(strcmp(value1, (char *) mapGet(map, key1)));

    MapResult const error2 = mapPut(map, key2, value2);
    ASSERT_EQ(MAP_SUCCESS, error2);
    ASSERT_TRUE(mapGet(map, key2));
    ASSERT_FALSE(strcmp(value2, (char *) mapGet(map, key2)));

    MapResult const error3 = mapPut(map, key2, value1);
    ASSERT_EQ(MAP_SUCCESS, error3);
    ASSERT_TRUE(mapGet(map, key2));
    ASSERT_FALSE(strcmp(value1, (char *) mapGet(map, key2)));

    mapDestroy(map);
}

TEST(Map, TestRemove) {
    Map const map = createEmptyMap();

    char const key1[] = "key1";
    char const value1[] = "value1";
    char const key2[] = "key2";
    char const value2[] = "value2";

    ASSERT_EQ(0, mapGetSize(map));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key1, (void *) value1));
    ASSERT_EQ(1, mapGetSize(map));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key2, (void *) value2));
    ASSERT_EQ(2, mapGetSize(map));
    ASSERT_EQ(MAP_SUCCESS, mapRemove(map, (void *) key1));
    ASSERT_EQ(1, mapGetSize(map));
    ASSERT_EQ(MAP_ITEM_DOES_NOT_EXIST, mapRemove(map, (void *) key1));
    ASSERT_EQ(1, mapGetSize(map));
    ASSERT_EQ(MAP_SUCCESS, mapRemove(map, (void *) key2));
    ASSERT_EQ(0, mapGetSize(map));
    ASSERT_EQ(MAP_ITEM_DOES_NOT_EXIST, mapRemove(map, (void *) key1));
    ASSERT_EQ(0, mapGetSize(map));

    mapDestroy(map);
}

TEST(Map, TestClear) {
    Map const map = createEmptyMap();

    ASSERT_EQ(0, mapGetSize(map));

    char const key1[] = "key1";
    char const value1[] = "value1";
    char const key2[] = "key2";
    char const value2[] = "value2";

    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key1, (void *) value1));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key2, (void *) value2));
    ASSERT_EQ(MAP_SUCCESS, mapClear(map));
    ASSERT_EQ(0, mapGetSize(map));
    ASSERT_FALSE(mapGetNext(map));
    ASSERT_FALSE(mapGetFirst(map));
    ASSERT_FALSE(mapGetNext(map));

    mapDestroy(map);
}

TEST(Map, TestGetFirst) {
    Map const map = createEmptyMap();

    ASSERT_FALSE(mapGetFirst(map));

    char const key1[] = "key1";
    char const value1[] = "value1";
    char const key2[] = "key2";
    char const value2[] = "value2";
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key2, (void *) value2));

    char * const first1 = (char *) mapGetFirst(map);
    ASSERT_FALSE(strcmp(key2, first1));
    free(first1);

    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key1, (void *) value1));

    char * const first2 = (char *) mapGetFirst(map);
    ASSERT_FALSE(strcmp(key1, first2));
    free(first2);

    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key1, (void *) value1));

    char * const first3 = (char *) mapGetFirst(map);
    ASSERT_FALSE(strcmp(key1, first3));
    free(first3);

    mapDestroy(map);
}

TEST(Map, TestGetNext) {
    Map const map = createEmptyMap();

    ASSERT_FALSE(mapGetNext(map));

    char const value[] = "value";
    char const key1[] = "key1";
    char const key2[] = "key2";
    char const key3[] = "key3";
    char const key4[] = "key4";
    char const key5[] = "key5";

    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key3, (void *) value));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key4, (void *) value));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key1, (void *) value));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key5, (void *) value));
    ASSERT_EQ(MAP_SUCCESS, mapPut(map, (void *) key2, (void *) value));

    char * const first = (char *) mapGetFirst(map);
    ASSERT_FALSE(strcmp(key1, first));
    free(first);

    char * const second = (char *) mapGetNext(map);
    ASSERT_FALSE(strcmp(key2, second));
    free(second);

    char * const third = (char *) mapGetNext(map);
    ASSERT_FALSE(strcmp(key3, third));
    free(third);

    char * const fourth = (char *) mapGetNext(map);
    ASSERT_FALSE(strcmp(key4, fourth));
    free(fourth);

    char * const fifth = (char *) mapGetNext(map);
    ASSERT_FALSE(strcmp(key5, fifth));
    free(fifth);

    mapDestroy(map);
}

TEST(Map, TestMapPutMemLeak)
{
    Map map = createEmptyMap();
    char const value[] = "value";
    char const key1[] = "key1";
    char const key2[] = "key2";
    char const key3[] = "key3";
    char const key4[] = "key4";
    char const key5[] = "key5";

    // test override
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key3, (void *) value));
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key3, (void *) value));
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key3, (void *) value));

    // test addition
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key4, (void *) value));
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key1, (void *) value));
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key5, (void *) value));
    ASSERT_TRUE(MAP_SUCCESS == mapPut(map, (void *) key2, (void *) value));

    // test remove
    ASSERT_TRUE(MAP_SUCCESS == mapRemove(map, (void *) key2));
    ASSERT_TRUE(MAP_ITEM_DOES_NOT_EXIST == mapRemove(map, (void *) key2));
    ASSERT_TRUE(MAP_SUCCESS == mapRemove(map, (void *) key1));
    ASSERT_TRUE(MAP_SUCCESS == mapRemove(map, (void *) key3));
    ASSERT_TRUE(MAP_SUCCESS == mapRemove(map, (void *) key4));
    ASSERT_TRUE(MAP_SUCCESS == mapRemove(map, (void *) key5));

    mapDestroy(map);
}
