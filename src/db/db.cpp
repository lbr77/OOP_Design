#include "db.h"

#include "../utils/log.h"
SQL *SQL::instance = nullptr;

SQL::SQL() {
    try {
        driver = get_driver_instance();
    } catch (sql::SQLException &e) {
        // throw std::runtime_error("Failed to get MySQL driver instance:" , std::string(e.what()));
        LOG_ERROR("Failed to get MySQL driver instance:", std::string(e.what()));
        exit(1);
    }
}

SQL::~SQL() {
    if (conn) {
        conn->close();
    }
}

SQL *SQL::getInstance() {
    if (instance == nullptr) {
        instance = new SQL();
    }
    return instance;
}

bool SQL::connect(const std::string &host, const std::string &user, const std::string &password,
                  const std::string &db) {
    try {
        conn.reset(driver->connect(host, user, password));

        conn->setSchema(db);
        LOG_INFO("Connected to MySQL database:", db);
        return true;
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to connect to MySQL:", std::string(e.what()));
        exit(1);
    }
}

std::unique_ptr<sql::ResultSet> SQL::executeQuery(const std::string &query) {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        LOG_DEBUG("Executing query:", query);
        return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to execute query:", std::string(e.what()));
        return nullptr;
    }
}

bool SQL::executeUpdate(const std::string &query) {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        LOG_DEBUG("Executing update:", query);
        return stmt->execute(query);
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to execute update:", std::string(e.what()));
        return false;
    }
}

std::unique_ptr<sql::PreparedStatement> SQL::prepareStatement(const std::string &query) {
    try {
        LOG_DEBUG("Preparing statement:", query);
        return std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement(query));
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to prepare statement:", std::string(e.what()));
        return nullptr;
    }
}
