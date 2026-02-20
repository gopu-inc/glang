// octog.h

#ifndef GLANG_OCTOG_H
#define GLANG_OCTOG_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * SOURCE_KIND
 * Définit la nature du flux d'entrée pour adapter la lecture.
 */
typedef enum {
    OCTO_FILE,      // Lecture depuis un fichier disque
    OCTO_BUFFER,    // Lecture depuis la mémoire (JIT/Eval)
    OCTO_STREAM     // Flux réseau ou pipe (sys/io)
} OctoKind;

/**
 * OCTO_READER
 * La structure centrale qui "mange" les octets du code G-Lang.
 */
typedef struct {
    const uint8_t* raw_data; // Pointeur vers les données brutes
    size_t size;             // Taille totale
    size_t cursor;           // Position actuelle
    
    uint32_t line;           // Ligne actuelle (pour le debug)
    uint32_t col;            // Colonne actuelle
    
    OctoKind kind;           // Origine des données
    uint8_t lookahead;       // Octet suivant pré-chargé
} OctoReader;

/**
 * FONCTIONS DE MANIPULATION (The Octo-Engine)
 */

// Initialise le lecteur avec un fichier ou un buffer
OctoReader* octo_init_file(const char* path);
OctoReader* octo_init_buf(const uint8_t* buffer, size_t size);

// Navigation dans le flux
uint8_t octo_peek(OctoReader* r);         // Regarde l'octet actuel sans avancer
uint8_t octo_next(OctoReader* r);         // Consomme et retourne l'octet
void    octo_skip_space(OctoReader* r);   // Ignore les blancs et commentaires
void    octo_rewind(OctoReader* r, size_t n); // Recule de n octets (si possible)

/**
 * GESTION DES ERREURS DE FLUX
 */
typedef struct {
    uint8_t err_code;
    const char* msg;
    uint32_t at_pos;
} OctoError;

// Vérifie si on a atteint la fin du flux (EOF)
#define OCTO_IS_EOF(r) ((r)->cursor >= (r)->size)

#endif

