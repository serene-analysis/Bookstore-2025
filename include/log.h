#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tfinance = std::pair<int,std::tuple<double, double> >;
using Tlog = std::pair<int, std::string>;

struct LogSystem{
    LogSystem() = default;
    ~LogSystem() = default;
    BlockList<Tfinance> finance_;
    BlockList<Tlog> log_;
    void move(double v, bool income);
    bool show(int count);
    bool report_finance();
    bool report_employee();
    bool log();
};