#include "gramm/octog.h"
#include "gramm/token.h"
#include "gramm/key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void execute_outl(const char* message);
void set_var(const char* name, const char* value);
char* get_var(const char* name);
Token lexer_next_token(OctoReader* r);

bool verbose_mode = false;

// Sécurité contre les boucles infinies
void safe_free(Token* t) {
    if (t->lexeme) {
        free(t->lexeme);
        t->lexeme = NULL;
    }
}

void skip_block(OctoReader* reader) {
    int depth = 1; 
    while (depth > 0) {
        Token t = lexer_next_token(reader);
        if (t.type == TK_EOF) break;
        if (t.type == TK_LBRACE) depth++;
        if (t.type == TK_RBRACE) depth--;
        safe_free(&t);
    }
}

void run_gopu_engine(OctoReader* r) {
    Token t;
    while ((t = lexer_next_token(r)).type != TK_EOF) {
        
        // --- GESTION DES VARIABLES (Même hors main) ---
        if (t.type == TK_KW_VARL) {
            while(t.type != TK_COLON && t.type != TK_EOF) { safe_free(&t); t = lexer_next_token(r); }
            safe_free(&t); t = lexer_next_token(r); // type
            while(t.type != TK_COLON && t.type != TK_EOF) { safe_free(&t); t = lexer_next_token(r); }
            safe_free(&t); t = lexer_next_token(r); // name
            char* name = t.lexeme ? strdup(t.lexeme) : NULL;
            while(t.type != TK_COLON && t.type != TK_EOF) { safe_free(&t); t = lexer_next_token(r); }
            safe_free(&t); t = lexer_next_token(r); // value
            if (name && t.lexeme) set_var(name, t.lexeme);
            if (name) free(name);
        }

        // --- GESTION DES SORTIES ---
        else if (t.type == TK_KW_OUTL) {
            while(t.type != TK_LPAREN && t.type != TK_EOF) { safe_free(&t); t = lexer_next_token(r); }
            safe_free(&t); t = lexer_next_token(r);
            if (t.type == TK_LIT_STR) execute_outl(t.lexeme);
            else if (t.type == TK_ID) {
                char* v = get_var(t.lexeme);
                if (v) execute_outl(v);
            }
        }
        
        // --- GESTION DES DEFINITIONS (On entre dans tout ce qui s'appelle main) ---
        else if (t.type == TK_KW_DEF) {
            Token name = lexer_next_token(r);
            if (name.lexeme && strcmp(name.lexeme, "main") != 0) {
                skip_block(r); // On ignore les autres
            }
            safe_free(&name);
        }

        safe_free(&t);
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3) return 1;
    if (argc > 3 && strcmp(argv[3], "--verbose") == 0) verbose_mode = true;
    OctoReader* reader = octo_init_file(argv[2]);
    if (!reader) return 1;
    if (verbose_mode) printf("[G-SYSTEM] Engine Boot... OK\n");
    run_gopu_engine(reader);
    return 0;
}

