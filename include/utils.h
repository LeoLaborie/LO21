#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>
#include "Position.h"

inline std::vector<Position> grillePetite(int r)
{
    std::vector<Position> res;
    for (int x = -r; x <= r; ++x)
    {
        for (int y = -r; y <= r; ++y)
        {
            int z = -x - y;
            if (std::abs(z) > r)
                continue;
            res.push_back({x, y, z});
        }
    }
    return res;
}

#endif // UTILS_H