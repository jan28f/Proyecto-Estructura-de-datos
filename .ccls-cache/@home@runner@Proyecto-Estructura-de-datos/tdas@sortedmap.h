#ifndef SORTEDMAP_h
#define SORTEDMAP_h

typedef struct TreeMap TreeMap;

typedef struct Pair {
     void * key;
     void * value;
} Pair;

TreeMap * createSortedMap(int (*lower_than_int) (void* key1, void* key2));

void insertSortedMap(TreeMap * tree, void* key, void * value);

void eraseSortedMap(TreeMap * tree, void* key);

Pair * searchSortedMap(TreeMap * tree, void* key);

Pair * upperBound(TreeMap * tree, void* key);

Pair * firstSortedMap(TreeMap * tree);

Pair * nextSortedMap(TreeMap * tree);

#endif /* SORTEDMAP_h */
