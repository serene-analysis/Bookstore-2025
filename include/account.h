#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tselected = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, long long > >;
using Taccount = std::pair<std::array<char,61>,std::tuple<std::array<char,61>, int, std::array<char,61>, Tselected > >;

struct AccountSystem{
    AccountSystem() = default;
    ~AccountSystem() = default;
    BlockList<Taccount> account_, stack_;
    bool exit();
    bool su(std::array<char,61>, std::array<char,61>);
    bool logout();
    bool signup(std::array<char,61>, std::array<char,61>, std::array<char,61>);
    bool passwd(std::array<char,61>, std::array<char,61>, std::array<char,61>);
    bool useradd(std::array<char,61>, std::array<char,61>, int, std::array<char,61>);
    bool remove(std::array<char,61>);
};