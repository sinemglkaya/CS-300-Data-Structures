//
// Created by Sinem Kaya on 23.11.2023.
//
#include <iostream>
#include <string>
#include "AvlTree.h"

using namespace std;

//This function computes the height of a node in the tree
template <class Key, class Value>
int AvlTree<Key, Value>::height(AvlNode<Key, Value> *t) const
{
    if (t == nullptr)
    {
        return -1;
    }
    return t->height;
}

// Utility function to get the maximum of two integers
template <class Key, class Value>
int AvlTree<Key, Value>::max(int lhs, int rhs) const
{
    if (rhs < lhs)
    {
        return lhs;
    }
    return rhs;
}

// Single rotation with left child (For right-right case)
template <class Key, class Value>
void AvlTree <Key, Value> :: rotate_With_LeftChild (AvlNode<Key, Value> * & k2Value) const
{
    AvlNode<Key, Value> * k1Value = k2Value->left;
    k2Value->left = k1Value->right;
    k1Value->right = k2Value;
    k2Value->height = max(height(k2Value->left), height(k2Value->right)) + 1;
    k1Value->height = max(height(k1Value->left), k2Value->height) + 1;
    k2Value = k1Value; // new root
}

// Single rotation with right child (For left-left case)
template <class Key, class Value>
void AvlTree <Key, Value> :: rotate_With_RightChild(AvlNode<Key, Value> * & k1Value) const
{
    AvlNode<Key, Value> *k2Value = k1Value->right;
    k1Value->right = k2Value->left;
    k2Value->left = k1Value;
    k1Value->height = max(height(k1Value->left), height(k1Value->right)) + 1;
    k2Value->height = max(height(k2Value->right), k1Value->height) + 1;
    k1Value = k2Value; // new root
}

// Double rotation: left child with its right child (For left-right case)
template <class Key, class Value>
void AvlTree <Key, Value> :: double_With_LeftChild(AvlNode<Key, Value> * & k3Value) const
{
    rotate_With_RightChild(k3Value->left);
    rotate_With_LeftChild(k3Value);
}

// Double rotation: right child with its left child (For right-left case)
template <class Key, class Value>
void AvlTree<Key, Value> ::double_With_RightChild (AvlNode<Key, Value> * & k4Value) const
{
    rotate_With_LeftChild(k4Value->right);
    rotate_With_RightChild(k4Value);
}

//This function inserts a new key-value pair into the tree. It performs necessary rotations if an imbalance is detected after the insertion.
template <class Key, class Value>
void AvlTree <Key, Value> :: insert (const Key & x, const Value & y, AvlNode<Key, Value> * & t) const
{
    if (t == nullptr)
    {
        t = new AvlNode<Key, Value>(x, y, nullptr, nullptr);
    }

    else if (x < t->key)
    {
        insert(x, y, t->left);

        if ( height( t->left ) - height( t->right ) == 2 )
        {
            if (x < t->left->key )  // X was inserted to the left-left subtree
            {
                rotate_With_LeftChild( t );
            }
            else			     // X was inserted to the left-right subtree
            {
                double_With_LeftChild(t);
            }

        }
    }

    else if (t->key < x)
    {
        insert(x, y, t->right);

        if ( height( t->right ) - height( t->left ) == 2 )
        {
            // height of the right subtree increased
            if ( t->right->key < x )
            {
                // X was inserted to right-right subtree
                rotate_With_RightChild( t );
            }
            else
            {
                // X was inserted to right-left subtree
                double_With_RightChild( t );
            }
        }
    }

    else
        ;
    //it ensures that the height of each node is correctly maintained
    t->height = max( height( t->left ), height( t->right ) ) + 1;
}

//The destructor and makeEmpty function are used for deallocating memory and cleaning up the tree.
template <class Key, class Value>
AvlTree<Key, Value> :: ~AvlTree()
{
    makeEmpty(root);
}

template <class Key, class Value>
void AvlTree<Key, Value>::makeEmpty(AvlNode<Key, Value> * & t) const
{
    if (t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}
//it is a standard in-order traversal function that prints the keys and associated values.
template <class Key, class Value>
void AvlTree<Key, Value>::printTree(AvlNode<Key, Value> * t) const
{
    if (t != nullptr)
    {
        printTree(t->left);
        cout << t->key << " occurs in documents: ";
        // Now we dereference the pointer to access WordItem and then loop over the documentItems vector
        for (const DocumentItem& docItem : t->value->documentItems)
        {
            cout << docItem.documentName << "," << docItem.count << " ";
        }
        cout << endl;
        printTree(t->right);
    }
}
//This function searches for a value given a key
template <class Key, class Value>
Value AvlTree<Key, Value>:: find(const Key & x, AvlNode<Key, Value> * t) const
{
    if (t == nullptr)
    {
        return nullptr; // Not found, return nullptr
    }
    else if (x < t->key)
    {
        return find(x, t->left); // Search in the left subtree
    }
    else if (t->key < x)
    {
        return find(x, t->right); // Search in the right subtree
    }
    else
    {
        return t->value; // Found, return the value
    }
}

// It searches for a document name within the node's value, and return the integer value
template <class Key, class Value>
int* AvlTree<Key, Value>::findDocName(const Key& word, const string & docName) const
{
    AvlNode<Key, Value>* node = finding(word); // This should return AvlNode*

    if (node != nullptr)
    {
        // Iterate over the documentItems of the WordItem
        for (const auto & docItem : node->value->documentItems)
        {
            if (docItem.documentName == docName)
            {
                return new int(docItem.count); // Return the count
            }
        }
    }
    return nullptr; // Return nullptr if not found
}

//This function finds the node with the minimum key in the tree.
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::findMin(AvlNode<Key, Value>* t) const
{
    if (t == nullptr)
    {
        return nullptr; // Base case
    }
    else if (t->left == nullptr)
    {
        return t; // Found the minimum
    }
    else
    {
        return findMin(t->left); // Recurse on the left subtree
    }
}

//This function removes a node with a given key from the tree, performing rotations if necessary to maintain the tree's balance.
template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key& x, AvlNode<Key, Value> *& t) const
{
    if (t == nullptr) {
        return; // Element not found, just return
    }

    if (x < t->key) {
        remove(x, t->left);  // Just call remove, don't assign
    }
    else if (x > t->key)
    {
        remove(x, t->right); // Just call remove, don't assign
    }
    else if (t->left != nullptr && t->right != nullptr)
    {
        // Node with two children: Get the inorder successor (smallest in the right subtree)
        t->key = findMin(t->right)->key;
        remove(t->key, t->right); // Just call remove, don't assign
    }
    else
    {
        // Node with one or no child
        AvlNode<Key, Value> *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }
}

//it returns the node containing the key.
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>:: finding(const Key& x, AvlNode<Key, Value> *t) const
{
    if (t == nullptr)
    {
        return nullptr; // Not found
    }
    else if (x < t->key)
    {
        return finding(x, t->left); // Search in the left subtree
    }
    else if (t->key < x)
    {
        return finding(x, t->right); // Search in the right subtree
    }
    else
    {
        return t; // Found, return the node
    }
}
