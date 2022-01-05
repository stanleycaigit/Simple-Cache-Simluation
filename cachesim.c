#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int intlog2(unsigned int x){
    unsigned int y = 0;
    while(x > 1){
        x >>= 1;
        ++y;
    }
    return y;
}

void directA(int cacheSize, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = (cacheSize/blockSize);
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    long int *cache = (long int*)malloc(sizeof(long int)*sets);
    for(int i = 0; i < sets; i++){
        cache[i] = -1;
    }
    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        address >>= bBits;
        long int setIdx = address & ((1 << sBits) - 1);
        long int tag = address >> sBits;

       if(operation == 'R'){
           if(cache[setIdx] != tag){
               misses++;
               reads++;
               cache[setIdx] = tag;
           }else{
               hits++;
           }
       }else if(operation == 'W'){
           if(cache[setIdx] == tag){
               hits++;
               writes++;
           }else{
                misses++;
                reads++;
                cache[setIdx] = tag;
                writes++;
           }
       }
    }
    free(cache);
    printf("Prefetch 0\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}
void directAPF(int cacheSize, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = (cacheSize/blockSize);
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    long int *cache = (long int*)malloc(sizeof(long int)*sets);
    for(int i = 0; i < sets; i++){
        cache[i] = -1;
    }
    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        long int pfAddress = (address + blockSize) >> bBits;
        long int pfsetIdx = pfAddress & ((1 << sBits) - 1);
        long int pfTag = pfAddress >> sBits;

        address >>= bBits;
        long int setIdx = address & ((1 << sBits) - 1);
        long int tag = address >> sBits;

       if(operation == 'R'){
           if(cache[setIdx] != tag){
               misses++;
               reads++;
               cache[setIdx] = tag;
               if(cache[pfsetIdx] != pfTag){
                   reads++;
                   cache[pfsetIdx] = pfTag;
               }
           }else{
               hits++;
           }
       }else if(operation == 'W'){
           if(cache[setIdx] == tag){
               hits++;
               writes++;
           }else{
                misses++;
                reads++;
                cache[setIdx] = tag;
                writes++;
                if(cache[pfsetIdx] != pfTag){
                   reads++;
                   cache[pfsetIdx] = pfTag;
               }
           }
       }
    }
    free(cache);
    printf("Prefetch 1\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}
void fullA(int cacheSize, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = 1;
    int lines = (cacheSize/blockSize);
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    long int *cache = (long int*)malloc(sizeof(long int)*lines);
    for(int i = 0; i < lines; i++){
        cache[i] = -1;
    }
    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        address >>= bBits;
        long int tag = address >> sBits;

        if(operation == 'R'){
            int i;
            for(i = 0; i < lines; i++){
                if(cache[i] == tag){
                    hits++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[i] = cache[i-1];
                }
                cache[0] = tag;
            }
        }else if(operation == 'W'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[i] == tag){
                    hits++;
                    writes++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[i] = cache[i-1];
                }
                cache[0] = tag;
                writes++;
            }
        }
    }
    free(cache);
    printf("Prefetch 0\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}
void fullAPF(int cacheSize, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = 1;
    int lines = (cacheSize/blockSize);
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    long int *cache = (long int*)malloc(sizeof(long int)*lines);
    for(int i = 0; i < lines; i++){
        cache[i] = -1;
    }
    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        long int pfAddress = (address + blockSize) >> bBits;
        long int pfTag = pfAddress >> sBits;

        address >>= bBits;
        long int tag = address >> sBits;

        if(operation == 'R'){
            int i;
            for(i = 0; i < lines; i++){
                if(cache[i] == tag){
                    hits++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[i] = cache[i-1];
                }
                cache[0] = tag;
                //check if prefetching is in cache
                for(i = 0; i < lines; i++){
                    if(cache[i] == pfTag){
                        break;
                    }
                }
                if(i == lines){
                    reads++;
                    //shift all elements in cache one position right
                    for(i = lines-1; i > 0; i--){
                        cache[i] = cache[i-1];
                    }
                    cache[0] = pfTag;
                }
            }
        }else if(operation == 'W'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[i] == tag){
                    hits++;
                    writes++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[i] = cache[i-1];
                }
                cache[0] = tag;
                writes++;
                //check if prefetching is in cache
                for(i = 0; i < lines; i++){
                    if(cache[i] == pfTag){
                        break;
                    }
                }
                if(i == lines){
                    reads++;
                    //shift all elements in cache one position right
                    for(i = lines-1; i > 0; i--){
                        cache[i] = cache[i-1];
                    }
                    cache[0] = pfTag;
                }
            }
        }
    }
    free(cache);
    printf("Prefetch 1\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}
void nWayA(int cacheSize, int n, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = (cacheSize/(blockSize*n));
    int lines = n;
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    //make 2-D array cache
    long int** cache = (long int**)malloc(sizeof(long int*) * sets);
    cache[0] = (long int*)malloc(sizeof(long int) * sets * lines);
    for(int i = 1; i < sets; ++i){
        cache[i] = cache[i-1] + lines;
    }
    for(int i = 0; i < sets; i++){
        for(int j = 0; j < lines; j++){
            cache[i][j] = -1;
        }
    }

    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        address >>= bBits;
        long int setIdx = address & ((1 << sBits) - 1);
        long int tag = address >> sBits;

        if(operation == 'R'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[setIdx][i] == tag){
                    hits++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[setIdx][i] = cache[setIdx][i-1];
                }
                cache[setIdx][0] = tag;
            }
        }else if(operation == 'W'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[setIdx][i] == tag){
                    hits++;
                    writes++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[setIdx][i] = cache[setIdx][i-1];
                }
                cache[setIdx][0] = tag;
                writes++;
            }
        }
    }

    free(cache[0]);
    free(cache);

    printf("Prefetch 0\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}
void nWayAPF(int cacheSize, int n, int blockSize, char* fileName){
    int hits = 0, misses = 0, reads = 0, writes = 0;

    int sets = (cacheSize/(blockSize*n));
    int lines = n;
    int bBits = intlog2(blockSize);
    int sBits = intlog2(sets);
    //make 2-D array cache
    long int** cache = (long int**)malloc(sizeof(long int*) * sets);
    cache[0] = (long int*)malloc(sizeof(long int) * sets * lines);
    for(int i = 1; i < sets; ++i){
        cache[i] = cache[i-1] + lines;
    }
    for(int i = 0; i < sets; i++){
        for(int j = 0; j < lines; j++){
            cache[i][j] = -1;
        }
    }

    char operation;
    long int address;

    FILE* traces = fopen(fileName, "r");
    while(1<2){
        fscanf(traces, "%*x");
        fscanf(traces, " %c %c", &operation, &operation);
        fscanf(traces, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        long int pfAddress = (address + blockSize) >> bBits;
        long int pfsetIdx = pfAddress & ((1 << sBits) - 1);
        long int pfTag = pfAddress >> sBits;

        address >>= bBits;
        long int setIdx = address & ((1 << sBits) - 1);
        long int tag = address >> sBits;

        if(operation == 'R'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[setIdx][i] == tag){
                    hits++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[setIdx][i] = cache[setIdx][i-1];
                }
                cache[setIdx][0] = tag;
                //check if prefetching tag is in cache
                for(i = 0; i<lines; i++){
                    if(cache[pfsetIdx][i] == pfTag){
                        break;
                    }
                }
                //prefetching tag is not in cache
                if(i == lines){
                    reads++;
                    //shift all elements in cache one position right
                    for(i = lines-1; i > 0; i--){
                        cache[pfsetIdx][i] = cache[pfsetIdx][i-1];
                    }
                    cache[pfsetIdx][0] = pfTag;
                }
            }
        }else if(operation == 'W'){
            int i;
            for(i = 0; i<lines; i++){
                if(cache[setIdx][i] == tag){
                    hits++;
                    writes++;
                    break;
                }
            }
            //tag is not in cache
            if(i == lines){
                misses++;
                reads++;
                
                //shift all elements in cache one position right
                for(i = lines-1; i > 0; i--){
                    cache[setIdx][i] = cache[setIdx][i-1];
                }
                cache[setIdx][0] = tag;
                writes++;
                //check if prefetching tag is in cache
                for(i = 0; i<lines; i++){
                    if(cache[pfsetIdx][i] == pfTag){
                        break;
                    }
                }
                //prefetching tag is not in cache
                if(i == lines){
                    reads++;
                    //shift all elements in cache one position right
                    for(i = lines-1; i > 0; i--){
                        cache[pfsetIdx][i] = cache[pfsetIdx][i-1];
                    }
                    cache[pfsetIdx][0] = pfTag;
                }
            }
        }
    }

    free(cache[0]);
    free(cache);

    printf("Prefetch 1\n");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
}

int main(int argc, char** argv){

    int cacheSize = atoi(argv[1]);
    int blockSize = atoi(argv[4]);
    //read trace file
    int trace;
    for(trace = 0; argv[5][trace] != '\0'; trace++){
    }trace++;
    char* fileName = (char*)malloc(sizeof(char*)*trace);
    strcpy(fileName, argv[5]);

    if(strcmp(argv[2], "direct") == 0){
        directA(cacheSize, blockSize, fileName);
        directAPF(cacheSize, blockSize, fileName);
    }else if(strcmp(argv[2], "assoc") ==0){
        fullA(cacheSize, blockSize, fileName);
        fullAPF(cacheSize, blockSize, fileName);
    }else{
        int nWay;
        sscanf(argv[2], "assoc:%d", &nWay);
        nWayA(cacheSize, nWay, blockSize, fileName);
        nWayAPF(cacheSize,nWay, blockSize, fileName);
    }

    free(fileName);
    return 0;

}
