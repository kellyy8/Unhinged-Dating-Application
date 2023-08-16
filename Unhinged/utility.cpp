//
//  utility.cpp
//
//  Created by Kelly Yu on 3/6/22.
//

#include <iostream>
#include "utility.h"
#include "provided.h"

//used for the set
bool operator<(const AttValPair& a, const AttValPair& b){
    if(a.attribute < b.attribute)
        return true;
    
    else if(a.attribute == b.attribute)
        return a.value < b.value;
    
    return false;
}

bool operator<(const EmailCount& a, const EmailCount& b){
    if(a.count > b.count)
        return true;
    
    if(a.count == b.count)
        return a.email < b.email;           //emails are never equal; either true or false on this one
    
    return false;
}
