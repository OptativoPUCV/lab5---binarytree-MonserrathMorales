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

/* Esta función recibe la función de comparación de claves y se crea un mapa para el árbol con su respectiva asignacion de memoria e inicializacion de cada componente de este. Se retorna el mapa creado.*/
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *) malloc(sizeof(TreeMap));
    if(map == NULL) exit(EXIT_FAILURE); // asignacion de memoria fallida
  
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}

/* Esta funcion inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo. Para esto, se  crea un nuevo nodo y se le asigna memoria para el par clave-valor y para los punteros izquierdo, derecho y padre.
--- Condiciones de busqueda: ---
- Si el árbol está vacío (sin raíz), se establece nodo como la raíz del árbol y se sale de la función.
- Si el árbol no está vacío, y si la clave ya existe en el árbol, no se inserta. El nuevo nodo se coloca a la izquierda si la clave es menor que el nodo actual, y a la derecha si es mayor.*/
void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * nodo = (TreeNode *) malloc(sizeof(TreeNode));
    if(nodo == NULL) exit(EXIT_FAILURE);
    nodo->pair = (Pair *) malloc(sizeof(Pair));
  
    nodo->pair->key = key;
    nodo->pair->value = value;
    nodo->left = NULL;
    nodo->right = NULL;
    nodo->parent = NULL;

    // arbol vacío
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

/* Esta función encuentra el nodo con la clave más pequeña en un árbol binario de búsqueda a partir de un nodo dado x siguiendo la rama izquierda del árbol hasta que no haya más hijos izquierdos. Luego devuelve ese nodo. Si x no tiene hijo izquierdo se retorna el mismo nodo. */
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

/* Esta función elimina un nodo de un arbol binario de busqueda. 
--- Existen 3 casos: ---
- nodo sin hijos Se anula el puntero del padre que apuntaba al nodo.
- nodo con un hijo: El padre del nodo pasa a ser padre de su hijo.
- nodo con dos hijos: Se encuentra el sucesor (el nodo con la clave más pequeña en el subárbol derecho) con la funcion minimum y se copian sus valores al nodo actual. Luego se elimina el sucesor llamando a removeNode recursivamente. */
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

/* Esta funcion busca el nodo con clave igual a key desde la raiz hasta llegar a una hoja o encontrar una coincidencia con la clave buscada.
--- Comparaciones en la busqueda: ---
- Si las claves son iguales, se devuelve el par asociado con ese nodo.
- Si la clave buscada es menor que la clave del nodo actual, se mueve hacia la izquierda explorando el subárbol izquierdo actualizando el puntero a current.
- Si la clave buscada es mayor que la clave del nodo actual, se mueve hacia la derecha explorando el subárbol derecho actualizando el puntero a current.
- Si no se encuentra un nodo con la clave buscada, la función devuelve NULL*/
Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;
    while(tree->current != NULL) {
      // misma clave
      if(is_equal(tree, key, tree->current->pair->key) == 1) {
        return tree->current->pair;
      }
      // key es menor que la clave del nodo
      else if(tree->lower_than(key, tree->current->pair->key) == 1) {
        tree->current = tree->current->left;
      }
      // key es mayor que la clave del nodo
      else {
        tree->current = tree->current->right;
      }
    }
    // no se encontró la clave
    return NULL;
}

/* Esta función busca el par clave-valor más cercano al valor dado (key) que sea mayor o igual en valor y se retorna el par clave-valor asociado a ub_node. Comienza desde la raíz hasta que el nodo temp sea NULL y temp tenga un par diferente de NULL (de lo contrario se rompe el ciclo).
--- Comparaciones en la busqueda: ---
- Si la clave dada es menor que la clave del nodo actual, se mueve el puntero temp hacia la izquierda explorando el subárbol izquierdo.
- Si la clave del nodo actual es menor que la clave dada, se mueve el puntero temp hacia la derecha explorando el subárbol derecho.
- Si ninguna de las condiciones anteriores se cumple, significa que se encontró un nodo cuya clave es igual a la clave dada y se retorna el par clave-valor asociado a ese nodo. */
Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode* ub_node = NULL;
  TreeNode* temp = tree->root;

  while(temp != NULL) {
    if(temp->pair != NULL) {
      // key es menor que la clave del nodo
      if(tree->lower_than(key, temp->pair->key) == 1) {
      ub_node = temp;
      temp = temp->left;
      }
      // la clave del nodo es menor que key
      else if(tree->lower_than(temp->pair->key, key) == 1) {
        temp = temp->right;
      }
      // misma clave
      else {
        return temp->pair;
      }
    }
    else {
      break;
    }
  }
  // se retorna NULL si todas las claves del arbol son menores
  if(ub_node != NULL) {
    return ub_node->pair;
  }
  else {
    return NULL;
  }
}

/* Esta funcion retorna el primer par clave-valor más pequeño del mapa y actualiza el puntero current del árbol para apuntar a ese nodo. */
Pair * firstTreeMap(TreeMap * tree) {
    if(tree->root == NULL) return NULL;
    TreeNode * minimo = minimum(tree->root);
    tree->current = minimo;
    return minimo->pair;
}

/* Esta funcion busca el siguiente par clave-valor en orden después del nodo actual current.
--- Comparaciones de busqueda: ---
- Si hay un subárbol derecho, se actualiza el puntero current al nodo mínimo en ese subárbol derecho utilizando la función minimum y luego retorna el par clave-valor de ese nodo mínimo.
- Si no hay subárbol derecho (es un nodo hoja o un nodo interno sin hijo derecho), se retrocede a través de los ancestros hasta encontrar un nodo que sea hijo izquierdo de su padre (o hasta que no hayan mas ancestros), se actualiza el current y retorna su par clave-valor.
- Si llega a la raíz sin encontrar un nodo adecuado retorna NULL (no hay un siguiente nodo en orden). */
Pair * nextTreeMap(TreeMap * tree) {
    if(tree->root == NULL || tree->current == NULL) return NULL;

    // siguiente al current en el subarbol derecho (1 hijo a la derecha)
    if(tree->current->right != NULL) {
      tree->current = minimum(tree->current->right);
      return tree->current->pair;
    }
    // ancestro al current en el subarbol derecho (no tiene hijo derecho)
    while (tree->current->parent != NULL && tree->current == tree->current->parent->right) { // (busqueda del ancestro)
        tree->current = tree->current->parent;
    }
    if (tree->current->parent != NULL) {
    tree->current = tree->current->parent;
    return tree->current->pair;
    }
  
    return NULL;
}