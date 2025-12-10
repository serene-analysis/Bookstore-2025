#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Interactor{
    Interactor() = default;
    ~Interactor() = default;
    bool end_;
    std::vector<std::string> readLine();
    void tian(AccountSystem &, BookSystem &, LogSystem &, Checker &);// Start the system
};