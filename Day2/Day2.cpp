#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

std::vector<int> parse_input(std::string& in, char delimiter = ' ')
{
    std::vector<int> result;
    std::stringstream input_stream(in);
    std::string parsed_number;

    while (std::getline(input_stream, parsed_number, delimiter))
    {
        int i = std::stoi(parsed_number);
        result.push_back(i);
    }
    return result;
}

int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)), 
                           (std::istreambuf_iterator<char>()));

    auto main_program = parse_input(main_input, ',');

    main_program[1] = 54;
    main_program[2] = 85;
    bool halt = false;
    for (int i = 0; i < main_program.size(); i += 4)
    {
        if (!halt) {
            if (main_program.at(i) == 1)
            {
                int opA = main_program[i + 1];
                int opB = main_program[i + 2];
                int result = main_program[i + 3];

                main_program[result] = main_program[opA] + main_program[opB];
            }
            else if (main_program.at(i) == 2)
            {
                int opA = main_program[i + 1];
                int opB = main_program[i + 2];
                int result = main_program[i + 3];

                main_program[result] = main_program[opA] * main_program[opB];
            }
            else if (main_program.at(i) == 99)
            {
                halt = true;
            }
        }
    }

#if (1)
    for (auto const& value : main_program)
    {
        std::cout << value << " ";
    }
#endif

    return 0;
}
