#pragma once
#include <functional>
#include <random>

static auto randomBool = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine(time(0)));
