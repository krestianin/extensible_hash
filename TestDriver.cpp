#include "ExtensibleHashTable.h"
#include <iostream>
using std::cout, std::endl;

// fix if we insert bucketSize+1 identical elements.

int main() {
    ExtensibleHashTable hashTable(2); // Create a hash table with bucket capacity of 2
    std::cout << "Inserting keys..." << std::endl;
    try
    {
        hashTable.insert(1);  
        // hashTable.insert(1);
        // hashTable.insert(1); 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    hashTable.insert(1);  // Binary: ...0001
    //  hashTable.print();
    hashTable.insert(5);  // Binary: ...0101
    //  hashTable.print();
    hashTable.insert(9);  // Binary: ...1001
    //  hashTable.print();
    hashTable.insert(13); // Binary: ...1101 - All these have the last two bits as '01'

    // Print the table to check the state after initial insertions
    std::cout << "State of hash table after first insertions:" << std::endl;
    hashTable.print();

    // Insert another key that requires using the third bit to distinguish
    std::cout << "Inserting key 29 to force an additional split..." << std::endl;
    hashTable.insert(29); // Binary: ..11101 - Now we need to consider more bits

    // Print the table to check the state after additional insertion
    std::cout << "State of hash table after inserting key 29:" << std::endl;
    // hashTable.print();
    cout << endl;


    // // Test insertions
    hashTable.insert(28);
    //  hashTable.print();
    hashTable.insert(20);
    //  hashTable.print();
    hashTable.insert(30);
    //  hashTable.print();
    // hashTable.insert(40);
    //  hashTable.print();
    // hashTable.insert(50);
    // hashTable.print();

    cout << "\nhashtable before removal";
    ExtensibleHashTable hashTable2(hashTable);
    hashTable2.print();
    // // Test find
    cout << "Finding 30: " << (hashTable.find(30) ? "Found" : "Not Found") << endl;
    cout << "Finding 100: " << (hashTable.find(100) ? "Found" : "Not Found") << endl;
    cout << endl;
    // // Test removal
    cout << "hashtable" << endl;
    hashTable.remove(30);
    hashTable.print();
    cout << "After removing 30, find 30: " << (hashTable.find(30) ? "Found" : "Not Found") << endl;
    cout << endl;
    cout << "hashtable2" << endl;
    hashTable2 = hashTable;
    hashTable2.print();
    // return 0;
}
