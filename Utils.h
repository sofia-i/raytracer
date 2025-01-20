//
// Created by Sofia Iannicelli on 1/17/25.
//

#ifndef RAYTRACER_2_UTILS_H
#define RAYTRACER_2_UTILS_H

struct IndexOfRefraction {
    static constexpr double VACUUM = 1.0;
    static constexpr double AIR = 1.0003;
    static constexpr double WATER = 1.33;
    static constexpr double ALCOHOL = 1.36;
    static constexpr double FUSED_QUARTZ = 1.46;
    static constexpr double CROWN_GLASS = 1.52;
    static constexpr double FLINT_GLASS = 1.65;
    static constexpr double SAPPHIRE = 1.77;
    static constexpr double DIAMOND = 2.42;
};

#endif //RAYTRACER_2_UTILS_H
