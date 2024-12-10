#ifndef _DAO_QQ_USER_H_
#define _DAO_QQ_USER_H_
#include "../../db/db.h"
#include "../../class/user.h"
namespace qq {
    void insertUser(User user) {
        std::string sql = "INSERT INTO user (username,password,created_at,birthday,address,bind_id) VALUES (?,?,?,?,?,?)";
        auto db = SQL::getInstance();
        auto prep = db->prepareStatement(sql);
        prep->setString(1,user.getUsername());
        prep->setString(2,user.getPassword());
        prep->setString(3,user.getCreatedAt());
        prep->setString(4,user.getBirthday());
        prep->setString(5,user.getAddress());
        prep->setInt(6,user.getBindId());  
        prep->executeUpdate();
    }
    User selectUserById(int id) {
        std::string sql = "SELECT id,username,created_at,birthday,address,bind_id FROM user_qq WHERE id = ?";
        auto db = SQL::getInstance();
        auto prep = db->prepareStatement(sql);
        prep->setInt(1,id);
        auto res = prep->executeQuery();
        User user;
        if(res->next()) {
            user.setId(res->getInt("id"));
            user.setUsername(res->getString("username"));
            user.setCreatedAt(res->getString("created_at"));
            user.setBirthday(res->getString("birthday"));
            user.setAddress(res->getString("address"));
            user.setBindId(res->getInt("bind_id"));
        }
        return user;
    }
    User selectUserByUsername(std::string username) {
        std::string sql = "SELECT id,username,created_at,birthday,address,bind_id FROM user_qq WHERE username = ?";
        auto db = SQL::getInstance();
        auto prep = db->prepareStatement(sql);
        prep->setString(1,username);
        auto res = prep->executeQuery();
        User user;
        if(res->next()) {
            user.setId(res->getInt("id"));
            user.setUsername(res->getString("username"));
            user.setCreatedAt(res->getString("created_at"));
            user.setBirthday(res->getString("birthday"));
            user.setAddress(res->getString("address"));
            user.setBindId(res->getInt("bind_id"));
        }
        return user;
    }
    void updateUser(User user) {
        std::string sql = "UPDATE user_qq SET username = ?,password = ?,created_at = ?,birthday = ?,address = ?,bind_id = ? WHERE id = ?";
        auto db = SQL::getInstance();
        auto prep = db->prepareStatement(sql);
        prep->setString(1,user.getUsername());
        prep->setString(2,user.getPassword());
        prep->setString(3,user.getCreatedAt());
        prep->setString(4,user.getBirthday());
        prep->setString(5,user.getAddress());
        prep->setInt(6,user.getBindId());
        prep->setInt(7,user.getId());
        prep->executeUpdate();
    }
    void deleteUser(int id) {
        std::string sql = "DELETE FROM user_qq WHERE id = ?";
        auto db = SQL::getInstance();
        auto prep = db->prepareStatement(sql);
        prep->setInt(1,id);
        prep->executeUpdate();
    }
}
#endif