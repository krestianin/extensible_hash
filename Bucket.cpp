#include <vector>
#include "Bucket.h"
using std::vector;

    Bucket::Bucket(int depth, int cap) : localDepth(depth), cap(cap) {};

    bool Bucket::isFull() const
    {
        return keys.size() >= cap;
    }
    void Bucket::insert(int key)
    {
        if (!isFull()) {
            keys.push_back(key);
        } else {
            throw std::runtime_error("Attempt to insert into a full bucket, splitBucket() might be not working properly");
        }
    }
    bool Bucket::remove(int key)
    {
        return (keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end()) != keys.end());
    }
    int Bucket::getLocalDepth() const
    {
        return localDepth;
    }
    void Bucket::increaseDepth()
    {
        localDepth++;
    }
    vector<int>& Bucket::getKeys()
    {
        return keys;
    } 
    bool Bucket::find(int key) const {
        return std::find(keys.begin(), keys.end(), key) != keys.end();
    }

    void Bucket::clear()
    {
        keys.clear();
    } // Clear keys after redistribution
    
