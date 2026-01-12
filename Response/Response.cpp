#include "Response.hpp"

Response::Response()
    : statusCode(200), statusMessage("OK"),
      protocol("HTTP"), version("1.0"),
      body("<h1>Hello World</h1>")
{
    statusMap[200] = "OK";
    statusMap[201] = "Created";
    statusMap[204] = "No Content";
    statusMap[301] = "Moved Permanently";
    statusMap[302] = "Found";
    statusMap[400] = "Bad Request";
    statusMap[403] = "Forbidden";
    statusMap[404] = "Not Found";
    statusMap[405] = "Method Not Allowed";
    statusMap[413] = "Payload Too Large";
    statusMap[415] = "Unsupported Media Type";
    statusMap[500] = "Internal Server Error";
    statusMap[501] = "Not Implemented";
    statusMap[503] = "Service Unavailable";
}

Response::~Response() {}

void Response::setStatus(int code, const std::string &message)
{
    statusCode = code;
    if (message.empty())
        statusMessage = statusMap[code];
    else
        statusMessage = message;
}

void Response::setHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}
static std::string toString(size_t n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

void Response::setBody(const std::string &b)
{
    body = b;
    headers["Content-Length"] = toString(body.size());
}

void Response::setVersion(const std::string &v)
{
    version = v;
}

int Response::getStatusCode() const { return statusCode; }
const std::string &Response::getStatusMessage() const { return statusMessage; }
const std::string &Response::getBody() const { return body; }

const std::map<std::string, std::string> &Response::getHeaders() const
{
    return headers;
}

bool Response::existFile(const char *path) const
{
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

bool Response::isDirectory(const char *path) const
{
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

std::string Response::getFileExtention(const std::string &path) const
{
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    return "";
}

std::string Response::readFile(const std::string &path) const
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file)
        return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string Response::getMimeType(const std::string &ext) const
{
    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css") return "text/css";
    if (ext == "js") return "application/javascript";
    if (ext == "json") return "application/json";
    if (ext == "xml") return "application/xml";
    if (ext == "txt") return "text/plain";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "png") return "image/png";
    return "application/octet-stream";
}

void Response::setContentType(const std::string &path)
{
    headers["Content-Type"] = getMimeType(getFileExtention(path));
}

void Response::processRequest(const Request &req, const Server &srv)
{
    setVersion(req.version);

    if (req.method == "GET")
        handleGet(req.path, req, srv);
    else
        sendError(405, "");
}

void Response::handleGet(const std::string &path,
                         const Request &req,
                         const Server &srv)
{
    if (existFile(path.c_str()))
    {
        servFile(path);
        return;
    }

    if (isDirectory(path.c_str()))
    {
        std::string index = path;
        if (index[index.length() - 1] != '/')
            index += "/";
        index += "index.html";

        if (existFile(index.c_str()))
        {
            servFile(index);
            return;
        }
    }

    sendError(404, "");
}

void Response::servFile(const std::string &path)
{
    std::string content = readFile(path);
    if (content.empty())
    {
        sendError(500, "Failed to read file");
        return;
    }

    setStatus(200, "");
    setContentType(path);
    setBody(content);
}

void Response::sendError(int code, const std::string &message)
{
    setStatus(code, message);
    servErrorPage(code);
}

void Response::servErrorPage(int code)
{
    setStatus(code, "");
    headers["Content-Type"] = "text/html";

    std::ostringstream html;
    html << "<!DOCTYPE html>"
         << "<html><head><title>"
         << code << " " << statusMessage
         << "</title></head><body>"
         << "<center><h1>"
         << code << " " << statusMessage
         << "</h1></center>"
         << "<hr><center>WebServer/1.0</center>"
         << "</body></html>";

    setBody(html.str());
}

std::string Response::build()
{
    std::ostringstream res;

    res << protocol << "/" << version << " "
        << statusCode << " " << statusMessage << "\r\n";

    for (std::map<std::string, std::string>::iterator it = headers.begin();
         it != headers.end(); ++it)
        res << it->first << ": " << it->second << "\r\n";

    res << "\r\n";

    if (statusCode != 204 && statusCode != 304 &&
        !(statusCode >= 100 && statusCode < 200))
        res << body;

    return res.str();
}
