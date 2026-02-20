// lexer.c

#include "../../include/gramm/token.h"
#include "../../include/gramm/octog.h"
#include "../../include/gramm/key.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour vérifier si un identifiant est un mot-clé G-Lang
TokenType identify_keyword(const char* s) {
    if (strcmp(s, K_PKG) == 0) return TK_KW_PKG;
    if (strcmp(s, K_BAR) == 0) return TK_KW_SBAR;
    if (strcmp(s, K_DEF) == 0) return TK_KW_DEF;
    if (strcmp(s, K_VAR) == 0) return TK_KW_VARL;
    if (strcmp(s, K_OUTL) == 0) return TK_KW_OUTL;
    if (strcmp(s, K_INPL) == 0) return TK_KW_INPL;
    if (strcmp(s, K_SYS) == 0) return TK_KW_SYS;
    if (strcmp(s, K_MUT) == 0) return TK_OP_MUNT;
    if (strcmp(s, K_TRY) == 0) return TK_KW_TENT;
    if (strcmp(s, K_XCP) == 0) return TK_KW_EXCP;
    // ... (Ajouter les 31 mots-clés ici)
    return TK_ID;
}




// lexer.c

#include "../../include/gramm/token.h"
#include "../../include/gramm/octog.h"
#include "../../include/gramm/key.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour vérifier si un identifiant est un mot-clé G-Lang
TokenType identify_keyword(const char* s) {
    if (strcmp(s, K_PKG) == 0) return TK_KW_PKG;
    if (strcmp(s, K_BAR) == 0) return TK_KW_SBAR;
    if (strcmp(s, K_DEF) == 0) return TK_KW_DEF;
    if (strcmp(s, K_VAR) == 0) return TK_KW_VARL;
    if (strcmp(s, K_OUTL) == 0) return TK_KW_OUTL;
    if (strcmp(s, K_INPL) == 0) return TK_KW_INPL;
    if (strcmp(s, K_SYS) == 0) return TK_KW_SYS;
    if (strcmp(s, K_MUT) == 0) return TK_OP_MUNT;
    if (strcmp(s, K_TRY) == 0) return TK_KW_TENT;
    if (strcmp(s, K_XCP) == 0) return TK_KW_EXCP;
    // ... (Ajouter les 31 mots-clés ici)
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








