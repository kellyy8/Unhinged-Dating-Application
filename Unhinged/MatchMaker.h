//
//  MatchMaker.h
//
//  Created by Kelly Yu on 3/7/22.
//

#ifndef MatchMaker_h
#define MatchMaker_h

#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "provided.h"

class MatchMaker{
public:
    MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
//    ~MatchMaker();
    std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
    
private:
    const MemberDatabase* m_mdb;
    const AttributeTranslator* m_at;
};

#endif /* MatchMaker_h */
