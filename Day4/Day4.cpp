#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#define INTERVAL_MIN 236491
#define INTERVAL_MAX 713787

//bool p1(const std::string& input)
//{
//    int count_adj = 0;
//    if (input.size() != 6) return false;
//    
//    if (!std::is_sorted(input.begin(), input.end())) return false;
//    
//    for (int i = 0; i < input.size(); i++)
//    {
//        if (input[i] == input[i + 1])
//            count_adj++;
//    }
//    
//    if ((count_adj > 0)) return true; 
//    else return false;
//}

const auto part1 = [](std::string& input)
{
    return std::is_sorted(input.begin(), input.end()) && std::adjacent_find(input.begin(), input.end()) != input.end();
};

const auto part2 = [](std::string& input) 
{
    if (!std::is_sorted(input.begin(), input.end())) return false;
    return std::any_of(input.begin(), input.end(),
        [&](auto v) { return std::count(input.begin(), input.end(), v) == 2; });
};


int main()
{
    int count = 0;
    int count2 = 0;
    
    for (int i = INTERVAL_MIN; i < INTERVAL_MAX; i++)
    {
        std::string iter = std::to_string(i);
        // if (p1(iter))
        //     count++;
        count += part1(iter);
        count2 += part2(iter);
    }

    std::cout << "[p1]: " << count << std::endl;
    std::cout << "[p2]: " << count2 << std::endl;

    return 0;
}