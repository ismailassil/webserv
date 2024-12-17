// Forward declarations
class Request;
class RequestBuilder;
class RequestParser;

// Value Objects for different parts of the request
struct RequestLine {
    std::string method;
    std::string uri;
    std::string version;
};

struct Header {
    std::string name;
    std::string value;
};

// Main Request Class using Builder Pattern
class Request {
private:
    RequestLine requestLine;
    std::map<std::string, std::string> headers;
    std::string body;

    // Make constructor private to enforce builder usage
    Request() {}
    friend class RequestBuilder;

public:
    const RequestLine& getRequestLine() const { return requestLine; }
    const std::map<std::string, std::string>& getHeaders() const { return headers; }
    const std::string& getBody() const { return body; }
};

// Builder Class
class RequestBuilder {
private:
    Request request;

public:
    RequestBuilder& setMethod(const std::string& method) {
        request.requestLine.method = method;
        return *this;
    }

    RequestBuilder& setUri(const std::string& uri) {
        request.requestLine.uri = uri;
        return *this;
    }

    RequestBuilder& setVersion(const std::string& version) {
        request.requestLine.version = version;
        return *this;
    }

    RequestBuilder& addHeader(const std::string& name, const std::string& value) {
        request.headers[name] = value;
        return *this;
    }

    RequestBuilder& setBody(const std::string& body) {
        request.body = body;
        return *this;
    }

    Request build() {
        validate();
        return request;
    }

private:
    void validate() {
        if (request.requestLine.method.empty() ||
            request.requestLine.uri.empty() ||
            request.requestLine.version.empty()) {
            throw std::runtime_error("400 Bad Request: Missing required request line components");
        }
    }
};

// Chain of Responsibility Pattern for parsing
class RequestParser {
protected:
    RequestParser* next;
    RequestBuilder& builder;

public:
    RequestParser(RequestBuilder& b) : next(nullptr), builder(b) {}
    virtual ~RequestParser() { delete next; }

    void setNext(RequestParser* n) { next = n; }

    virtual void parse(std::istream& stream) {
        if (next) next->parse(stream);
    }
};

// Concrete parsers
class RequestLineParser : public RequestParser {
public:
    RequestLineParser(RequestBuilder& builder) : RequestParser(builder) {}

    void parse(std::istream& stream) override {
        std::string line;
        if (std::getline(stream, line)) {
            std::istringstream iss(line);
            std::string method, uri, version;
            
            if (!(iss >> method >> uri >> version)) {
                throw std::runtime_error("400 Bad Request: Invalid request line");
            }

            builder.setMethod(method)
                   .setUri(uri)
                   .setVersion(version);
        }
        RequestParser::parse(stream);
    }
};

class HeaderParser : public RequestParser {
public:
    HeaderParser(RequestBuilder& builder) : RequestParser(builder) {}

    void parse(std::istream& stream) override {
        std::string line;
        while (std::getline(stream, line) && !line.empty()) {
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                throw std::runtime_error("400 Bad Request: Invalid header format");
            }

            std::string name = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            // Trim whitespace
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" ") + 1);

            builder.addHeader(name, value);
        }
        RequestParser::parse(stream);
    }
};

// Usage Example
class WebServer {
public:
    Request parseRequest(const std::string& rawRequest) {
        RequestBuilder builder;
        std::istringstream stream(rawRequest);

        // Set up parsing chain
        RequestLineParser requestLineParser(builder);
        HeaderParser headerParser(builder);

        requestLineParser.setNext(&headerParser);

        // Start parsing
        try {
            requestLineParser.parse(stream);
            return builder.build();
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Error parsing request: ") + e.what());
        }
    }
};