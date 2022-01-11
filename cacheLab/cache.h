#ifndef CACHELAB_CACHE_H
#define CACHELAB_CACHE_H
#include <stdlib.h>
typedef struct {
    int validBit;
    int tagBits;
    int block;
    int frequency;
    int recentness;
} Line;

int LRU_Finder(Line* setLine, int numOfCacheLines){
    int LRU_index = 0;
    for (int i = 0; i < numOfCacheLines; i++){
        if (setLine[i].recentness < setLine[LRU_index].recentness){
            LRU_index = i;
        }
    }
    return LRU_index;
}

int LFU_finder(Line* setLine, int numOfCacheLines){
    int LFU_index = 0;
    for (int i = 0; i < numOfCacheLines; i++){
        if (setLine[i].frequency < setLine[LFU_index].frequency){
            LFU_index = i;
        }
    }
    return LFU_index;
}

void cacheSetUp (Line** set, int numOfCacheSets, int numOfCacheLines){
    for (int i = 0; i < numOfCacheSets; i++){
        set[i] = malloc(sizeof (Line) * numOfCacheLines);
        for (int j = 0; j < numOfCacheLines; j++){
            set[i][j].validBit = 0;
            set[i][j].tagBits = 0;
            set[i][j].block = 0;
            set[i][j].frequency = 0;
            set[i][j].recentness = 0;
        }
    }
}

void freeAndClose(Line** set, int numOfCacheSets, FILE* file){
    for (int i = 0; i < numOfCacheSets; i++) {
        free(set[i]);
    }
    free(set);
    fclose(file);
}

#endif //CACHELAB_CACHE_H
