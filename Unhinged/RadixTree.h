//
//  RadixTree.h
//
//  Created by Kelly Yu on 3/12/22.
//
//
#ifndef RadixTree_h
#define RadixTree_h

#include <string>
#include <iostream>

template <typename ValueType>
class RadixTree {
public:
    RadixTree();
    ~RadixTree();
    void insert(std::string key, const ValueType& value);
    ValueType* search(std::string key) const;

private:
    //key == key/prefix/key minus prefix
    //hasValue == indicator for end of key
    //value == ValueType that the key maps to
    //edges == pointers to other RadixTreeNodes
    struct RadixNode{
        RadixNode(std::string k = "") : edges(){
            m_key = k;
            m_value = nullptr;
        }

        ~RadixNode(){
            if(m_value != nullptr){
                delete m_value;
                m_value = nullptr;
            }
        }
        std::string m_key;
        ValueType* m_value;             //dynam allocation
        RadixNode* edges[128];          //ascii 0-127
    };

    void deleteAllNodes(RadixNode*& deleteNode);
    RadixNode root;
};

template <typename ValueType>
inline
RadixTree<ValueType>::RadixTree(){}

template <typename ValueType>
inline
RadixTree<ValueType>::~RadixTree(){
    for(int i=0; i<128; i++)
        deleteAllNodes(root.edges[i]);
}

template <typename ValueType>
inline
void RadixTree<ValueType>::deleteAllNodes(RadixNode*& deleteNode){
    if(deleteNode == nullptr)
        return;

    //delete all edges stemming from deleteNode
    for(int i=0; i<128; i++){
        if(deleteNode->edges[i] != nullptr){
            deleteAllNodes(deleteNode->edges[i]);
        }
    }

    delete deleteNode;
    deleteNode = nullptr;
}

template <typename ValueType>
inline
void RadixTree<ValueType>::insert(std::string key, const ValueType& value){         //NEWKEY, NEWVALUE
    RadixNode* curr = &root;
    RadixNode* parent = curr;           //points to one node above curr later

    int first = key[0];         //first = ascii value of the character

    //add brand new key
    if(curr->edges[first] == nullptr){
        RadixNode* rnode = new RadixNode(key);
        rnode->m_value = new ValueType(value);
        curr->edges[first] = rnode;
//        std::cerr << "Added new key!" << std::endl;
        return;
    }

    //go down the radix tree; curr == pointer to the curr node I am analyzing NEWKEY with
    //parent holds onto current before current moves on; one above curr node
    parent = curr;
    curr = root.edges[first];

    //analysis with curr node
    while(1==1){
        //NEWKEY is an existing key, update the value ; toast == toast, update
        if(curr->m_key == key){
            delete curr->m_value;
            curr->m_value = new ValueType(value);
            return;
        }

        //not ==, or else would have passed the full equality check
        //move to following node or add SUFFIX
        if(curr->m_key.size() < key.size()){
            int sizeOfCurrKey = static_cast<int>(curr->m_key.size());
            std::string partOfNewKey = key.substr(0, sizeOfCurrKey);
            if(curr->m_key == partOfNewKey){
                //retrieve the ascii code for the first letter of the new suffix
                int startOfSuffix = key[sizeOfCurrKey];

                //add new suffix, because the remainder of NEWKEY is not found anywhere
                if(curr->edges[startOfSuffix] == nullptr){
                    std::string remainderOfNewKey = key.substr(sizeOfCurrKey, key.size());
                    RadixNode* rnode = new RadixNode(remainderOfNewKey);
                    rnode->m_value = new ValueType(value);
                    curr->edges[startOfSuffix] = rnode;
//                    std::cerr << "Added new suffix!" << remainderOfNewKey << std::endl;
                    return;
                }
                //move current pointer to point to the next node we do analysis with
                //restart analysis with the remainder of the key
                else{
                    key = key.substr(sizeOfCurrKey, key.size());
                    parent = curr;
                    curr = curr->edges[startOfSuffix];                          //ONE MOVE DOWN ONE LEVEL
//                    std::cerr << "move down one level" << std::endl;
                    continue;
                }
            }
        }

        if(key.size() < curr->m_key.size()){
            int sizeOfNewKey = static_cast<int>(key.size());
            std::string partOfCurrKey = curr->m_key.substr(0, sizeOfNewKey);

            //all of NEWKEY is found in curr key; NEWKEY is a prefix to curr key; break the prefix
            if(key == partOfCurrKey){
                //create new prefix
                RadixNode* rnode = new RadixNode(key);
                rnode->m_value = new ValueType(value);

                int edgeSlot = key[0];
                //link parent to new prefix
                parent->edges[edgeSlot] = rnode;
                //link new prefix to current prefix
                edgeSlot = curr->m_key[sizeOfNewKey];
                rnode->edges[edgeSlot] = curr;
                //update current prefix
                curr->m_key = curr->m_key.substr(sizeOfNewKey, curr->m_key.size());

//                std::cerr << "Added new prefix!" << std::endl;
//                std::cerr << "New Prefix: " << key << " --- Edited prefix: " << curr->m_key << std::endl;
                return;
            }
        }

        //otherwise, the current node's key and NEWKEY match partially; share part of an existing prefix
        //break the prefix and add a new suffix
        //key[i] after the loop == first character that did not match in curr node's key
//        std::cerr << "Partial prefix found!" << std::endl;
        int i;
        for(i=1; i < curr->m_key.size() ; i++){
            if(curr->m_key[i] != key[i]){
                break;
            }
        }

        key = key.substr(i, key.size());            //shorten key; first char up to last char

        //break the prefix
        std::string firstPartP = curr->m_key.substr(0, i);
        std::string secondPartP = curr->m_key.substr(i, curr->m_key.size());

        //add a new node for the first part of the prefix
        //link parent's edge to that new node
        //link this new node's edge to the current node
        RadixNode* firstPart = new RadixNode(firstPartP);
        int edgeSlot = firstPartP[0];
        parent->edges[edgeSlot] = firstPart;
        edgeSlot = secondPartP[0];
        firstPart->edges[edgeSlot] = curr;

        //update the current node with the last part of the prefix;
        curr->m_key = secondPartP;

        //add new node for the suffix
        RadixNode* newSuffix = new RadixNode(key);
        newSuffix->m_value = new ValueType(value);

        //link the new node's edge (prefix) to the new node (suffix)
        edgeSlot = key[0];
        firstPart->edges[edgeSlot] = newSuffix;

//        std::cerr << "Broke prefix and added suffix!" << std::endl;
//        std::cerr << "First part of prefix: " << firstPartP << " Second part of prefix: " << secondPartP << std::endl;
//        std::cerr << "Suffix: " << key << std::endl;
        return;
    }
}

template <typename ValueType>
inline
ValueType* RadixTree<ValueType>::search(std::string key) const{
    if(key == ""){
//        std::cerr << "No keys are empty." << std::endl;
        return nullptr;
    }

    int edgeSlot = key[0];
    RadixNode* currNode = root.edges[edgeSlot];

    while(1==1){
        if(currNode == nullptr)
            return nullptr;

        int i;

        //for the remaining key, compare its characters with curr node's key's characters 1:1
        for(i=0; i<key.size() && i<currNode->m_key.size() ; i++){
            //at least 1 mismatch means key is not found
            if(key[i] != currNode->m_key[i]){
//                std::cerr << "Mismatch found." << std::endl;
                return nullptr;
            }
        }
        
        //i must now be == to either key.size() or m_key.size() to continue the search; passed for loop

        //looked through all of current node's key's chars, follow an edge to check for remaining of key's chars
        if(i == currNode->m_key.size() && i < key.size()){
            key = key.substr(i, key.size());
            edgeSlot = key[0];
            if(currNode->edges[edgeSlot] == nullptr){                //no further nodes contain remainder of key
//                std::cerr << "Only part of word found." << std::endl;
                return nullptr;
            }
            else{
                currNode = currNode->edges[edgeSlot];
                continue;
            }
        }

        if(i == key.size()){
            //currNode key has characters left; key is only a part of an existing word
            if(i < currNode->m_key.size()){
//                std::cerr << "Key is part of an existing word." << std::endl;
                return nullptr;
            }

            //currNode key has no characters left (i==size), but contains no valueType; key only matches a prefix
            if(currNode->m_value == nullptr){
//                std::cerr << "Key is a prefix to other words." << std::endl;
                return nullptr;
            }
        }

//        std::cerr << "Word found!" << std::endl;
        return currNode->m_value;
    }
}

#endif /* RadixTree_h */
