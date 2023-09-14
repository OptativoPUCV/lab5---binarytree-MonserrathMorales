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


void insertTreeMap(TreeMap * tree, void* key, void * value) {

}

TreeNode * minimum(TreeNode * x){

    return NULL;
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
Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* nodo = (TreeNode *) malloc(sizeof(TreeNode));
    if(nodo == NULL) exit(EXIT_FAILURE);
    long indice = 0;
  
    while(1) {
      if(nodo->pair[indice]->key && strcmp(nodo->pair[indice]->key, key) == 0) {
        tree->current = nodo;
        return nodo->pair[tree->current];
      }
      indice++;
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
