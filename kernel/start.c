#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>

void kernel_start(void) {
    // Initialisation des composants du noyau
    init_console();
    kmalloc_init(); 
    sti();
    setup_base(0);

    printf("=== Démarrage du kernel ===\n");
    print_mem(32);

    // Activation de la pagination
    printf("\n=== Activation de la pagination ===\n");
    initialise_paging();
    print_mem(32);

    // Vérification de CR0 pour s'assurer que la pagination est activée
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    printf("Valeur de CR0 après activation : 0x%x\n", cr0);

    // Vérification des entrées du Page Directory
    printf("\n=== État du Page Directory ===\n");
    for (int i = 0; i < 5; i++) {
        printf("Page directory[%d]: 0x%x\n", i, kernel_directory->entries[i].frame_addr << 12);
    }

    // Test de lecture sur une adresse virtuelle avant allocation
    uint32_t *test_addr = (uint32_t *)0xC0100000;
    printf("\nLecture d'une adresse virtuelle non allouée (devrait causer un problème) : 0x%x\n", *test_addr);

    // Allocation d'une nouvelle page et test d'écriture/lecture
    uint32_t virt_addr = 0xC0200000;
    uint32_t *new_page = (uint32_t *)alloc_page_entry(virt_addr, 1, 1);
    printf("\nNouvelle page virtuelle allouée : 0x%x\n", (uint32_t)new_page);

    *new_page = 0xDEADBEEF; // Écriture dans la nouvelle page
    printf("Valeur écrite en 0xC0200000 : 0x%x\n", *new_page);

    // Vérification d'une adresse non mappée (doit générer une page fault si bien géré)
    uint32_t *invalid_addr = (uint32_t *)0xC0300000;
    printf("\nLecture d'une adresse virtuelle non mappée : %x\n", *invalid_addr);

    // Force une écriture sur une adresse non mappée (devrait causer une erreur de page)
    printf("\nTentative d'écriture sur une adresse non mappée :\n");
    *invalid_addr = 0xBADC0DE;

    // Boucle infinie pour éviter la sortie du kernel
    while (1) {
        hlt();
    }
}
