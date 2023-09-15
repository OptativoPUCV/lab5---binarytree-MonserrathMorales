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
      current = current->left;
    }
  return current;
}

/* elimina el nodo *node* del árbol *tree*. Recuerde que para eliminar un node existen 3 casos:
**Nodo sin hijos:** Se anula el puntero del padre que apuntaba al nodo
**Nodo con un hijo:** El padre del nodo pasa a ser padre de su hijo
**Nodo con dos hijos:** Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). Reemplace los datos (key,value) de *node* con los del nodo "minimum". Elimine el nodo minimum (para hacerlo puede usar la misma función *removeNode*). */
void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;
  
    // caso 1: eliminar nodo sin hijos
    if(node->left == NULL && node->right == NULL) {
      if(node == tree->root) {
        tree->root = NULL;
      }
      else {
        if(node->parent->left == node) {
          node->parent->left = NULL;
        }
        else {
          node->parent->right = NULL;
        }
      }
      free(node);
    }

    // caso 2: eliminar un nodo con un hijo
    else if((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL)) {
      if(node == tree->root) {
        if(node->left != NULL) {
          tree->root = node->left;
          node->left->parent = NULL;
        }
        else {
          tree->root = node->right;
          node->right->parent = NULL;
        }
      }
      else {
        if(node->parent->left == node) { // el nodo esta a la izquierda del padre
          if(node->left != NULL) { // el hijo esta a la izquierda
            node->parent->left = node->left;
            node->left->parent = node->parent;
          }
          else { // el hijo esta a la derecha
            node->parent->left = node->right;
            node->right->parent = node->parent;
          }
        }
        else { // el nodo esta a la derecha del padre
          if(node->left != NULL) { // el hijo esta a la izquierda
            node->parent->right = node->left;
            node->left->parent = node->parent;
          }
          else { // el hijo esta a la derecha
            node->parent->right = node->right;
            node->right->parent = node->parent;
          }
        }
      free(node);
      }
    }
    // caso 3: eliminar un nodo con dos hijos
    else {
      TreeNode * sucesor = minimum(node->right);
      node->pair->key = sucesor->pair->key;
      node->pair->value = sucesor->pair->value;
      removeNode(tree, sucesor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key) {
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

/* retorna el **Pair** con clave igual a key. En caso de no encontrarlo retorna el primer par asociado a una clave mayor o igual a key. 
Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave *mayor o igual a key*. Finalmente retorne el par del nodo ub\_node. */
Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode* ub_node = NULL;
  TreeNode* temp = tree->root;

  while(tree->root != NULL) {
    if(tree->lower_than(key, temp->pair->key) == 1) {
      ub_node = temp;
      temp = temp->left;
    }
    else if(tree->lower_than(temp->pair->key, key) == 1) {
      temp = temp->right;
    }
    // clave = clave del nodo
    else {
      return temp->pair;
    }
  }
  // Null si todas las claves son menores
  return ub_node->pair;
}

/* Pair* firstTreeMap(TreeMap* tree) retorna el primer **Pair** del mapa (el menor).
Pair* nextTreeMap(TreeMap* tree)  retornar el siguiente **Pair** del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero. */
Pair * firstTreeMap(TreeMap * tree) {
    if(tree->root == NULL) return NULL;
    TreeNode * minimo = minimum(tree->root);
    tree->current = minimo;
    return minimo->pair;
}

/* NEXT se refiere al valor más cercano al current, por ejemplo 12->13 en el subarbol derecho*/
Pair * nextTreeMap(TreeMap * tree) {
    if(tree->root == NULL || tree->current == NULL) return NULL;

    // siguiente al current en el subarbol derecho (1 hijo a la derecha)
    if(tree->current->right != NULL) {
      tree->current = minimum(tree->current->right);
      return tree->current->pair;
    }
    // ancestro al current en el subarbol derecho (no tiene hijo derecho)
    while (tree->current->parent != NULL && tree->current == tree->current->parent->right) {
        tree->current = tree->current->parent;
    }
    if (tree->current->parent != NULL) {
    tree->current = tree->current->parent;
    return tree->current->pair;
    }
  
    return NULL;
}