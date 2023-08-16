//
//  MatchMaker.cpp
//
//  Created by Kelly Yu on 3/7/22.
//

#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "provided.h"
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "utility.h"
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at){
    m_mdb = &mdb;        //make a const reference variable copy
    m_at = &at;
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const{
    std::vector<EmailCount> empty;
    if(threshold <= 0)
        threshold = 1;
    
    if((m_mdb->GetMemberByEmail(email) == nullptr))
       return empty;
    
    //retrieve matchee by their email
    const PersonProfile* matchee = m_mdb->GetMemberByEmail(email);
       
    //collect all unique compatible av pairs
    std::set<AttValPair> setOfCompAV;
    for(int i=0; i<matchee->GetNumAttValPairs(); i++){
        //retrieve each of matchee's av pair
        AttValPair matcheeAV;
        matchee->GetAttVal(i, matcheeAV);
        
        //retrieve the compatible pairs for matchee's av
        std::vector<AttValPair> vOfCompVals = m_at->FindCompatibleAttValPairs(matcheeAV);
        
        //add new and unique compatible pairs to a set
        for(int j=0; j<vOfCompVals.size(); j++){
           setOfCompAV.insert(vOfCompVals[j]);
        }
    }
    
    //create iterators
    std::set<AttValPair>::iterator set_it;
    std::map<std::string, int>::iterator map_it;
    
    //create map container to hold all matches' emails and number of items they match with matchee
    std::map<std::string, int> emailsAndCounts;

    //for each compatible AV, retrieve the emails of matches
    for(set_it = setOfCompAV.begin(); set_it!=setOfCompAV.end(); set_it++){
        std::vector<std::string> matchEmails = m_mdb->FindMatchingMembers(*set_it);
        
        //for each match's email, update the count or add the new match's email to map
        for(int i=0; i<matchEmails.size(); i++){
            if(matchEmails[i] == matchee->GetEmail())
                continue;
            
            map_it = emailsAndCounts.find(matchEmails[i]);
            if(map_it == emailsAndCounts.end()){
                emailsAndCounts[matchEmails[i]] = 1;
            }
            else{
                (map_it->second)++;
            }
        }
    }

//    std::set<EmailCount> setOfEC;
//    //for every match's email & count, make & add emailCount object to set if count passes threshold
//    //set orders the emailCount objects by count first, then emails to break ties
//    for(map_it = emailsAndCounts.begin(); map_it != emailsAndCounts.end(); map_it++){
//        if(map_it->second >= threshold){
//            setOfEC.insert(EmailCount(map_it->first, map_it->second));
//        }
//    }
//
//    //add all the ordered and valid matches to a vector from the set
//    std::set<EmailCount>::iterator set_it2;
//    std::vector<EmailCount> vOfEC;
//    for(set_it2 = setOfEC.begin(); set_it2 != setOfEC.end(); set_it2++){
//        vOfEC.push_back(*set_it2);
//    }
    
    //for every match's email & count, make & add emailCount object to set if count passes threshold
    std::vector<EmailCount> vOfEC;
    for(map_it = emailsAndCounts.begin(); map_it != emailsAndCounts.end(); map_it++){
        if(map_it->second >= threshold){
            vOfEC.push_back(EmailCount(map_it->first, map_it->second));
        }
    }
    
    //then, sort the emailCount objects by count first, then use emails to break ties
    std::sort(vOfEC.begin(), vOfEC.end());
    
    return vOfEC;
}

