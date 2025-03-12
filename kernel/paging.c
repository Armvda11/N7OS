#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include <n7OS/console.h>
#include <string.h>
#include <stdio.h>

PageDirectory *kernel_directory;

/** Initialise une entrée de table des pages */
void setPageEntry(page_table_entry_t *entry, uint32_t new_page, int is_writeable, int is_kernel) {
    if (!entry) return;  // Protection contre un pointeur NULL

    entry->present = 1;
    entry->rw = is_writeable;
    entry->user = is_kernel ? 0 : 1;
    entry->frame_addr = new_page >> 12;
}


/** Mappe une adresse virtuelle à une adresse physique */
void map_page(uint32_t virtual_address, uint32_t physical_address) {
    if (!kernel_directory) {
        printf("Erreur: kernel_directory non initialisé !\n");
        return;
    }

    uint32_t directory_index = virtual_address >> 22;
    uint32_t table_index = (virtual_address >> 12) & 0x3FF;

    if (!kernel_directory->entries[directory_index].present) {
        PageTable *new_table = (PageTable *) kmalloc_a(sizeof(PageTable));
        if (!new_table) {
            printf("Erreur: Échec de l'allocation d'une table de pages !\n");
            return;
        }
        memset(new_table, 0, sizeof(PageTable));

        kernel_directory->entries[directory_index].present = 1;
        kernel_directory->entries[directory_index].rw = 1;
        kernel_directory->entries[directory_index].frame_addr = ((uint32_t)new_table) >> 12;
    }

    PageTable *table = (PageTable *) (kernel_directory->entries[directory_index].frame_addr << 12);
    setPageEntry(&table->entries[table_index], physical_address, 1, 1);
}

/** Initialise la pagination */
void initialise_paging() {
    printf("=== Initialisation de la pagination ===\n");

    init_mem();

    kernel_directory = (PageDirectory *) kmalloc_a(sizeof(PageDirectory));
    if (!kernel_directory) {
        printf("Erreur: Échec de l'allocation de kernel_directory !\n");
        return;
    } 
    memset(kernel_directory, 0, sizeof(PageDirectory));

    // Allocation des tables de pages uniquement si nécessaire
    for (int i = 0; i < 256; ++i) { // Uniquement les pages nécessaires (ex: 1 Mo)
        PageTable *new_page_table = (PageTable *) kmalloc_a(sizeof(PageTable));
        if (!new_page_table) {
            printf("Erreur: Échec de l'allocation d'une table de pages !\n");
            return;
        }
        memset(new_page_table, 0, sizeof(PageTable));

        kernel_directory->entries[i].present = 1;
        kernel_directory->entries[i].rw = 1;
        kernel_directory->entries[i].frame_addr = ((uint32_t)new_page_table) >> 12;
    }

    for (uint32_t i = 0; i < 0x100000; i += PAGE_SIZE) {
        map_page(i, i);
    }

    printf("Pagination activée !\n");
}


/** Allocation d'une page virtuelle */
PageTable* alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    uint32_t directory_index = address >> 22;
    uint32_t table_index = (address >> 12) & 0x3FF;

    // si la table des pages n'existe pas, on la crée et on l'ajoute à la table des pages
    if (!kernel_directory->entries[directory_index].present) {
        // on crée une nouvelle table des pages
        PageTable *new_table = (PageTable *) kmalloc_a(sizeof(PageTable));
        // on l'initialise
        memset(new_table, 0, sizeof(PageTable));
        kernel_directory->entries[directory_index].present = 1; // on la rend présente
        kernel_directory->entries[directory_index].rw = 1; // on la rend accessible en écriture
        kernel_directory->entries[directory_index].frame_addr = ((uint32_t)new_table) >> 12; // on lui donne l'adresse de la table des pages
    }

    // on récupère la table des pages
    PageTable *table = (PageTable *)(kernel_directory->entries[directory_index].frame_addr << 12);
    uint32_t physical_page = findfreePage(); // on trouve une page libre et on la récupère


    setPageEntry(&table->entries[table_index], physical_page, is_writeable, is_kernel); // on initialise l'entrée de la table des pages
    return table;
}
