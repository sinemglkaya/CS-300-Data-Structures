#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <sstream>
#include "AvlTree.h"
#include "AvlTree.cpp"

using namespace std;

// Function to normalize a word by removing non-alphabetic characters and converting to lowercase
string newString (string & word)
{
    string result;
    for (char ch : word) {
        if (isalpha(ch)) {
            result += tolower(ch); //changing to the small character
        }
    }
    return result;
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
    AvlTree<string, WordItem *> myAvlTree; // AVL Tree to store word items

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
            string clearWord = newString(firstWord);
            if (!clearWord.empty())
            {
                AllFiles[i].file_words.push_back(clearWord); // Store words in the vector after cleaning
            }
        }

        // Now process the words for the current document
        for (const string & word : AllFiles[i].file_words)
        {
            WordItem * existingWordItem = myAvlTree.find(word);
            if (existingWordItem != nullptr) {
                // Word exists, update count in document
                auto & docItems = existingWordItem->documentItems;
                auto it = find_if(docItems.begin(), docItems.end(),
                                  [&](const DocumentItem& item) {
                                      return item.documentName == AllFiles[i].file_names;
                                  });
                if (it != docItems.end()) {
                    it->count++;
                } else {
                    docItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                }
            }
            else
            {
                // New word, create a new WordItem and insert into AVL Tree
                WordItem* newWordItem = new WordItem();
                newWordItem->word = word;
                newWordItem->documentItems.push_back(DocumentItem{AllFiles[i].file_names, 1});
                myAvlTree.insert(word, newWordItem);
            }
        }
    }

    cin.ignore(); // Clear the input buffer

    string line;
    bool flag2 = true;
    while (flag2)
    {
        cout << endl << "Enter queried words in one line: ";
        getline(cin, line); // Read the entire line of words

        // Parse the line into individual words
        istringstream iss(line);
        string input;
        vector<string> queried_words;


        while (iss >> input)
        {
            // Normalize the query word
            string orderedQuery = newString(input);
            queried_words.push_back(orderedQuery); // Store the order of queried words

        }

        if (queried_words[0] == "endofinput")
        {
            flag2 = false; // Exit loop if 'endofinput' is entered
        }

        else if (queried_words[0] == "remove" && queried_words.size() > 1)
        {
            // Remove a word from the AVL Tree
            myAvlTree.remove(queried_words[1]);
            cout << queried_words[1] << " has been REMOVED" << endl;
        }

        else
        {
            // Search for queried words in each document
            int counting_word =0;
            for (int j =0; j < numberOfFile; j++)
            {
                bool flag = true;
                int number = 0;
                for (int k=0; k < queried_words.size();k++)
                {
                    int * myNum = myAvlTree.findDocName(queried_words[k], AllFiles[j].file_names);

                    if (myNum != nullptr)
                    {
                        number++;
                    }
                }
                if (number == queried_words.size())
                {
                    // If all queried words are found in a document
                    for (int k=0; k < queried_words.size();k++)
                    {
                        int * myNum = myAvlTree.findDocName(queried_words[k], AllFiles[j].file_names);

                        if (myNum != nullptr)
                        {
                            counting_word++;

                            if (flag)
                            {
                                cout << "in Document " << AllFiles[j].file_names;
                            }
                            flag = false;
                            cout << ", " << queried_words[k] << " found " << *myNum << " times";
                        }
                    }
                    if (counting_word != 0)
                    {
                        cout << "." << endl;
                    }
                }
            }
            if (counting_word == 0)
            {
                cout << "No document contains the given query" << endl;
            }
        }
    }
    //myTree.printTree();
    return 0;
}

