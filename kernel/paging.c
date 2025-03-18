#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include "inttypes.h"
#include "string.h"
#include "stdio.h"
#include "types.h"

PageDirectory pageDirectory;

#define BIT_PAGING 0x80000000;
/** Affiche les informations de pagination */


/** initialiser une entrée de la table des pages avec les paramètres donnés */
void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
  page_table_entry->page_entry.present= 1;
  page_table_entry->page_entry.accessed= 0;
  page_table_entry->page_entry.dirty= 0;
  page_table_entry->page_entry.rw= is_writeable;
  page_table_entry->page_entry.user= ~is_kernel;
  page_table_entry->page_entry.page= new_page>>12;
}


uint32_t initialise_paging() {

  /* marqueur pour savoir où s'arrête la mémoire actuellement
   * allouée au kernel */
  uint32_t index= 0;

  /* initialiser la mémoire physique */
  init_mem();

  pageDirectory= (PageDirectory) kmalloc_a (sizeof(PDE) * 1024);
  memset(pageDirectory, 0, sizeof(PDE) * 1024);

  for (size_t i = 0; i < 1024; ++i) {
    // PageTable new_page_table = (PageTable) findfreePage();
    PageTable new_page_table= (PageTable) kmalloc_a(sizeof(PTE)*1024);
    memset(new_page_table, 0, sizeof(PTE) * 1024);
    pageDirectory[i].dir_entry.present = 1;
    pageDirectory[i].dir_entry.rw = 1;
    pageDirectory[i].dir_entry.page_table = 0xfffff & (((uint32_t) new_page_table) >> 12);
    index= (uint32_t) new_page_table + sizeof(PTE) * 1024;
  }

  /* IMPORTANT: on mappe toute la mémoire actuellement utilisée
   * sinon on pourra plus y accéder après activation de la pagination */
  for (size_t i = 0; i < index; i += PAGE_SIZE) {
    alloc_page_entry(i, 1, 1);
  }



  /* load l'adresse du page directory dans le registre cr3 */
  __asm__ __volatile__("mov %0, %%cr3" : : "r" (pageDirectory));
  
  /* lire et afficher l'adresse du registre cr3 */
  uint32_t cr3;
  __asm__ __volatile__("mov %%cr3, %0" : "=r" (cr3));




//   /* activer le paging en mettant à 1 le bon bit */
  uint32_t cr0;
  __asm__ __volatile__("mov %%cr0, %0" : "=r" (cr0));
  cr0 = cr0 | BIT_PAGING;
   __asm__ __volatile__("mov %0, %%cr0" : : "r" (cr0));


//   printf("page directory", pageDirectory);


  return (uint32_t) pageDirectory;
}



PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel ) {
  /* address = adresse virtuelle à allouer 
   * address = idx_PDE | idx_PTE | offset
   *             10    |    10   |   12
   */

  PageTable page_table;

  /* on recupere l'entree dans le répertoire de page
  * une entree contient l'adresse de la table de page + bits de controle */
  PDE page_dir_entry = pageDirectory[(address >> 22)]; 

  /* on recupere l'adresse de la page de table dans le répertoire de page */
  page_table = (PTE*) (page_dir_entry.dir_entry.page_table << 12);

  /* recherche d'une page libre dans la memoire physique */
  uint32_t phy_page = findfreePage(); 
  //setPage(phy_page);

//  printf("phy_page: %x\n", phy_page);

  /* mise a jour de l'entree dans la page de table */
  setPageEntry(&page_table[0x3ff & (address >> 12)],
               phy_page, is_writeable, is_kernel);

  return page_table;
}
