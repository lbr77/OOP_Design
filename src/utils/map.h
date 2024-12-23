#ifndef _U_MAP_H
#define _U_MAP_H

#pragma once
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#define SET(key, value) KV::getInstance()->set(key, value)
#define GET(key) KV::getInstance()->get(key)
#define DEL(key) KV::getInstance()->del(key)
#define EXISTS(key) KV::getInstance()->exists(key)
#define EXPIRE(key, seconds) KV::getInstance()->expire(key, seconds)
#define TTL(key) KV::getInstance()->ttl(key)

class KV {
   private:
    static std::unique_ptr<KV> instance;
    static std::mutex mutex;

    std::map<std::string, std::string> store;
    std::map<std::string, std::chrono::system_clock::time_point> expireTime;
    std::mutex dataMutex;

    // 私有构造函数
    KV() = default;

    void cleanExpired() {
        std::lock_guard<std::mutex> lock(dataMutex);
        auto now = std::chrono::system_clock::now();
        for (auto it = expireTime.begin(); it != expireTime.end();) {
            if (it->second <= now) {
                store.erase(it->first);
                it = expireTime.erase(it);
            } else {
                ++it;
            }
        }
    }

   public:
    // 禁止拷贝和赋值
    KV(const KV&) = delete;
    KV& operator=(const KV&) = delete;

    static KV* getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance.reset(new KV());
        }
        return instance.get();
    }

    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(dataMutex);
        store[key] = value;
    }

    std::optional<std::string> get(const std::string& key) {
        cleanExpired();
        std::lock_guard<std::mutex> lock(dataMutex);
        auto it = store.find(key);
        if (it != store.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool del(const std::string& key) {
        std::lock_guard<std::mutex> lock(dataMutex);
        expireTime.erase(key);
        return store.erase(key) > 0;
    }

    bool exists(const std::string& key) {
        cleanExpired();
        std::lock_guard<std::mutex> lock(dataMutex);
        return store.find(key) != store.end();
    }

    bool expire(const std::string& key, int seconds) {
        std::lock_guard<std::mutex> lock(dataMutex);
        if (!exists(key)) {
            return false;
        }
        auto expiry = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
        expireTime[key] = expiry;
        return true;
    }

    int ttl(const std::string& key) {
        std::lock_guard<std::mutex> lock(dataMutex);
        auto it = expireTime.find(key);
        if (it == expireTime.end()) {
            return -1;
        }
        auto now = std::chrono::system_clock::now();
        auto remaining = std::chrono::duration_cast<std::chrono::seconds>(it->second - now).count();
        return remaining > 0 ? remaining : -2;
    }
};

// 静态成员初始化
std::unique_ptr<KV> KV::instance = nullptr;
std::mutex KV::mutex;
#endif