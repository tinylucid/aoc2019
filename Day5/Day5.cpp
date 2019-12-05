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


int get_instruction_mode(const std::vector<int>& memory, int ip, bool immediate) {
    if (immediate) {
        return ip;
    }
    return memory[ip];
}


void execute_program(std::vector<int>& code, const int input)
{
    auto ip = code.begin();

    while (ip != code.end())
    {
        const int opcode = *(ip++);
        const int instruction = opcode % 100;
        const bool mode_for_opA = ((opcode / 100) % 10) == 1;
        const bool mode_for_opB = ((opcode / 1000) % 10) == 1;

        switch (instruction)
        {
        case 99: // halt
        {
            return;
        }
        case 1: // add
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = opA + opB;
            break;
        }
        case 2: // mul
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = opA * opB;
            break;
        }
        case 3: // input
        {
            const int opA = *(ip++);
            code[opA] = input;
            break;
        }
        case 4: // print
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            std::cout << opA << std::endl;
            break;
        }
        case 5: // jump if not zero
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            if (opA != 0)
                ip = code.begin() + opB;

            break;
        }
        case 6: // jump if zero
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            if (opA == 0)
                ip = code.begin() + opB;
            break;
        }
        case 7: // less than
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            if (opA < opB)
                code[opC] = 1;
            else
                code[opC] = 0;
            break;
        }
        case 8:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            if (opA == opB)
                code[opC] = 1;
            else
                code[opC] = 0;
            break;
        }
        default:
            throw std::exception{ "intruction with wrong opcode " };
        }
    }
}

int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));

    std::vector<int> main_program = parse_input(main_input, ',');

    //execute_program(main_program, 1); // part 1
    execute_program(main_program, 5); // part 2

#if (0)
    for (auto const& value : main_program)
    {
        std::cout << value << " ";
    }
#endif
    return 0;
}
