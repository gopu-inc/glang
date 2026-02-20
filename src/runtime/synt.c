#include <string.h>
// synt.c

#include "../../include/gramm/synt.h"
#include <stdio.h>
#include <stdlib.h>
ASTNode* synt_parse_program(GLParser* parser) {
    ASTNode* root = create_node(NODE_ROOT, "PROGRAM", 0, 0);
    
    // Règle G-Lang : Un fichier DOIT commencer par un package
    if (peek_token(parser).type == TK_KW_PKG) {
        add_child(root, parse_package_block(parser));
    } else {
        printf("[CRITICAL ERROR]: G-Lang files must start with a 'package' block.\n");
        parser->has_error = true;
    }

    while (peek_token(parser).type != TK_EOF) {
        // Parsing des sbar, def, class, etc.
        // Chaque mot-clé de key.h est ici aiguillé
    }
    
    return root;
}

ASTNode* create_node(NodeType type, const char* value, uint32_t line, uint32_t col) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->token_value = strdup(value);
    node->child_count = 0;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {}

Token peek_token(GLParser* parser) {
    return parser->tokens[parser->current];
}

ASTNode* parse_package_block(GLParser* parser) {
    return create_node(NODE_PACKAGE_BLOC, "pkg", 0, 0);
}

