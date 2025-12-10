#pragma once
#include "blocklist.h"
#include "account.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tbook = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, long long > >;

struct BookSystem{
    BookSystem() = default;
    ~BookSystem() = default;
    BlockList<Tbook> book_;
    bool show(std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>);
    bool buy(std::array<char,61>, long long, AccountSystem &);
    bool select(std::array<char,61>, AccountSystem &);
    bool modify(std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, AccountSystem &);
    bool import(long long, double, AccountSystem &);
};