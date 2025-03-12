#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define SCR_TAB ((uint16_t *)SCREEN_ADDR)
#define TAB_SIZE 4  // Ajout d'une constante pour la tabulation

int ligne = 0;
int colonne = 0;
uint8_t text_color = CHAR_COLOR;

/**
 * Remplit une zone de mémoire avec une valeur de 16 bits.
 */
void memset_16(uint16_t *dest, uint16_t value, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        dest[i] = value;
    }
}

/**
 * Met à jour la position du curseur.
 */
void set_cursor()
{
    int pos = ligne * VGA_WIDTH + colonne;
    outb(CMD_LOW, PORT_CMD);
    outb(pos & 0xFF, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb((pos >> 8) & 0xFF, PORT_DATA);
}

/**
 * Efface l'écran et remet le curseur à (0,0).
 */
void clear_screen()
{
    memset_16(SCR_TAB, (text_color << 8) | ' ', MAX_CURSOR_POS);
    ligne = 0;
    colonne = 0;
    set_cursor();
}

/**
 * Fait défiler l'écran vers le haut d'une ligne.
 */
void console_scroll()
{
    uint16_t *last_line = SCR_TAB + (VGA_HEIGHT - 1) * VGA_WIDTH;

    memmove(SCR_TAB, SCR_TAB + VGA_WIDTH, (VGA_WIDTH * (VGA_HEIGHT - 1)) * sizeof(uint16_t));
    memset_16(last_line, (text_color << 8) | ' ', VGA_WIDTH);

    ligne = VGA_HEIGHT - 1;
    colonne = 0;
    set_cursor();
}

/**
 * Initialise la console et efface l'écran.
 */
void init_console()
{
    text_color = CHAR_COLOR;
    clear_screen();
}

/**
 * Modifie la couleur du texte et du fond.
 */
void console_setcolor(uint8_t fg, uint8_t bg)
{
    fg = (fg > 0xF) ? WHITE : fg;
    bg = (bg > 0xF) ? BLACK : bg;
    text_color = (bg << 4) | (fg & 0x0F);
}


/**
 * Affiche un caractère à l'écran en gérant les caractères spéciaux.
 */
void console_putchar(char c)
{
    switch (c)
    {
    case '\n': // Nouvelle ligne
        ligne++;
        colonne = 0;
        if (ligne >= VGA_HEIGHT)
        {
            console_scroll();
        }
        break;

    case '\r': // Retour au début de la ligne
        colonne = 0;
        break;

    case '\b': // Backspace : efface le dernier caractère
        if (colonne > 0)
        {
            colonne--;
        }
        else if (ligne > 0)
        {
            ligne--;
            colonne = VGA_WIDTH - 1;
        }
        SCR_TAB[ligne * VGA_WIDTH + colonne] = (text_color << 8) | ' ';
        break;

    case '\t': // Tabulation
        colonne = (colonne + 4) & ~(4 - 1);
        if (colonne >= VGA_WIDTH)
        {
            ligne++;
            colonne = 0;
        }
        break;

    case '\f': // Effacement de l’écran
        clear_screen();
        break;

    default: // Affichage normal
        SCR_TAB[ligne * VGA_WIDTH + colonne] = (text_color << 8) | c;
        colonne++;
        if (colonne >= VGA_WIDTH)
        {
            colonne = 0;
            ligne++;
            if (ligne >= VGA_HEIGHT)
            {
                console_scroll();
            }
        }
        break;
    }
}

/**
 * Affiche une chaîne de caractères sur la console.
 */
void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i++)
    {
        console_putchar(s[i]);
    }
    set_cursor(); // Mise à jour du curseur après affichage
}

/**
 * Test de la console.
 */void test() {
    printf("### TEST DE LA CONSOLE VGA ###\n\n");

    // ✅ TEST 1 : Affichage de lignes normales
    for (int i = 0; i < 5; i++) {
        printf("Ligne normale %d\n", i);
    }

    // ✅ TEST 2 : Gestion du retour chariot (\r)
    printf("\nRetour chariot test : ABCDEFG\r123\n");
    // Attendu : "123DEFG"

    // ✅ TEST 3 : Backspace (\b) - Effacement de caractères
    printf("Effacement : ABCD\b\bXY\n");
    // Attendu : "Effacement : ABXY"

    // ✅ TEST 4 : Tabulation (\t)
    printf("Tabulation : A\tB\tC\tD\n");
    // Attendu : "Tabulation : A   B   C   D"

    // ✅ TEST 5 : Effacement de l’écran (\f)
    printf("\nEffacement de l’écran en 3s...\n");
    for (volatile int i = 0; i < 100000000; i++);  // Petit délai artificiel
    printf("\fÉcran nettoyé !\n");

    // ✅ TEST 6 : Défilement (scrolling)
    printf("\nTest de scrolling...\n");
    for (int i = 0; i < 30; i++) {
        printf("Ligne scroll %d\n", i);
    }

    // ✅ TEST 7 : Changement de couleur avec reset
    printf("\nTest des couleurs...\n");

    console_setcolor(WHITE, BLUE);
    printf("Texte Blanc sur Bleu\n");
    console_setcolor(WHITE, BLACK);  // Reset couleur

    console_setcolor(YELLOW, RED);
    printf("Texte Jaune sur Rouge\n");
    console_setcolor(WHITE, BLACK);

    console_setcolor(GREEN, BLACK);
    printf("Texte Vert sur Noir\n");
    console_setcolor(WHITE, BLACK);

    // ✅ TEST 8 : Affichage de caractères spéciaux (ASCII standard)
    printf("\nCaractères spéciaux : !@#$%%^&*()_+-={}[]:;\"'<>,.?/~\n");

    // ✅ TEST 9 : Long texte sans retour à la ligne (avec correction)
    printf("Texte long : Ceci est une tres longue phrase qui devrait remplir toute une ligne sans retour ");
    printf("à la ligne. Voyons ce qui se passe !\n");

    printf("\n### FIN DES TESTS ###\n");
}


