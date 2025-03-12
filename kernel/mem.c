#include <n7OS/mem.h>
#include <n7OS/kheap.h>
#include <stdio.h>

#define TOTAL_PAGES (LAST_MEMORY_INDEX / PAGE_SIZE)  // 16 Mo / 4 Ko = 4096 pages
#define BITMAP_SIZE (TOTAL_PAGES / 32)  // 4096 / 32 = 128 entrées

static uint32_t memory_bitmap[BITMAP_SIZE];  // Bitmap mémoire (1 = alloué, 0 = libre)

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    memory_bitmap[page_index / 32] |= (1 << (page_index % 32));
}

/**
 * @brief Désalloue la page
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    memory_bitmap[page_index / 32] &= ~(1 << (page_index % 32));
}

uint32_t findfreePage() {
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (memory_bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(memory_bitmap[i] & (1 << j))) {
                    uint32_t addr = (i * 32 + j) * PAGE_SIZE;
                    setPage(addr);
                    return addr;
                }
            }
        }
    }
    return (uint32_t)-1;  // Aucune page libre trouvée
}

/**
 * @brief Initialise le bitmap mémoire
 * 
 * Initialise le bitmap mémoire en marquant toutes les pages comme libres
 */
void init_mem() {
    // Initialisation du bitmap mémoire (toutes les pages sont libres)
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        memory_bitmap[i] = 0;
    }
    // Marquage des pages réservées pour le noyau
    for (uint32_t addr = 0; addr < 0x100000; addr += PAGE_SIZE) {
        setPage(addr);
    }
}

void print_mem(uint32_t limit) {
    for (uint32_t i = 0; i < limit; i++) {
        uint32_t word_index = i / 32;
        uint32_t bit_index = i % 32;
        printf("%s ", (memory_bitmap[word_index] & (1 << bit_index)) ? "[X]" : "[ ]");
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void freePage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t word_index = page_index / 32;
    uint32_t bit_index = page_index % 32;

    if (!(memory_bitmap[word_index] & (1 << bit_index))) {
        printf("Erreur: La page 0x%x est déjà libre !\n", addr);
        return;
    }

    clearPage(addr);
}
