<<<<<<< HEAD
#include <n7OS/mem.h>

=======
// fichier qui gère la mémoire physique du système et qui permet de gérer les pages mémoires libres et allouées.
// mem.c
#include "inttypes.h"
#include "n7OS/kheap.h"
#include "types.h"
#include <n7OS/mem.h>


/** le bitmap pour savoir les pages libres */
static bitmap_t bmp;

/** le début de l'espace mémoire réservé pour toutes les pages */
static uint32_t adresse_premiere_page;




>>>>>>> prepa/main
/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
  BITMAP_SET(bmp, PAGE_ADR_TO_INDEX(addr), PAGE_ALLOUE);
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
  BITMAP_SET(bmp, PAGE_ADR_TO_INDEX(addr), PAGE_LIBRE);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
  uint32_t index = 0;

  while (index < NOMBRE_PAGES) {
    if (!BITMAP_IS_SEGMENT_FULL(bmp, index)) { // Vérifie si un bloc est plein
      while (BITMAP_GET(bmp, index)) {
        index++;
        if (index >= NOMBRE_PAGES)
          return 0; // Plus de mémoire disponible
      }
      BITMAP_SET(bmp, index, PAGE_ALLOUE);
      return PAGE_INDEX_TO_ADR(index);
    }
    index += TAILLE_UNITE_BITMAP; // Sauter directement les blocs pleins
  }

  return 0;
}



/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
  bmp = (bitmap_t) kmalloc_a(BITMAP_TAILLE_TABLEAU(NOMBRE_PAGES) * sizeof(BITMAP_SEGMENT_TYPE));
  if (!bmp) {
    printf("Erreur : Impossible d'allouer le bitmap de gestion mémoire\n");
    return;
  }
  BITMAP_FILL(bmp, NOMBRE_PAGES, 0);

  adresse_premiere_page = 0;
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
  int n = 0;
  for (size_t i = 0; i < NOMBRE_PAGES; i++) {
    n += BITMAP_GET(bmp, i);
  }
  printf("nombre de pages utilisées = %d\n", n);
}
