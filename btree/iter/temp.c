/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
void bst_init(bst_node_t **tree) // funguje
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) // funguje
{
    bst_node_t *current = tree;
    while (current != NULL) // pokial niesom na listovom uzle
    {
        if (current->key < key) // ak je kluc mensi ako hladany
            current = current->right;
        else if (current->key > key) // ak je kluc vacsi ako hladany
            current = current->left;
        else
        {
            *value = current->value;
            return true;
        }
    }
    return false;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) // funguje
{
    while (tree != NULL)
    {
        if ((*tree) == NULL)
        {
            *tree = malloc(sizeof(struct bst_node));
            if (*tree == NULL)
                return;

            (*tree)->left = (*tree)->right = NULL;
            (*tree)->key = key;
            (*tree)->value = value;
            return;
        }
        if (key == (*tree)->key)
        {
            (*tree)->value = value;
            return;
        }
        else if (key > (*tree)->key)
        {
            tree = &(*tree)->right;
        }
        else if (key < (*tree)->key)
        {
            tree = &(*tree)->left;
        }
    }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) // POGU ide, ale je to clownfiesta, nejde free
{
    if (*tree == NULL || target == NULL)
        return;
    bst_node_t *temp = *tree;
    bst_node_t *temp_parent = *tree;
    bst_node_t *temp_left = NULL;

    while (1) // iterujem cez strom po pravych uzloch a hladam key
    {
        temp = temp->right;
        if (temp->right == NULL)
        {
            break;
        }
        temp_parent = temp_parent->right;
    }                        // na konci mam v temp uzol s klucom a v temp_parent jeho rodica
    target->key = temp->key; // aktualizujem hodnoty
    target->value = temp->value;
    if (temp->left != NULL)
    {
        temp_left = temp->left;
    }

    if (temp_parent->left == temp) // ak je to lavy potomok
    {
        free(temp);
        temp_parent->left = NULL;
        temp = NULL;
    }
    if (temp_parent->right == temp) // ak je to pravy potomok
    {
        free(temp);
        temp_parent->right = NULL;
        temp = NULL;
    }
    if (temp_left != NULL)
    {
        temp_parent->right = temp_left;
    }

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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) // ani nahodou
{

    if (*tree == NULL)
        return;
    bst_node_t *current = *tree;
    while (current != NULL) // iterujem az dojdem na listovy uzol
    {
        if (current->key < key) // idem do praveho potomka
        {
            if (current->right != NULL)
                current = current->right;
            else
                break;
        }
        else if (current->key > key) // idem do laveho
        {
            if (current->left != NULL)
                current = current->left;
            else
                break;
        }
        else // ak nemozem ist ani doprava ani dolava
            break;
    }
    if (key != current->key) // ak som v spravnom uzle ale ma zlu hodnotu
        return;

    bst_node_t *temp = *tree;
    bst_node_t *parent = NULL;

    while (temp != NULL && temp->key != key)
    {
        parent = temp;
        if (temp->key > key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (current->left == NULL && current->right == NULL) // ak nema laveho ani praveho potomka
    {
        if (parent->left->key == key)
        {
            free(current);
            current = NULL;
            parent->left = NULL;
        }
        else
        {
            free(current);
            current = NULL;
            parent->right = NULL;
        }
    }
    else if (current->right == NULL) // ak ma len laveho potomka
    {
        bst_node_t *temp = current->left;
        if (parent->left->key == key)
        {
            free(current);
            current = NULL;
            parent->left = temp;
        }
        else
        {
            free(current);
            current = NULL;
            parent->right = temp;
        }
    }
    else if (current->left == NULL) // ak ma len praveho potomka
    {
        bst_node_t *temp = current->right;
        if (parent->left->key == key)
        {
            free(current);
            current = NULL;
            parent->left = temp;
        }
        else
        {
            free(current);
            current = NULL;
            parent->right = temp;
        }
    }
    else // ak ma oboch potomkov
    {
        bst_replace_by_rightmost(current, &current->left);
    }
    return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) // ide POG
{
    if (tree == NULL)
        return;
    stack_bst_t *temp_stack = malloc(sizeof(stack_bst_t));
    if (temp_stack == NULL)
        return;
    stack_bst_init(temp_stack);

    do
    {
        if ((*tree) == NULL)
        {
            if (!stack_bst_empty(temp_stack))
            {
                (*tree) = stack_bst_top(temp_stack);
                stack_bst_pop(temp_stack);
            }
        }
        else
        {
            if ((*tree)->right != NULL)
            {
                stack_bst_push(temp_stack, (*tree)->right);
            }
            bst_node_t *delete = (*tree);
            (*tree) = (*tree)->left;
            free(delete);
            delete = NULL;
        }
    } while (((*tree) != NULL) || (!stack_bst_empty(temp_stack)));
    free(temp_stack);
    return;
}
/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) // funguje az do konca
{
    if (tree == NULL)
        return;
    while (tree != NULL)
    {
        bst_print_node(tree);
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
    return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
    if (tree == NULL)
        return;
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL)
        return;
    stack_bst_init(stack);

    bst_leftmost_preorder(tree, stack);
    while (!stack_bst_empty(stack))
    {
        tree = stack_bst_top(stack);
        stack_bst_pop(stack);
        bst_leftmost_preorder(tree->right, stack);
    }
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    if (tree == NULL)
        return;
    while (tree != NULL)
    {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
    return;
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
    if (tree == NULL)
        return;
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL)
        return;
    stack_bst_init(stack);

    bst_leftmost_inorder(tree, stack);
    while (!stack_bst_empty(stack))
    {
        tree = stack_bst_top(stack);
        stack_bst_pop(stack);
        bst_print_node(tree);
        bst_leftmost_inorder(tree->right, stack);
    }
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
    if (tree == NULL)
        return;
    while (tree != NULL)
    {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
    return;
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
    if (tree == NULL)
        return;
    stack_bst_t *stack_t = malloc(sizeof(stack_bst_t));
    if (stack_t == NULL)
        return;
    stack_bst_init(stack_t);

    stack_bool_t *stack_b = malloc(sizeof(stack_bool_t));
    if (stack_b == NULL)
        return;
    stack_bool_init(stack_b);

    bst_leftmost_postorder(tree, stack_t, stack_b);
    bool processed;
    while (!stack_bst_empty(stack_t))
    {
        tree = stack_bst_top(stack_t);
        stack_bst_pop(stack_t);
        processed = stack_bool_top(stack_b);
        stack_bool_pop(stack_b);
        if (processed)
        {
            stack_bst_push(stack_t, tree);
            stack_bool_push(stack_b, false);
            bst_leftmost_postorder(tree->right, stack_t, stack_b);
        }
        else
            bst_print_node(tree);
    }
    free(stack_t);
    free(stack_b);
}
