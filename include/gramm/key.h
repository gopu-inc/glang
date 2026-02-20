// key.h

#ifndef GLANG_KEY_H
#define GLANG_KEY_H

// --- MOTS CLÉS DE STRUCTURE (10) ---
#define K_PKG    "package"  // Bloc d'importation {}
#define K_BAR    "sbar"     // Barre de structure / Espace de nom isolé
#define K_DEF    "def"      // Définition de bloc logique
#define K_CLS    "class"    // Modèle d'objet gopu
#define K_EXT    "estand"   // Extension de classe/struct (Héritage)
#define K_RET    "return"   // Retour de valeur
#define K_FOR    "for"      // Boucle itérative
#define K_NON    "nonlocal" // Accès scope parent
#define K_MUT    "munt"     // Mutation de mémoire (utilisé dans sys)

// --- MOTS CLÉS D'ACTIONS & I/O (10) ---
#define K_WRITE  "write"    // Ecriture flux
#define K_READ   "read"     // Lecture flux
#define K_OUTL   "outl"     // Print standard (English log)
#define K_LOGL   "logl"     // Logging système
#define K_IN     "in"       // Entrée/Interruption
#define K_STOP   "stop"     // Arrêt immédiat (Error handling)
#define K_EXEC   "exec"     // Exécution d'un thread/process
#define K_WAIT   "wait"     // Attente asynchrone
#define K_LINK   "link"     // Liaison de pointeur/référence
#define K_CAST   "morph"    // Conversion de type (au lieu de cast)
#define K_INPL   "inpl"     // input() a la gopu

// --- MOTS CLÉS LOGIQUES & ÉTATS (11) ---

#define K_VAR    "varl"     // Déclaration
#define K_IF     "if"       // Condition
#define K_ELSE   "else"     // Sinon
#define K_WHILE  "while"    // Boucle conditionnelle
#define K_XCP    "except"   // Capture d'erreur
#define K_TRY    "tent"     // Tentative (Try)
#define K_SYNC   "atomic"   // Bloc thread-safe
#define K_HIDE   "priv"     // Encapsulation privée
#define K_OPEN   "publ"     // Encapsulation publique
#define K_VOID   "nulla"    // Valeur nulle
#define K_SYS    "sys"      // Appel noyau G-Lagage

/*
const char* GLANG_ALL_KEYS[] = {
    K_PKG, K_BAR, K_DEF, K_CLS, K_EXT, K_RET, K_FOR, K_NON, K_MUT,
    K_WRITE, K_READ, K_OUTL, K_LOGL, K_IN, K_STOP, K_EXEC, K_WAIT, 
    K_LINK, K_CAST, K_VAR, K_IF, K_ELSE, K_WHILE, K_XCP, K_TRY, // Fix ici
    K_SYNC, K_INPL, K_HIDE, K_OPEN, K_VOID, K_SYS, NULL
};
*/

extern const char* GLANG_ALL_KEYS[];

#endif

