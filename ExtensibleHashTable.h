#include <vector>
#include "Bucket.h"

class ExtensibleHashTable {
private:
    std::vector<Bucket*> directory;
    int globalDepth;
    int bucketCapacity;

    int hash(int key) const;
    void splitBucket(int index);
    void doubleDirectory();
    void clear();

public:
    ExtensibleHashTable(int bucketSize = 4);
    ~ExtensibleHashTable();
    ExtensibleHashTable(const ExtensibleHashTable& other);
    ExtensibleHashTable& operator=(const ExtensibleHashTable& other); 

    bool find(int key) const;
    void insert(int key);
    bool remove(int key);
    void print() const;
};
