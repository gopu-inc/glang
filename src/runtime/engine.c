// src/runtime/engine.c

#include "../../include/native/std_tools.h"
#include "../../include/native/community_net.h"
#include "../../include/gramm/noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- GESTION DES VARIABLES (Lien avec vars.c) ---
extern void set_var(const char* name, const char* value);
extern char* get_var(const char* name);

// --- EXÉCUTION DES FONCTIONS G-LANG ---

void execute_outl(ASTNode* node) {
    if (!node || node->child_count == 0) return;
    
    // Si l'enfant est une variable, on va chercher sa valeur
    char* val = get_var(node->children[0]->token_value);
    if (val) {
        printf("%s\n", val);
    } else {
        // Sinon on affiche la valeur brute (littéral)
        printf("%s\n", node->children[0]->token_value);
    }
}

void execute_var_decl(ASTNode* node) {
    // Structure attendue : NODE_VAR_DECL -> [TYPE, NOM, VALEUR]
    if (node->child_count >= 3) {
        const char* name = node->children[1]->token_value;
        const char* value = node->children[2]->token_value;
        set_var(name, value);
    }
}

// --- LE DISPATCHER CENTRAL (Le cœur de l'industrie) ---

void gopu_execute_node(ASTNode* node) {
    if (!node) return;
    // Sécurité critique : si on a un compte d'enfants mais pas de tableau
    if (node->child_count > 0 && node->children == NULL) {
        return; 
    }
   

    switch (node->type) {
        case NODE_ROOT:
        case NODE_PACKAGE_BLOC:
            // On parcourt les enfants pour les exécuter un par un
            for (uint32_t i = 0; i < node->child_count; i++) {
                gopu_execute_node(node->children[i]);
            }
            break;

        case NODE_OUTL:
            execute_outl(node);
            break;

        case NODE_VAR_DECL:
            execute_var_decl(node);
            break;

        case NODE_SYS_CALL:
            // On appelle tes outils dans std_tools.h
            // execute_sys_call(node->children[0]->token_value, ...);
            break;

        case NODE_STOP:
            fprintf(stderr, "[STOP] %s\n", node->token_value);
            exit(1);

        // --- À CONTINUER POUR LES 29 AUTRES ---
        // case NODE_IF: execute_if(node); break;
        // case NODE_WHILE: execute_while(node); break;
        
        default:
            // Par défaut, on descend dans l'arbre
            for (uint32_t i = 0; i < node->child_count; i++) {
                gopu_execute_node(node->children[i]);
            }
            break;
    }
}
