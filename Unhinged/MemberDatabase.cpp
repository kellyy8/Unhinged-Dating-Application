//
//  MemberDatabase.cpp
//
//  Created by Kelly Yu on 3/6/22.
//

#include "MemberDatabase.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "PersonProfile.h"
#include "provided.h"
#include "RadixTree.h"

MemberDatabase::MemberDatabase(){}

bool MemberDatabase::LoadDatabase(std::string filename){
    std::ifstream infile(filename);
    if(!infile){
        return false;
    }
    
    while(!infile.eof()){
        //create PersonProfile objects by reading in each chunk of information
        //read in and store strings from infile to string var
        std::string name;           //get name
        getline(infile, name);
        
        if(name == "")
            continue;
        
        std::string email;          //get email
        getline(infile, email);
        PersonProfile* newPP = new PersonProfile(name, email);              //directly allocated, so MD directly deallocates it

        std::string numAV;
        getline(infile, numAV);
        int n = std::stoi(numAV);

        for(int i=0; i<n; i++){
            std::string att;
            getline(infile, att, ',');
            std::string val;
            getline(infile, val);
            newPP->AddAttValPair(AttValPair(att, val));
        }

        //add PersonProfile object to PP container
        allPPs.push_back(newPP);
        //reads in the newline in between each person data
        std::string newline;
        getline(infile, newline);
    }
        
    for(int j=0; j < allPPs.size(); j++){
        PersonProfile* person = allPPs[j];       //retrieve person profile
        
        //check if email already mapped to a person; return false if it is
        PersonProfile* ptr = emailsToPP.search(person->GetEmail());
        if(ptr != nullptr){
            return false;
        }
        //else, map the email to local person profile with empty radix tree
        emailsToPP.insert(person->GetEmail(), PersonProfile(person->GetName(), person->GetEmail()));           //radix insert
                                         //dynam allo bc creating regular local objects will cause the destructor to destroy PersonProfile
                                        //and the still necessary radix tree that PersonProfile object holds
        
        //for every attribute value pair of the person, map it to Person's email
        for(int k=0; k < person->GetNumAttValPairs(); k++){
            //convert av to one string
            AttValPair av;
            person->GetAttVal(k, av);
            std::string avString = av.attribute + ',' + av.value;

            //check if av pair already maps to set of emails; update emails list if so
            std::vector<std::string>* setOfEmails = avToEmails.search(avString);
            if(setOfEmails == nullptr){
                std::vector<std::string> newSetOfEmails;
                newSetOfEmails.push_back(person->GetEmail());
                avToEmails.insert(avString, newSetOfEmails);        //radix insert
            }
            //since all persons hav distinct attval pairs, then their emails will only be added for distinct av pairs once; no check for dups needed
            else{
                setOfEmails->push_back(person->GetEmail());          //edit the mapped to vector
            }
        }
    }
    
    //fill each person's radix trees
    std::vector<PersonProfile*>::iterator it = allPPs.begin();
    for(; it != allPPs.end(); it++){
        PersonProfile* temp = emailsToPP.search((*it)->GetEmail());
        for(int i=0; temp!=nullptr && i<(*it)->GetNumAttValPairs(); i++){
            AttValPair addPair;
            (*it)->GetAttVal(i, addPair);
            temp->AddAttValPair(addPair);
        }
    }
    return true;
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const{
    //make the input a strictly lowercase string with one comma in between; case sensitivity
    std::string inputString = input.attribute + ',' + input.value;
    std::vector<std::string>* emailsOfMatchesPtr = avToEmails.search(inputString);

    if(emailsOfMatchesPtr == nullptr){
        std::vector<std::string> emptyVector;
        return emptyVector;
    }

    return *emailsOfMatchesPtr;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const{
    return emailsToPP.search(email);
}

MemberDatabase::~MemberDatabase(){
    //radix tree DOES NOT delete the dynamically PersonProfiles, but it DOES delete PersonProfiles' radix trees
    //when the destructor is called on MemberDatabase's radix tree that maps strings to PersonProfiles
    //when MemberDatabase is destructed
    std::vector<PersonProfile*>::iterator it = allPPs.begin();
    while(!allPPs.empty()){
        delete *it;
        it = allPPs.erase(it);
    }
}
