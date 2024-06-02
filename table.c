#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "table.h"

#define TABLE_MAX_LOAD 0.75

void initable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entities = NULL;
}

void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entities, table->capacity);
    initTable(table);
}

static Entry* findEntry(Entry* entities, int capacity, ObjString* key) {
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entities[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL) tombstone = entry;                
            }
        } else if (entry->key == key) {
            return entry;
        }
        index = (index + 1) % capacity;
    } 
}

static void adjustTable(Table* table, int capacity) {
    
    Entry* entities = ALLOCATE(Entry, capacity);

    for (int i = 0; i < capacity; i++) {
        entities[i].key = NULL;
        entities[i].value = NIL_VAL;
    }

    table->count = 0;

    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entities[i];
        if (entry->key == NULL) continue;

        Entry* dest = findEntry(entities, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entities, table->capacity);

    table->entities = entities;
    table->capacity = capacity;
}

bool tableSet(Table* table, ObjString* key, Value value) {
    
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustTable(table, capacity);
    }

    Entry* entry = findEntry(table->entities, table->capacity, key);
    bool isNewKey = entry->key == NULL;

    if (isNewKey && IS_NIL(entry->value)) table->count++;
    
    entry->key = key;
    entry->value = value;
    return true;
}

bool tableDelete(Table* table, ObjString* key) {
    if (table->count == 0) return false;

    // Find the entry.
    Entry* entry = findEntry(table->entities, table->capacity, key);
    if (entry->key == NULL) return false;
    // Place a tombstone in the entry.
    entry->key = NULL;
    entry->value = BOOL_VAL(true);

    return true;
}

void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entities[i];
        if (entry->key != NULL) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash) {

    if (table->count == 0)  return NULL; 

    uint32_t index = hash % table->capacity; 

    for (;;) {
        Entry* entry = &table[index];
        if (entry->key == NULL) {
            // stop if we find an empty non-tombstone entry
            if (IS_NIL(entry->key)) return NULL;

        } else if (entry->key->length == length && 
                    entry->key->hash == hash && 
                        memcmp(entry->key->chars, chars, length) == 0) {
                // we found it
            return entry->key;
        }

        index = (index + 1) % table->capacity;
    }
}

bool tableGet(Table* table, ObjString* key, Value* value) {
    if (table->count == 0) return false;

    Entry* entry = findEntry(table->entities, table->capacity, key);
    if (entry->key == NULL) return false;

    *value = entry->value;
    return true;
}