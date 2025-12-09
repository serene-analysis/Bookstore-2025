#pragma once
#include "log.h"
#include "account.h"
#include "book.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
struct Checker{
    Checker() = default;
    ~Checker() = default;
    bool operate(std::vector<std::string>, AccountSystem &, BookSystem &, LogSystem &);
};