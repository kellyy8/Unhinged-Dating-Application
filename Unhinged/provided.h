#ifndef _PROVIDED_H_
#define _PROVIDED_H_

#include <string>

struct AttValPair {
    AttValPair(std::string att, std::string val)
     : attribute(att), value(val)
    {}

    AttValPair()
    {}

    std::string attribute;
    std::string value;

    bool operator==(const AttValPair& rhs) const {
        return (attribute == rhs.attribute  &&  value == rhs.value);
    }

    bool operator!=(const AttValPair& rhs) const {
        return !(*this == rhs);
    }
};

struct EmailCount {
    EmailCount(std::string em, int ct)
     : email(em), count(ct)
    {}

    std::string email;
    int count;
};

#endif //_PROVIDED_H_
