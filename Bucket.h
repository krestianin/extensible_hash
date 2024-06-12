#include <vector>

class Bucket {
private:
    std::vector<int> keys;
    int localDepth;
    int cap; 

public:
    Bucket(int depth, int cap);
    bool isFull() const;
    void insert(int key);
    bool remove(int key);
    int getLocalDepth() const;
    void increaseDepth();
    std::vector<int>& getKeys(); 
    bool find(int key) const;
    void clear();
};
