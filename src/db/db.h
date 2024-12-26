#ifndef _MYSQL_DB_H_
#define _MYSQL_DB_H_
#pragma once
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <time.h>
#include <experimental/chrono>
#include <memory>
#include <string>
class SQL {
   private:
    static SQL *instance;
    sql::Driver *driver;
    std::chrono::system_clock::time_point lastConnectTime;
    std::string host;
    std::string user;
    std::string password;
    std::string dbName;
    std::unique_ptr<sql::Connection> conn;
    // std::unique_ptr<sql::PreparedStatement> stmt;
    std::map<std::string, std::unique_ptr<sql::PreparedStatement>> stmts;
    SQL();

   public:
    static SQL *getInstance();
    SQL(const SQL &) = delete;             // no copy
    SQL &operator=(const SQL &) = delete;  // no value

    ~SQL();

    bool connect(const std::string &host, const std::string &user, const std::string &password,
                 const std::string &db);

    std::unique_ptr<sql::ResultSet> executeQuery(const std::string &query);

    bool executeUpdate(const std::string &query);

    std::unique_ptr<sql::PreparedStatement> prepareStatement(const std::string &query);
};
#endif