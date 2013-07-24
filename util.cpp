//
// util.cpp
//

#include <string>
#include <vector>
#include "util.h"

using namespace std;

int util::split(vector<string> & v, string &s, string &delim) {

    int start = 0, end = 0;

    while ((end = s.find(delim, start)) != string::npos) {

        v.push_back(s.substr(start, end - start));

        //
        // start position modified based on delimiter length
        // to handle multi character delimiter
        //
        start = end + delim.length();

    }

    v.push_back(s.substr(start));

    return v.size();

}
