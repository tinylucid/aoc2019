#include <iostream>
#include <vector>
#include <assert.h>

constexpr int OP_ADD = 1;
constexpr int OP_MUL = 2;
constexpr int OP_IN = 3;
constexpr int OP_OUT = 4;
constexpr int OP_JMPNZ = 5;
constexpr int OP_JMPZ = 6;
constexpr int OP_LS = 7;
constexpr int OP_EQ = 8;
constexpr int OP_BASEOFF = 9;
constexpr int OP_HALT = 99;

constexpr int MOD_POSITION = 0;
constexpr int MOD_IMMEDIATE = 1;
constexpr int MOD_RELBASE = 2;

long long read_memory(std::vector<long long>& memory, ptrdiff_t const off)
{
    if (off < 0) throw std::runtime_error("index out of bounds");

    if (off >= memory.size())
        memory.resize(off + 1, 0);

    return memory[off];
}

void write_memory(std::vector<long long>& memory, ptrdiff_t const off, long long const value)
{
    if (off < 0) throw std::runtime_error("index out of bounds");

    if (off >= memory.size())
        memory.resize(off + 1, 0);

    memory[off] = value;
}


long long read_value(std::vector<long long>& memory, ptrdiff_t const ip, ptrdiff_t const rel_base, int const mode)
{
    ptrdiff_t off = -1;

    switch (mode)
    {
    case MOD_POSITION:  off = memory[ip];            break;
    case MOD_IMMEDIATE: off = ip;                    break;
    case MOD_RELBASE:   off = rel_base + memory[ip]; break;
    default:            throw std::runtime_error("invalid access mode");
    }

    return read_memory(memory, off);
}

void write_value(std::vector<long long>& memory, ptrdiff_t const ip, ptrdiff_t const rel_base, int const mode, long long const value)
{
    ptrdiff_t off = -1;

    switch (mode)
    {
    case MOD_POSITION:  off = memory[ip];            break;
    case MOD_IMMEDIATE: throw std::runtime_error("cannot write in immediate mode");
    case MOD_RELBASE:   off = rel_base + memory[ip]; break;
    default:            throw std::runtime_error("invalid access mode");
    }

    write_memory(memory, off, value);
}

void execute_program(std::vector<long long> memory)
{
    ptrdiff_t ip = 0;
    ptrdiff_t rel_base = 0;

    while (true)
    {
        long long inst = memory[ip];
        int opcode = inst % 100;
        inst /= 100;
        int mod1 = inst % 10; inst /= 10;
        int mod2 = inst % 10; inst /= 10;
        int mod3 = inst % 10; inst /= 10;

        if (opcode == OP_HALT) break;

        assert(opcode >= OP_ADD && opcode <= OP_BASEOFF);

        switch (opcode)
        {
        case OP_ADD:
        {
            assert(mod3 != MOD_IMMEDIATE);
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            long long param2 = read_value(memory, ip + 2, rel_base, mod2);
            write_value(memory, ip + 3, rel_base, mod3, param1 + param2);
            ip += 4;
        }
        break;
        case OP_MUL:
        {
            assert(mod3 != MOD_IMMEDIATE);
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            long long param2 = read_value(memory, ip + 2, rel_base, mod2);
            write_value(memory, ip + 3, rel_base, mod3, param1 * param2);
            ip += 4;
        }
        break;
        case OP_IN:
        {
            assert(mod1 != MOD_IMMEDIATE);
            long long value;
            std::cout << ':'; std::cin >> value;
            write_value(memory, ip + 1, rel_base, mod1, value);
            ip += 2;
        }
        break;
        case OP_OUT:
        {
            long long param = read_value(memory, ip + 1, rel_base, mod1);
            std::cout << param << '\n';
            ip += 2;
        }
        break;
        case OP_JMPNZ:
        {
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            if (param1 != 0)
                ip = read_value(memory, ip + 2, rel_base, mod2);
            else
                ip += 3;
        }
        break;
        case OP_JMPZ:
        {
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            if (param1 == 0)
                ip = read_value(memory, ip + 2, rel_base, mod2);
            else
                ip += 3;
        }
        break;
        case OP_LS:
        {
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            long long param2 = read_value(memory, ip + 2, rel_base, mod2);
            write_value(memory, ip + 3, rel_base, mod3, param1 < param2 ? 1 : 0);
            ip += 4;
        }
        break;
        case OP_EQ:
        {
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            long long param2 = read_value(memory, ip + 2, rel_base, mod2);
            write_value(memory, ip + 3, rel_base, mod3, param1 == param2 ? 1 : 0);
            ip += 4;
        }
        break;
        case OP_BASEOFF:
        {
            long long param1 = read_value(memory, ip + 1, rel_base, mod1);
            rel_base += param1;
            ip += 2;
        }
        break;
        }
    }
}

int main()
{
    // tests
    //execute_program({ 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 });
    //execute_program({ 104,1125899906842624,99 });
    //execute_program({ 1102,34915192,34915192,7,4,7,99,0 });
    std::vector<long long> input{1102,34463338,34463338,63,1007,63,34463338,63,1005,63,53,1102,1,3,1000,109,988,209,12,9,1000,209,6,209,3,203,0,1008,1000,1,63,1005,63,65,1008,1000,2,63,1005,63,904,1008,1000,0,63,1005,63,58,4,25,104,0,99,4,0,104,0,99,4,17,104,0,99,0,0,1101,0,493,1024,1102,1,38,1015,1101,20,0,1011,1101,0,509,1026,1101,0,32,1018,1101,0,333,1022,1102,1,0,1020,1101,326,0,1023,1101,0,33,1010,1101,21,0,1016,1101,25,0,1004,1102,28,1,1008,1102,1,506,1027,1102,488,1,1025,1101,0,27,1013,1101,1,0,1021,1101,0,34,1019,1101,607,0,1028,1102,1,23,1003,1102,26,1,1007,1102,29,1,1009,1101,31,0,1000,1102,37,1,1012,1101,30,0,1005,1101,602,0,1029,1101,36,0,1002,1102,1,22,1001,1102,1,35,1014,1102,24,1,1006,1102,39,1,1017,109,4,21102,40,1,6,1008,1010,40,63,1005,63,203,4,187,1106,0,207,1001,64,1,64,1002,64,2,64,109,13,1206,3,221,4,213,1106,0,225,1001,64,1,64,1002,64,2,64,109,-5,1208,-9,22,63,1005,63,241,1106,0,247,4,231,1001,64,1,64,1002,64,2,64,109,-5,21107,41,40,3,1005,1010,263,1106,0,269,4,253,1001,64,1,64,1002,64,2,64,109,-1,1202,3,1,63,1008,63,29,63,1005,63,295,4,275,1001,64,1,64,1106,0,295,1002,64,2,64,109,16,21108,42,42,-8,1005,1014,313,4,301,1105,1,317,1001,64,1,64,1002,64,2,64,109,-4,2105,1,5,1001,64,1,64,1105,1,335,4,323,1002,64,2,64,109,-5,1207,-4,28,63,1005,63,355,1001,64,1,64,1105,1,357,4,341,1002,64,2,64,109,2,21102,43,1,-1,1008,1014,45,63,1005,63,377,1106,0,383,4,363,1001,64,1,64,1002,64,2,64,109,-10,1208,-3,36,63,1005,63,401,4,389,1106,0,405,1001,64,1,64,1002,64,2,64,109,6,21107,44,45,1,1005,1012,423,4,411,1105,1,427,1001,64,1,64,1002,64,2,64,109,4,21101,45,0,3,1008,1018,45,63,1005,63,453,4,433,1001,64,1,64,1105,1,453,1002,64,2,64,109,-23,2101,0,10,63,1008,63,36,63,1005,63,475,4,459,1106,0,479,1001,64,1,64,1002,64,2,64,109,26,2105,1,6,4,485,1105,1,497,1001,64,1,64,1002,64,2,64,109,4,2106,0,5,1105,1,515,4,503,1001,64,1,64,1002,64,2,64,109,-25,1201,10,0,63,1008,63,26,63,1005,63,537,4,521,1105,1,541,1001,64,1,64,1002,64,2,64,109,18,21101,46,0,-1,1008,1014,43,63,1005,63,565,1001,64,1,64,1106,0,567,4,547,1002,64,2,64,109,-6,1201,-4,0,63,1008,63,33,63,1005,63,587,1105,1,593,4,573,1001,64,1,64,1002,64,2,64,109,22,2106,0,-3,4,599,1105,1,611,1001,64,1,64,1002,64,2,64,109,-28,2102,1,-2,63,1008,63,22,63,1005,63,633,4,617,1105,1,637,1001,64,1,64,1002,64,2,64,109,-1,21108,47,44,9,1005,1011,653,1105,1,659,4,643,1001,64,1,64,1002,64,2,64,109,10,2107,24,-8,63,1005,63,681,4,665,1001,64,1,64,1105,1,681,1002,64,2,64,109,-11,2107,31,4,63,1005,63,697,1106,0,703,4,687,1001,64,1,64,1002,64,2,64,109,8,2101,0,-8,63,1008,63,23,63,1005,63,727,1001,64,1,64,1105,1,729,4,709,1002,64,2,64,109,-16,2108,21,10,63,1005,63,749,1001,64,1,64,1106,0,751,4,735,1002,64,2,64,109,17,2108,36,-8,63,1005,63,769,4,757,1105,1,773,1001,64,1,64,1002,64,2,64,109,-10,1207,1,23,63,1005,63,791,4,779,1105,1,795,1001,64,1,64,1002,64,2,64,109,-3,2102,1,6,63,1008,63,22,63,1005,63,815,1106,0,821,4,801,1001,64,1,64,1002,64,2,64,109,16,1205,7,837,1001,64,1,64,1105,1,839,4,827,1002,64,2,64,109,-5,1202,0,1,63,1008,63,30,63,1005,63,863,1001,64,1,64,1106,0,865,4,845,1002,64,2,64,109,4,1205,9,883,4,871,1001,64,1,64,1106,0,883,1002,64,2,64,109,16,1206,-7,899,1001,64,1,64,1106,0,901,4,889,4,64,99,21102,1,27,1,21101,915,0,0,1105,1,922,21201,1,47633,1,204,1,99,109,3,1207,-2,3,63,1005,63,964,21201,-2,-1,1,21102,942,1,0,1105,1,922,22102,1,1,-1,21201,-2,-3,1,21101,957,0,0,1106,0,922,22201,1,-1,-2,1105,1,968,22101,0,-2,-2,109,-3,2106,0,0};

    execute_program(input);
}