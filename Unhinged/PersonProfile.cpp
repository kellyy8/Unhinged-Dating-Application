//
//  main.cpp
//
//  Created by Kelly Yu on 3/6/22.
//

#include "PersonProfile.h"
#include <iostream>
#include <string>
#include <vector>
#include "utility.h"
#include "RadixTree.h"
#include <algorithm>

PersonProfile::PersonProfile(std::string name, std::string email): m_name(name), m_email(email){}
std::string PersonProfile::GetName() const{return m_name;}
std::string PersonProfile::GetEmail() const{return m_email;}

void PersonProfile::AddAttValPair(const AttValPair &attval){
    std::vector<std::string>* vectorValPtr = aToV.search(attval.attribute);
    if(vectorValPtr != nullptr){
        //binary search to see if attval.value is found in vector of values for that attribute
        bool foundDup = std::binary_search(vectorValPtr->begin(), vectorValPtr->end(), attval.value);
        if(!foundDup){
            vectorValPtr->push_back(attval.value);            //store existing a with unique v
            m_AttValPairs.push_back(attval);
        }
    }
    //key not found; make new attribute key and map it to new vector, add the value
    else if(vectorValPtr == nullptr){
        std::vector<std::string> values;
        values.push_back(attval.value);
        aToV.insert(attval.attribute, values);            //store unique a with unique v
        m_AttValPairs.push_back(attval);
    }
}

int PersonProfile::GetNumAttValPairs() const{
    return static_cast<int>(m_AttValPairs.size());
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair &attval) const{
    if(attribute_num < 0 || attribute_num >= m_AttValPairs.size())
        return false;

    attval = m_AttValPairs[attribute_num];
    return true;
}
