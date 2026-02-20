// engine.c

#include "../../include/native/std_tools.h"
#include "../../include/native/community_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// La sortie standard G-Lang (English ONLY)
void execute_outl(const char* message) {
    // Plus de [G-LOG], juste le message pur pour l'industrie
    printf("%s\n", message); 
}

// Les appels système ne parlent que si nécessaire
void execute_sys_call(const char* cmd, const char* arg) {
    if (strcmp(cmd, "TIME") == 0) {
        // Code pour obtenir le temps réel
    }
}


char* execute_inpl() {
    char* input = malloc(1024);
    printf("> "); // Prompt pour l'utilisateur
    if (fgets(input, 1024, stdin)) {
        input[strcspn(input, "\n")] = 0; // Nettoyer le \n
        return input;
    }
    return NULL;
}

