#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* name;
    char* value;
} GVariable;

GVariable storage[100]; // Capacité de 100 variables pour commencer
int var_count = 0;

void set_var(const char* name, const char* value) {
    for(int i=0; i<var_count; i++) {
        if(strcmp(storage[i].name, name) == 0) {
            free(storage[i].value);
            storage[i].value = strdup(value);
            return;
        }
    }
    storage[var_count].name = strdup(name);
    storage[var_count].value = strdup(value);
    var_count++;
}

char* get_var(const char* name) {
    for(int i=0; i<var_count; i++) {
        if(strcmp(storage[i].name, name) == 0) return storage[i].value;
    }
    return NULL;
}

