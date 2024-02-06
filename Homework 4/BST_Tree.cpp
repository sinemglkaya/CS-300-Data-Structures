//
// Created by Sinem Kaya on 23.11.2023.
//

#include <string>
#include "BST_Tree.h"

using namespace std;

struct DocumentItem{
    string documentName;
    int count;
};

struct WordItem{
    string word;
    vector<DocumentItem> documentItems; // List of DocumentItem’s. In order to keep the documents
    bool operator!=(const WordItem& rhs) const {
        return this->word != rhs.word;
    }
    bool operator==(const WordItem& rhs) const {
        return this->word == rhs.word;
    }
    // just in case if I use
};

// This function inserts a new key-value pair into the tree
template <class Key, class Value>
void BST_Tree<Key, Value>::insert(const Key &x, const Value &y, BSTNode<Key, Value> *&t) const {
    if (t == nullptr) {
        t = new BSTNode<Key, Value>(x, y, nullptr, nullptr);
    } else if (x < t->key) {
        insert(x, y, t->left);
    } else if (t->key < x) {
        insert(x, y, t->right);
    }
}

// The destructor and makeEmpty function are used for deallocating memory and cleaning up the tree
template <class Key, class Value>
BST_Tree<Key, Value>::~BST_Tree() {
    makeEmpty(root);
}

template <class Key, class Value>
void BST_Tree<Key, Value>::makeEmpty(BSTNode<Key, Value> *&t) const {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

// This function searches for a value given a key
template <class Key, class Value>
Value BST_Tree<Key, Value>::find(const Key &x, BSTNode<Key, Value> *t) const {
    if (t == nullptr) {
        return nullptr; // Not found
    } else if (x < t->key) {
        return find(x, t->left); // Search in the left subtree
    } else if (t->key < x) {
        return find(x, t->right); // Search in the right subtree
    }
    return t->value; // Found
}


// It searches for a document name within the node's value, and return the integer value
template <class Key, class Value>
int* BST_Tree<Key, Value>::findDocName(const Key& word, const string & docName) const
{
    BSTNode<Key, Value>* node = finding(word); // This should return BSTNode*

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

//it returns the node containing the key.
template <class Key, class Value>
BSTNode<Key, Value>* BST_Tree<Key, Value>:: finding(const Key& x, BSTNode<Key, Value> *t) const
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
