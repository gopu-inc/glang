#include "../../include/gramm/synt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- UTILITAIRES DE NAVIGATION ---

Token advance_token(GLParser* parser) {
    if (parser->current < parser->count) {
        return parser->tokens[parser->current++];
    }
    return parser->tokens[parser->count - 1];
}

Token peek_token(GLParser* parser) {
    return parser->tokens[parser->current];
}

bool check(GLParser* parser, TokenType type) {
    if (peek_token(parser).type == TK_EOF) return false;
    return peek_token(parser).type == type;
}

Token consume(GLParser* parser, TokenType type, const char* message) {
    if (check(parser, type)) return advance_token(parser);
    printf("[SYNTAX ERROR] Line %d: %s\n", peek_token(parser).line, message);
    parser->has_error = true;
    return peek_token(parser);
}

// --- GESTION DE L'ARBRE (AST) ---

ASTNode* create_node(NodeType type, const char* value, uint32_t line, uint32_t col) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->token_value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->child_count = 0;
    node->line = line;
    node->col = col;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    parent->children = realloc(parent->children, sizeof(ASTNode*) * (parent->child_count + 1));
    parent->children[parent->child_count] = child;
    parent->child_count++;
}

// --- LOGIQUE DE PARSING SPÉCIFIQUE ---

// Parse : outl("message") ou outl(variable)
ASTNode* parse_outl(GLParser* parser) {
    Token t = consume(parser, TK_KW_OUTL, "Expected 'outl'");
    ASTNode* node = create_node(NODE_OUTL, "outl", t.line, t.col);
    
    consume(parser, TK_LPAREN, "Expected '(' after outl");
    
    Token val = advance_token(parser);
    if (val.type == TK_LIT_STR || val.type == TK_ID) {
        NodeType nt = (val.type == TK_LIT_STR) ? NODE_LITERAL : NODE_IDENTIFIER;
        add_child(node, create_node(nt, val.lexeme, val.line, val.col));
    }
    
    consume(parser, TK_RPAREN, "Expected ')' after argument");
    return node;
}

// Parse : varl:str:name:"value" ou varl:str:name:val
ASTNode* parse_var_declaration(GLParser* parser) {
    Token start = consume(parser, TK_KW_VARL, "Expected 'varl'");
    
    consume(parser, TK_COLON, "Expected ':' after varl");
    Token type = advance_token(parser); // str, int, etc.
    consume(parser, TK_COLON, "Expected ':' after type");
    
    Token name = consume(parser, TK_ID, "Expected variable name");
    ASTNode* var_node = create_node(NODE_VAR_DECL, name.lexeme, start.line, start.col);
    
    if (check(parser, TK_COLON)) {
        consume(parser, TK_COLON, "Expected ':'");
        Token val = advance_token(parser);
        add_child(var_node, create_node(NODE_LITERAL, val.lexeme, val.line, val.col));
    }
    
    return var_node;
}

// Parse : def name() { ... }
ASTNode* parse_definition(GLParser* parser) {
    consume(parser, TK_KW_DEF, "Expected 'def'");
    Token name = consume(parser, TK_ID, "Expected function name");
    ASTNode* def_node = create_node(NODE_DEF, name.lexeme, name.line, name.col);
    
    consume(parser, TK_LPAREN, "Expected '('");
    consume(parser, TK_RPAREN, "Expected ')'");
    consume(parser, TK_LBRACE, "Expected '{'");
    
    while (!check(parser, TK_RBRACE) && !check(parser, TK_EOF)) {
        if (check(parser, TK_KW_OUTL)) add_child(def_node, parse_outl(parser));
        else if (check(parser, TK_KW_VARL)) add_child(def_node, parse_var_declaration(parser));
        else advance_token(parser); // Skip inconnu pour éviter boucle infinie
    }
    
    consume(parser, TK_RBRACE, "Expected '}'");
    return def_node;
}

// --- POINT D'ENTRÉE ---

ASTNode* synt_parse_program(GLParser* parser) {
    ASTNode* root = create_node(NODE_ROOT, "PROGRAM", 0, 0);

    while (peek_token(parser).type != TK_EOF) {
        Token t = peek_token(parser);
        
        if (t.type == TK_KW_PKG) {
            // Pour l'instant on skip le bloc package simple
            advance_token(parser);
            consume(parser, TK_LBRACE, "Expected '{' after package");
            while(!check(parser, TK_RBRACE) && !check(parser, TK_EOF)) advance_token(parser);
            consume(parser, TK_RBRACE, "Expected '}'");
        } 
        else if (t.type == TK_KW_DEF) {
            add_child(root, parse_definition(parser));
        } 
        else if (t.type == TK_KW_VARL) {
            add_child(root, parse_var_declaration(parser));
        } 
        else if (t.type == TK_KW_OUTL) {
            add_child(root, parse_outl(parser));
        }
        else {
            // On ignore les tokens non gérés à la racine pour ne pas crash
            advance_token(parser);
        }
    }
    
    return root;
}
