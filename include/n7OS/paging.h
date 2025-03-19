/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
<<<<<<< HEAD
    // a completer
=======
  uint32_t present    : 1;   // Page present in memory
  uint32_t rw         : 1;   // Read-only if clear, readwrite if set
  uint32_t user       : 1;   // Supervisor level only if clear
  uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
  uint32_t dirty      : 1;   // Has the page been written to since last refresh?
  uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
  uint32_t page       : 20;  // Page address (shifted right 12 bits)
>>>>>>> prepa/main
} page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
<<<<<<< HEAD
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 
=======
  page_table_entry_t page_entry;
  uint32_t value;
} PTE; /* PTE = Page Table Entry */
>>>>>>> prepa/main

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;

<<<<<<< HEAD
=======
/** définition d'une entrée du répertoire de pages = PageDirectoryEntry = PDE */
typedef struct {
  uint32_t present    : 1;   // Page table is present in memory
  uint32_t rw         : 1;   // Read-only if clear, readwrite if set
  uint32_t user       : 1;   // Supervisor level only if clear
  uint32_t reserved   : 9;   // Some useful bits not we dont use them for now
  uint32_t page_table : 20;  // Page table address
} page_dir_entry_t;

typedef union {
  page_dir_entry_t dir_entry;
  uint32_t value;
} PDE;

typedef PDE * PageDirectory;

// NE SURTOUT PAS UTILISER LE BITFIELD CAR IL PREND PAS LA BONNE TAILLE
// /** format d'une adresse virtuelle grace a bitfield
//  * ATTENTION: le résultat peut dépendre de l'endian du proco */
// typedef struct {
//   uint32_t offset : 12; /* offset dans la page */
//   uint32_t index_pte : 10; /* index table des pages */
//   uint32_t index_pde : 10; /* index répertoire des pages */
// } adr_virt_t;

>>>>>>> prepa/main
/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
<<<<<<< HEAD
void initialise_paging();
=======
uint32_t initialise_paging();
>>>>>>> prepa/main

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> prepa/main
