#include "ExtensibleHashTable.h"
#include <iostream>
#include <unordered_map>
#include <vector>
using std::cout, std::endl, std::vector;

ExtensibleHashTable::ExtensibleHashTable(int bucketSize) : bucketCapacity(bucketSize), globalDepth(1) {
    // Initialize directory with 2^globalDepth buckets
    directory.resize(2); 
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

    // cout << endl;
    // cout  << bucketIndex % (1 << oldBucket->getLocalDepth()) << endl;

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
    int index = hash(key);  
    if (directory[index] != nullptr) {
        return directory[index]->remove(key);
    }
    return false;
}

void ExtensibleHashTable::print() const {
    vector<const Bucket*> printedBuckets;  // Vector to track printed buckets

    for (size_t i = 0; i < directory.size(); ++i) {
        Bucket* currentBucket = directory[i];

        cout << i << ": ";

        if (std::find(printedBuckets.begin(), printedBuckets.end(), currentBucket) == printedBuckets.end()) {
            cout << std::hex << std::showbase << reinterpret_cast<uintptr_t>(currentBucket) << std::dec;

            if (currentBucket != nullptr) {
                const vector<int>& keys = currentBucket->getKeys();
                cout << " --> [";
                for (size_t j = 0; j < keys.size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << keys[j];  
                }
                if (keys.size() == 0)
                    cout << " - ";
                for (size_t j = keys.size(); j < bucketCapacity; j++) {
                    cout << ", - ";
                }
                cout << "] (" << currentBucket->getLocalDepth() << ")";
                printedBuckets.push_back(currentBucket);  
                cout << endl;
            }
        } else {
            cout << currentBucket;
            cout << endl;
        }
    }
}
