#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <iostream>
#include <cmath>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

enum class Finger{pinky, ring, middle, index};
enum class Hand: bool{left = 0, right = 1};

using keyPosition = std::pair<int,int>;
const std::map<char, std::tuple<keyPosition, Finger, Hand>> KEYBOARD = {
    {'1', std::make_tuple(std::make_pair(0,0), Finger::pinky,  Hand::left)},  {'2', std::make_tuple(std::make_pair(1,0), Finger::ring,   Hand::left)},  {'3', std::make_tuple(std::make_pair(2,0), Finger::middle, Hand::left)},
    {'4', std::make_tuple(std::make_pair(3,0), Finger::index,  Hand::left)},  {'5', std::make_tuple(std::make_pair(4,0), Finger::index,  Hand::left)},  {'6', std::make_tuple(std::make_pair(5,0), Finger::index,  Hand::right)},
    {'7', std::make_tuple(std::make_pair(6,0), Finger::index,  Hand::right)}, {'8', std::make_tuple(std::make_pair(7,0), Finger::middle, Hand::right)}, {'9', std::make_tuple(std::make_pair(8,0), Finger::ring,   Hand::right)},
    {'0', std::make_tuple(std::make_pair(9,0), Finger::pinky,  Hand::right)}, {'q', std::make_tuple(std::make_pair(0,1), Finger::pinky,  Hand::left)},  {'w', std::make_tuple(std::make_pair(1,1), Finger::ring,   Hand::left)},
    {'e', std::make_tuple(std::make_pair(2,1), Finger::middle, Hand::left)},  {'r', std::make_tuple(std::make_pair(3,1), Finger::index,  Hand::left)},  {'t', std::make_tuple(std::make_pair(4,1), Finger::index,  Hand::left)},
    {'y', std::make_tuple(std::make_pair(5,1), Finger::index,  Hand::right)}, {'u', std::make_tuple(std::make_pair(6,1), Finger::index,  Hand::right)}, {'i', std::make_tuple(std::make_pair(7,1), Finger::middle, Hand::right)},
    {'o', std::make_tuple(std::make_pair(8,1), Finger::ring,   Hand::right)}, {'p', std::make_tuple(std::make_pair(9,1), Finger::pinky,  Hand::right)}, {'a', std::make_tuple(std::make_pair(0,2), Finger::pinky,  Hand::left)},
    {'s', std::make_tuple(std::make_pair(1,2), Finger::ring,   Hand::left)},  {'d', std::make_tuple(std::make_pair(2,2), Finger::middle, Hand::left)},  {'f', std::make_tuple(std::make_pair(3,2), Finger::index,  Hand::left)},
    {'g', std::make_tuple(std::make_pair(4,2), Finger::index,  Hand::left)},  {'h', std::make_tuple(std::make_pair(5,2), Finger::index,  Hand::right)}, {'j', std::make_tuple(std::make_pair(6,2), Finger::index,  Hand::right)},
    {'k', std::make_tuple(std::make_pair(7,2), Finger::middle, Hand::right)}, {'l', std::make_tuple(std::make_pair(8,2), Finger::ring,   Hand::right)}, {';', std::make_tuple(std::make_pair(9,2), Finger::pinky,  Hand::right)},
    {'z', std::make_tuple(std::make_pair(0,3), Finger::pinky,  Hand::left)},  {'x', std::make_tuple(std::make_pair(1,3), Finger::ring,   Hand::left)},  {'c', std::make_tuple(std::make_pair(2,3), Finger::middle, Hand::left)},
    {'v', std::make_tuple(std::make_pair(3,3), Finger::index,  Hand::left)},  {'b', std::make_tuple(std::make_pair(4,3), Finger::index,  Hand::left)},  {'n', std::make_tuple(std::make_pair(5,3), Finger::index,  Hand::right)},
    {'m', std::make_tuple(std::make_pair(6,3), Finger::index,  Hand::right)}, {',', std::make_tuple(std::make_pair(7,3), Finger::middle, Hand::right)}, {'.', std::make_tuple(std::make_pair(8,3), Finger::ring,   Hand::right)}};

typedef struct {
    int cost;
    int parent;
} cell;

class TypoCost {
private:
    string _target;
    string _typo;
    int _target_index;
    int _typo_index;
    vector<vector<cell> > _LU;

    int _min_cost;
    vector<string> _operations;

protected:
    [[nodiscard]] int matching(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const;
    [[nodiscard]] int deleting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const;
    [[nodiscard]] int inserting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const;
    [[nodiscard]] int substituting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const;
    [[nodiscard]] int transposing(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const;

    void insert_out(const int& target_index, const int& typo_index);
    void delete_out(const int& target_index, const int& typo_index);
    void substitute_out(const int& target_index, const int& typo_index);
    void transpose_out(const int& target_index, const int& typo_index);

public:
    TypoCost(string target, string typo, int target_index, int typo_index): _target(target), _typo(typo), _target_index(target_index), _typo_index(typo_index)
    {_LU.resize(target_index+1, vector<cell>(typo_index+1, {-1,-1}));}

    string get_target(){return _target;}
    string get_typo(){return _typo;}
    int get_target_index(){return _target_index;}
    int get_typo_index(){return _typo_index;}
    int get_min_cost(){return _LU[_target_index][_typo_index].cost;}
    
    vector<string> get_vector_operations(){return _operations;}

    int recursion(int target_index, int typo_index);
    void reconstruct_path(int target_index, int typo_index);
};

#endif
