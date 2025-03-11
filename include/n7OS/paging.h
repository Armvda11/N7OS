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
    // a completer
} page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
void initialise_paging();





// /**
//  * Alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
//  */
// typedef struct {
//     uint8_t present : 1;  //
//     uint8_t read_write : 1; // Indique si la page est accessible en écriture
//     uint8_t user_supervisor : 1; // Indique si la page est accessible par le noyau ou par l'utilisateur
//     uint8_t reserved1 : 2;  // Disponible pour une utilisation spécifique
//     uint8_t accessed : 1; // Indique si la page a été accédée
//     uint8_t dirty : 1; // Indique si la page a été modifiée
//     uint8_t reserved2 : 2; // Disponible pour une utilisation spécifique
//     uint8_t available : 3; // Disponible pour une utilisation spécifique
//     uint32_t frame : 20; // Adresse de la page en mémoire physique
// } page_table_entry_t;


// /**
//  * Une entrée dans la table de page peut être manipulée en utilisant
//  * la structure page_table_entry_t ou directement la valeur
//  */
// typedef union {
//     page_table_entry_t page_entry;
//     uint32_t value;
// } PTE; // PTE = Page Table Entry
// // PTE pt_entry;
// // pt_entry.page_entry.present = 1;
// // pt_entry.value = 0x00000001;
// //pt_entry.value = PAGE_SIZE|WRITABLE;
// //pt_entry.value


// typedef PTE * PageTable;

// typedef struct {
//     uint8_t present : 1;  //
//     uint8_t read_write : 1; // Indique si la page est accessible en écriture
//     uint8_t user_supervisor : 1; // Indique si la page est accessible par le noyau ou par l'utilisateur
//     uint16_t reserved :  9; // Disponible pour une utilisation spécifique
//     uint32_t frame : 20; // Adresse de la page en mémoire physique
// } page_directory_entry_t;

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif