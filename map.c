#include <assert.h>
#include <stdlib.h>
#include "./mtm_map/map.h"


typedef struct MapElements *MapElement;

struct MapElements{
    MapDataElement data;
    MapKeyElement key;
	MapElement nextIndex;
};

struct Map_t{
    MapElement currentIndex;
    MapElement firstIndex;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
	freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if ( !copyDataElement || !copyKeyElement || !freeDataElement || !freeKeyElement || !compareKeyElements) {
		return NULL;
    }
    MapElement mapElement = malloc(sizeof(*mapElement));
    if (mapElement == NULL) {
		return NULL;
    }
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        free(mapElement);
		return NULL;
    }
	mapElement->data = NULL;
    mapElement->key = NULL;
    mapElement->nextIndex = NULL;
    map->currentIndex = mapElement;
    map->firstIndex = mapElement;
    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;
	return map;
}

void mapDestroy(Map map){
    if (map == NULL){
        return;
    }
    while ((map->firstIndex->nextIndex) != NULL){
        if ((map->firstIndex->nextIndex->data) != NULL){
            (map->freeDataElement)(map->firstIndex->nextIndex->data);
        }
        if ((map->firstIndex->nextIndex->key) != NULL){
            (map->freeKeyElement)(map->firstIndex->nextIndex->key);
        }
		MapElement toDelete = map->firstIndex->nextIndex;
		map->firstIndex->nextIndex =map->firstIndex->nextIndex->nextIndex;
		free(toDelete);
	}
    free(map->firstIndex);
    free(map);
}
Map mapCopy(Map map){
    if (map == NULL){
        return NULL;
    }
    MapElement newMapElement = malloc(sizeof(*(newMapElement)));
    if (newMapElement == NULL) {
		return NULL;
    }
    Map newMap = malloc(sizeof(*newMap));
    if (newMap == NULL) {
        free(newMapElement);
		return NULL;
    }
    newMapElement->data = NULL;
    newMapElement->key = NULL;
    newMapElement->nextIndex = NULL;
    newMap->currentIndex = newMapElement;
    newMap->firstIndex = newMapElement;
    newMap->copyDataElement = map->copyDataElement;
    newMap->copyKeyElement = map->copyKeyElement;
    newMap->freeDataElement = map->freeDataElement;
    newMap->freeKeyElement = map->freeKeyElement;
    newMap->compareKeyElements = map->compareKeyElements;
    map->currentIndex = map->firstIndex;
    while (map->currentIndex->nextIndex != NULL){
        MapElement *new_map_element_ptr = &(newMapElement->nextIndex);
        MapElement newMapElement = malloc(sizeof(*(newMapElement)));
        if (newMapElement == NULL) {
            mapDestroy (newMap);
		    return NULL;
        }
        *new_map_element_ptr = newMapElement;
        MapDataElement newDataElement = (map->copyDataElement)(map->currentIndex->nextIndex->data);
        MapKeyElement newKeyElement = (map->copyKeyElement)(map->currentIndex->nextIndex->key);
        if (newDataElement == NULL || newKeyElement == NULL){
            mapDestroy(newMap);
            return NULL;
        }
        newMapElement->data = newDataElement;
        newMapElement->key = newKeyElement;
        newMapElement->nextIndex = NULL;
        map->currentIndex = map->currentIndex->nextIndex;
    }
	return newMap;
}
int mapGetSize(Map map){
    if (map == NULL){
        return -1;
    }
    int element_count = 0;
    MapElement current_map = map->firstIndex;
    while (current_map->nextIndex != NULL){
        element_count++;
        current_map = current_map->nextIndex;
    }
    return element_count;
}

bool mapContains(Map map, MapKeyElement element){
    if (map == NULL || element == NULL){
        return false;
    }
    MapElement current_map = map->firstIndex;
    while (current_map->nextIndex != NULL){
        if ((map->compareKeyElements)((current_map->nextIndex->key),element) == 0){
            return true;
        }
        current_map = current_map->nextIndex;
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if (map == NULL || keyElement == NULL || dataElement == NULL){
        return MAP_NULL_ARGUMENT;
    }
    MapElement current_map = map->firstIndex;
    while (current_map->nextIndex != NULL){
        if ((map->compareKeyElements)((current_map->nextIndex->key),keyElement) == 0){
            (map->freeDataElement)(current_map->nextIndex->data);
            current_map->nextIndex->data = (map->copyDataElement)(dataElement);
            if (current_map->nextIndex->data == NULL){
                return MAP_OUT_OF_MEMORY;
            }
            return MAP_SUCCESS;
        }
        current_map = current_map->nextIndex;
    }
    map->currentIndex = map->firstIndex;
    MapElement newMapElement = malloc(sizeof(*(newMapElement)));
    if (newMapElement == NULL) {
	    return MAP_OUT_OF_MEMORY;
    }
    newMapElement->key = (map->copyKeyElement)(keyElement);
    if (newMapElement->key == NULL){
        free (newMapElement);
        return MAP_OUT_OF_MEMORY;
    }
    newMapElement->data = (map->copyDataElement)(dataElement);
    if (newMapElement->data == NULL){
        map->freeKeyElement(newMapElement->key);
        free(newMapElement);
        return MAP_OUT_OF_MEMORY;
    }
    while (map->currentIndex->nextIndex != NULL){
        if ((map->compareKeyElements)(keyElement,map->currentIndex->nextIndex->key) < 0){
            newMapElement->nextIndex = map->currentIndex->nextIndex;
            map->currentIndex->nextIndex = newMapElement;
            return MAP_SUCCESS;
        }
        map->currentIndex = map->currentIndex->nextIndex;
    }
    map->currentIndex->nextIndex = newMapElement;
    newMapElement->nextIndex = NULL;
    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if (map == NULL || keyElement == NULL){
        return NULL;
    }
    MapElement current_map_element = map->firstIndex;
    while (current_map_element->nextIndex != NULL){
        if ((map->compareKeyElements)((current_map_element->nextIndex->key),keyElement) == 0){
            return (current_map_element->nextIndex->data);
        }
        current_map_element = current_map_element->nextIndex;
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement){
    if(map == NULL || keyElement == NULL){
        return MAP_NULL_ARGUMENT;
    }
    map->currentIndex = map->firstIndex;
    while (map->currentIndex->nextIndex != NULL){
        if ((map->compareKeyElements)((map->currentIndex->nextIndex->key), keyElement) == 0){
            MapElement map_element_ptr = (map->currentIndex->nextIndex->nextIndex);
            (map->freeDataElement)(map->currentIndex->nextIndex->data);
            (map->freeKeyElement)(map->currentIndex->nextIndex->key);
            free(map->currentIndex->nextIndex);
            map->currentIndex->nextIndex = map_element_ptr;
            return MAP_SUCCESS;
        }
        map->currentIndex = map->currentIndex->nextIndex;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map){
    if (map == NULL || (map->firstIndex->nextIndex) == NULL){
        return NULL;
    }
    map->currentIndex = map->firstIndex->nextIndex;
    MapKeyElement key_copy = (map->copyKeyElement)(map->currentIndex->key);
    if (key_copy == NULL){
        return NULL;
    }
    return key_copy;
}

MapKeyElement mapGetNext(Map map){
    if (map == NULL || (map->currentIndex) == NULL || (map->currentIndex->nextIndex == NULL)){
        return NULL;
    }
    map->currentIndex = map->currentIndex->nextIndex;
    MapKeyElement key_copy = (map->copyKeyElement)(map->currentIndex->key);
    if (key_copy == NULL){
        return NULL;
    }
    return key_copy;
}

MapResult mapClear(Map map){
    if (map == NULL){
        return MAP_NULL_ARGUMENT;
    }
    while ((map->firstIndex->nextIndex) != NULL){
        (map->freeDataElement)(map->firstIndex->nextIndex->data);
        (map->freeKeyElement)(map->firstIndex->nextIndex->key);
		MapElement toDelete = map->firstIndex->nextIndex;
		map->firstIndex->nextIndex = map->firstIndex->nextIndex->nextIndex;
		free(toDelete);
	}
    return MAP_SUCCESS;
}
