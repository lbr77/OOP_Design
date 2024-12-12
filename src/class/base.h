#ifndef _BASH_CLASS_H_
#define _BASE_CLASS_H_
#pragma once
class Base {  // abstract one
   public:
    int getId() const {
        return id;
    }
    void setId(int id) {
        this->id = id;
    }
    // virtual ~Base() {}         // virtual destructor
    // virtual void print() = 0;  // pure virtual function
   private:
    int id;  // 主键
};

#endif