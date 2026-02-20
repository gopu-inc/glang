// token.h

#ifndef GLANG_TOKEN_H
#define GLANG_TOKEN_H

#include <stdint.h>

typedef enum {
    TK_EOF = 0,
    
    // --- IDENTIFIANTS ET LITÉRAUX ---
    TK_ID,              // Noms de variables, fonctions, sbar
    TK_LIT_STR,         // "G-Lang"
    TK_LIT_INT,         // 1024
    TK_LIT_FL,          // 3.14
    TK_LIT_BL,          // true / false
    
    // --- MOTS-CLÉS DE STRUCTURE (K_PKG, K_BAR, K_DEF...) ---
    TK_KW_PKG,          // package
    TK_KW_SBAR,         // sbar (Namespace isolé)
    TK_KW_DEF,          // def (Fonction/Méthode)
    TK_KW_CLS,          // class
    TK_KW_EXT,          // estand (Héritage)
    TK_KW_RET,          // return
    TK_KW_NONLOCAL,     // nonlocal
    TK_KW_VARL,         // varl (Déclaration)
    
    // --- MOTS-CLÉS DE CONTRÔLE & FLUX ---
    TK_KW_IF,           // if
    TK_KW_ELSE,         // else
    TK_KW_WHILE,        // while
    TK_KW_FOR,          // for
    TK_KW_TENT,         // tent (Try)
    TK_KW_EXCP,         // except
    TK_KW_ATOMIC,       // atomic (Sync)
    
    // --- MOTS-CLÉS I/O & SYSTÈME ---
    TK_KW_OUTL,         // outl() -> Output English
    TK_KW_INPL,         // inpl() -> Input English (Nouveau !)
    TK_KW_LOGL,         // logl() -> System log
    TK_KW_WRITE,        // write
    TK_KW_READ,         // read
    TK_KW_SYS,          // sys()
    TK_KW_IN,           // in()
    TK_KW_STOP,         // stop
    TK_KW_EXEC,         // exec
    
    // --- OPÉRATEURS SPÉCIAUX G-LANG ---
    TK_OP_ASSIGN,       // :
    TK_OP_TYPE,         // :: (Optionnel pour typage strict)
    TK_OP_GLUE,         // . (Accès membre)
    TK_OP_MATCH,        // :== (Comparaison système)
    TK_OP_MUNT,         // !> (Mutation/Transfert)
    TK_OP_NOT,          // !
    
    // --- PONCTUATION ---
    TK_LBRACE, TK_RBRACE, // { }
    TK_LPAREN, TK_RPAREN, // ( )
    TK_LBRACK, TK_RBRACK, // [ ]
    TK_COMMA,             // ,
    TK_SEMICOLON,        // ;
    TK_COLON              // :
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;       // Le texte brut (ex: "varl")
    uint32_t line;      // Debug précis
    uint32_t col;
    uint8_t depth;      // Profondeur de scope (utile pour sbar)
} Token;

#endif

