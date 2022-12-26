//
// Created by ohadr on 12/23/2022.
//

#ifndef TEAM_CPP_HASH_TABLE_H
#define TEAM_CPP_HASH_TABLE_H

#include <iostream>
#include <cmath>
#include "Pair.h"

const int DEFAULT_SIZE = 11;
const float GET_BIGGER = 0.75;
const float GET_SMALLER = 0.25;

template<class T, class K>
class HashTable{
private:
    class Node;
    Node** m_tableOfNodes;
    int m_sizeOfTable;
    int m_numOfMembers;

    Node* findByNode(const K& key){
        int position = hashingFunc(key);
        if(m_tableOfNodes[position] == nullptr){
            return nullptr;
        }
        else{
            Node* positionToSearch = m_tableOfNodes[position];
            while(positionToSearch != nullptr){
                if(positionToSearch->key() == key){
                    return positionToSearch;
                }
                else{
                    positionToSearch = positionToSearch->m_next;
                }
            }
            return nullptr;
        }
    }

    void efficientSize(){
        float ratio = (float)m_numOfMembers / (float)m_sizeOfTable;
        if(ratio > GET_SMALLER && ratio < GET_BIGGER){
            return;
        }
        else{
            Node** newSizeTable;
            int newSize;
            if(ratio >= 0.75){
                newSize = m_sizeOfTable * 2;
                newSizeTable = new Node*[newSize];
            }
            else{
                newSize = round(m_sizeOfTable / 2);
                if (newSize < DEFAULT_SIZE){
                    return;
                }
                newSizeTable = new Node*[newSize];
            }
            for(int i = 0; i < newSize; i++){
                newSizeTable[i] = nullptr;
            }
            for(int i = 0; i < m_sizeOfTable; i++){
                Node* ptr = m_tableOfNodes[i];
                while (ptr != nullptr){
                    int hash = ptr->key() % newSize;
                    Node* tempNode = new Node(ptr->data(), ptr->key());
                    if(newSizeTable[hash] != nullptr){
                        Node* ptrOfNew = newSizeTable[hash];
                        while(ptrOfNew->m_next != nullptr){
                            ptrOfNew = ptrOfNew->m_next;
                        }
                        ptrOfNew->m_next = tempNode;
                        tempNode->m_prev = ptrOfNew;
                    }
                    else{
                        newSizeTable[hash] = tempNode;
                    }
                    ptr = ptr->m_next;
                }
            }
            m_tableOfNodes = newSizeTable;
            m_sizeOfTable = newSize;
        }
    }

public:
    explicit HashTable(int sizeOfTable = DEFAULT_SIZE) : m_sizeOfTable(sizeOfTable), m_numOfMembers(0){
        m_tableOfNodes = new Node*[sizeOfTable];
        for(int i = 0; i < m_sizeOfTable; i++){
            m_tableOfNodes[i] = nullptr;
        }
    }

    //help function for destroyer
    void deleter(Node** arr, int size){
        for(int i = 0; i < size; i++){
            if(arr[i] != nullptr && arr[i]->m_next != nullptr){
                Node* toDelete = arr[i];
                arr[i] = arr[i]->m_next;
                delete toDelete;
            }
        }
    }

    ~HashTable(){
        deleter(m_tableOfNodes, m_sizeOfTable);
    }

    HashTable(const HashTable& ht){
        m_sizeOfTable = ht.m_sizeOfTable;
        m_numOfMembers = ht.m_numOfMembers;
        m_tableOfNodes = new Node*[m_sizeOfTable];
        for(int i = 0; i < m_sizeOfTable; i++){
            m_tableOfNodes[i] = nullptr;
        }
        for(int i = 0; i < m_sizeOfTable; i++){
            Node* ptr = ht.m_tableOfNodes[i];
            while (ptr != nullptr){
                int hash = hashingFunc(ptr->key());
                Node* tempNode = new Node(ptr->data(), ptr->key());
                if(m_tableOfNodes[hash] != nullptr){
                    Node* ptrOfNew = m_tableOfNodes[hash];
                    while(ptrOfNew->m_next != nullptr){
                        ptrOfNew = ptrOfNew->m_next;
                    }
                    ptrOfNew->m_next = tempNode;
                    tempNode->m_prev = ptrOfNew;
                }
                else{
                    m_tableOfNodes[hash] = tempNode;
                }
                ptr = ptr->m_next;
            }
        }

    }
    HashTable& operator=(const HashTable& ht){
        if(ht == *this){
            return *this;
        }
        Node** temp = m_tableOfNodes;
        m_tableOfNodes = ht.m_tableOfNodes;
        m_sizeOfTable = ht.m_sizeOfTable;
        m_numOfMembers = ht.m_numOfMembers;
        deleter(temp);
        return *this;
    }

    int getSize(){
        return m_numOfMembers;
    }

    bool isEmpty(){
        return (m_numOfMembers == 0);
    }
    int hashingFunc(const K keyToHash){
        return keyToHash % m_sizeOfTable;
    }

    Pair<T,K>* find(const K& key){
        if(this->isEmpty()){
            return nullptr;
        }
        Node* result = this->findByNode(key);
        if(result != nullptr){
            return result->m_nodeData;
        }
        return nullptr;
    }

    void insert(const T& data, const K& key){
        if(find(key) != nullptr){
            return;
        }
        Node* newItem = new Node(data, key);
        int position = hashingFunc(key);
        Node* ptr = m_tableOfNodes[position];
        if(m_tableOfNodes[position] != nullptr){
            while(ptr->m_next != nullptr){
                ptr = ptr->m_next;
            }
            ptr->m_next = newItem;
            newItem->m_prev = ptr;
        }
        else{
            m_tableOfNodes[position] = newItem;
        }
        m_numOfMembers++;
        m_tableOfNodes[position]->m_sizeOfChain++;
        this->efficientSize();
    }

    void remove(const K& key){
        Pair<T, K>* checkerIfInTable = find(key);
        if(checkerIfInTable == nullptr){
            return;
        }
        int position = hashingFunc(key);
        Node* ptr  = m_tableOfNodes[position];
        if(m_tableOfNodes[position]->m_next == nullptr){ //check if is the only one in chain
            m_tableOfNodes[position] = ptr->m_next;
            delete ptr;
        } else{
            while(ptr->key() != key){
                ptr = ptr->m_next;
            }
            ptr->m_prev->m_next = ptr->m_next;
            if(ptr->m_next != nullptr){ //check if last
                ptr->m_next->m_prev = ptr->m_prev;
            }
            delete ptr;
            m_tableOfNodes[position]->m_sizeOfChain--;
        }
        m_numOfMembers--;
        this->efficientSize();
    }


///-----------------------------------implementation of class Node-----------------------------------
private:
    class Node{
            friend HashTable;
            Pair<T,K>* m_nodeData;
            Node* m_prev;
            Node* m_next;
            int m_sizeOfChain;

            Node(const T& dataPtr, const K& key) : m_nodeData(nullptr), m_prev(nullptr), m_next(nullptr), m_sizeOfChain(0){
                m_nodeData = new Pair<T,K>(dataPtr,key);
            }
            Node(const Pair<T,K>& pairData) : m_nodeData(nullptr), m_prev(nullptr), m_next(nullptr), m_sizeOfChain(0){
                m_nodeData = new Pair<T,K>(pairData);
            }
            explicit Node() : m_nodeData(nullptr), m_prev(nullptr), m_next(nullptr), m_sizeOfChain(0){}
        public:
            Node(const Node& n) = delete;
            Node& operator= (const Node& n){ //only copy the data NOT change the pointer to the close ones
                if(this == &n){
                    return *this;
                }
                m_nodeData = n.m_nodeData;
                return *this;
            }

            bool empty(){
                return (!m_nodeData);
            }
            T& data(){
                return m_nodeData->data();
            }

            const K& key(){
                return m_nodeData->key();
            }
            ~Node(){
                delete m_nodeData;
            }
        };

    ///------------------------------------help & test functions-----------------------------
public:
    void printChain(int numNode){
        if(numNode >= m_sizeOfTable){
            std::cout << false << std::endl;
        }
        if(m_tableOfNodes[numNode] == nullptr){
            std::cout << "EMPTY" << std::endl;
        }
        else {
            Node* ptr = m_tableOfNodes[numNode];
            while (ptr != nullptr){
                std::cout << ptr->key() << ", ";
                ptr = ptr->m_next;
            }
            std::cout << std::endl;
        }
    }

    void printTable(){
        std::cout << "-------PRINT TABLE-------" << std::endl;
        std::cout << "Size: " << m_sizeOfTable << ", Num members: " << m_numOfMembers << std::endl;
        for(int i = 0; i < m_sizeOfTable; i++){
            std::cout << "spot " << i << ": " ;
            this->printChain(i);
        }
        std::cout << "-------FINISH-------" << std::endl;
    }


    };

#endif //TEAM_CPP_HASH_TABLE_H
