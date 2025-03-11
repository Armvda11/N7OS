#include <n7OS/mem.h>
#include <stdio.h>

// Définition des types uint32_t, uint16_t, uint8_t pour éviter d'inclure stdint.h
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;


#define TOTAL_PAGES (LAST_MEMORY_INDEX / PAGE_SIZE) // 16Mo / 4Ko = 4096 pages
#define BITMAP_SIZE (TOTAL_PAGES / 32) // 4096 / 32 = 128 entrées

static uint32_t memory_bitmap[BITMAP_SIZE]; // Bitmap des pages allouées (1 = allouée, 0 = libre)

/**
 * @brief Marque une page mémoire comme allouée.
 * 
 * @param addr Adresse de la page à marquer.
 */
void setPage(uint32_t addr) {
    if (addr % PAGE_SIZE != 0) {
        printf("Erreur: Adresse non alignee sur PAGE_SIZE !\n");
        return;
    }
    uint32_t page_index = addr / PAGE_SIZE;
    memory_bitmap[page_index / 32] |= (1 << (page_index % 32));
}

/**
 * @brief Libère une page mémoire.
 * 
 * @param addr Adresse de la page à libérer.
 */
void clearPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t word_index = page_index / 32;
    uint32_t bit_index = page_index % 32;

    printf("Debug: Avant clear, memory_bitmap[%d] = 0x%x\n", word_index, memory_bitmap[word_index]);

    memory_bitmap[word_index] &= ~(1 << bit_index); // Met à zéro le bit

    printf("Debug: Apres clear, memory_bitmap[%d] = 0x%x\n", word_index, memory_bitmap[word_index]);
}

/**
 * @brief Trouve la première page mémoire libre et l'alloue.
 * 
 * @return uint32_t Adresse de la page trouvée, ou -1 si aucune page libre.
 */
uint32_t findfreePage() {
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (memory_bitmap[i] != 0xFFFFFFFF) { // Vérifie si un bit est libre
            for (uint32_t j = 0; j < 32; j++) {
                uint32_t mask = 1 << j;
                if (!(memory_bitmap[i] & mask)) { // Trouve le premier bit libre
                    uint32_t addr = (i * 32 + j) * PAGE_SIZE;
                    setPage(addr);
                    return addr;
                }
            }
        }
    }
    return (uint32_t)-1; // Aucune page libre trouvée
}

/**
 * @brief Initialise la gestion mémoire et réserve les pages nécessaires.
 */
void init_mem() {
    // Initialisation du bitmap mémoire (toutes les pages marquées comme libres)
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        memory_bitmap[i] = 0;
    }

    // Réserver les premières pages pour le noyau et la mémoire basique (1 Mo)
    for (uint32_t addr = 0; addr < 0x100000; addr += PAGE_SIZE) {
        setPage(addr);
    }

    printf("gestionnaire mémoire initialise : %d pages gérées\n", TOTAL_PAGES);
}

/**
 * @brief Affiche l'état des pages mémoire.
 * 
 * @param limit Nombre de pages à afficher (évite d'afficher tout en une fois).
 */
void print_mem(uint32_t limit) {
    printf("Etat de la memoire physique :\n");
    
    for (uint32_t i = 0; i < limit; i++) {
        uint32_t word_index = i / 32;
        uint32_t bit_index = i % 32;
        uint32_t is_allocated = (memory_bitmap[word_index] & (1 << bit_index)) ? 1 : 0;

        printf("%s ", is_allocated ? "[X]" : "[ ]");

        if ((i + 1) % 16 == 0) printf("\n");
    }

    // Ajoute un affichage direct du bitmap en hexadécimal pour debug
    printf("\nBitmap memoire brute :\n");
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        printf("memory_bitmap[%d] = 0x%x\n", i, memory_bitmap[i]);
    }
    printf("\n");
}

/**
 * @brief Libère une page et effectue la défragmentation (optionnel).
 */
void freePage(uint32_t addr) {
    printf("Debug: Tentative de liberation de la page a 0x%x\n", addr);
    
    uint32_t page_index = addr / PAGE_SIZE;
    printf("Debug: Index de la page = %d, Mot bitmap = %d, Bit = %d\n", 
           page_index, page_index / 32, page_index % 32);
    
    // Vérifie si la page est bien allouée avant de la libérer
    if (!(memory_bitmap[page_index / 32] & (1 << (page_index % 32)))) {
        printf("Erreur: La page 0x%x etait deja libre !\n", addr);
        return;
    }
    
    clearPage(addr);
    printf("Page 0x%x liberee avec succes.\n", addr);
}

