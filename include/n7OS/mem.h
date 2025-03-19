/**
 * @file mem.h
 * @brief Gestion de l'allocation des pages de la mémoire physique
 */

 #ifndef _MEM_H
 #define _MEM_H
 
 #include <inttypes.h>
 
 /**
  * @brief Adresse de la dernière ligne adressable de la mémoire (16Mo)
  */
 #define LAST_MEMORY_INDEX 0XFFFFFF
 
 /**
  * @brief Taille d'une page en mémoire (4096 octets = 4 Ko)
  */
 #define PAGE_SIZE 0x1000
 
 /** Nombre total de pages mémoire disponibles */
 #define NOMBRE_PAGES ((LAST_MEMORY_INDEX + (PAGE_SIZE - 1)) / PAGE_SIZE)
 
 /** Constantes du bitmap pour l'état des pages */
 #define PAGE_ALLOUE 1
 #define PAGE_LIBRE  0
 
 /** Définition des segments du bitmap */
 #define BITMAP_SEGMENT_TYPE uint32_t
 #define TAILLE_BYTE 8
 #define TAILLE_UNITE_BITMAP (TAILLE_BYTE * sizeof(BITMAP_SEGMENT_TYPE))
 
 /** Type du bitmap */
 typedef BITMAP_SEGMENT_TYPE* bitmap_t;
 
 /**
  * @brief Taille du tableau utilisé pour représenter le bitmap
  */
 #define BITMAP_TAILLE_TABLEAU(n) (((n) + TAILLE_UNITE_BITMAP - 1) / TAILLE_UNITE_BITMAP)
 
 /**
  * @brief Vérifie si un segment est complètement alloué
  */
 #define BITMAP_IS_SEGMENT_FULL(bmp, i) \
     (bmp[(i) / TAILLE_UNITE_BITMAP] == (BITMAP_SEGMENT_TYPE)-1)
 
 /**
  * @brief Remplit le bitmap avec la valeur `bit`
  */
 #define BITMAP_FILL(bmp, n, bit) \
     memset((bmp), ((bit) ? 0xFF : 0x00), BITMAP_TAILLE_TABLEAU(n) * sizeof(BITMAP_SEGMENT_TYPE))
 
 /**
  * @brief Définit ou récupère un bit spécifique du bitmap
  */
 #define BITMAP_SET(bmp, i, bit) (bit ? (bmp[i / TAILLE_UNITE_BITMAP] |= (1 << (i % TAILLE_UNITE_BITMAP))) \
                                      : (bmp[i / TAILLE_UNITE_BITMAP] &= ~(1 << (i % TAILLE_UNITE_BITMAP))))
 
 #define BITMAP_GET(bmp, i) ((bmp[i / TAILLE_UNITE_BITMAP] >> (i % TAILLE_UNITE_BITMAP)) & 0x1)
 


 /** transformer un indice du bitmap en adresse physique */
#define PAGE_INDEX_TO_ADR(i) (adresse_premiere_page + (i) * PAGE_SIZE)

/** transformer une adresse de page en indice du bitmap */
#define PAGE_ADR_TO_INDEX(adr) (((adr) - adresse_premiere_page) / PAGE_SIZE)
 /**
  * @brief Initialise la mémoire et le bitmap
  */
 void init_mem();
 
 /**
  * @brief Marque une page comme allouée
  */
 void setPage(uint32_t addr);
 
 /**
  * @brief Libère une page
  */
 void clearPage(uint32_t addr);
 
 /**
  * @brief Trouve la première page libre et l'alloue
  */
 uint32_t findfreePage();
 
 /**
  * @brief Affiche l'état de la mémoire physique
  */
 void print_mem();
 
 #endif
 
