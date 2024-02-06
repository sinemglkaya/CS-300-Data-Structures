//
// Created by Sinem Kaya on 19.12.2023.
//


#ifndef UNTITLED22_HASHTABLE_H
#define UNTITLED22_HASHTABLE_H


#include <vector>
#include <list>
#include <string>
#include <chrono>

using namespace std;

struct RehashInfo {
    int previousTableSize;
    int newTableSize;
    int unique_Word_Count;
    double loadFactor;
};

template <class HashedObj>
class HashTable {
public:

    explicit HashTable(const HashedObj & notFound, int size = 53, double loadFactor = 0.9);

    const HashedObj & find( const HashedObj & x ) const;

    void makeEmpty();

    void insert2(const HashedObj & item, const string& currentDocName);
    void insertForRehash(const HashedObj & x);

    const HashTable & operator=( const HashTable & rhs );
    const HashedObj & getNotFoundItem() const {
        return ITEM_NOT_FOUND;
    }

    int getCurrentSize() const;
    int getTableSize() const;

    const vector<RehashInfo>& getRehashSteps() const {
        return rehashSteps;
    }

private:
    struct HashNode{
        HashedObj item;
        bool is_Occupied;
        bool is_Deleted;

        HashNode( const HashedObj & i = HashedObj( )) : is_Occupied(false), is_Deleted(false), item(i) {}
    };

    vector<RehashInfo> rehashSteps;

    vector<HashNode> hash_table;
    int current_Size;
    const HashedObj ITEM_NOT_FOUND;
    double loadFactor; // Load factor for the hash table

    int myhash(const HashedObj & x) const;
    bool isActive( int currPos ) const;
    int findPosition( const HashedObj & x ) const;
    void rehash( );
};



#endif //UNTITLED22_HASHTABLE_H