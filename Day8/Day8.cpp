#include <iostream>
#include <fstream>
#include <cstring>
#define DEF 20000
std::ifstream file_in("input.txt");

int width = 25, height = 6, result, min_digit_0 = 1 << 29, img[6][25];
char input[DEF];

int main()
{
    file_in >> input;
    int layer{ 0 };
    
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
            img[i][j] = 2;
    }

    while (layer < strlen(input))
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (img[i][j] == 2)
                    img[i][j] = input[layer + i * width + j] - '0';
            }
        }
        layer += width * height;
    }
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (img[i][j])
                std::cout << "#";
            else
                std::cout << '.';
        }
        std::cout << std::endl;
    }

    
    return 0;
}