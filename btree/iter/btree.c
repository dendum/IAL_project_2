/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL) {
        if (key == tree->key) {
            *value = tree->value;
            return true;
        }

        if (key < tree->key) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }

    return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    while (*tree != NULL) {
        if (key < (*tree)->key) {
            tree = &(*tree)->left;
        } else if (key > (*tree)->key) {
            tree = &(*tree)->right;
        } else if (key == (*tree)->key) {
            (*tree)->value = value;
            return;
        }
    }

    (*tree) = (bst_node_t *) malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (target == NULL)
        return;

    bst_node_t *active = *tree;

    while ((*tree)->right != NULL) {
        tree = &(*tree)->right;
        active = *tree;
    }

    target->key = (*tree)->key;
    target->value = (*tree)->value;
    *tree = (*tree)->left;
    free(active);

}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL)
        return;

    bst_node_t *useless;

    while ((*tree) != NULL) {
        if (key < (*tree)->key) {
            tree = &(*tree)->left;
            continue;
        } else if (key > (*tree)->key) {
            tree = &(*tree)->right;
            continue;
        } else if (key == (*tree)->key) {
            useless = *tree;

            if (useless->right != NULL && useless->left != NULL) {
                bst_replace_by_rightmost(useless, &useless->left);
                return;
            } else if (useless->right != NULL && useless->left == NULL) {
                *tree = useless->right;
            } else if (useless->right == NULL && useless->left != NULL) {
                *tree = useless->left;
            } else {
                *tree = NULL;
            }

            free(useless);
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_node_t *useless;

    if (*tree != NULL)
        stack_bst_push(&stack, *tree);
    while (!stack_bst_empty(&stack)) {
        *tree = stack_bst_pop(&stack);
        if ((*tree)->left != NULL) {
            stack_bst_push(&stack, (*tree)->left);
        }
        if ((*tree)->right != NULL) {
            stack_bst_push(&stack, (*tree)->right);
        }

        useless = *tree;
        *tree = NULL;
        free(useless);
    }
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        bst_add_node_to_items(tree, items);
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_preorder(tree,&stack, items);
    while (!stack_bst_empty(&stack)){
        tree = stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right,&stack, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit,tree);
        tree = tree->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_inorder(tree,&stack);
    while (!stack_bst_empty(&stack)){
        tree = stack_bst_pop(&stack);
        bst_add_node_to_items(tree, items);
        bst_leftmost_inorder(tree->right,&stack);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL){
        stack_bst_push(to_visit,tree);
        stack_bool_push(first_visit,true);
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bool_t stackBool;
    stack_bool_init(&stackBool);

    bool fromLeft;
    bst_leftmost_postorder(tree,&stack,&stackBool);
    while (!stack_bst_empty(&stack)){
        tree = stack_bst_top(&stack);
        fromLeft = stack_bool_pop(&stackBool);
        if (fromLeft) {
            stack_bool_push(&stackBool,false);
            bst_leftmost_postorder(tree->right,&stack,&stackBool);
        }else {
            stack_bst_pop(&stack);
            bst_add_node_to_items(tree, items);
        }
    }
}
