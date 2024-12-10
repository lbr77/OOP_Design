#ifndef _USER_CLASS_H_
#define _USER_CLASS_H_
#include"base.h"
#include"nlohmann/json.hpp"
class User : public Base {
private:
    int id;
    std::string username;
    std::string password;
    std::string created_at;
    std::string birthday;
    std::string address;
    int bind_id;
public: 
    User(int id=0, std::string username="", std::string password="", std::string created_at="", std::string birthday="", std::string address="", int bind_id=0)
    : id(id), username(username), password(password), created_at(created_at), birthday(birthday), address(address), bind_id(bind_id) {}
    ~User() {}
    void print() {
    }
    json to_json() {
        json j;
        j["id"] = id;
        j["username"] = username;
        j["password"] = password;
        j["created_at"] = created_at;
        j["birthday"] = birthday;
        j["address"] = address;
        j["bind_id"] = bind_id;
        return j;
    }
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    std::string getUsername() const {
        return username;
    }
    void setUsername(std::string username) {
        this->username = username;
    }
    std::string getPassword() const {
        return password;
    }
    void setPassword(std::string password) {
        this->password = password;
    }
    std::string getCreatedAt() const {
        return created_at;
    }
    void setCreatedAt(std::string created_at) {
        this->created_at = created_at;
    }
    std::string getBirthday() const {
        return birthday;
    }
    void setBirthday(std::string birthday) {
        this->birthday = birthday;
    }
    std::string getAddress() const {
        return address;
    }
    void setAddress(std::string address) {
        this->address = address;
    }
    int getBindId() const {
        return bind_id;
    }
    void setBindId(int bind_id) {
        this->bind_id = bind_id;
    }
};

#endif