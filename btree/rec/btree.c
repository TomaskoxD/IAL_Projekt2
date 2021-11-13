/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) //funguje
{
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) // funguje
{
  if (tree == NULL) // ak je strom prazdny
    return false;
  if (key < tree->key) // ak mam ist viac dolava, norim sa a ako parameter davam lavy uzol
    return bst_search(tree->left, key, value);
  else if (key > tree->key) // ak mam ist viac doprava, norim sa a ako parameter davam pravy uzol
    return bst_search(tree->right, key, value);
  else // ak som sa zanoril tak ze sa nemozem ist ani dolava ani doprava, som spravne a vraciam hodnotu
    *value = tree->value;
  return true;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) // funguje
{
  if (*tree == NULL) // ak som sa zanoril tak hlboko ze uz som na listovom uzle, vytvaram nody uzol
  {
    *tree = malloc(sizeof(struct bst_node));
    if (*tree == NULL)
      return;

    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }

  if (key < (*tree)->key) // ak potrebujem ist viac dolava
    bst_insert(&(*tree)->left, key, value);
  else if (key > (*tree)->key) // ak potrebujem ist viac doprava
    bst_insert(&(*tree)->right, key, value);
  else // ak som narazil na rovnaku hodnotu key, aktualizujem value
    (*tree)->value = value;

  return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) // asi ide
{

  if (*tree == NULL || target == NULL)
    return;
  if ((*tree)->right != NULL) // rekurzivne idem az na most right
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
    return;
  }
  target->key = (*tree)->key; // aktualizujem hodnoty
  target->value = (*tree)->value;

  bst_node_t *temp = *tree; // najpravsia lava do najpravsej
  *tree = temp->left;
  free(temp);

  return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) // asi ide
{
  if (*tree == NULL)
    return;

  if (key < (*tree)->key && (*tree)->left != NULL) // ak mam ist hlbsie dolava
    bst_delete(&(*tree)->left, key);
  else if (key > (*tree)->key && (*tree)->right != NULL) // ak mam ist hlbsie doprava
    bst_delete(&(*tree)->right, key);
  else if (key == (*tree)->key) // ak som v spravnom uzle
  {
    if ((*tree)->left == NULL && (*tree)->right == NULL) // ak nema laveho ani praveho potomka
    {
      free(*tree);
      (*tree) = NULL;
    }
    else if ((*tree)->right == NULL) // ak ma len laveho potomka
    {
      bst_node_t *temp = *tree;
      *tree = temp->left;
      free(temp);
    }
    else if ((*tree)->left == NULL) // ak ma len praveho potomka
    {
      bst_node_t *temp = *tree;
      *tree = temp->right;
      free(temp);
    }
    else // ak ma oboch potomkov
      bst_replace_by_rightmost((*tree), &(*tree)->left);
  } // ak som ziadny taky uzol nenasiel, return
  return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) // funguje... hah nefunguje
{
  if (*tree != NULL)
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) // funguje az do konca
{
  if (tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
