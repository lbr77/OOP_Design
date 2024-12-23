#include <iostream>

#include "src/class/reflection.h"
#include "src/dao/user.h"
#include "src/db/db.h"
#include "src/utils/log.h"
int main() {
    SET_LOG_LEVEL(LogLevel::DEBUG);
    SQL::getInstance()->connect("tcp://localhost:3306", "root", "liborui77", "database");
    // test login
    // auto user = Login(701,"bc91743b05d17ef727d4e522b63c729c","QQ");
    // LOG_INFO(to_json(user));
    // test register
    // user = Register("dick","bc91743b05d17ef727d4e522b63c729c","QQ");
    // LOG_INFO(to_json(user));
    // test update
    // auto user = Update("nick","2025-01-02","New York",704);
    // LOG_INFO(to_json(user));
    // test search
    // auto users = Search("boss","QQ");
    // for(auto u : users) {
    //     LOG_INFO(to_json(u));
    // }
    // auto users = GetCommonFriends(552);
    // for(auto u : users) {
    //     LOG_INFO(to_json(u));
    // }
}