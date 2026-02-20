// std_tools.h

#ifndef GLANG_STD_TOOLS_H
#define GLANG_STD_TOOLS_H
#include <stddef.h>
#include <stdint.h>


/* * 1. MUNT_CORE (std/munt.gg) -> Gestion mémoire brute
 * Utilise malloc/free mais avec la sécurité G-Lang.
 */
void* native_munt_alloc(size_t size);
void  native_munt_transfer(void* src, void* dest); // Pour l'opérateur !>

/* * 2. CHRONO (std/chrono.gg) -> Temps réel haute précision
 * Indispensable pour sys(TIME).
 */
double native_get_nanos();
void   native_sleep_micros(uint64_t micros);

/* * 3. FILE_IO (std/fs.gg) -> Système de fichiers industriel
 * Supporte les verrous (locks) pour éviter la corruption.
 */
int  native_fs_open(const char* path, int flags);
void native_fs_lock(int fd);

/* * 4. PROC_ENG (std/proc.gg) -> Gestion des processus
 * Pour le mot-clé exec et sys(MUNT, "proc").
 */
int  native_proc_spawn(const char* cmd);
void native_proc_kill(int pid);

/* * 5. CRYPTO_BASE (std/vault.gg) -> Sécurité native
 * Hashing SHA-256 et AES direct en C.
 */
void native_vault_hash(const char* input, char* output);

/* * 6. MATH_ADV (std/math.gg) -> Calcul matriciel et flottant
 * Optimisé pour les processeurs modernes (SIMD).
 */
double native_math_sqrt(double val);

/* * 7. REFLECT (std/mirror.gg) -> Introspection
 * Permet au langage de connaître ses propres structures au runtime.
 */
const char* native_get_type_name(int type_id);

/* * 8. THREAD_SYNC (std/atomic.gg) -> Concurrence
 * Pour le mot-clé atomic { ... }.
 */
void native_sync_mutex_lock(void* mutex);
void native_sync_mutex_unlock(void* mutex);

#endif

