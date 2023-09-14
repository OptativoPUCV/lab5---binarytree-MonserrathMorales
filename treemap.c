#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

/* Esta función recibe la función de comparación de claves y crea un mapa (TreeMap) inicializando sus variables. 
El siguiente código muestra como inicializar la función de comparación. Reserve memoria, inicialice el resto de variables y retorne el mapa. */
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *) malloc(sizeof(TreeMap));
    if(map == NULL) exit(EXIT_FAILURE);
  
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}

/* inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo.
Para insertar un dato, primero debe realizar una búsqueda para encontrar donde debería ubicarse. Luego crear el nuevo nodo y enlazarlo. Si la clave del dato ya existe retorne sin hacer nada (recuerde que el mapa no permite claves repetidas). */
void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * nodo = (TreeNode *) malloc(sizeof(TreeNode));
    if(nodo == NULL) exit(EXIT_FAILURE);
    nodo->pair = (Pair *) malloc(sizeof(Pair));
  
    nodo->pair->key = key;
    nodo->pair->value = value;
    nodo->left = NULL;
    nodo->right = NULL;
    nodo->parent = NULL;

    if(tree->root == NULL) {
      tree->root = nodo;
      return;
    }
    tree->current = tree->root;
    while(1) {
      // caso 1: la clave ya existe
      if(is_equal(tree, key, tree->current->pair->key) == 1) {
        return;
      }
      // caso 2: la clave es menor que el current
      else if(tree->lower_than(key, tree->current->pair->key) == 1) {
        if(tree->current->left == NULL) {
          nodo->parent = tree->current;
          tree->current->left = nodo;
          tree->current = nodo;
          return;
        }
        else {
          tree->current = tree->current->left;
        }
      }
      // caso 3: la clave es mayor que el current
      else {
        if(tree->current->right == NULL) {
          nodo->parent = tree->current;
          tree->current->right = nodo;
          tree->current = nodo;
          return;
        }
        else {
          tree->current = tree->current->right;
        }
      }
    }
}

/* retorna el **nodo con la mínima clave** ubicado en el subárbol con raiz x. Para obtener el nodo tiene que, a partir del nodo x, irse por la rama izquierda hasta llegar al final del subárbol. Si x no tiene hijo izquierdo se retorna el mismo nodo. */
TreeNode * minimum(TreeNode * x){
    TreeNode * current = x;
  
    if(current->left == NULL) {
      return current;
    }
    while(current->left != NULL) {
      current = current->left
    }




  
    return current;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

/* busca el nodo con clave igual a key y retorna el **Pair** asociado al nodo. Si no se encuentra la clave retorna NULL.
Recuerde hacer que el current apunte al nodo encontrado. */
/* int resultado = map->lower_than(key1,key2);
Esta función retorna 1 si key1<key2. */
/* int is_equal(TreeMap* tree, void* key1, void* key2);
Retorna 1 si las claves son iguales y 0 si no lo son. */
Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;
    while(tree->current != NULL) {
      if(is_equal(tree, key, tree->current->pair->key) == 1) {
        return tree->current->pair;
      }
      else if(tree->lower_than(key, tree->current->pair->key) == 1) {
        tree->current = tree->current->left;
      }
      else {
        tree->current = tree->current->right;
      }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
