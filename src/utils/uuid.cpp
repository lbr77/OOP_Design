#include "uuid.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>

UUIDGenerator* UUIDGenerator::instance = nullptr;
std::mutex UUIDGenerator::mutex;

UUIDGenerator::UUIDGenerator() : gen(rd()), dis(0, 15) {}

UUIDGenerator* UUIDGenerator::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new UUIDGenerator();
    }
    return instance;
}

std::string UUIDGenerator::generateV4() {
    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << (dis(gen) & 0x3 | 0x8);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    }
    return ss.str();
}

std::string UUIDGenerator::generateV1() {
    auto now = std::chrono::system_clock::now();
    auto timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    // 时间低位
    ss << std::setw(8) << (uint32_t)(timestamp & 0xFFFFFFFF) << "-";
    // 时间中位
    ss << std::setw(4) << (uint16_t)((timestamp >> 32) & 0xFFFF) << "-";
    // 时间高位和版本
    ss << "1" << std::setw(3) << (uint16_t)((timestamp >> 48) & 0x0FFF) << "-";
    // 变体和时钟序列
    ss << std::setw(4) << (dis(gen) & 0x3FFF | 0x8000) << "-";

    // MAC地址（使用随机值替代）
    for (int i = 0; i < 6; i++) {
        ss << std::setw(2) << dis(gen);
    }

    return ss.str();
}