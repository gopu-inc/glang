// noeud.h

#ifndef GLANG_NOEUD_H
#define GLANG_NOEUD_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    // --- RACINE ET MODULES ---
    NODE_ROOT,          // Sommet de l'arbre
    NODE_PACKAGE_BLOC,  // Le bloc package { ... }
    NODE_IMPORT,        // Une ligne d'importation (sys/json.hgg)
    
    // --- STRUCTURES DE DONNÉES ---
    NODE_SBAR,          // Espace de nom isolé (sbar Network {})
    NODE_CLASS,         // Définition de classe
    NODE_STRUCT,        // Définition de structure
    NODE_EXTEND,        // Liaison d'héritage (estand)
    
    // --- LOGIQUE ET FONCTIONS ---
    NODE_DEF,           // Définition de fonction/méthode
    NODE_PARAMS,        // Liste des paramètres de fonction
    NODE_RETURN,        // Retour de valeur
    NODE_NONLOCAL,      // Référence hors scope
    
    // --- DÉCLARATIONS ET TYPES ---
    NODE_VAR_DECL,      // varl:type:
    NODE_TYPE_REF,      // Référence à un type (str, int, obj)
    NODE_ASSIGN,        // Assignation simple (:)
    NODE_MUNT,          // Mutation système (!>)
    
    // --- CONTRÔLE DE FLUX ---
    NODE_IF,            
    NODE_ELSE,
    NODE_WHILE,
    NODE_FOR,
    NODE_TENT,          // Bloc d'essai (tent)
    NODE_EXCEPT,        // Capture d'erreur (except)
    NODE_STOP,          // Arrêt d'urgence
    
    // --- APPELS ET NATIFS ---
    NODE_CALL,          // Appel de fonction standard
    NODE_SYS_CALL,      // Appel noyau (sys)
    NODE_IN_CALL,       // Appel interruption (in)
    NODE_OUTL,          // Logique de sortie (English log)
    NODE_INPL,          // Logique d'entrée
    
    // --- EXPRESSIONS ---
    NODE_EXPR_MATCH,    // Comparaison système (:==)
    NODE_LITERAL,       // Valeurs brutes (102, "hello")
    NODE_IDENTIFIER     // Noms de variables
} NodeType;

// Structure d'un nœud AST pour gopu.inc
typedef struct ASTNode {
    NodeType type;
    
    // Données du nœud
    char* token_value;       // Valeur textuelle (nom de fonction, string, etc.)
    uint32_t metadata;       // Utilisé pour stocker des drapeaux (ex: priv/publ)
    
    // Arborescence
    struct ASTNode* parent;
    struct ASTNode** children;
    uint32_t child_count;
    
    // Debug
    uint32_t line;
    uint32_t col;
} ASTNode;

// Fonctions de gestion de l'arbre
ASTNode* create_node(NodeType type, const char* value, uint32_t line, uint32_t col);
void add_child(ASTNode* parent, ASTNode* child);
void free_tree(ASTNode* root);

#endif


