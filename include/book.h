#pragma once
#include "blocklist.h"
#include "account.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tbook = std::pair<std::array<char,61>, std::tuple<std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61> > >;

struct BookSystem{
    BookSystem() = default;
    ~BookSystem() = default;
    BlockList<Tbook> book_;
    void show(std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>);
    void buy(std::array<char,61>, int);
    void select(std::array<char,61>, AccountSystem &);
    void modify(std::array<char,61>, std::array<char,61>, std::array<char,61>, std::array<char,61>, double, AccountSystem &);
    void import(int, double);
};