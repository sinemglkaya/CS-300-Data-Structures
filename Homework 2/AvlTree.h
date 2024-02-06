//
// Created by Sinem Kaya on 23.11.2023.
//

#ifndef UNTITLED21_AVLTREE_H
#define UNTITLED21_AVLTREE_H
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct DocumentItem{
    string documentName;
    int count;
};

struct WordItem{
    string word;
    vector<DocumentItem> documentItems; // List of DocumentItem’s. In order to keep the documents
};

template <class Key, class Value>
class AvlTree;

template <class Key, class Value>
class AvlNode{
    Key key;
    Value value;
    AvlNode *left;
    AvlNode *right;
    int height;
    AvlNode(const Key & theKey, const Value & theValue, AvlNode * lt, AvlNode * rt, int h = 0) : key(theKey), value(theValue), left(lt), right(rt), height(h) {}
    friend class AvlTree <Key, Value>;
};


template <class Key, class Value>
class AvlTree{
public:
    AvlTree(): root(nullptr) {}
    ~AvlTree();


    void insert(const Key & x, const Value & y) {
        insert(x, y, root);
    }

    void printTree() const {
        if (isEmpty()) {
            cout << "Empty tree" << endl;
        }
        else
        {
            printTree(root);
        }
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    Value find (const Key & x) const
    {
        return find(x, root); // Calls the private, recursive version of find
    }

    int * findDocName(const Key& word, const string & docName) const;

    AvlNode<Key, Value>* finding(const Key& k) const
    {
        return finding(k, root); // Start the search from the root
    }

    void remove(const Key& x)
    {
        remove(x, root );
    }

private:
    AvlNode <Key, Value> * root;

    Value find(const Key & x, AvlNode<Key, Value> * t) const;

    AvlNode<Key, Value>* finding(const Key& x, AvlNode<Key, Value> *t) const;
    AvlNode<Key, Value>* findMin(AvlNode<Key, Value>* t) const;


    void insert(const Key & x, const Value & y, AvlNode<Key, Value> * & t) const;
    void makeEmpty(AvlNode<Key, Value> * & t) const;
    void printTree(AvlNode<Key, Value> *t) const;
    int height(AvlNode<Key, Value> *t) const;
    int max(int lhs, int rhs) const;

    void rotate_With_LeftChild(AvlNode<Key, Value> * & k2Value) const;
    void rotate_With_RightChild(AvlNode<Key, Value> * & k1Value) const;
    void double_With_LeftChild(AvlNode<Key, Value> * & k3Value) const;
    void double_With_RightChild(AvlNode<Key, Value> * & k4Value) const;
    void remove (const Key& x, AvlNode<Key, Value> *& t) const;
};


#endif //UNTITLED21_AVLTREE_H



