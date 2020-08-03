#include "driver.hpp"

//calculates the distance between two keys in target and typo string
[[nodiscard]] int key_distance(const char& target_char, const char& typo_char) noexcept
{
    int row_dist = std::abs(std::get<0>(KEYBOARD.find(tolower(target_char))->second).second - std::get<0>(KEYBOARD.find(tolower(typo_char))->second).second);
    int col_dist = std::abs(std::get<0>(KEYBOARD.find(tolower(target_char))->second).first - std::get<0>(KEYBOARD.find(tolower(typo_char))->second).first);
    if(row_dist > col_dist)
    {return row_dist;}
    return col_dist;
}

//calculates cost of matching, returns 10 if not matching so as not to be chosen
[[nodiscard]] int TypoCost::matching(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const
{
    if (target[target_index] == typo[typo_index])
    {return 0;}
    else
    {return 10;}
}

//calculates the insertion cost for each character
[[nodiscard]] int TypoCost::inserting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const
{
    int lowest{6};//character before or after space/space after anything else/character after space
    Hand target_hand = std::get<2>(KEYBOARD.find(tolower(target[target_index]))->second);
    Hand typo_hand = std::get<2>(KEYBOARD.find(tolower(typo[typo_index]))->second);
    if(typo[typo_index] == ' ')
    {
        int typo_key_row = std::get<0>(KEYBOARD.find(tolower(typo[typo_index-1]))->second).second;
        if(typo_index > 1 && 2 < lowest && typo_key_row == 3)//space after key on bottom row
        {lowest = 2;}
    }
    else if(typo_index > 1)
    {
        Hand typo_hand_prev = std::get<2>(KEYBOARD.find(tolower(typo[typo_index-1]))->second);
        int dist = key_distance(typo[typo_index], typo[typo_index-1]);
        if(typo[typo_index] == typo[typo_index-1])//repeated character
        {lowest = 1;}
        if(dist && dist < lowest && typo_hand_prev == typo_hand)//after a key on the same hand
        {lowest = dist;}
        if(5 < lowest && typo_hand_prev != typo_hand)//after a key on opposite hand
        {lowest = 5;}
    }
    else if(typo_index < typo.length()-1)
    {
        Hand typo_hand_next = std::get<2>(KEYBOARD.find(tolower(typo[typo_index+1]))->second);
        int dist = key_distance(typo[typo_index], typo[typo_index+1]);
        if(dist && dist < lowest && typo_hand_next == typo_hand) //before a key on the same hand
        {lowest = dist;}
        if(5 < lowest && typo_hand_next == typo_hand) //before a key on opposite hand
        {lowest = 5;}
    }
    return lowest;
}

//calculates the deletion cost for each character
[[nodiscard]] int TypoCost::deleting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const
{
    int lowest{6};//char after space/key on other hand/first char in string
    if(target[target_index] == ' ')//space
    {lowest = 3;}
    if(target_index > 1 && target[target_index-1] != 0)
    {
        Hand target_hand = std::get<2>(KEYBOARD.find(tolower(target[target_index]))->second);
        Hand target_hand_prev = std::get<2>(KEYBOARD.find(tolower(target[target_index-1]))->second);
        if(target[target_index] == target[target_index-1])//repeated character
        {lowest = 1;}
        if(2 < lowest && target_hand == target_hand_prev)//character after another key on the same hand
        {lowest = 2;}
    }
    return lowest;
}

//this function calculates the substituting cost for each character
[[nodiscard]] int TypoCost::substituting(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const
{
    int lowest{6};//space for anything or anything for space
    int dist = key_distance(target[target_index], typo[typo_index]);

    auto target_tuple = KEYBOARD.find(tolower(target[target_index]))->second;
    auto typo_tuple = KEYBOARD.find(tolower(typo[typo_index]))->second;
    Hand target_hand = std::get<2>(target_tuple);
    Hand typo_hand = std::get<2>(typo_tuple);
    Finger target_finger = std::get<1>(target_tuple);
    Finger typo_finger = std::get<1>(typo_tuple);

    if(target_hand != typo_hand && target_finger != typo_finger)//key for another on different finger, other hand
    {lowest = 5;}
    if(dist && dist < lowest && target_hand == typo_hand)//key for another on the same hand
    {lowest = dist;}
    if(target_hand != typo_hand && target_finger == typo_finger)//key for another on same finger, other hand
    {lowest = 1;}
    return lowest;
}

//this function calculates the transposing cost for each character
[[nodiscard]] int TypoCost::transposing(const std::string& target, const std::string& typo, const int& target_index, const int& typo_index) const
{
    int lowest{10};

    //if(target_index > 1 && typo_index > 1 && target[target_index] == typo[typo_index-1] && typo[typo_index] == target[target_index-1])
    //{
    Hand target_hand = std::get<2>(KEYBOARD.find(tolower(target[target_index]))->second);
    Hand target_hand_prev = std::get<2>(KEYBOARD.find(tolower(target[target_index-1]))->second);
    if(target[target_index] == ' ' || typo[typo_index] == ' ')//space with anything else
    {lowest = 3;}
    if(target_hand == target_hand_prev)//keys on same hand
    {lowest = 2;}
    else//keys on different hand
    {lowest = 1;}
    //}
    return lowest;
}

//this function recurses through each character in the target and typo string to find the cost of each
int TypoCost::recursion(int target_index, int typo_index)
{
    vector<int> opt;    //vector to save cost of each option (selection, deletion, etc) for each character
    int min_cost;

    if (_LU[target_index][typo_index].cost != -1)   //memoization check
    {return _LU[target_index][typo_index].cost;}
    else if (target_index == 0 && typo_index == 0)  // base case
    {_LU[target_index][typo_index].cost = 0;}

    else if (target_index == 0 && typo_index > 0)   // recurrence 1, if typo string is longer than target string, find insertion cost for inserting extra chars into target string
    {
        _LU[target_index][typo_index].cost = recursion(target_index, typo_index - 1) + inserting(_target, _typo, target_index, typo_index);
        _LU[target_index][typo_index].parent = 1;
    }
    else if (typo_index == 0 && target_index > 0)   // recurrence 2, if target string is longer than typo string, find deleting cost for deleting extra chars from target string
    {
        _LU[target_index][typo_index].cost = recursion(target_index - 1, typo_index) + deleting(_target, _typo, target_index, typo_index);
        _LU[target_index][typo_index].parent = 2;
    }
    else    // recurrence 3, find matching, inserting and deleting cost for each character
    {
        opt.push_back(recursion(target_index - 1, typo_index - 1) + matching(_target, _typo, target_index, typo_index));        //0
        opt.push_back(recursion(target_index, typo_index - 1) + inserting(_target, _typo, target_index, typo_index));           //1
        opt.push_back(recursion(target_index - 1, typo_index) + deleting(_target, _typo, target_index, typo_index));            //2
        opt.push_back(recursion(target_index - 1, typo_index - 1) + substituting(_target, _typo, target_index, typo_index));    //3
        if(target_index >= 2 && typo_index >= 2 && _target[target_index] == _typo[typo_index-1] && _typo[typo_index] == _target[target_index-1])
        {opt.push_back(recursion(target_index - 2, typo_index - 2) + transposing(_target, _typo, target_index, typo_index));}   //4
        else
        {opt.push_back(recursion(target_index - 1, typo_index - 1)+10);}//dont want to choose transposing

        _LU[target_index][typo_index].cost = opt[0];
        _LU[target_index][typo_index].parent = 0;
        int k{};
        for (const auto option:opt)     //save the lowest cost in the lookup table
        {
            if (k > 0 && option < _LU[target_index][typo_index].cost)
            {
                _LU[target_index][typo_index].cost = option;
                _LU[target_index][typo_index].parent = k;
            }
            k++;
        }
        opt.clear();
    }
    //if(typo_index == _typo.length()-1 && target_index == _target.length()-1)
    //{
    //    for(auto look: _LU)
    //    {
    //        for(auto up: look)
    //        {
    //            std::cout << up.cost << "\t";
    //        }
    //        std::cout << std::endl<<std::endl;
    //    }
    //}
    return _LU[target_index][typo_index].cost;
}

//----------------------TESTING----------------------------//

//reconstructs the path to figure out which operations were done
void TypoCost::reconstruct_path(int target_index, int typo_index)
{
    if (_LU[target_index][typo_index].parent == -1)
        return;
    if (_LU[target_index][typo_index].parent == 0)
    {
        reconstruct_path(target_index - 1, typo_index - 1);
        return;
    }
    if (_LU[target_index][typo_index].parent == 1)
    {
        reconstruct_path(target_index, typo_index - 1);
        insert_out(target_index, typo_index);
        return;
    }
    if (_LU[target_index][typo_index].parent == 2)
    {
        reconstruct_path(target_index - 1, typo_index);
        delete_out(target_index, typo_index);
        return;
    }
    if (_LU[target_index][typo_index].parent == 3)
    {
        reconstruct_path(target_index - 1, typo_index - 1);
        substitute_out(target_index, typo_index);
        return;
    }
    if (_LU[target_index][typo_index].parent == 4)
    {
        reconstruct_path(target_index - 2, typo_index - 2);
        transpose_out(target_index, typo_index);
        return;
    }
}

void TypoCost::insert_out(const int& target_index, const int& typo_index)
{
    string temp = "Insert " + string(1, _typo[typo_index]) + " before " + to_string(typo_index);
    _operations.push_back(temp);

    //cout << "Insert " << _typo[typo_index] << " before " << typo_index-1 << endl;
}

void TypoCost::delete_out(const int& target_index, const int& typo_index)
{
    string temp = "Delete " + to_string(target_index);
    _operations.push_back(temp);

    //cout << "Delete " << target_index << endl;
}

void TypoCost::substitute_out(const int& target_index, const int& typo_index)
{
    string temp = "Substitute " + string(1, _typo[typo_index]) + " at " + to_string(typo_index);
    _operations.push_back(temp);

    //cout << "Substitute " << _typo[typo_index] << " at " << typo_index << endl;
}

void TypoCost::transpose_out(const int& target_index, const int& typo_index)
{
    string temp = "Transposition " + to_string(typo_index-1) + '-' + to_string(typo_index);
    _operations.push_back(temp);

    //cout << "Transposition " << typo_index-1 << "-" << typo_index << endl;
}
