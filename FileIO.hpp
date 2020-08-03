#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <fstream>
#include <sstream>

#include "driver.hpp"

class FileIO {
public:
    FileIO() {}

    //reads info from input file
    void readFile(vector<TypoCost> &stringData);
    void writeFile(vector<TypoCost> &stringData);
};

#endif
