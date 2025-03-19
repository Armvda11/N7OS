#include <n7OS/cpu.h>
#include <inttypes.h>
<<<<<<< HEAD
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>

void kernel_start(void)
{
    init_console();
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);

    // lancement des interruptions
    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
=======
#include <n7OS/console.h>
#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>

void kernel_start(void) 
{
    uint32_t pdir;

    // Initialisation des composants du noyau
    init_console();
    kmalloc_init();

    printf("\n=== Démarrage du kernel ===\n");

    // Activation de la pagination
    printf("\n=== Activation de la pagination ===\n");
    pdir = initialise_paging();
    printf("=== Pagination activée ===\n");

    setup_base(pdir);
    sti();

    // // Vérification mémoire avant allocation
    print_mem(32);

    // printf("\n=== Test d'allocation et de mapping de pages ===\n");

    // // Allouer et mapper une page virtuelle
    uint32_t virt_addr = 0xC0100000;  // Adresse virtuelle choisie
     uint32_t *mapped_page = (uint32_t *)alloc_page_entry(virt_addr, 1, 1);
    
    if (mapped_page) {
        printf("Page virtuelle 0x%x mappée correctement.\n", virt_addr);
        *mapped_page = 0xDEADBEEF;  // Écriture dans la page mappée
        printf("Valeur écrite à 0x%x : 0x%x\n", virt_addr, *mapped_page);
    } else {
        printf("Échec du mapping de la page 0x%x.\n", virt_addr);
    }

    // // Vérification mémoire après allocation
    print_mem(32);

    // Allouer une page avant d'y accéder
    printf("\n=== Allocation et mapping de 0xC0200000 ===\n");
    uint32_t *valid_addr = (uint32_t *)alloc_page_entry(0xC0200000, 1, 1);
    
    if (valid_addr) {
        printf("Page virtuelle 0xC0200000 correctement allouée.\n");
        *valid_addr = 0x12345678;  // Écriture dans la page
        printf("Valeur écrite en 0xC0200000 : 0x%x\n", *valid_addr);
    } else {
        printf("Échec du mapping de la page 0xC0200000.\n");
    }


    // Boucle infinie pour éviter la sortie du kernel
    while (1) {
>>>>>>> prepa/main
        hlt();
    }
}
