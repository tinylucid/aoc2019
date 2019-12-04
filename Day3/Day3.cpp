#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

typedef std::pair<int, int> Point;

namespace {
    std::vector<Point> pairs_from_step(char direction, int distance, const Point& position)
    {
        int x = position.first, y = position.second;
        std::vector<Point> steps;

        for (auto i = 0; i < distance; i++)
        {
            if (direction == 'U' || direction == 'D')
            {
                steps.emplace_back(std::make_pair(x, direction == 'U' ? ++y : --y));
                continue;
            }
            else if (direction == 'R' || direction == 'L')
                steps.emplace_back(std::make_pair(direction == 'R' ? ++x : --x, y));
        }
        return steps;
    }

    std::vector<Point> create_pairs(const std::string& line)
    {
        std::istringstream input_string_stream(line);
        std::string token;
        std::vector<Point> steps;
        Point last_step = std::make_pair(0, 0);

        while (std::getline(input_string_stream, token, ','))
        {
            auto direction = token.front();
            auto length = std::stoi(token.substr(1, token.length() - 1));
            auto new_steps = pairs_from_step(direction, length, last_step);
            steps.insert(steps.end(), new_steps.begin(), new_steps.end());
            last_step = steps.back();
        }

        return steps;
    }


    int manhattan(const Point& pair)
    {
        return abs(pair.first) + abs(pair.second);
    }
} // END OF NAMESPACE

int main()
{
    auto start_profiler = std::chrono::high_resolution_clock::now();
    std::ifstream input_stream("input.txt");
    std::string wire1, wire2;

    std::getline(input_stream, wire1);
    std::getline(input_stream, wire2);

    auto w1_pairs = create_pairs(wire1);
    auto w2_pairs = create_pairs(wire2);

    std::vector<Point> w1_p_sorted{ w1_pairs.size() }, w2_p_sorted{ w2_pairs.size() };

    std::partial_sort_copy(w1_pairs.begin(), w1_pairs.end(), w1_p_sorted.begin(), w1_p_sorted.end());
    std::partial_sort_copy(w2_pairs.begin(), w2_pairs.end(), w2_p_sorted.begin(), w2_p_sorted.end());

    std::vector<Point> intersection;

    std::set_intersection(w1_p_sorted.begin(), w1_p_sorted.end(), w2_p_sorted.begin(), w2_p_sorted.end(), std::back_inserter(intersection));
    std::sort(intersection.begin(), intersection.end(),
        [&](Point first, Point second) { return manhattan(first) < manhattan(second); });

    auto stop_profiler = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop_profiler - start_profiler);

    std::cout << "[p1] Result: " << manhattan(intersection.front()) << std::endl;

    std::vector<int> steps;
    steps.reserve(intersection.size());
    for (auto const& current : intersection)
    {
        auto w1 = std::find_if(w1_pairs.begin(), w1_pairs.end(), [&](Point tmp) {
            return tmp.first == current.first && tmp.second == current.second;
            });
        auto w2 = std::find_if(w2_pairs.begin(), w2_pairs.end(), [&](Point tmp) {
            return tmp.first == current.first && tmp.second == current.second;
            });
        steps.push_back(2 + w1 - w1_pairs.begin() + w2 - w2_pairs.begin());
    }
    std::cout << "[p2] " << *std::min_element(steps.begin(), steps.end()) << std::endl;
}