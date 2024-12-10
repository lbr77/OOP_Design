#ifndef _MYSQL_DB_H_
#define _MYSQL_DB_H_
#pragma once
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include<memory>
#include<string>
class SQL{
private:
    static SQL *instance;
    sql::Driver *driver;
    std::unique_ptr<sql::Connection> conn;
    SQL();
public:
    static SQL* getInstance();
    SQL(const SQL&) = delete; // no copy 
    SQL& operator=(const SQL&) = delete; // no value 

    ~SQL();

    bool connect(const std::string &host,const std::string &user,
                const std::string &password,const std::string &db);

    std::unique_ptr<sql::ResultSet> executeQuery(const std::string &query);

    bool executeUpdate(const std::string &query);

    std::unique_ptr<sql::PreparedStatement> prepareStatement(const std::string &query);
};
#endif