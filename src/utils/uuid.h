#ifndef _UTIL_UUID_
#define _UTIL_UUID_
#pragma once
#include <array>
#include <mutex>
#include <random>
#include <string>

#define UUID(x) UUIDGenerator::getInstance()->generateV##x()
class UUIDGenerator {
   private:
    static UUIDGenerator* instance;
    static std::mutex mutex;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

    UUIDGenerator();

   public:
    static UUIDGenerator* getInstance();

    // 生成Version 4 UUID (基于随机数)
    std::string generateV4();

    // 生成Version 1 UUID (基于时间戳)
    std::string generateV1();

    // 禁止拷贝和赋值
    UUIDGenerator(const UUIDGenerator&) = delete;
    void operator=(const UUIDGenerator&) = delete;
};
#endif