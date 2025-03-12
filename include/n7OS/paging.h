#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 0x1000  // 4 KB

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t reserved   : 2;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t unused     : 5;
    uint32_t frame_addr : 20;
} page_table_entry_t;

typedef struct {
    page_table_entry_t entries[PAGE_TABLE_SIZE];
} PageTable;

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t reserved   : 5;
    uint32_t accessed   : 1;
    uint32_t unused     : 6;
    uint32_t frame_addr : 20;
} page_directory_entry_t;

typedef struct {
    page_directory_entry_t entries[PAGE_DIRECTORY_SIZE];
} PageDirectory;

// Fonctions de la pagination pour le nooyau , 
extern PageDirectory *kernel_directory; // Table des pages du n

void initialise_paging();
PageTable* alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);

#endif
