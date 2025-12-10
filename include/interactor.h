#pragma once

#include "blocklist.h"
#include "account.h"
#include "checker.h"
#include "book.h"
#include "log.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Interactor{
    bool end_;
    Interactor(){
        end_ = false;
        return;
    }
    ~Interactor() = default;
    std::vector<std::string> readLine();
    void tian(AccountSystem &, BookSystem &, LogSystem &, Checker &);// Start the system
};