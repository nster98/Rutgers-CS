#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

typedef struct Node
{
    unsigned long long int tag;
}
Node;

struct Node** hashp;

// No Prefetch
int reads = 0;
int writes = 0;
int hits = 0;
int misses = 0;

// With Prefetch
int preads = 0;
int pwrites = 0;
int phits = 0;
int pmisses = 0;

int pref = 0;

struct Node** createHash(int setCount, int assoc)
{
    struct Node** hash = (struct Node**) malloc(sizeof(struct Node*) * setCount);

    for (int i = 0; i < setCount; i++)
    {
        hash[i] = (struct Node*) malloc(sizeof(struct Node) * assoc);

        for (int j = 0; j < assoc; j++)
        {
            hash[i][j].tag = 0;
        }
    }

    return hash;
}

void add(int assoc, unsigned long long int index, unsigned long long int tag)
{
    if (hashp[index][0].tag == 0)
        hashp[index][0].tag = tag;
    else
    {
        for (int i = assoc - 2; i >= 0; i--)
        {
            if (hashp[index][0].tag == 0)
                continue;
            else
                hashp[index][i + 1].tag = hashp[index][i].tag;
        }
        hashp[index][0].tag = tag;
    }
}

void shiftUp(int assoc, unsigned long long int index, int j)
{
    unsigned long long int tempTag = hashp[index][j].tag;

    for (int i = j - 1; i >= 0; i--)
    {
        if (hashp[index][i].tag == 0)
            continue;
        else
            hashp[index][i + 1].tag = hashp[index][i].tag;
    }
    hashp[index][0].tag = tempTag;
}

void clean(int setCount, int assoc)
{
    for (int i = setCount - 1; i >= 0; i--)
    {
        free(hashp[i]);
    }
    free(hashp);
}

int execute(int setCount, int assoc, char command, unsigned long long int index, unsigned long long int tag)
{
    int exists = 0;
    int ret = 0;
    int location;

    for (int i = 0; i < assoc; i++)
    {
        if (hashp[index][i].tag == tag)
        {
            exists = 1;
            location = i;
            break;
        }
    }

    if (exists == 1 && command == 'R')
    {
        shiftUp(assoc, index, location);

        if (pref == 0)
        {
            hits++;
            ret = 1;
        }
        else
        {
            phits++;
            ret = 1;
        }
    }
    else if (exists == 0 && command == 'R')
    {
        if (pref == 0)
        {
            misses++;
            reads++;
            add(assoc, index, tag);
            ret = 2;
        }
        else
        {
            pmisses++;
            preads++;
            add(assoc, index, tag);
            ret = 2;
        }
    }
    else if (exists == 1 && command == 'W')
    {
        shiftUp(assoc, index, location);

        if (pref == 0)
        {
            hits++;
            writes++;
            ret = 3;
        }
        else
        {
            phits++;
            pwrites++;
            ret = 3;
        }
    }
    else if (exists == 0 && command == 'W')
    {
        if (pref == 0)
        {
            misses++;
            reads++;
            writes++;
            add(assoc, index, tag);
            ret = 4;
        }
        else
        {
            pmisses++;
            preads++;
            pwrites++;
            add(assoc, index, tag);
            ret = 4;
        }
    }
    else if (exists == 0 && command == 'P')
    {
        preads++;
        add(assoc, index, tag);
        ret = 5;
    }

    return ret;
}

int main(int argc, char* argv[])
{
    int cacheSize = atoi(argv[1]);
    char* associativity = argv[2];
    char* policy = argv[3];
    int blockSize = atoi(argv[4]);

    int assoc;
    int setCount = 0;

    // Check to see if cache size is power of 2
    if (ceil(log(cacheSize) / log(2)) != floor(log(cacheSize) / log(2)))
    {
        printf("error\n");
        return 0;
    }

    if (strcmp(policy, "lru") != 0)
    {
        printf("error\n");
        return 0;
    }

    // C = S * A * B (cacheSize = setCount * assoc * blockSize)
    if (strcmp(associativity, "direct") == 0)
        assoc = 1; // Direct: A = 1
    else if (strcmp(associativity, "assoc") == 0)
        assoc = cacheSize / blockSize; // Full Associative: S = 1
    else if (associativity[5] == ':')
    {
        assoc = atoi(&associativity[6]); // MIGHT NEED TO CHANGE THIS, DOES NOT GET NUMBERS BIGGER THAN 8
        if (ceil(log(assoc) / log(2)) != floor(log(assoc) / log(2)))
        {
            printf("error\n");
            return 0;
        }
    }

    if (strcmp(associativity, "assoc") == 0)
        setCount = 1;
    else
        setCount = cacheSize / (assoc * blockSize);

    hashp = createHash(setCount, assoc);

    FILE* file = fopen(argv[5], "r");
    if (file == NULL)
    {
        printf("error\n");
        return 0;
    }

    char cmd;
    unsigned long long int address;

    int obits = log(blockSize) / log(2);
    int ibits = log(setCount) / log(2);

    while (fscanf(file, "%*s %c %llx", &cmd, &address) == 2)
    {
        unsigned long long int index = address >> obits;

        if (ibits == 0)
            index = 0;
        else
            index = index % (unsigned long long int)pow(2, ibits);

        unsigned long long int tag = address >> (obits + ibits);

        execute(setCount, assoc, cmd, index, tag);
    }

    pref = 1;
    clean(setCount, assoc); // MIGHT MAKE THIS A FUNCTION
    hashp = createHash(setCount, assoc);
    rewind(file);

    while (fscanf(file, "%*s %c %llx", &cmd, &address) == 2)
    {
        unsigned long long int index = address >> obits;

        if (ibits == 0)
            index = 0;
        else
            index = index % (unsigned long long int)pow(2, ibits);

        unsigned long long int tag = address >> (obits + ibits);

        int miss = execute(setCount, assoc, cmd, index, tag);

        if (miss == 2 || miss == 4)
        {
            unsigned long long int address2 = address + blockSize;
            unsigned long long int index2 = address2 >> obits;

            if (ibits == 0)
                index2 = 0;
            else
                index2 = index2 % (unsigned long long int)pow(2, ibits);

            unsigned long long int tag2 = address2 >> (obits + ibits);

            execute(setCount, assoc, 'P', index2, tag2);
        }
    }

    clean(setCount, assoc);

    printf("no-prefetch\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", reads, writes, hits, misses);
    printf("with-prefetch\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", preads, pwrites, phits, pmisses);

    fclose(file);

    return 0;
}
