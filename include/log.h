#pragma once
#include "blocklist.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using Tfinance = std::pair<int,std::pair<double, double> >;
using Tlog = std::pair<int, std::string>;

struct LogSystem{
    BlockList<Tfinance> finance_;
    BlockList<Tlog> log_;
    LogSystem(){
        finance_.initialise("finance.data");
        log_.initialise("log.data");
        return;
    }
    ~LogSystem() = default;
    void move(double, bool);
    bool show(int);
    bool report_finance();
    bool report_employee();
    bool log();
};