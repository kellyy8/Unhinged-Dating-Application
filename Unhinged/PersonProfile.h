//
//  PersonProfile.h
//
//  Created by Kelly Yu on 3/6/22.
//

#ifndef PersonProfile_h
#define PersonProfile_h

#include <string>
#include <vector>
#include "provided.h"
#include "RadixTree.h"

class PersonProfile{
public:
    PersonProfile(std::string name, std::string email);
//    ~PersonProfile();
    std::string GetName() const;
    std::string GetEmail() const;
    void AddAttValPair(const AttValPair& attval);
    int GetNumAttValPairs() const;
    bool GetAttVal(int attribute_num, AttValPair& attval) const;
    
private:
    std::string m_name;
    std::string m_email;

    std::vector<AttValPair> m_AttValPairs;
    RadixTree<std::vector<std::string> > aToV;               //map att strings to vector of value strings
};

#endif /* PersonProfile_h */
