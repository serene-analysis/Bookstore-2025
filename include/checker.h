#pragma once
#include "log.h"
#include "account.h"
#include "book.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using String = std::array<char,61>;
enum Infotype{
    UserID, Password, CurrentPassword, NewPassword,
    Username,
    Privilege,
    ISBN,
    BookName, Author,
    Keyword,
    Quantity,
    Price, TotalCost,
    Count
};
String turn(std::string);
bool single_keyword(std::string);
bool duplicated_keyword(std::string);
std::string remove_pre_suf(std::string, Infotype);
struct Checker{
    Checker() = default;
    ~Checker() = default;
    bool valid(std::string, Infotype);
    bool pre_suf_valid(std::string, Infotype);
    bool operate(std::vector<std::string>, AccountSystem &, BookSystem &, LogSystem &, Interactor &);
};