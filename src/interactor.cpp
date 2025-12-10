#include "interactor.h"
#include "checker.h"
#include <cassert>
std::vector<std::string> Interactor::readLine(){
    std::vector<std::string> ret;
    std::string got;
    char ch = getchar();
    while(ch != '\n'){
        if(ch == '\r'){
            assert(getchar() == '\n');
            break;
        }
        if(ch == ' '){
            if(!got.empty()){
                ret.push_back(got), got.clear();
            }
        }
        else{
            got += ch;
        }
        ch = getchar();
    }
    return ret;
}

void Interactor::tian(AccountSystem &account, BookSystem &book, LogSystem &log, Checker &checker){
    assert(account.signup(turn("root"), turn("sjtu"), turn("root"), true));
    while(!end_){
        checker.operate(readLine(), account, book, log, *this);
    }
    return;
}