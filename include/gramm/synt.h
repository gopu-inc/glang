// synt.h

#ifndef GLANG_SYNT_H
#define GLANG_SYNT_H

#include "token.h"
#include "noeud.h"
#include "gram.h"

/**
 * GL_PARSER
 * L'état vivant de l'analyseur pendant qu'il lit ton code .gl
 */
typedef struct {
    Token* tokens;           // Liste des tokens générés par le Lexer
    uint32_t current;        // Index du token en cours de lecture
    uint32_t count;          // Nombre total de tokens
    
    bool panic_mode;         // Flag pour éviter l'avalanche d'erreurs
    bool has_error;          // Indique si le code est invalide
    
    ParserState state;       // Vient de gram.h (Global, Pkg, Sbar, etc.)
} GLParser;

/**
 * FONCTIONS DE PILOTAGE
 */

// Initialise le parser avec les tokens du Lexer
void synt_init(GLParser* parser, Token* tokens, uint32_t count);

// Point d'entrée principal : transforme tout le fichier en un Arbre (AST)
ASTNode* synt_parse_program(GLParser* parser);

/**
 * LOGIQUE DE PARSING SPÉCIFIQUE (Gopu Style)
 */

// Gère le bloc initial : package { ... }
ASTNode* parse_package_block(GLParser* parser);

// Gère l'isolation : sbar Name { ... }
ASTNode* parse_sbar_declaration(GLParser* parser);

// Gère la déclaration de variables : varl:type: name = val
ASTNode* parse_var_declaration(GLParser* parser);

// Gère les fonctions et méthodes : def name(args) { ... }
ASTNode* parse_definition(GLParser* parser);

// Gère les appels système : sys(CMD, ARGS)
ASTNode* parse_sys_call(GLParser* parser);

/**
 * UTILITAIRES DE SÉCURITÉ
 */

// Vérifie si le token actuel est celui attendu, sinon lève une erreur
Token consume(GLParser* parser, TokenType type, const char* message);

// Regarde le token actuel sans le consommer
Token peek_token(GLParser* parser);

// Avance d'un token
Token advance_token(GLParser* parser);

// Synchronise le parser après une erreur pour continuer l'analyse
void synchronize(GLParser* parser);

#endif

