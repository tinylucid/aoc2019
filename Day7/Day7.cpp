#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

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

int load_arg(std::vector<int>& memory, int pos, int mode)
{
    switch (mode)
    {
    case 0: { //position
        const auto index = (size_t)memory[pos];
        const auto arg = memory[index];
        return arg;
    } break;

    case 1: {
        const auto arg = memory[pos];
        return arg;
    } break;

    default: {
        std::cout << "Invalid mode for offset " << pos;
        exit(1);
    }
    }
    return 0;
}

struct Amp {
    int _id = 0;
    int ip = 0;
    std::vector<int> program;
    std::vector<int> input;
    bool halted = false;

    void run_program(std::vector<int>& output)
    {
        while (!halted) {
            const auto opcode = program[ip] % 100;
            switch (opcode)
            {
            case 99: {
             //   std::cout << "HALT" << std::endl;
                halted = true;
            } break;

            case 1: { // add
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                const auto result = program[ip + 3];
                program[result] = arg1 + arg2;
                ip += 4;
            } break;

            case 2: { // mul
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                const auto result = program[ip + 3];
                program[result] = arg1 * arg2;
                ip += 4;
            } break;

            case 3: { // input
                const auto index = program[ip + 1];
                if (input.empty()) {
                //    std::cout << "Amp " << _id << " needs an input" << std::endl;
                    return;
                }
                const auto input_val = input[0];
                (void)input.erase(input.begin());
                program[index] = input_val;
                ip += 2;
            } break;

            case 4: { // output
                const auto out_val = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                output.push_back(out_val);
                ip += 2;
            } break;

            case 5: { // JZ
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = (size_t)load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                if (arg1 != 0)
                    ip = arg2;
                else
                    ip += 3;
            } break;

            case 6: { // JNZ
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = (size_t)load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                if (arg1 == 0)
                    ip = arg2;
                else
                    ip += 3;
            } break;

            case 7: { // LT
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                const auto result = (size_t)program[ip + 3];
                program[result] = ((arg1 < arg2) ? 1 : 0);
                ip += 4;
            } break;

            case 8: { // EQ
                const auto arg1 = load_arg(program, ip + 1, (program[ip] / 100) % 10);
                const auto arg2 = load_arg(program, ip + 2, (program[ip] / 1000) % 10);
                const auto result = (size_t)program[ip + 3];
                program[result] = ((arg1 == arg2) ? 1 : 0);
                ip += 4;
            } break;

            default: {
                std::cout << "Invalid opcode" << opcode << std::endl;
                exit(1);
            } break;

            }
        }
    }
};


void print_phases(const std::vector<int>& phases)
{
    bool first = true;
    for (auto phase : phases) {
        if (!first)
            std::cout << ", ";

        first = false;
        std::cout << phase;
    }
    std::cout << std::endl;
}

int main()
{
    std::ifstream file_stream("input.txt");
    std::string main_input((std::istreambuf_iterator<char>(file_stream)),
        (std::istreambuf_iterator<char>()));
    std::vector<int> main_program = parse_input(main_input, ',');
    // PRINT_VECTOR(main_program);
    // PART[1]   std::vector<int> phases{ 0, 1, 2, 3, 4 };
    
    std::vector<int> phases{ 5, 6, 7, 8, 9 }; // PART[2]
    std::vector<int> largest_output_phases;
    int largest_output = 0;
    do {
        int input = 0;
       // std::cout << "Running phase >> ";
        std::vector<Amp> amps;
        //print_phases(phases);
        int amp_id = 1;
        for (auto phase : phases)
        {
            Amp amp;
            amp._id = amp_id++;
            amp.program = main_program;
            amp.input.push_back(phase);
            amps.push_back(std::move(amp));
        }
        amps[0].input.push_back(0); // initiating cycle by sending 0 to fist Amp just once
        for (int amp_index = 0; !amps[4].halted; amp_index = (amp_index + 1) % 5)
        {
            amps[amp_index].run_program(amps[(amp_index + 1) % 5].input);
        }
        const auto output = amps[0].input.back();
        if (output > largest_output) {
            largest_output = output;
            largest_output_phases = phases;
        }
    } while (std::next_permutation(phases.begin(), phases.end()));
    std::cout << largest_output << std::endl;
   // print_phases(largest_output_phases);
    return 0;
}
