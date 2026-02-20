#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gramm/octog.h"
#include "gramm/token.h"
#include "gramm/noeud.h"  // Ajout indispensable pour ASTNode
#include "gramm/synt.h"   // Contient GLParser

// Déclarations externes pour éviter les warnings
extern ASTNode* synt_parse_program(GLParser* parser);
extern void engine_run(ASTNode* root);
extern Token lexer_next_token(OctoReader* r);

int main(int argc, char** argv) {
    // Vérification G-Lang : gl run <fichier>
    if (argc < 3) {
        printf("Gopu Runtime v1.0\nUsage: gl run <file.gl>\n");
        return 1;
    }

    // 1. Initialisation du lecteur Octo
    OctoReader* reader = octo_init_file(argv[2]);
    if (!reader) {
        printf("[CRITICAL]: Could not open source file: %s\n", argv[2]);
        return 1;
    }

    // 2. Lexing : Transformation du texte en liste de Tokens
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

    // 3. Configuration du Parser avec les tokens récoltés
    GLParser parser;
    parser.tokens = token_list;
    parser.current = 0;
    parser.count = token_count;
    parser.has_error = false;
    parser.panic_mode = false;

    // 4. Parsing : Construction de l'Arbre Syntaxique (AST)
    ASTNode* root = synt_parse_program(&parser);

    // 5. Exécution : L'Engine Gopu fait le travail
    if (root && !parser.has_error) {
        engine_run(root);
    } else {
        printf("[FAILED]: Compilation error. Execution aborted.\n");
    }

    // On laisse le système nettoyer la mémoire à la fermeture pour les tests
    return 0;
}
