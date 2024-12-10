#include <gtest/gtest.h>
#include "../src/http/struct.h"

class HTTPRequestTest : public ::testing::Test {
protected:
    HTTPRequest req;
    void SetUp() override {
        req = HTTPRequest();
    }
};

TEST_F(HTTPRequestTest, EmptyRequestTest) {
    const char* buffer = "";
    ASSERT_FALSE(bufferToRequest(buffer, req));
}

TEST_F(HTTPRequestTest, MultipleHeadersTest) {
    const char* buffer = 
        "GET /test HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Accept: text/html\r\n" 
        "User-Agent: Mozilla/5.0\r\n"
        "\r\n";
    
    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["host"], "example.com");
    EXPECT_EQ(req.headers["accept"], "text/html");
    EXPECT_EQ(req.headers["user-agent"], "Mozilla/5.0");
}

TEST_F(HTTPRequestTest, HeaderCaseInsensitiveTest) {
    const char* buffer =
        "GET /test HTTP/1.1\r\n"
        "Content-Type: text/html\r\n"
        "ACCEPT: application/json\r\n"
        "\r\n";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["content-type"], "text/html");
    EXPECT_EQ(req.headers["accept"], "application/json");
}

TEST_F(HTTPRequestTest, ComplexCookieTest) {
    const char* buffer =
        "GET /test HTTP/1.1\r\n"
        "Cookie: id=1234; session=abcd; theme=dark; lang=en\r\n"
        "\r\n";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.cookies["id"], "1234");
    EXPECT_EQ(req.cookies["session"], "abcd");
    EXPECT_EQ(req.cookies["theme"], "dark");
    EXPECT_EQ(req.cookies["lang"], "en");
}

TEST_F(HTTPRequestTest, ComplexJSONTest) {
    const char* buffer = 
        "POST /api/data HTTP/1.1\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "{\"user\":{\"name\":\"john\",\"age\":30},\"settings\":{\"theme\":\"dark\"}}";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.data["user"]["name"], "john");
    EXPECT_EQ(req.data["user"]["age"], 30);
    EXPECT_EQ(req.data["settings"]["theme"], "dark");
}

TEST_F(HTTPRequestTest, ComplexFormURLEncodedTest) {
    const char* buffer =
        "POST /submit HTTP/1.1\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "\r\n"
        "user=john&age=30&preferences=dark&language=en";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.data["user"], "john");
    EXPECT_EQ(req.data["age"], "30");
    EXPECT_EQ(req.data["preferences"], "dark");
    EXPECT_EQ(req.data["language"], "en");
}

TEST_F(HTTPRequestTest, MalformedHeaderTest) {
    const char* buffer =
        "GET /test HTTP/1.1\r\n"
        "BadHeader\r\n"
        "Host: example.com\r\n"
        "\r\n";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["host"], "example.com");
}
TEST_F(HTTPRequestTest, CookieTest) {
    const char* buffer = 
        "GET / HTTP/1.1\r\n"
        "Cookie: session=abc123; user=john\r\n"
        "\r\n";
    
    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.cookies["session"], "abc123");
    EXPECT_EQ(req.cookies["user"], "john");
}

TEST_F(HTTPRequestTest, JSONContentTest) {
    const char* buffer = 
        "POST /api/data HTTP/1.1\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "{\"name\":\"john\",\"age\":30}";
    
    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.data["name"], "john");
    EXPECT_EQ(req.data["age"], 30);
}

TEST_F(HTTPRequestTest, FormURLEncodedTest) {
    const char* buffer = 
        "POST /submit HTTP/1.1\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "\r\n"
        "name=john&age=30";
    
    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.data["name"], "john");
    EXPECT_EQ(req.data["age"], "30");
}

TEST_F(HTTPRequestTest, InvalidJSONTest) {
    const char* buffer = 
        "POST /api/data HTTP/1.1\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "{invalid_json}";
    
    ASSERT_FALSE(bufferToRequest(buffer, req));
}
TEST_F(HTTPRequestTest, EmptyHeaderValueTest) {
    const char* buffer = 
        "GET /test HTTP/1.1\r\n"
        "Header1:\r\n"
        "Header2: value2\r\n"
        "\r\n";
    
    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["header1"], "");
    EXPECT_EQ(req.headers["header2"], "value2");
}

TEST_F(HTTPRequestTest, DuplicateHeaderTest) {
    const char* buffer =
        "GET /test HTTP/1.1\r\n" 
        "Content-Type: text/html\r\n"
        "Content-Type: application/json\r\n"
        "\r\n";

    ASSERT_FALSE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["content-type"], "application/json");
}

TEST_F(HTTPRequestTest, LongRequestURITest) { // url too long?
    std::string longURI(2048, 'a');
    std::string buffer = "GET /" + longURI + " HTTP/1.1\r\n\r\n"; // 2048 characters ? 
    
    ASSERT_TRUE(bufferToRequest(buffer.c_str(), req));
}

TEST_F(HTTPRequestTest, SpecialCharactersInHeaderTest) {
    const char* buffer =
        "GET /test HTTP/1.1\r\n"
        "X-Custom-Header: !@#$%^&*()\r\n"
        "\r\n";

    ASSERT_TRUE(bufferToRequest(buffer, req));
    EXPECT_EQ(req.headers["x-custom-header"], "!@#$%^&*()");
}



class HTTPResponseTest : public ::testing::Test {
protected:
    HTTPResponse res;
    char* buffer;
    int buffer_len;

    void SetUp() override {
        buffer = nullptr;
        buffer_len = 0;
    }

    void TearDown() override {
        if (buffer) {
            delete[] buffer;
        }
    }
};

TEST_F(HTTPResponseTest, BasicResponseTest) {
    res.version = "HTTP/1.1";
    res.status_code = 200;
    res.status = "OK";
    res.headers["Content-Type"] = "text/html";
    res.body = "<html><body>Hello, world!</body></html>";

    ASSERT_EQ(responseToBuffer(res, buffer, buffer_len), 0);
    std::string expected_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 39\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>Hello, world!</body></html>";
    EXPECT_EQ(std::string(buffer, buffer_len), expected_response);
}

TEST_F(HTTPResponseTest, EmptyBodyTest) {
    res.version = "HTTP/1.1";
    res.status_code = 204;
    res.status = "No Content";
    res.headers["Content-Type"] = "text/html";
    res.body = "";

    ASSERT_EQ(responseToBuffer(res, buffer, buffer_len), 0);
    std::string expected_response = 
        "HTTP/1.1 204 No Content\r\n"
        "Content-Length: 0\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";
    EXPECT_EQ(std::string(buffer, buffer_len), expected_response);
}

TEST_F(HTTPResponseTest, MissingStatusCodeTest) {
    res.version = "HTTP/1.1";
    res.status_code = 0;
    res.headers["Content-Type"] = "text/html";
    res.body = "<html><body>Error</body></html>";

    ASSERT_EQ(responseToBuffer(res, buffer, buffer_len), 0);
    std::string expected_response = 
        "HTTP/1.1 500 Internal Server Error\r\n"
        "Content-Length: 31\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>Error</body></html>";
    EXPECT_EQ(std::string(buffer, buffer_len), expected_response);
}

TEST_F(HTTPResponseTest, CustomHeaderTest) {
    res.version = "HTTP/1.1";
    res.status_code = 200;
    res.status = "OK";
    res.headers["Content-Type"] = "application/json";
    res.headers["X-Custom-Header"] = "CustomValue";
    res.body = "{\"key\":\"value\"}";

    ASSERT_EQ(responseToBuffer(res, buffer, buffer_len), 0);
    std::string expected_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 15\r\n"
        "Content-Type: application/json\r\n"
        "X-Custom-Header: CustomValue\r\n"
        "\r\n"
        "{\"key\":\"value\"}";
    EXPECT_EQ(std::string(buffer, buffer_len), expected_response);
}

TEST_F(HTTPResponseTest, LargeBodyTest) {
    res.version = "HTTP/1.1";
    res.status_code = 200;
    res.status = "OK";
    res.headers["Content-Type"] = "text/plain";
    res.body = std::string(10000, 'a');

    ASSERT_EQ(responseToBuffer(res, buffer, buffer_len), 0);
    std::string expected_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 10000\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n" + std::string(10000, 'a');
    EXPECT_EQ(std::string(buffer, buffer_len), expected_response);
}