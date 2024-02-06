//
// Created by Sinem Kaya on 23.11.2023.
//

#ifndef UNTITLED21_AVLTREE_H
#define UNTITLED21_AVLTREE_H
#include <string>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

template <class Key, class Value>
class BST_Tree;

template <class Key, class Value>
class BSTNode{
    Key key;
    Value value;
    BSTNode *left;
    BSTNode *right;
    BSTNode(const Key & theKey, const Value & theValue, BSTNode * lt, BSTNode * rt) : key(theKey), value(theValue), left(lt), right(rt) {}
    friend class BST_Tree <Key, Value>;
};


template <class Key, class Value>
class BST_Tree{
public:
    BST_Tree(): root(nullptr) {}
    ~BST_Tree();


    void insert(const Key & x, const Value & y) {
        insert(x, y, root);
    }
    Value find (const Key & x) const
    {
        return find(x, root); // Calls the private, recursive version of find
    }

    int * findDocName(const Key& word, const string & docName) const;

    BSTNode<Key, Value>* finding(const Key& k) const
    {
        return finding(k, root); // Start the search from the root
    }

private:
    BSTNode <Key, Value> * root;

    Value find(const Key & x, BSTNode<Key, Value> * t) const;

    BSTNode<Key, Value>* finding(const Key& x, BSTNode<Key, Value> *t) const;


    void insert(const Key & x, const Value & y, BSTNode<Key, Value> * & t) const;
    void makeEmpty(BSTNode<Key, Value> * & t) const;
};

#endif //UNTITLED21_AVLTREE_H



