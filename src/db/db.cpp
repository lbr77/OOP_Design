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
    
    // Check if connection needs refresh (300s = 5 minutes)
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - instance->lastConnectTime).count();
    
    if (instance->conn && elapsed > 300) {
        LOG_INFO("Refreshing database connection after", std::to_string(elapsed), "seconds");
        instance->conn->close();
        instance->conn.reset();
        instance->connect(instance->host, instance->user, instance->password, instance->dbName);
        instance->lastConnectTime = now;
    }
    
    return instance;
}

bool SQL::connect(const std::string &host, const std::string &user, const std::string &password,
                  const std::string &db) {
    this->host = host;
    this->user = user;
    this->password = password;
    this->dbName = db;
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
        LOG_INFO("Executing query:", query);
        return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to execute query:", std::string(e.what()));
        return nullptr;
    }
}

bool SQL::executeUpdate(const std::string &query) {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        LOG_INFO("Executing update:", query);
        return stmt->execute(query);
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to execute update:", std::string(e.what()));
        return false;
    }
}
std::unique_ptr<sql::PreparedStatement> SQL::prepareStatement(const std::string &query) {
    try {

        LOG_INFO("Preparing statement:", query);
        auto it = stmts.find(query);
        if (it != stmts.end()) {
            // Return a new PreparedStatement using the same query since the cached one is unique_ptr
            // return std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement(query));
            it->second.reset();
            // return std::move(it->second);
        }
        
        // Create new PreparedStatement and store in cache
        stmts[query] = std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement(query));
        return std::move(stmts[query]);
    } catch (sql::SQLException &e) {
        LOG_ERROR("Failed to prepare statement:", std::string(e.what()));
        return nullptr;
    }
}