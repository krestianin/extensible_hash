#include "ExtensibleHashTable.h"
#include <iostream>
#include <unordered_map>
#include <vector>
using std::cout, std::vector;

ExtensibleHashTable::ExtensibleHashTable(int bucketSize) : bucketCapacity(bucketSize), globalDepth(1) {
    // Initialize directory with 2^globalDepth buckets
    directory.resize(2); // nullptr
    for (int i = 0; i < 2; i++) {
        directory[i] = new Bucket(globalDepth, bucketCapacity);
    }
}
ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable& other)
{
    this->bucketCapacity = other.bucketCapacity;
    this->globalDepth = other.globalDepth;
    this->directory.resize(1 << globalDepth); 

    std::unordered_map<Bucket*, Bucket*> copiedBuckets;
    for (int i = 0; i < (1 << globalDepth); i++) 
    {
        Bucket* oldBucket = other.directory[i];
        if (copiedBuckets.find(oldBucket) == copiedBuckets.end()) 
        {
            this->directory[i] = new Bucket(other.directory[i]->getLocalDepth(), bucketCapacity);
            vector<int> keys = other.directory[i]->getKeys();
            for (int key : keys) {
                directory[i]->insert(key);
            }
            copiedBuckets[oldBucket] = this->directory[i];
        }
        else
        {
            directory[i] = copiedBuckets[oldBucket];
        }
    }
}

void ExtensibleHashTable::clear()
{
    vector<Bucket*> deletedBuckets;
    for (Bucket* bucket : directory) 
    {
        if (bucket && std::find(deletedBuckets.begin(), deletedBuckets.end(), bucket) == deletedBuckets.end()) {
            delete bucket;
            deletedBuckets.push_back(bucket);
        }
    }
}

ExtensibleHashTable::~ExtensibleHashTable() 
{
   this->clear();
}



ExtensibleHashTable& ExtensibleHashTable::operator=(const ExtensibleHashTable& other)
{
    if (this != &other) {  

        this->clear();
        directory.clear();  
        
        this->bucketCapacity = other.bucketCapacity;
        this->globalDepth = other.globalDepth;
        this->directory.resize(1 << globalDepth); 


        std::unordered_map<Bucket*, Bucket*> copiedBuckets;
        for (int i = 0; i < (1 << globalDepth); i++) 
        {
            Bucket* oldBucket = other.directory[i];
            if (copiedBuckets.find(oldBucket) == copiedBuckets.end()) 
            {
                this->directory[i] = new Bucket(other.directory[i]->getLocalDepth(), bucketCapacity);
                vector<int> keys = other.directory[i]->getKeys();
                for (int key : keys) {
                    directory[i]->insert(key);
                }
                copiedBuckets[oldBucket] = this->directory[i];
            }
            else
            {
                directory[i] = copiedBuckets[oldBucket];
            }
        }
    }
    return *this;
}

int ExtensibleHashTable::hash(int key) const{
    return key % (1 << globalDepth);
}

bool ExtensibleHashTable::find(int key) const {
    int index = hash(key);
    return directory[index]->find(key);
}

void ExtensibleHashTable::splitBucket(int bucketIndex) {
    Bucket* oldBucket = directory[bucketIndex];
    int oldDepth = oldBucket->getLocalDepth();

    if (oldDepth == globalDepth) {
        doubleDirectory();
    }

    int mask = 1 << oldDepth;

    Bucket* newBucket = new Bucket(oldDepth + 1, bucketCapacity);

    // Redistribute keys


    // directory[bucketIndex % (1 << oldBucket->getLocalDepth())] = newBucket;

    // cout << std::endl;
    // cout  << bucketIndex % (1 << oldBucket->getLocalDepth()) << std::endl;

    for (size_t i = 0; i < directory.size(); ++i) 
    {
        if ((i & mask) == mask && (directory[i] == oldBucket)) {
            directory[i] = newBucket;
        }
    }

    vector<int> oldKeys = oldBucket->getKeys();
    oldBucket->clear(); 
    oldBucket->increaseDepth(); 
    for (int key : oldKeys) {
        int newIndex = hash(key); 
        directory[newIndex]->insert(key);
    }
    }

void ExtensibleHashTable::doubleDirectory() {
    int newSize = 1 << (globalDepth + 1); // 2^(globalDepth+1)
    directory.resize(newSize);
    for (int i = newSize - 1; i >= (1 << globalDepth); i--) {
        directory[i] = directory[i % (1 << globalDepth)];
        //directory[2 * i + 1] = directory[i];
    }
    globalDepth++;
}

void ExtensibleHashTable::insert(int key) {
    int index = hash(key);
    while (directory[index]->isFull()) {

        if (std::all_of(directory[index]->getKeys().begin(), directory[index]->getKeys().end(), [key](int k) { return k == key; })) {
            throw std::runtime_error("Cannot insert: all elements in the bucket are identical and the bucket is full.");
        }
        
        splitBucket(index);
        index = hash(key);
    }
    directory[index]->insert(key);
}

bool ExtensibleHashTable::remove(int key) {
    int index = hash(key);  // Calculate the appropriate index for the key
    if (directory[index] != nullptr) {
        return directory[index]->remove(key);
    }
    return false;
}

#include <bitset>
// #include <iomanip>
int getKeyIndex(int key, int globalDepth) {
    // Convert the key to binary using std::bitset
    std::bitset<8> binary(key);  // Assuming a maximum of 32 bits for demonstration
    
    // Extract the rightmost 'globalDepth' bits from the binary representation
    int mask = (1 << globalDepth) - 1;  // Creates a bitmask with globalDepth number of 1s at the least significant positions
    int index = key & mask;  // Apply the bitmask to get the last 'globalDepth' bits as an integer

    return index;
}
// void ExtensibleHashTable::print() const {
//     std::cout << "Extensible Hash Table Structure:" << std::endl;
//     std::cout << "Global Depth: " << globalDepth << std::endl;

//     for (size_t i = 0; i < directory.size(); ++i) {
//         Bucket* currentBucket = directory[i];

//         std::bitset<4> index(i);
//         // Print the index and the pointer in hexadecimal to show where it points
//         cout << "Index " << index << ": ";
//         cout << std::hex << std::showbase << reinterpret_cast<uintptr_t>(currentBucket) << std::dec;

//         // Check if the bucket is not null and print its contents
//             const vector<int>& keys = currentBucket->getKeys();
//             cout << " --> [";
//             for (size_t j = 0; j < keys.size(); j++) {
//                 if (j > 0) cout << ", ";
//                 std::bitset<16> binaryKey(keys[j]);
//                 cout << binaryKey;
//             }
//             cout << "] (Local Depth: " << currentBucket->getLocalDepth() << ")";

//         cout << std::endl;
//     }
// }


#include <bitset>
#include <set>

void ExtensibleHashTable::print() const {
    std::set<const Bucket*> printedBuckets;  // Set to track printed buckets

    std::cout << "Extensible Hash Table Structure:" << std::endl;
    std::cout << "Global Depth: " << globalDepth << std::endl;

    for (size_t i = 0; i < directory.size(); ++i) {
        Bucket* currentBucket = directory[i];

        std::bitset<4> index(i);  // Use bitset to format the index nicely
        std::cout << "Index " << index << ": ";

        // Print the bucket's pointer in hexadecimal only if it has not been printed yet
        if (printedBuckets.find(currentBucket) == printedBuckets.end()) {
            std::cout << std::hex << std::showbase << reinterpret_cast<uintptr_t>(currentBucket) << std::dec;

            // Check if the bucket is not null and print its contents
            if (currentBucket != nullptr) {
                const std::vector<int>& keys = currentBucket->getKeys();
                std::cout << " --> [";
                for (size_t j = 0; j < keys.size(); j++) {
                    if (j > 0) std::cout << ", ";
                    std::bitset<16> binaryKey(keys[j]);  // Display key in binary
                    std::cout << binaryKey;
                }
                for (size_t j = keys.size(); j <= bucketCapacity - 1; j++) {
                    if (j == 0)
                        cout << " - ";
                    else
                        cout << ", - ";
                }
                std::cout << "] (" << currentBucket->getLocalDepth() << ")";
                printedBuckets.insert(currentBucket);  // Mark this bucket as printed
                    std::cout << std::endl;
            }
        } else {
            std::cout << std::hex << std::showbase << reinterpret_cast<uintptr_t>(currentBucket) << std::dec;
            std::cout << std::endl;
        }
    }
}