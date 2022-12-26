#include "Hash_Table.h"
#include "Team.h"
using std::string;

int main() {
    std::cout << "START!" << std::endl;
    ///------INIT TABLE-------//
    HashTable<Player, int> testTable = HashTable<Player, int>();
    for(int i = 0; i < 10; i++){ ///should be here maximizing size
        Player toInsert = Player(i);
        testTable.insert(toInsert, toInsert.getPlayerId());
    }

    ///------test removes------
    for(int i = 1; i < 10; i+=2){
        testTable.remove(i);
        testTable.printTable();
    }

    ///------test fail remove-------
    testTable.remove(1);

    ///------test insert random key spot------
    Player* toInsert = new Player(1239);
    testTable.insert(*toInsert, toInsert->getPlayerId());
    delete toInsert;

    ///------check chaining-----
    for(int i = 0; i < 3; i++){
        Player toInsert = Player(i+100);
        testTable.insert(toInsert, toInsert.getPlayerId());
        testTable.printTable();
    }
    ///-----------check encounters----------//
    std::cout << "IS FOUND 11 ?" << testTable.find(11) << std::endl;
    std::cout << "IS FOUND 1 ?" << testTable.find(4) << std::endl;
    testTable.printTable();

    ///------check remove first item in chain (chain > 1)------//
    testTable.remove(0);
    testTable.printTable();

    ///------check minimizing size------//
    //begin with 20 spots with 8 members
    testTable.remove(101);
    testTable.remove(6);
    testTable.remove(8);
    testTable.remove(2);
    //should be 10 spots with 4 members
    testTable.printTable();

    ///------check big chaining------//
    for(int i = 0; i < 30; i++){
        Player* toInsert = new Player(i*88);
        testTable.insert(*toInsert, toInsert->getPlayerId());
    }
    testTable.printTable();
    std::cout << "IS FOUND 2900 ?" << testTable.find(2900) << std::endl; //last in chain
    std::cout << "IS FOUND 2000 ?" << testTable.find(2000) << std::endl; //mid chain
    for(int i = 10; i < 20; i++){
        testTable.remove(i*88);
    }
    testTable.printTable();

    return 0;
}
