#include "dtRandom.h"

std::mt19937 &dot2d::RandomHelper::getEngine() {
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    return engine;
}
