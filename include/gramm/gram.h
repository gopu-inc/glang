// gram.h

#ifndef GLANG_GRAM_H
#define GLANG_GRAM_H

#include <stdbool.h>

/* * NIVEAUX DE PRÉCÉDENCE (Priorité d'exécution)
 * Du plus faible au plus fort.
 */
typedef enum {
    PREC_NONE = 0,
    PREC_LOWEST,      // Déclarations (varl)
    PREC_ASSIGN,      // Assignation simple (:)
    PREC_MUNT,        // Mutation système (!>) - Priorité sur l'assignation
    PREC_LOGIC,       // Condition (if, while)
    PREC_MATCH,       // Comparaison système (:==)
    PREC_TERM,        // Addition, Soustraction
    PREC_FACTOR,      // Multiplication, Division
    PREC_UNARY,       // Négation (!), Signe (-)
    PREC_CALL,        // Appels (func(), sys(), in())
    PREC_PRIMARY      // Valeurs atomiques, Parenthèses
} Precedence;

/* * ÉTATS DU PARSER
 * Gère le contexte global pour savoir si on est dans un package, un sbar ou un thread.
 */
typedef enum {
    STATE_GLOBAL,     // Racine du fichier
    STATE_IN_PKG,     // Dans le bloc package { ... }
    STATE_IN_SBAR,    // Dans un sbar (isolation active)
    STATE_IN_CLASS,   // Dans une classe (héritage actif)
    STATE_IN_DEF,     // Dans une fonction (scope local)
    STATE_IN_TENT     // Dans un bloc de sécurité (tent/except)
} ParserState;

/* * STRUCTURE DE RÈGLE GRAMMATICALE
 * Associe un token à sa fonction de traitement selon le contexte.
 */
typedef struct {
    void (*prefix)();  // Fonction pour les expressions commençant par ce token
    void (*infix)();   // Fonction pour les expressions entre deux tokens
    Precedence precedence;
} GramRule;

/* * CONFIGURATION DE LA GRAMMAIRE GOPU
 */
typedef struct {
    ParserState current_state;
    bool has_package;      // Le fichier a-t-il défini son package ?
    bool in_atomic_block;  // Est-on dans une zone thread-safe ?
    int scope_depth;       // Profondeur actuelle ({ = +1)
} GGrammarConfig;

// Prototypes de validation grammaticale
bool validate_structure(GGrammarConfig* config);
bool is_system_operation(Precedence prec);

#endif

