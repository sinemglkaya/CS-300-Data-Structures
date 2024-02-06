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


int searchInDocuments(const std::vector<fileContent>& allFiles, const std::string& word, const std::string& documentName) {
    // Find the document with the specified name
    auto it = std::find_if(allFiles.begin(), allFiles.end(), [&](const fileContent& f) {
        return f.file_names == documentName;
    });

    if (it != allFiles.end()) {
        // Count the occurrences of the word in the file_words vector
        int count = std::count(it->file_words.begin(), it->file_words.end(), word);
        return count;  // Return the count of the word in the document
    }

    return -1;  // Document not found
}



void quickSortFirst(vector<string>& words, int low, int high) {
    if (low < high) {
        string pivot = words[low];
        int left = low;
        int right = high;
        while (left < right) {
            while (left < right && words[right] >= pivot) right--;
            words[left] = words[right];
            while (left < right && words[left] <= pivot) left++;
            words[right] = words[left];
        }
        words[left] = pivot;
        quickSortFirst(words, low, left - 1);
        quickSortFirst(words, left + 1, high);
    }
}



void quickSortRandom(std::vector<std::string>& words, int low, int high) {
    if (low < high) {
        int pivotIndex = low + rand() % (high - low);
        std::swap(words[pivotIndex], words[low]);
        std::string pivot = words[low];
        int left = low;
        int right = high;
        while (left < right) {
            while (left < right && words[right] >= pivot) right--;
            words[left] = words[right];
            while (left < right && words[left] <= pivot) left++;
            words[right] = words[left];
        }
        words[left] = pivot;
        quickSortRandom(words, low, left - 1);
        quickSortRandom(words, left + 1, high);
    }
}

int medianOfThreeWords(std::vector<std::string>& words, int a, int b, int c) {
    if (words[a] < words[b])
        return (words[b] < words[c]) ? b : ((words[a] < words[c]) ? c : a);
    else
        return (words[a] < words[c]) ? a : ((words[b] < words[c]) ? c : b);
}

void quickSortMedian(std::vector<std::string>& words, int low, int high) {
    if (low < high) {
        int pivotIndex = medianOfThreeWords(words, low, high, (low + high) / 2);
        std::swap(words[pivotIndex], words[low]);
        std::string pivot = words[low];
        int left = low;
        int right = high;
        while (left < right) {
            while (left < right && words[right] >= pivot) right--;
            words[left] = words[right];
            while (left < right && words[left] <= pivot) left++;
            words[right] = words[left];
        }
        words[left] = pivot;
        quickSortMedian(words, low, left - 1);
        quickSortMedian(words, left + 1, high);
    }
}


void insertionSort(std::vector<std::string>& words) {
    for (size_t i = 1; i < words.size(); i++) {
        std::string key = words[i];
        int j = i - 1;
        while (j >= 0 && words[j] > key) {
            words[j + 1] = words[j];
            j--;
        }
        words[j + 1] = key;
    }
}


void merge(std::vector<std::string>& arr, int l, int m, int r) {
    int i = l, j = m + 1;
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            i++;
        } else {
            std::string value = arr[j];
            int index = j;
            while (index != i) {
                arr[index] = arr[index - 1];
                index--;
            }
            arr[i] = value;
            i++;
            m++;
            j++;
        }
    }
}

void mergeSortInPlace(std::vector<std::string>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortInPlace(arr, l, m);
        mergeSortInPlace(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void heapify(std::vector<std::string>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<string>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}



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
    int durationSearch;

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

    vector<fileContent> BinaryVector_QuickSort_First = AllFiles;
    vector<fileContent> BinaryVector_QuickSort_Random = AllFiles;
    vector<fileContent> BinaryVector_QuickSort_Median = AllFiles;
    vector<fileContent> BinaryVector_MergeSort = AllFiles;
    vector<fileContent> BinaryVector_InsertionSort = AllFiles;
    vector<fileContent> BinaryVector_HeapSort = AllFiles;




    auto startQuickSort_First = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_QuickSort_First) {
        quickSortFirst(file.file_words, 0, file.file_words.size() - 1);
    }
    auto endQuickSort_First = chrono::high_resolution_clock::now();
    int durationQuickSort_First = chrono::duration_cast<chrono::nanoseconds>(endQuickSort_First - startQuickSort_First).count();


    auto startQuickSort_Random = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_QuickSort_Random) {
        quickSortRandom(file.file_words, 0, file.file_words.size() - 1);
    }
    auto endQuickSort_Random = chrono::high_resolution_clock::now();
    int durationQuickSort_Random = chrono::duration_cast<chrono::nanoseconds>(endQuickSort_Random - startQuickSort_Random).count();


    auto startQuickSort_Median = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_QuickSort_Median) {
        quickSortMedian(file.file_words, 0, file.file_words.size() - 1);
    }
    auto endQuickSort_Median = chrono::high_resolution_clock::now();
    int durationQuickSort_Median = chrono::duration_cast<chrono::nanoseconds>(endQuickSort_Median - startQuickSort_Median).count();

    auto startMergeSort = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_MergeSort) {
        mergeSortInPlace(file.file_words, 0, file.file_words.size() - 1);
    }
    auto endMergeSort = chrono::high_resolution_clock::now();
    int durationMergeSort = chrono::duration_cast<chrono::nanoseconds>(endMergeSort - startMergeSort).count();

    auto startHeapSort = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_HeapSort) {
        heapSort(file.file_words);
    }
    auto endHeapSort = chrono::high_resolution_clock::now();
    int durationHeapSort = chrono::duration_cast<chrono::nanoseconds>(endHeapSort - startHeapSort).count();


    auto startInsertionSort = chrono::high_resolution_clock::now();
    for (auto& file : BinaryVector_InsertionSort) {
        insertionSort(file.file_words);
    }
    auto endInsertionSort = chrono::high_resolution_clock::now();
    int durationInsertionSort = chrono::duration_cast<chrono::nanoseconds>(endInsertionSort - startInsertionSort).count();


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

    bool documentContainsAllWords;
    bool anyDocumentContainsAllWords = false;

    auto startSearch = chrono::high_resolution_clock::now();

    for (int j = 0; j < numberOfFile; j++) {
        bool foundAnyWord = false;


        int inner_count = 0;
        for (const string& word : queried_words) {
            int count = searchInDocuments(BinaryVector_QuickSort_Median, word, AllFiles[j].file_names);
            if (count > 0)
            {
                inner_count++;
            }
        }

        if (inner_count == queried_words.size())
        {
            cout << "In Document " << AllFiles[j].file_names;

            for (const string& word : queried_words) {
                int count = searchInDocuments(BinaryVector_QuickSort_Median, word, AllFiles[j].file_names);
                if (count > 0) {  // Check if the word is found and count is greater than 0
                    cout << ", " << word << " found " << count << " times";
                    foundAnyWord = true;
                }
            }

            if (foundAnyWord) {
                cout << "." << endl;
            }
        }
    }

    auto endSearch = chrono::high_resolution_clock::now();
    durationSearch = chrono::duration_cast<chrono::microseconds>(endSearch - startSearch).count();

    if (counting_word == 0)
    {
        cout << "No document contains the given query" << endl;
        cout << "No document contains the given query" << endl;
        cout << "No document contains the given query" << endl;
    }

    cout << endl << "Binary Search Tree Time: " << durationBST << endl;
    cout << "Hash Table Time: " << durationHash << endl;
    cout << "Binary Search Time: " << durationSearch << endl;

    if (durationQuickSort_First > durationQuickSort_Random && durationQuickSort_Random > durationQuickSort_Median)
    {
        cout << endl << "Quick Sort(median) Time: " << durationQuickSort_Median << endl;
        cout << "Quick Sort(random) Time: " << durationQuickSort_Random << endl;
        cout << "Quick Sort(first) Time: " << durationQuickSort_First << endl;
    }
    else if (durationQuickSort_First > durationQuickSort_Median && durationQuickSort_Median > durationQuickSort_Random)
    {
        cout << endl << "Quick Sort(random) Time: " << durationQuickSort_Random << endl;
        cout << "Quick Sort(median) Time: " << durationQuickSort_Median << endl;
        cout << "Quick Sort(first) Time: " << durationQuickSort_First << endl;
    }

    else if (durationQuickSort_Random > durationQuickSort_First && durationQuickSort_First > durationQuickSort_Median)
    {
        cout << endl << "Quick Sort(median) Time: " << durationQuickSort_Median << endl;
        cout << "Quick Sort(first) Time: " << durationQuickSort_First << endl;
        cout << "Quick Sort(random) Time: " << durationQuickSort_Random << endl;
    }

    else if (durationQuickSort_Median > durationQuickSort_Random && durationQuickSort_Random > durationQuickSort_First)
    {
        cout << endl << "Quick Sort(first) Time: " << durationQuickSort_First << endl;
        cout << "Quick Sort(random) Time: " << durationQuickSort_Random << endl;
        cout << "Quick Sort(median) Time: " << durationQuickSort_Median << endl;
    }

    cout << "Merge Sort Time: " << durationMergeSort << endl;
    cout << "Heap Sort Time: " << durationHeapSort << endl;
    cout << "Insertion Sort Time: " << durationInsertionSort << endl;



    // Calculate and print the speedup
    double speed_Up1 = static_cast<double>(durationBST) / durationHash;
    cout << endl << "Speed Up BST/HST: " << speed_Up1 << endl;


    double speed_Up6 = static_cast<double>(durationMergeSort) / durationQuickSort_Median;
    cout << "Speed Up Merge Sort/Quick Sort(Median): " << speed_Up6 << endl;

    double speed_Up7 = static_cast<double>(durationHeapSort) / durationQuickSort_Median;
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << speed_Up7 << endl;

    double speed_Up8 = static_cast<double>(durationInsertionSort) / durationQuickSort_Median;
    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << speed_Up8 << endl;



    if (durationSearch > durationBST)
    {
        double speed_Up2 = static_cast<double>(durationSearch) / durationBST;
        cout << endl << "Speed Up Binary Search / Binary Search Tree Time: " << speed_Up2 << endl;
    }
    else if (durationSearch < durationBST)
    {
        double speed_Up3 = static_cast<double>(durationBST) / durationSearch;
        cout << endl << "Speed Up Binary Search Tree Time / Binary Search: " << speed_Up3 << endl;
    }


    if (durationSearch > durationHash)
    {
        double speed_Up4 = static_cast<double> (durationSearch) / durationHash;
        cout << "Speed Up Binary Search / Hash Table Time: " << speed_Up4 << endl;
    }
    else if (durationSearch < durationHash)
    {
        double speed_Up5 = static_cast<double>(durationHash) / durationSearch;
        cout << "Speed Up Hash Table / Binary Search: " << speed_Up5 << endl;
    }

    return 0;
}
