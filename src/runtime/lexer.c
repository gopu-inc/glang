// lexer.c

#include "../../include/gramm/token.h"
#include "../../include/gramm/octog.h"
#include "../../include/gramm/key.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour vérifier si un identifiant est un mot-clé G-Lang
TokenType identify_keyword(const char* s) {
    // --- MOTS CLÉS DE STRUCTURE ---
    if (strcmp(s, K_PKG) == 0)   return TK_KW_PKG;
    if (strcmp(s, K_BAR) == 0)   return TK_KW_SBAR;
    if (strcmp(s, K_DEF) == 0)   return TK_KW_DEF;
    if (strcmp(s, K_CLS) == 0)   return TK_KW_CLS;
    if (strcmp(s, K_EXT) == 0)   return TK_KW_EXT;
    if (strcmp(s, K_RET) == 0)   return TK_KW_RET;
    if (strcmp(s, K_FOR) == 0)   return TK_KW_FOR;
    if (strcmp(s, K_NON) == 0)   return TK_KW_NONLOCAL;
    if (strcmp(s, K_MUT) == 0)   return TK_OP_MUNT; 

    // --- MOTS CLÉS D'ACTIONS & I/O ---
    if (strcmp(s, K_WRITE) == 0) return TK_KW_WRITE;
    if (strcmp(s, K_READ) == 0)  return TK_KW_READ;
    if (strcmp(s, K_OUTL) == 0)  return TK_KW_OUTL;
    if (strcmp(s, K_LOGL) == 0)  return TK_KW_LOGL;
    if (strcmp(s, K_IN) == 0)    return TK_KW_IN;
    if (strcmp(s, K_STOP) == 0)  return TK_KW_STOP;
    if (strcmp(s, K_EXEC) == 0)  return TK_KW_EXEC;
    if (strcmp(s, K_INPL) == 0)  return TK_KW_INPL;
    if (strcmp(s, K_SYS) == 0)   return TK_KW_SYS;

    // --- MOTS CLÉS LOGIQUES & ÉTATS ---
    if (strcmp(s, K_VAR) == 0)   return TK_KW_VARL;
    if (strcmp(s, K_IF) == 0)    return TK_KW_IF;
    if (strcmp(s, K_ELSE) == 0)  return TK_KW_ELSE;
    if (strcmp(s, K_WHILE) == 0) return TK_KW_WHILE;
    if (strcmp(s, K_XCP) == 0)   return TK_KW_EXCP;
    if (strcmp(s, K_TRY) == 0)   return TK_KW_TENT;
    if (strcmp(s, K_SYNC) == 0)  return TK_KW_ATOMIC;

    // --- GESTION DES MOTS MANQUANTS (Temporaire) ---
    // Ces mots-clés ne sont pas encore dans ton enum TokenType (token.h)
    // On les fait tomber dans TK_ID pour que ça compile
    if (strcmp(s, K_WAIT) == 0)  return TK_ID; 
    if (strcmp(s, K_LINK) == 0)  return TK_ID;
    if (strcmp(s, K_CAST) == 0)  return TK_ID; // morph
    if (strcmp(s, K_HIDE) == 0)  return TK_ID; // priv
    if (strcmp(s, K_OPEN) == 0)  return TK_ID; // publ
    if (strcmp(s, K_VOID) == 0)  return TK_ID; // nulla

    return TK_ID;
}

Token lexer_next_token(OctoReader* r) {
    Token t;
    t.lexeme = NULL; // Sécurité initiale
    
    // 1. Sécurité : si le lecteur est invalide
    if (!r || !r->raw_data || OCTO_IS_EOF(r)) {
        t.type = TK_EOF;
        t.line = r ? r->line : 0;
        t.col = r ? r->col : 0;
        return t;
    }

    octo_skip_space(r);
    
    t.line = r->line;
    t.col = r->col;

    if (OCTO_IS_EOF(r)) { 
        t.type = TK_EOF; 
        return t; 
    }

    uint8_t c = octo_peek(r);

    // 2. Gestion des Strings "..."
    if (c == '"') {
        octo_next(r);
        char buf[1024]; int i = 0;
        while (!OCTO_IS_EOF(r) && octo_peek(r) != '"' && i < 1023) {
            buf[i++] = octo_next(r);
        }
        if (!OCTO_IS_EOF(r)) octo_next(r); // Skip le " fermant
        buf[i] = '\0';
        t.type = TK_LIT_STR;
        t.lexeme = strdup(buf);
        return t;
    }

    // 3. Gestion des Identifiants et Mots-clés
    if (isalpha(c) || c == '_') {
        char buf[256]; int i = 0;
        while (!OCTO_IS_EOF(r) && (isalnum(octo_peek(r)) || octo_peek(r) == '_') && i < 255) {
            buf[i++] = octo_next(r);
        }
        buf[i] = '\0';
        t.type = identify_keyword(buf); 
        t.lexeme = strdup(buf);
        return t;
    }

    // 4. Gestion des Nombres (Int et Float)
    if (isdigit(c)) {
        char buf[64]; int i = 0;
        while (!OCTO_IS_EOF(r) && isdigit(octo_peek(r)) && i < 63) {
            buf[i++] = octo_next(r);
        }
        if (!OCTO_IS_EOF(r) && octo_peek(r) == '.') {
            buf[i++] = octo_next(r); // Consomme le point
            while (!OCTO_IS_EOF(r) && isdigit(octo_peek(r)) && i < 63) {
                buf[i++] = octo_next(r);
            }
            t.type = TK_LIT_FL;
        } else {
            t.type = TK_LIT_INT;
        }
        buf[i] = '\0';
        t.lexeme = strdup(buf);
        return t;
    }

    // 5. Opérateurs complexes (Lookahead) et Ponctuation
    c = octo_next(r); // On avance d'un cran

    if (c == ':') {
        if (octo_peek(r) == '=') {
            octo_next(r);
            if (octo_peek(r) == '=') {
                octo_next(r);
                t.type = TK_OP_MATCH;
                t.lexeme = strdup(":==");
                return t;
            }
        } else if (octo_peek(r) == ':') {
            octo_next(r);
            t.type = TK_OP_TYPE;
            t.lexeme = strdup("::");
            return t;
        }
        t.type = TK_COLON; // Utilisé dans tes varl:str:
        t.lexeme = strdup(":");
        return t;
    }

    if (c == '!') {
        if (octo_peek(r) == '>') {
            octo_next(r);
            t.type = TK_OP_MUNT;
            t.lexeme = strdup("!>");
            return t;
        }
        t.type = TK_OP_NOT;
        t.lexeme = strdup("!");
        return t;
    }

    // 6. Caractères simples (Ponctuation)
    char single_char[2] = {(char)c, '\0'};
    t.lexeme = strdup(single_char);

    switch (c) {
        case '(': t.type = TK_LPAREN; break;
        case ')': t.type = TK_RPAREN; break;
        case '{': t.type = TK_LBRACE; break;
        case '}': t.type = TK_RBRACE; break;
        case '[': t.type = TK_LBRACK; break;
        case ']': t.type = TK_RBRACK; break;
        case ',': t.type = TK_COMMA; break;
        case ';': t.type = TK_SEMICOLON; break;
        case '.': t.type = TK_OP_GLUE; break;
        default:  t.type = TK_ID; break; // Fallback par défaut si inconnu
    }

    return t;
}








