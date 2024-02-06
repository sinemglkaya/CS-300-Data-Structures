//
// Created by Sinem Kaya on 19.12.2023.
//

#include "HashTable.h"
#include "BST_Tree.cpp"

#include <vector>
#include <list>
#include <string>


//checks if a number is prime.
bool isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}

//finds the next prime number after a given number
int nextPrime( int n )
{
    if ( n % 2 == 0 )
        n++;

    for ( ; ! isPrime( n ); n += 2 )
        ;

    return n;
}

//constructor
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size, double loadFactor)
        : ITEM_NOT_FOUND(notFound), hash_table(nextPrime(size)), loadFactor(loadFactor){
    makeEmpty();
}

//Clears the hash table, setting all positions to unoccupied.
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty() {
    current_Size = 0;
    for (auto & entry : hash_table)
        entry.is_Occupied = false;
}

template <class HashedObj>
void HashTable<HashedObj>::insert2(const HashedObj & item, const string& currentDocName) {
    int currPos = findPosition(item);

    //It finds the correct position for the item
    if (isActive(currPos)) {
        // Word found, update DocumentItem
        auto & docItems = hash_table[currPos].item.documentItems;
        auto it = find_if(docItems.begin(), docItems.end(),
                          [&](const DocumentItem & di) { return di.documentName == currentDocName; });

        if (it != docItems.end()) {
            // Document already exists, increase count
            it->count++;
        } else {
            // New document, add to list
            hash_table[currPos].item.documentItems.push_back(DocumentItem{currentDocName, 1});
        }
    }
    else
    {
        // word cannot found insert new
        hash_table[currPos] = HashNode(item);
        hash_table[currPos].is_Occupied = true;
        current_Size++;
    }
    //If the load factor is exceeded, it triggers rehashing.
    if (current_Size >= hash_table.size() * loadFactor) {
        rehash();
    }

}

//sed during rehashing to insert items into the new hash table.
template <class HashedObj>
void HashTable<HashedObj>::insertForRehash(const HashedObj & x)
{
    // Insert x as active
    int currPos = findPosition(x);
    if (isActive(currPos))
        return;

    hash_table[currPos] = HashNode(x);
    hash_table[currPos].is_Occupied = true;

    if (++current_Size > hash_table.size() / 2)
        rehash();
}

//Searches for an item in the hash table and returns it if found.
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x) const {
    int currPos = findPosition(x);
    if (isActive(currPos))
        return hash_table[currPos].item;
    else
        return ITEM_NOT_FOUND;
}

//Return the total size of the hash table
template <class HashedObj>
int HashTable<HashedObj>::getTableSize() const {
    return hash_table.size();
}

//Return the number of elements in the hash table
template <class HashedObj>
int HashTable<HashedObj>::getCurrentSize() const {
    return current_Size;
}

// this function ensures that the hash table operates efficiently even as more elements are added.
template <class HashedObj>
void HashTable<HashedObj>::rehash() {

    int previousTableSize = hash_table.size();
    double previousLoadFactor = static_cast<double>(current_Size) / previousTableSize;

    // Create new double-sized, empty table
    vector<HashNode> old_Table = hash_table;
    hash_table.resize(nextPrime(2 * previousTableSize));
    for (auto & entry : hash_table) {
        entry.is_Occupied = false;
    }

    // Copy table over
    current_Size = 0;
    for (auto & entry : old_Table)
        if (entry.is_Occupied)
            insertForRehash(entry.item);

    int newTableSize = hash_table.size();
    double currentLoadFactor = static_cast<double>(current_Size) / newTableSize;

    // Populate rehash information
    RehashInfo info;
    info.previousTableSize = previousTableSize;
    info.newTableSize = newTableSize;
    info.unique_Word_Count = current_Size + 1;
    info.loadFactor = currentLoadFactor;

    // Add rehash information to the vector
    rehashSteps.push_back(info);
}

//Checks if a position in the hash table is occupied and not marked as deleted.
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currPos) const {
    return hash_table[currPos].is_Occupied && !hash_table[currPos].is_Deleted;
}

//Finds the appropriate position for an item in the hash table using a linear probing method.
template <class HashedObj>
int HashTable<HashedObj>::findPosition(const HashedObj & x) const {
    int offset = 1;
    int currPos = myhash(x);

    // Assume table is half-empty, and table.length is prime.
    while (hash_table[currPos].is_Occupied && hash_table[currPos].item != x)
    {
        currPos += offset;  // Compute ith probe
        offset += 2;
        if (currPos >= hash_table.size())
            currPos -= hash_table.size();
    }
    return currPos;
}

//Allows for the assignment of one HashTable object to another.
template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=(const HashTable & rhs) {
    if (this != &rhs) {
        hash_table = rhs.hash_table;
        current_Size = rhs.current_Size;
    }
    return * this;
}

/**
 * it is a hash function that converts a HashedObj into an integer hash value.
 * This function iterates over each character in the word string of WordItem,
 * combining them into an integer hash value.
 */

template <class HashedObj>
int HashTable<HashedObj>::myhash(const HashedObj & x) const {
    int hashVal = 0;
    for (char ch : x.word) {
        hashVal = 37 * hashVal + ch;
    }
    hashVal %= hash_table.size();
    if (hashVal < 0) {
        hashVal += hash_table.size();
    }
    return hashVal;
}

// Template class instantiation
template class HashTable<WordItem>;


