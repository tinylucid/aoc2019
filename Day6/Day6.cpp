#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <map>

std::vector<std::string> parse_input(std::string& in, char delimiter = ' ')
{
    std::vector<std::string> result;
    std::stringstream input_stream(in);
    std::string parsed_number;
    while (std::getline(input_stream, parsed_number, delimiter))
    {
        std::string i = parsed_number;
        result.push_back(i);
    }
    return result;
}

int sum_of_orbits(std::map<std::string, std::string>& m)
{
    auto iter = m.begin();
    std::string helper;
    int result { 0 };

    while (iter != m.end())
    {
        helper = iter->first;
        while (m.count(helper) > 0)
        {
            ++result;
            helper = m[helper];
        }
        ++iter;
    }
    return result;
}

//int shortest_path_to_santa(std::map<std::string, std::string>& m)
//{
//    auto src = m.find("YOU");
//    auto dest = m.find("SAN");
//    int result{ 0 }, min_result{ 0 };
//    std::string helper;
//
//    if (src != m.end())
//    {
//        helper = src->second;
//        while (src->second != dest->second)
//        {
//            if (result < min_result)
//                min_result = result;
//
//            ++result;;
//            helper = m[helper];
//
//
//        }
//        ++src;
//
//        return min_result;
//    }
//    else
//        throw std::exception{ "I couldn't find myself in map :(" };
//
//    return result;
//}

int shortest_path_to_santa(std::map<std::string, std::string>& m)
{
    std::map<std::string, int> santa_map{};
    int result{ 1 };
    auto helper = m["SAN"];
    while (m.count(helper) > 0)
    {
        helper = m[helper];
        santa_map[helper] = result;
        ++result;
    }
    int you{ 1 };
    helper = m["YOU"];
    while (santa_map.count(helper) == 0)
    {
        helper = m[helper];
        ++you;
    }
    return (you-1 + santa_map[helper]);
}

int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));

    std::vector<std::string> input_v = parse_input(main_input, '\n');
    std::map<std::string, std::string> orbiters;
    for (std::vector<std::string>::iterator it = input_v.begin(); it != input_v.end(); ++it) {
        orbiters[(*it).substr(4)] = (*it).substr(0, 3);
    }

    // std::cout << "[p1]: " << sum_of_orbits(orbiters) << std::endl;
    std::cout << "[p2]: " << shortest_path_to_santa(orbiters) << std::endl;

    return 0;
}