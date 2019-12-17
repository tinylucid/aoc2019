#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <optional>
#include <unordered_map>
#include <deque>

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

int run_amp(std::vector<int>& program, const std::vector<int>& phase_setting)
{
    int amp_signal = 0;
    for (int amp = 0; amp < 5; amp++)
    {
        execute_program(program, phase_setting[amp]);
        // storing output of amp A as input for amp B
        // then output of amp B as input of amp C ...
    }
}


int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));
    std::vector<int> main_program = parse_input(main_input, ',');
    std::vector<int> phases{ 0, 1, 2, 3, 4 };
    //auto max_out = std::numeric_limits<int>::min();

    PRINT_VECTOR(main_program);
    auto result = run_amp(main_program, phases);
    // part 1

    std::cout << result << std::endl;
    return 0;
}
