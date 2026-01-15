#include "../request/RequestParser.hpp"
#include "Response.hpp"
#define CHUNK_SIZE 4096

void validateRequest(Request &req, Server *srv);

Response::Response()
    : statusCode(200),
      statusMessage("OK"),
      protocol("HTTP"),
      version("1.0"),
      body("<h1>Hello World</h1>"),
      LargeFile(false)
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

void Response::setBody(const std::string &body)
{
    this->body = body;
    headers["Content-Length"] = toString(body.size());
    std::cout << "=====> " << headers["Content-Length"] << " <===\n";
}
void Response::setVersion(const std::string &version)
{
    this->version = version;
}

int Response::getStatusCode() const
{
    return statusCode;
}

const std::string &Response::getStatusMessage() const
{
    return statusMessage;
}

const std::string &Response::getBody() const
{
    return body;
}

bool Response::existFile(const char *path) const
{
    struct stat path_stat;
    return (stat(path, &path_stat) == 0 && S_ISREG(path_stat.st_mode));
}

bool Response::isDirectory(const char *path) const
{
    struct stat path_stat;
    return (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode));
}

std::string Response::getFileExtention(const std::string &path) const
{
    std::size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    return "";
}

void Response::setContentLength(const std::string &path)
{
    long size;
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file)
        return;

    file.seekg(0, std::ios::end);
    size = file.tellg();
    headers["Content-Length"] = toString(size);

    file.close();
}

void Response::setContentType(const std::string &path)
{
    std::string ext = getFileExtention(path);
    headers["Content-Type"] = getMimeType(ext);
}

std::string Response::readFile(const std::string &path) const
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file)
        return "";

    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

const std::map<std::string, std::string> &Response::getHeaders() const
{
    return headers;
}

std::string Response::getMimeType(const std::string &extension) const
{
    // if (extension == "html" || extension == "htm")
    //     return "text/html";
    if (extension == "css")
        return "text/css";
    if (extension == "js")
        return "application/javascript";
    if (extension == "json")
        return "application/json";
    if (extension == "xml")
        return "application/xml";
    if (extension == "txt")
        return "text/plain";
    if (extension == "jpg" || extension == "jpeg")
        return "image/jpeg";
    if (extension == "png")
        return "image/png";
    return "text/html";
}

bool Response::isLargeFile() const
{
    return LargeFile;
}

std::string Response::getFilePath() const
{
    return filePath;
}

size_t Response::getFileSize() const
{
    return fileSize;
}

void Response::processRequest(Request &req, Server &ser)
{
    validateRequest(req, &ser);
    setVersion(req.version);
    std::cout << "+++++++++++++++++++++++++++++" << std::endl;
    std::cout << req.method << std::endl;
    // std::cout << req.loc->root << std::endl;
    std::cout << req.full_path << std::endl;
    std::cout << req.status << std::endl;
    std::cout << req.version << std::endl;
    std::cout << "+++++++++++++++++++++++++++++" << std::endl;
    if (req.status != 200)
    {
        sendError(req.status, "");
        return;
    }

    if (req.method == "GET")
    {
        handleGet(req.full_path, req, ser);
    }
    else
    {
        sendError(405, "");
    }
}
void Response::handleDirectory(const std::string &path,
                               const Request &req,
                               const Server &srv)
{
    if (path[path.size() - 1] != '/')
    {
        setStatus(301, "");
        headers["Location"] = path + "/";
        headers["Content-Length"] = "0";
        body.clear();
        return ;
    }

    std::string indexFile = path + "index.html";
    if (existFile(indexFile.c_str()))
    {
        servFile(indexFile);
        return ;
    }

    else
        sendError(403, "");
}

void Response::handleGet(const std::string &path, const Request &req, const Server &srv)
{
    if (existFile(path.c_str()))
    {
        servFile(path);
        return;
    }

    if (isDirectory(path.c_str()))
    {
        handleDirectory(path, req, srv);
        return;
    }
    /*
        if (req.loc.autoindexenabled())
        {
            generateautoindex(path);
            return ;
        }

    */
    else
        sendError(404, "");
}
void Response::servFile(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        sendError(404, "");
        return;
    }

    fileSize = st.st_size;
    setStatus(200, "");
    setContentType(path);

    headers["Content-Length"] = toString(fileSize);

    if (fileSize <= CHUNK_SIZE)
    {
        LargeFile = false;
        std::string content = readFile(path);
        setBody(content);
    }
    else
    {
        LargeFile = true;
        filePath = path;
        body.clear();
    }
}
void Response::sendError(int code, const std::string &message)
{
    if (!message.empty())
        setStatus(code, message);
    else
        setStatus(code, "");
    if (code != 200)
        servErrorPage(code);
}

void Response::servErrorPage(int code)
{
    Request req;
    setStatus(code, "");
    headers["Content-Type"] = "text/html";

    std::ostringstream html;
    html << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head><title>" << code << " " << statusMessage << "</title></head>\n"
         << "<body>\n"
         << "<center><h1>" << code << " " << statusMessage << "</h1></center>\n"
         << "<hr><center>WebServer/1.0</center>\n"
         << "</body>\n"
         << "</html>\n";
    setBody(html.str());
}

std::string Response::build()
{
    std::ostringstream response;

    response << version << " "
             << statusCode << " " << statusMessage << "\r\n";

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        response << it->first << ": " << it->second << "\r\n";
    }

    response << "\r\n";

    if (statusCode != 204 && statusCode != 304 &&
        !(statusCode >= 100 && statusCode < 200))
    {
        response << body;
    }

    return response.str();
}
