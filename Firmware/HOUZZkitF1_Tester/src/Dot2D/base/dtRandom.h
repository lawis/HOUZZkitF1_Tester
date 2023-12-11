#ifndef __DT_RANDOM_H_
#define __DT_RANDOM_H_

#include <random>
#include <cstdlib>
#include "dtMacros.h"

/**
* @addtogroup base
* @{
*/
NS_DT_BEGIN

class  RandomHelper {
public:
    template<typename T>
    static T random_real(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        auto &mt = RandomHelper::getEngine();
        return dist(mt);
    }

    template<typename T>
    static T random_int(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        auto &mt = RandomHelper::getEngine();
        return dist(mt);
    }
private:
    static std::mt19937 &getEngine();
};

template<typename T>
inline T random(T min, T max) {
    return RandomHelper::random_int<T>(min, max);
}

template<>
inline float random(float min, float max) {
    return RandomHelper::random_real(min, max);
}

template<>
inline long double random(long double min, long double max) {
    return RandomHelper::random_real(min, max);
}

template<>
inline double random(double min, double max) {
    return RandomHelper::random_real(min, max);
}

inline int random() {
    return dot2d::random(0, RAND_MAX);
};

inline float rand_minus1_1() {
    return ((std::rand() / (float)RAND_MAX) * 2) -1;
};

inline float rand_0_1() {
    return std::rand() / (float)RAND_MAX;
};

NS_DT_END
// end group
/// @}

#endif //__DT_RANDOM_H_