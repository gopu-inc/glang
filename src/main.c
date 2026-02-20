#include "gramm/octog.h"
#include "gramm/token.h"
#include "gramm/key.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Prototype de la fonction engine
void engine_run(ASTNode* root);
Token lexer_next_token(OctoReader* r);

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: gl run <file.gl>\n");
        return 1;
    }

    // 1. Initialisation de l'OctoReader
    OctoReader* reader = octo_init_file(argv[2]);
    if (!reader) {
        printf("Error: Could not open file %s\n", argv[2]);
        return 1;
    }

    // 2. Lexing complet (on stocke tous les tokens d'abord)
    uint32_t capacity = 1024;
    Token* token_list = malloc(sizeof(Token) * capacity);
    uint32_t token_count = 0;

    while (1) {
        Token t = lexer_next_token(reader);
        if (token_count >= capacity) {
            capacity *= 2;
            token_list = realloc(token_list, sizeof(Token) * capacity);
        }
        token_list[token_count++] = t;
        if (t.type == TK_EOF) break;
    }

    // 3. Initialisation du Parser
    GLParser parser;
    parser.tokens = token_list;
    parser.current = 0;
    parser.count = token_count;
    parser.has_error = false;
    parser.panic_mode = false;

    // 4. Parsing
    ASTNode* root = synt_parse_program(&parser);

    // 5. Exécution par l'Engine (Seulement si pas d'erreur)
    if (root && !parser.has_error) {
        engine_run(root);
    } else {
        printf("[CRITICAL]: Parser failed or AST is empty.\n");
    }

    // Nettoyage (optionnel pour test)
    // free(token_list); 
    return 0;
}
