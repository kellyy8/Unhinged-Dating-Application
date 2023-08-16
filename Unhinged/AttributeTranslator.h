//
//  AttributeTranslator.h
//
//  Created by Kelly Yu on 3/6/22.
//

#ifndef AttributeTranslator_h
#define AttributeTranslator_h

#include <string>
#include <vector>
#include "provided.h"
#include "RadixTree.h"

class AttributeTranslator{
public:
    AttributeTranslator();
//    ~AttributeTranslator();
    bool Load(std::string filename);
    std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
     RadixTree< std::vector<AttValPair> > srcToComp;   //map src AV strings to comp AV structs
};

#endif /* AttributeTranslator_h */
