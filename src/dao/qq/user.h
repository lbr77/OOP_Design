#ifndef _DAO_QQ_USER_H_
#define _DAO_QQ_USER_H_
#pragma once
#include "../../class/qq.h"
#include "../../db/db.h"
namespace qq {

User selectUserByUsername(std::string username) {  // 查
    auto conn = SQL::getInstance();
    std::string sql =
        "SELECT id,username,password,created_at,birthday,address FROM users_qq WHERE username = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, username);
    auto res = stmt->executeQuery();
    if (res->next()) {
        return User(res->getInt("id"), res->getString("username"), res->getString("password"),
                    res->getString("created_at"), res->getString("birthday"),
                    res->getString("address"));
    }
    return User();
}
User insertUser(User user) {  // 增
    auto conn = SQL::getInstance();
    std::string sql =
        "INSERT INTO users_qq (username, password, birthday, address) VALUES (?, ?, ?,  "
        "?)";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, user.getUsername());
    stmt->setString(2, user.getPassword());
    stmt->setString(3, user.getBirthday());
    stmt->setString(4, user.getAddress());
    stmt->executeUpdate();
    return selectUserByUsername(user.getUsername());
}

bool updateUser(User user) {  // 改
    auto conn = SQL::getInstance();
    std::string sql =
        "UPDATE users_qq SET username = ?, password = ?, birthday = ?, address = ? "
        "WHERE id = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setString(1, user.getUsername());
    stmt->setString(2, user.getPassword());
    stmt->setString(3, user.getBirthday());
    stmt->setString(4, user.getAddress());
    stmt->setInt(6, user.getId());
    return stmt->executeUpdate() == 1;
}
User selectUserById(int id) {  // 查
    auto conn = SQL::getInstance();
    std::string sql =
        "SELECT id,username,password,created_at,birthday,address FROM users_qq WHERE id = ?";
    auto stmt = conn->prepareStatement(sql);
    stmt->setInt(1, id);
    auto res = stmt->executeQuery();
    if (res->next()) {
        return User(res->getInt("id"), res->getString("username"), res->getString("password"),
                    res->getString("created_at"), res->getString("birthday"),
                    res->getString("address"));
    }
    return User();
}
}  // namespace qq

#endif