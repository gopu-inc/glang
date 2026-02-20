// octog.c

#include "../../include/gramm/octog.h"
#include "../../include/gramm/token.h"
#include "../../include/gramm/key.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Initialisation depuis un fichier
OctoReader* octo_init_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size <= 0) { fclose(file); return NULL; }
    rewind(file);

    uint8_t* buffer = malloc(size + 1);
    if (!buffer) { fclose(file); return NULL; }

    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0'; // Sentinel
    fclose(file);

    OctoReader* r = malloc(sizeof(OctoReader));
    if (!r) { free(buffer); return NULL; }
    
    r->raw_data = buffer;
    r->size = read_size;
    r->cursor = 0;
    r->line = 1;
    r->col = 1;
    return r;
}


uint8_t octo_next(OctoReader* r) {
    if (OCTO_IS_EOF(r)) return 0;
    uint8_t c = r->raw_data[r->cursor++];
    if (c == '\n') { r->line++; r->col = 1; }
    else { r->col++; }
    return c;
}

uint8_t octo_peek(OctoReader* r) {
    if (OCTO_IS_EOF(r)) return 0;
    return r->raw_data[r->cursor];
}

void octo_skip_space(OctoReader* r) {
    while (!OCTO_IS_EOF(r)) {
        uint8_t c = octo_peek(r);
        if (isspace(c)) { octo_next(r); }
        else if (c == '#') { // Commentaire G-Lang
            while (!OCTO_IS_EOF(r) && octo_peek(r) != '\n') octo_next(r);
        } else break;
    }
}

