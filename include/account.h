#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tselected = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61> > >;
using Taccount = std::pair<std::array<char,61>,std::tuple<std::array<char,61>, int, std::array<char,61>, Tselected > >;

struct AccountSystem{
    AccountSystem() = default;
    ~AccountSystem() = default;
    BlockList<Taccount> account_, stack_;
    void exit();
    void su(std::array<char,61>, std::array<char,61>);
    void logout();
    void signup(std::array<char,61>, std::array<char,61>, std::array<char,61>);
    void passwd(std::array<char,61>, std::array<char,61>, std::array<char,61>);
    void useradd(std::array<char,61>, std::array<char,61>, int, std::array<char,61>);
    void remove(std::array<char,61>);
};