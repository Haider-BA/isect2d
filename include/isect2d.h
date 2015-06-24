#pragma once

#include <cmath>
#include <set>
#include <vector>

#include "aabb.h"
#include "vec2.h"
#include "obb.h"

inline static std::set<std::pair<int, int>> intersect(const std::vector<isect2d::AABB>& _aabbs, isect2d::Vec2 _split, isect2d::Vec2 _resolution) {
    struct AABBPair {
        const isect2d::AABB* aabb;
        unsigned int index;
    };
    
    std::set<std::pair<int, int>> pairs;
    int n = int(_split.x * _split.y);
    std::vector<AABBPair>* gridAABBs = new std::vector<AABBPair>[n];
    
    const float xpad = _resolution.x / _split.x;
    const float ypad = _resolution.y / _split.y;
    
    float x = 0.f, y = 0.f;

    for (int i = 0; i < _split.x; ++i) {
        for (int j = 0; j < _split.y; ++j) {
            isect2d::AABB grid(x, y, x + xpad, y + ypad);
            for (unsigned int index = 0; index < _aabbs.size(); ++index) {
                const isect2d::AABB* aabb = &_aabbs[index];
                if (grid.intersect(*aabb)) {
                    gridAABBs[int(j + i * _split.x)].push_back({aabb, index});
                }
            }
            x += xpad;
            x = fmodf(x, _resolution.x);
        }
        y += ypad;
    }
    
    for (int i = 0; i < n; ++i) {
        auto& v = gridAABBs[i];
        for (size_t j = 0; j < v.size(); ++j) {
            for (size_t k = j + 1; k < v.size(); ++k) {
                if (v[j].index != v[k].index && v[j].aabb->intersect(*v[k].aabb)) {
                    pairs.insert({ v[j].index, v[k].index });
                }
            }
        }
    }
    
    delete[] gridAABBs;
    return std::move(pairs);
}

inline static std::set<std::pair<int, int>> intersect(const std::vector<isect2d::AABB>& _aabbs) {
    std::set<std::pair<int, int>> pairs;
    
    if (_aabbs.size() == 0) {
        return pairs;
    }
    
    for (size_t i = 0; i < _aabbs.size(); ++i) {
        for (size_t j = i + 1; j < _aabbs.size(); ++j) {
            if (_aabbs[i].intersect(_aabbs[j])) {
                pairs.insert({ i, j });
            }
        }
    }
    
    return pairs;
}

