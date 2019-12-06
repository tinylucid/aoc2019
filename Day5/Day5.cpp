#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define PRINT_VECTOR(iterable) for (auto const& iter : iterable) { std::cout << iter << " "; } std::cout << std::endl; 

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

enum Instructions {
    ADD = 1,
    MUL,
    INPUT,
    PRINT,
    JNZ,
    JZ,
    JL,
    JE,
    HALT = 99
};

int get_instruction_mode(const std::vector<int>& memory, int ip, bool immediate)
{
    if (immediate)
        return ip;

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
        case Instructions::HALT:
        {
            return;
        }
        case Instructions::ADD:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = opA + opB;
            break;
        }
        case Instructions::MUL:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = opA * opB;
            break;
        }
        case Instructions::INPUT:
        {
            const int opA = *(ip++);
            code[opA] = input;
            break;
        }
        case Instructions::PRINT:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            std::cout << opA << std::endl;
            break;
        }
        case Instructions::JNZ:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            if (opA != 0)
                ip = code.begin() + opB;
            break;
        }
        case Instructions::JZ:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            if (opA == 0)
                ip = code.begin() + opB;
            break;
        }
        case Instructions::JL:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = (opA < opB) ? 1 : 0;
            break;
        }
        case Instructions::JE:
        {
            const int opA = get_instruction_mode(code, *(ip++), mode_for_opA);
            const int opB = get_instruction_mode(code, *(ip++), mode_for_opB);
            const int opC = *(ip++);
            code[opC] = (opA == opB) ? 1 : 0;
            break;
        }
        default:
        {
            throw std::exception{ "intruction with wrong opcode " };
        }
        } // end of switch statement
    }
}


int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));
    std::vector<int> main_program = parse_input(main_input, ',');

    PRINT_VECTOR(main_program);

    int user_input;
    std::cout << "Please enter input for your program" << std::endl;
    if (!(std::cin >> user_input))
    {
        std::cout << "Bad user input! Restart the program and try again." << std::endl;
        std::cin.clear();
    }
    // [p1]: insert 1; [p2]: insert 5;
    execute_program(main_program, user_input); // part 2
    return 0;
}
