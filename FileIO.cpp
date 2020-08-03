#include "FileIO.hpp"

const std::string FILENAME_IN = "input.txt";
const std::string FILENAME_OUT = "output.txt";

//reads in each pair of target and typo strings from input file
//saves these pairs of strings in a vector
void FileIO::readFile(vector<TypoCost> &stringData)
{
    ifstream _file(FILENAME_IN);
    //if file successfully opens
    if (_file)
    {
        //variables
        int i = 0;
        string instances;
        string string_1, string_2, blank;

        //get number of problem instances from first line of input file
        getline(_file, instances);

        //get each pair of target and typo strings and save in vector
        while (i < stoi(instances))
        {
            //target string
            getline(_file, string_1);
            //typo string
            getline(_file, string_2);
            TypoCost temp(string_1, string_2, string_1.length(), string_2.length());
            stringData.push_back(temp);
            //blank line in between each set of strings
            getline(_file, blank);
            i++;
        }
    }

    //if input file does not open print error message
    else
    {
        cout << "\nFile could not be opened.\n";
    }

    //close input file
    _file.close();
}

void FileIO::writeFile(vector<TypoCost> &stringData)
{
    //open output file
    ofstream output;
    output.open(FILENAME_OUT);

    for (auto i : stringData)
    {
        output << i.get_min_cost() << endl;
        for (auto t : i.get_vector_operations())
        {
            output << t << endl;
        }
        output << endl;
    }
    output.close();
}
