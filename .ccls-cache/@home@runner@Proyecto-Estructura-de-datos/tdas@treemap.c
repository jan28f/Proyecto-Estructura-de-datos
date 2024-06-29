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

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2))
{
    TreeMap *nuevoArbol = (TreeMap *)malloc(sizeof(TreeMap));
    if (nuevoArbol == NULL) return NULL;

    nuevoArbol->root = NULL;
    nuevoArbol->current = NULL;
    nuevoArbol->lower_than = lower_than;

    return nuevoArbol;
}


void insertTreeMap(TreeMap *tree, void *key, void *value)
{
    if (tree == NULL) return;

    TreeNode *nuevoNodo = createTreeNode(key, value);
    if (tree->root == NULL)
    {
        tree->root = nuevoNodo;
        tree->current = nuevoNodo;
    }
    else
    {
        TreeNode *actual = tree->root;
        TreeNode *padreActual = NULL;
        while (actual != NULL)
        {
            padreActual = actual;
            if (is_equal(tree, key, actual->pair->key))
            {
                free(nuevoNodo);
                return;
            }
            else
            {
                if (tree->lower_than(key, actual->pair->key))
                    actual = actual->left;
                else
                    actual = actual->right;
            }
        }

        nuevoNodo->parent = padreActual;
        if (tree->lower_than(key, padreActual->pair->key))
            padreActual->left = nuevoNodo;
        else
            padreActual->right = nuevoNodo;
        tree->current = nuevoNodo;
    }
}

TreeNode *minimum(TreeNode *x)
{
    while (x->left != NULL)
        x = x->left;

    return x;
}


void removeNode(TreeMap *tree, TreeNode *node)
{
    if (tree == NULL || tree->root == NULL) return;

    // Si no tiene hijos
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    }
    else
    {
        // Si tiene 2 hijos
        if (node->left != NULL && node->right != NULL)
        {
            TreeNode *menor = minimum(node->right);
            Pair *pair = menor->pair;
            removeNode(tree, menor);
            node->pair = pair;
            
            return;
        }
        else
        {
            // Si tiene un hijo
            TreeNode *hijo = NULL;
            if (node->left != NULL) hijo = node->left;
            else hijo = node->right;
            hijo->parent = node->parent;
            
            if (node->parent->left == node) node->parent->left = hijo;
            else node->parent->right = hijo;
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *aux = tree->root;
    while (aux != NULL) // Se comienza a recorrer el arbol
    {
        // Verifica si es la clave buscada
        if (is_equal(tree, key, aux->pair->key) == 1)
        { // Si lo es
            tree->current = aux;
            return aux->pair;
        }
        else
        {
            // Si es menor, se mueve a la izquierda
            if (tree->lower_than(key, aux->pair->key) == 1)
                aux = aux->left;
            else // Si es mayor, se mueve a la derecha
                aux = aux->right;
        }
    }
    
    return NULL;
}


Pair *upperBound(TreeMap * tree, void* key)
{
    if (tree == NULL) return NULL;

    Pair *resultado = searchTreeMap(tree, key);
    if (resultado != NULL) return resultado;
    else
    {
        TreeNode *actual = tree->root;
        while (actual != NULL)
        {
            if (tree->lower_than(key, actual->pair->key))
            {
                resultado = actual->pair;
                actual = actual->left;
            }
            else
            {
                if (!tree->lower_than(key, actual->pair->key))
                    actual = actual->right;
                else
                    return actual->pair;
            }
        }

        return resultado;
    }
}

Pair *firstTreeMap(TreeMap * tree)
{
    if (tree == NULL || tree->root == NULL) return NULL;

    // Consigue al minimo, y actualiza el current
    TreeNode *aux = minimum(tree->root);
    tree->current = aux;
    
    return aux->pair;
}

Pair *nextTreeMap(TreeMap * tree)
{
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *aux = tree->current;
    if (aux->right != NULL)
    {
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    else
    {
        TreeNode *padre = aux->parent;
        while (padre != NULL && aux == padre->right)
        {
            aux = padre;
            padre = padre->parent;
        }
        tree->current = padre;
        if (padre != NULL) return padre->pair;
    }
    return NULL;
}
