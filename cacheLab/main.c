#include <stdio.h>
#include <math.h>
#include "cache.h"
#include <string.h>

int main() {
    int numOfCacheSets, // --- S
        numOfCacheLines, // -- E
        blockSize, // -------- B
        numOfBits, // -------- m
        hitTime, // ---------- h
        missPenalty, // ------ p
        hits   = 0,
        misses = 0,
        cycles = 0,
        LRU    = 0;

    char replacementPolicy[4] = {'\0'}; // - LFU or LRU

    double missRate = 0;

    FILE* file = fopen("C:\\Users\\davis\\CLionProjects\\assignment2\\cacheLab\\input.txt", "r");
    fscanf (file,"%d %d %d %d %s %d %d\n",
           &numOfCacheSets,
           &numOfCacheLines,
           &blockSize,
           &numOfBits,
           replacementPolicy,
           &hitTime,
           &missPenalty
           );

    int maxNumOfAddrs   = pow(2, numOfBits),// ----------------------- M
        setIndexBits    = log2(numOfCacheSets), // ----------------------- s
        blockOffsetBits = log2(blockSize), // ---------------------------- b
        tagBits         = numOfBits - (setIndexBits + blockOffsetBits), // t
        cacheSize       = blockSize * numOfCacheLines * numOfCacheSets; // C

    Line** set = malloc(sizeof (Line) * numOfCacheSets);
    cacheSetUp(set, numOfCacheSets,numOfCacheLines);

    long userInput = 0,
         setIndex  = 0,
         tag       = 0;

    int LRU_index  = 0,
        LFU_index  = 0,
        setCap     = 0;

    while (fscanf(file,"%lx", &userInput), userInput != -1){

        setIndex = (userInput >> blockOffsetBits) & ((1 << setIndexBits) - 1);
        tag = (userInput >> (setIndexBits + blockOffsetBits)) & ((1 << tagBits) - 1);

        Line* setLine = set[setIndex];

        int ifAppropriateCacheBlockSize = (numOfCacheLines > 1 && numOfCacheLines < cacheSize/blockSize) || numOfCacheLines == cacheSize/blockSize;

        if (numOfCacheLines == 1){
            if (setLine->validBit == 0 || (setLine ->validBit == 1 && setLine->tagBits != tag)){
                printf("%lx M\n", userInput);
                setLine->validBit = 1;
                setLine->tagBits = tag;
                misses++;
                cycles += missPenalty + hitTime;
            } else {
                printf("lx H \n", userInput);
                setLine->tagBits = tag;
                hits++;
                cycles += hitTime;
            }
        } else if (ifAppropriateCacheBlockSize) {
            setCap = 0;
            for(int i = 0; i < numOfCacheLines; i++){
                if(setLine[i].validBit == 0){
                    printf("%lx M\n", userInput);
                    setLine[i].validBit = 1;
                    setLine[i].tagBits = tag;
                    setLine[i].frequency = 0;
                    setLine[i].recentness = ++LRU;
                    misses++;
                    cycles += missPenalty + hitTime;
                    break;
                } else if (setLine[i].tagBits != tag){
                    setCap++;
                } else {
                    printf("%lx, H\n", userInput);
                    setLine[i].frequency++;
                    setLine[i].recentness = ++LRU;
                    hits++;
                    cycles += hitTime;
                    break;
                }
                int needToEvict = setCap == numOfCacheLines;
                if (needToEvict){
                    misses++;
                    cycles += missPenalty + hitTime;
                    printf("%lx M\n", userInput);

                    if(strcmp("LRU", replacementPolicy) == 0){
                        LRU_index = LRU_Finder(setLine, numOfCacheLines);
                        setLine[LRU_index].tagBits = tag;
                        setLine[LRU_index].recentness = ++LRU;
                    } else if (strcmp("LFU", replacementPolicy) == 0){
                        LFU_index = LFU_finder(setLine, numOfCacheLines);
                        setLine[LFU_index].tagBits = tag;
                        setLine[LRU_index].frequency = 0;
                    }
                }
            }
        }
    }
    missRate = ((double)misses/(hits + misses)) * 100;
    printf("%.f %d \n", missRate, cycles);
    freeAndClose(set, numOfCacheSets, file);
}