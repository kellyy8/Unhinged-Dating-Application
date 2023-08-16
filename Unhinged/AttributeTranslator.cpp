//
//  AttributeTranslator.cpp
//
//  Created by Kelly Yu on 3/6/22.
//

#include "AttributeTranslator.h"
#include <iostream>
#include <fstream>
#include <string>

AttributeTranslator::AttributeTranslator(){}

bool AttributeTranslator::Load(std::string filename){
    std::ifstream infile(filename);
    if(!infile){
        return false;
    }
    
    //process file while mapping all src av pairs to a vector of comp av pairs
    while(!infile.eof()){
        //retrieve source av pair
        std::string srcAtt;
        getline(infile, srcAtt, ',');
        
        //in case there are leftover empty lines after the last translation
        if(srcAtt == "")
            continue;
        
        std::string srcVal;
        getline(infile, srcVal, ',');
        
        //make source av string
        std::string srcAV = srcAtt + ',' + srcVal;
        
        //retrieve info to create compatible av pair
        std::string compAtt;
        getline(infile, compAtt, ',');
        std::string compVal;
        getline(infile, compVal);
        AttValPair newCompAV(compAtt, compVal);
        
        //check if source av pair alreay has vector of compatible av pairs
        std::vector<AttValPair>* setOfCompAVPtr = srcToComp.search(srcAV);
        
        //if no vector of comp av, create new vector with the comp av
        if(setOfCompAVPtr == nullptr){
            std::vector<AttValPair> newSetOfComp;
            newSetOfComp.push_back(newCompAV);
            srcToComp.insert(srcAV, newSetOfComp);
        }
        //check if comp av is alrdy in vector
        else{
            //avoid duplication
            std::vector<AttValPair>::iterator it = setOfCompAVPtr->begin();
            for(; it!=setOfCompAVPtr->end(); it++){
                if(*it == newCompAV)
                    break;
            }
            //if not alrdy in vector, add comp av to vector
            if(it == setOfCompAVPtr->end())
                setOfCompAVPtr->push_back(AttValPair(newCompAV));
        }
    }
    
    return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const{
    std::string avString = source.attribute + ',' + source.value;
    std::vector<AttValPair>* setOfCompAVPtr = srcToComp.search(avString);
    
    if(setOfCompAVPtr == nullptr){
        std::vector<AttValPair> emptyVector;
        return emptyVector;
    }
    
    return *setOfCompAVPtr;
}
