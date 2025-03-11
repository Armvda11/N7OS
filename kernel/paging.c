#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL

#define PAGE_SIZE 0x1000
#define WRITABLE 0x2

void initialise_paging() {
    // A compléter
    // il va aller chercher avec le kmalloc il va allouer une page pour la table de page
    // il va allouer une page pour le répertoire de page
    // il va remplir le répertoire de page
    // il va activer la pagination
    // il va activer le bit de pagination dans le registre CR0
    // il va activer le registre CR3


    // Dans le début du initialistaion , on va tout mettre à 0
    
}


PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab= NULL;

    return pgtab;
}
