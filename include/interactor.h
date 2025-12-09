#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Interactor{
    Interactor() = default;
    ~Interactor() = default;
    std::vector<std::string> readLine();
};