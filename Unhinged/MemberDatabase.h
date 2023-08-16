//
//  MemberDatabase.h
//
//  Created by Kelly Yu on 3/6/22.
//

#ifndef MemberDatabase_h
#define MemberDatabase_h

#include <string>
#include <vector>
#include "RadixTree.h"
#include "PersonProfile.h"
#include "provided.h"

class PersonProfile;

class MemberDatabase{
public:
    MemberDatabase();
    ~MemberDatabase();
    bool LoadDatabase(std::string filename);
    std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
    const PersonProfile* GetMemberByEmail(std::string email) const;
    //does that mean i cannot change what this pointer points to ? or the contents the pointer points to
    
private:
    std::vector<PersonProfile*> allPPs;                     //store all PP info to create radix trees
    RadixTree< std::vector<std::string> > avToEmails;
    RadixTree< PersonProfile > emailsToPP;
    
};


#endif /* MemberDatabase_h */
