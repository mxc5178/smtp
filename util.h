//
// util.h
//
// implementation of 
//      split(v, s, d)
//
//  defined in util.cpp as per best practice
//

#include <string>
#include <vector>

#ifndef UTIL_H
#define UTIL_H

class util{

    public:
        //
        // split string on delimiter
        //
        static int split(std::vector<std::string> & v, std::string &s, std::string &d);

        //
        // constructor ... not necessary because methods are all static and
        // do not require an instance of the class
        //
        util();
};

#endif

