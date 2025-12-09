#include "checker.h"
bool Checker::operate(std::vector<std::string> str, AccountSystem &account, BookSystem &book, LogSystem &log){
    if(str.empty()){
        return false;
    }
    std::string fir = *str.begin();
    if(fir == "quit" || fir == "exit"){
        account.exit();
        return true;
    }
    
}