#include <boost/regex.hpp>
#include <iostream>
#include <string>


int calculate_fuel(int mass)
{
    return (mass / 3) - 2;
}

int r_calculate_fuel(int const& current_fuel, int sum)
{
    auto tmp = calculate_fuel(current_fuel);
    if (tmp > 0)
    {
        sum += tmp;
        tmp = r_calculate_fuel(tmp, sum);
    } else {
        return sum;
    }
}

int main()
{
    std::string line;
    boost::regex pat("^Subject: (Re: |Aw: )*(.*)");

    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }

    //std::string input_file = "input.txt";
    //std::ifstream fs(input_file);
    //int current, sum = 0, total_sum = 0;

    //while (fs.good())
    //{
    //    int temp_sum = 0;
    //    fs >> current;
    //    if (fs.eof()) { break; }
    //    sum += calculate_fuel(current);
    //    total_sum += r_calculate_fuel(current, temp_sum);
    //}

    //fs.close();

    //std::cout << "First sum is: " << sum << std::endl;
    //std::cout << "Total sum is: " << total_sum << std::endl;

    return 0;
}