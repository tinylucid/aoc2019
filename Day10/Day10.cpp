#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>
#include <vector>
#include <tuple>

enum class SpaceObject
{
    Void,
    Asteroid
};

template <typename T> class SpaceMap
{
    std::vector<T> m_data;
    unsigned int m_rows = 0;
    unsigned int m_cols = 0;

public:
    template <typename T1> SpaceMap(SpaceMap<T1> _map)
    {
        std::tie(m_rows, m_cols) = _map.get_dimensions();
        m_data = std::vector<T>(m_rows * m_cols);
    }

    SpaceMap(const SpaceMap& _map)
    {
        std::tie(m_rows, m_cols) = _map.get_dimensions();
        m_data = _map.m_data;
    }

    SpaceMap(std::string _filename)
    {
        std::ifstream input_file(_filename);
        if (input_file.is_open())
        {
            std::string line_string;
            while (std::getline(input_file, line_string))
            {
                m_cols = line_string.length();
                m_rows++;
                for (auto ch : line_string)
                    m_data.push_back((ch == '#') ? SpaceObject::Asteroid : SpaceObject::Void);
            }
            input_file.close();
        }
    }

    constexpr auto get_dimensions() const { return std::make_pair(m_rows, m_cols); }
    constexpr auto& at_position(unsigned int x, unsigned int y) { return m_data.at(x + y * m_cols); }
};

using Spacemap = SpaceMap<SpaceObject>;
using Sightmap = SpaceMap<unsigned int>;

int main()
{
    std::string file_name{ "input.txt" };
    auto asteroid_map = Spacemap(file_name);
    auto sight_map = Sightmap(asteroid_map);
    const auto [h, w] = static_cast<std::pair<int, int>>(asteroid_map.get_dimensions());
    int best_x = w, best_y = h;
    unsigned int best_count = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (asteroid_map.at_position(x, y) == SpaceObject::Asteroid)
            {
                auto copy_map = asteroid_map;
                auto _w = w;
                auto _h = h;
                auto check = [&](int x1, int y1) -> void {
                    if (copy_map.at_position(x1, y1) == SpaceObject::Asteroid)
                    {
                        sight_map.at_position(x, y)++;
                        int delta_x = (x1 - x) / std::gcd(x1 - x, y1 - y);
                        int delta_y = (y1 - y) / std::gcd(x1 - x, y1 - y);
                        for (int x_del = x1 + delta_x, y_del = y1 + delta_y;
                            x_del >= 0 && x_del < _w
                            && y_del >= 0 && y_del < _h;
                            x_del += delta_x, y_del += delta_y)
                            copy_map.at_position(x_del, y_del) = SpaceObject::Void;
                    }
                };
                

                for (int y1 = y; y1 < h; y1++) {
                    for (int x1 = (y1 == y) ? x + 1 : 0; x1 < w; x1++) {
                        /* right-down */
                        check(x1, y1);
                    }
                }

                for (int y1 = y; y1 >= 0; y1--) {
                    for (int x1 = (y1 == y) ? x - 1 : w - 1; x1 >= 0; x1--) {
                        /* left-up */
                        check(x1, y1);
            }
        }

                if (sight_map.at_position(x, y) > best_count)
                {
                    best_count = sight_map.at_position(x, y);
                    best_x = x; best_y = y;
                }
            }
            std::cout << best_x << " " << best_y << " " << best_count << std::endl;

        }
    }

    return 0;
}