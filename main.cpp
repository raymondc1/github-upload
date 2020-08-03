//g++ -o typo *.cpp

#include "driver.hpp"
#include "FileIO.hpp"

int main()
{
    vector<TypoCost> stringData;
    FileIO _file;

    _file.readFile(stringData);

    for (int i = 0; i < stringData.size(); i++)
    {
        int min = stringData[i].recursion(stringData[i].get_target_index(), stringData[i].get_typo_index());
        stringData[i].reconstruct_path(stringData[i].get_target_index(), stringData[i].get_typo_index());
    }
    _file.writeFile(stringData);
    
    return 0;
}
