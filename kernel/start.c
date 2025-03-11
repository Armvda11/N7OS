#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>

// Définition manuelle des types uint32_t, uint16_t, uint8_t
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/**
 * Point d'entrée du kernel.
 * Initialise la console et effectue des tests mémoire.
 */
void kernel_start(void) {
    init_console();
    sti();
    setup_base(0);

    printf("initialisation de la memoire...\n");
    init_mem();
    
    printf("etat initial de la memoire :\n");
    print_mem(64);

    uint32_t page1 = findfreePage();
    uint32_t page2 = findfreePage();
    uint32_t page3 = findfreePage();

    printf("\nApres allocation de 3 pages :\n");
    print_mem(64);

    printf("\nliberation de la page 1 (0x%x)...\n", page1);
    freePage(page1);

    printf("\napres liberation de la page 1 :\n");
    print_mem(64);

    while (1) {
        hlt();
    }
}
