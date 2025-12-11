#include "interactor.h"
#include "checker.h"
#include <cassert>
#include <iomanip>
std::vector<std::string> Interactor::readLine(){
    std::vector<std::string> ret;
    std::string got;
    char ch = getchar();
    while(ch != '\n'){
        if(ch == EOF){
            end_ = true;
            break;
        }
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
    if(!got.empty()){
        ret.push_back(got);
    }
    return ret;
}

void Interactor::tian(AccountSystem &account, BookSystem &book, LogSystem &log, Checker &checker){
    std::cout << std::fixed << std::setprecision(2);
    if(account.account_.number == 0){
        //std::cout << "create root" << std::endl;
        assert(account.signup(turn("root"), turn("sjtu"), turn("root"), true));
    }
    //std::cout << "ok!" << std::endl;
    while(!end_){
        auto got = readLine();
        if(end_){
            return;
        }
        if(!checker.operate(got, account, book, log, *this)){
            std::cout << "Invalid" << std::endl;
        }
    }
    return;
}