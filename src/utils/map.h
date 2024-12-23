#ifndef _U_MAP_H
#define _U_MAP_H
#include <map>
#include <string>
class Map {
   private:
    static std::map<std::string, std::string> *m;

   public:
    Map() {}
    ~Map() {
        delete m;
    }
    void newMap() {
        if (m == nullptr) {
            m = new std::map<std::string, std::string>;
        }
    }
    std::map<std::string, std::string> *getInstance() {
        return m;
    }
}
#endif