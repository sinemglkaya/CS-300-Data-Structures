#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "BST_Tree.h"
#include "BST_Tree.cpp"
#include "HashTable.h"

using namespace std;


vector<string> newString(const string& word) {
    vector<string> resultParts;
    string part;

    for (char ch : word) {
        if (isalpha(ch)) {
            // Add to the current part if it's an alphabetic character
            part += tolower(ch);
        } else {
            // If non-alphabetic and there is a current part, push it to the result
            if (!part.empty()) {
                resultParts.push_back(part);
                part.clear();
            }
        }
    }

    // Add the last part if there's any
    if (!part.empty()) {
        resultParts.push_back(part);
    }

    return resultParts;
}


// Struct to store file content with file name and a vector of words in the file
struct fileContent{
    string file_names;
    vector<string> file_words;
};


int main()
{
    int numberOfFile;
    cout << "Enter number of input files: ";
    cin >> numberOfFile;
    vector<fileContent> AllFiles(numberOfFile);
    BST_Tree<string, WordItem *> myBST_Tree; // BSTree to store word items
    // Initialize HashTable
    WordItem notFoundItem;
    HashTable<WordItem> myHashTable(notFoundItem); // Adjust the size as needed
    vector<int> BST_Search_Time;
    vector<int> Hash_Search_Times;
    int durationBST;
    int durationHash;



    for (int i = 0; i < numberOfFile; ++i)
    {
        cout << "Enter " << i + 1 << ". filename: ";
        cin >> AllFiles[i].file_names;

        ifstream file (AllFiles[i].file_names);
        if (!file) {
            cout << AllFiles[i].file_names << " cannot open the file." << endl;
            continue;
        }

        string firstWord;
        while (file >> firstWord) {
            vector<string> clearWords = newString(firstWord);
            for (const string& word : clearWords) {
                if (!word.empty()) {
                    AllFiles[i].file_words.push_back(word);// Store each word in the vector after cleaning
                }
            }
        }


        // Now process the words for the current document

        auto startBST = chrono::high_resolution_clock::now();
        for (const string & word : AllFiles[i].file_words)
        {
            WordItem * existingWordItem = myBST_Tree.find(word);
            if (existingWordItem != nullptr) {
                // Word exists, update count in document
                auto & docItems = existingWordItem->documentItems;
                auto iterator = find_if(docItems.begin(), docItems.end(),
                                  [&](const DocumentItem& item) {
                                      return item.documentName == AllFiles[i].file_names;
                                  });
                if (iterator != docItems.end()) {
                    iterator->count++;
                } else {
                    docItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                }
            }
            else
            {
                // New word, create a new WordItem and insert into AVL Tree
                WordItem *newWordItem = new WordItem();
                newWordItem->word = word;
                newWordItem->documentItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                myBST_Tree.insert(word, newWordItem);
            }
        }
        auto endBST = chrono::high_resolution_clock::now();
        durationBST = chrono::duration_cast<chrono::microseconds>(endBST - startBST).count();
        BST_Search_Time.push_back(durationBST);

        //start time
        auto startHash = chrono::high_resolution_clock::now();
        // Now process the words for the current document
        for (const string & word : AllFiles[i].file_words) {
            WordItem secondItem;
            secondItem.word = word;
            auto & docItems = secondItem.documentItems;

            const WordItem & foundItem = myHashTable.find(secondItem);

            if (foundItem != notFoundItem) {
                // Word exists, update count in document
                auto iterator = find_if(docItems.begin(), docItems.end(),
                                  [&](const DocumentItem& item) {
                                      return item.documentName == AllFiles[i].file_names;
                                  });
                if (iterator != docItems.end()) {
                    iterator->count++;
                } else {
                    docItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                }
                myHashTable.insert2(secondItem, AllFiles[i].file_names);
            }
            else {
                // New word, insert into Hash Table
                secondItem.documentItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                myHashTable.insert2(secondItem, AllFiles[i].file_names);
            }
        }
        auto endHash = chrono::high_resolution_clock::now();
        durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash).count();
        Hash_Search_Times.push_back(durationHash);
    }

    // printing the rehash process
    const auto& rehashSteps = myHashTable.getRehashSteps();
    for (const auto& info : rehashSteps) {
        cout << "rehashed..." << endl << "previous table size:" << info.previousTableSize
             << ", new table size: " << info.newTableSize
             << ", current unique word count " << info.unique_Word_Count
             << ", current load factor: " << info.loadFactor << endl;
    }


    int unique_Word_Count = myHashTable.getCurrentSize(); // Assuming this method returns the number of unique words
    double loadRatio = static_cast<double>(unique_Word_Count) / myHashTable.getTableSize(); // Assuming getTableSize() returns the size of the hash table

    cout << endl << "After preprocessing, the unique word count is " << unique_Word_Count << ". ";
    cout << "Current load ratio is " << loadRatio << endl;


    cin.ignore(); // Clear the input buffer

    string line;

    cout << "Enter queried words in one line: ";
    getline(cin, line); // Read the entire line of words

    // Parse the line into individual words
    istringstream iss(line);
    string input;
    vector<string> queried_words;

    while (iss >> input)
    {
        // Normalize the query word
        vector<string> parts = newString(input);
        queried_words.insert(queried_words.end(), parts.begin(), parts.end());

    }

    // Search for queried words in each document
    int counting_word =0;

    for (int j =0; j < numberOfFile; j++)
    {
        bool flag = true;
        int number = 0;
        for (int k = 0; k < queried_words.size(); k++) {
            int *myNum = myBST_Tree.findDocName(queried_words[k], AllFiles[j].file_names);

            if (myNum != nullptr) {
                number++;
            }
        }
        if (number == queried_words.size())
        {
            // If all queried words are found in a document
            for (int k = 0; k < queried_words.size(); k++) {
                int * myNum = myBST_Tree.findDocName(queried_words[k], AllFiles[j].file_names);

                if (myNum != nullptr) {
                    counting_word++;

                    if (flag) {
                        cout << "in Document " << AllFiles[j].file_names;
                    }
                    flag = false;
                    cout << ", " << queried_words[k] << " found " << *myNum << " times";
                }
            }
            if (counting_word != 0) {
                cout << "." << endl;
            }
        }
    }

    for (int j =0; j < numberOfFile; j++)
    {
        bool contain_allwords = true;
        for (const string& queriedWord : queried_words) {
            WordItem secondItem;
            secondItem.word = queriedWord;
            const WordItem& foundItem = myHashTable.find(secondItem);
            const WordItem& notFoundItem = myHashTable.getNotFoundItem();
            if (foundItem == notFoundItem) {
                contain_allwords = false;
                break; // Word not found in the hash table
            }

            // Check if the word is in the current document
            auto iterator = find_if(foundItem.documentItems.begin(), foundItem.documentItems.end(),
                              [&](const DocumentItem& item) {
                                  return item.documentName == AllFiles[j].file_names;
                              });

            if (iterator == foundItem.documentItems.end()) {
                contain_allwords = false;
                break; // Word not in this document
            }
        }

        if (contain_allwords) {
            cout << "in Document " << AllFiles[j].file_names;
            for (const string& queriedWord : queried_words) {
                WordItem secondItem;
                secondItem.word = queriedWord;
                const WordItem& foundItem = myHashTable.find(secondItem);

                auto iterator = find_if(foundItem.documentItems.begin(), foundItem.documentItems.end(),
                                  [&](const DocumentItem& item) {
                                      return item.documentName == AllFiles[j].file_names;
                                  });

                if (iterator != foundItem.documentItems.end()) {
                    cout << ", " << queriedWord << " found " << iterator->count << " times";
                    counting_word++;
                }
            }
            cout << "." << endl;
        }
    }

    if (counting_word == 0)
    {
        cout << "No document contains the given query" << endl;
        cout << "No document contains the given query" << endl;
    }

    cout << endl << "Time: " << durationBST << endl;
    cout << endl << "Time: " << durationHash << endl;

    // Calculate and print the speed up
    double speed_Up = static_cast<double>(durationBST) / durationHash;
    cout << "Speed Up: " << speed_Up << endl;

    return 0;
}
