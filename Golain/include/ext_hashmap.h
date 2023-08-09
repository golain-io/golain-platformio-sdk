#pragma once 
#pragma once
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 100

typedef struct KeyValuePair KeyValuePair;

struct KeyValuePair {
    char* key;
    void* value;
    KeyValuePair* next;
};

typedef struct HashMap HashMap;

struct HashMap {
    KeyValuePair* buckets[HASHMAP_SIZE];
};

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + (*key);
        key++;
    }
    return hash % HASHMAP_SIZE;
}

KeyValuePair* createKeyValuePair(const char* key, void* value) {
    KeyValuePair* kvp = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    kvp->key = strdup(key);
    kvp->value = value;
    kvp->next = NULL;
    return kvp;
}

HashMap* createHashMap() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

void insert(HashMap* map, const char* key, void* value) {
    unsigned int index = hash(key);
    KeyValuePair* kvp = createKeyValuePair(key, value);
    kvp->next = map->buckets[index];
    map->buckets[index] = kvp;
}

void* get(HashMap* map, const char* key) {
    unsigned int index = hash(key);
    KeyValuePair* kvp = map->buckets[index];
    while (kvp != NULL) {
        if (strcmp(kvp->key, key) == 0) {
            return kvp->value;
        }
        kvp = kvp->next;
    }
    return NULL; // Key not found
}

void freeHashMap(HashMap* map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        KeyValuePair* kvp = map->buckets[i];
        while (kvp != NULL) {
            KeyValuePair* temp = kvp;
            kvp = kvp->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map);
}


HashMap* global_map = NULL;
// int main() {
//     HashMap* map = createHashMap();

//     int value1 = 5;
//     double value2 = 10.5;
//     char* value3 = "Hello, world!";

//     insert(map, "integer", &value1);
//     insert(map, "double", &value2);
//     insert(map, "string", value3);

//     int* intValue = (int*)get(map, "integer");
//     double* doubleValue = (double*)get(map, "double");
//     char* stringValue = (char*)get(map, "string");

//     if (intValue) {
//         printf("Integer value: %d\n", *intValue);
//     } else {
//         printf("Integer value not found\n");
//     }

//     if (doubleValue) {
//         printf("Double value: %lf\n", *doubleValue);
//     } else {
//         printf("Double value not found\n");
//     }

//     if (stringValue) {
//         printf("String value: %s\n", stringValue);
//     } else {
//         printf("String value not found\n");
//     }

//     freeHashMap(map);

//     return 0;
// }
