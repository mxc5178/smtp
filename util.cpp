#include <string>
#include <iostream>
#include <fstream>
#include "util.h"

using namespace std;

void util::writeData(string f, string data){

    //
    // Open the file 
    // Write the data to the file 
    // Close the file
    //

    ofstream file;
    file.open (f.c_str(), ios::out | ios::app);
    file << data << "\n";
    file.close();
}
