#include "../request/RequestParser.hpp"
#include "../server/include/Server.hpp"
#include "Response.hpp"
#include "cgi.hpp"
#define CHUNK_SIZE 4096

void validateRequest(Request &req, Server *srv);

Response::Response()
    : statusCode(200),
      statusMessage("OK"),
      protocol("HTTP"),
      version("1.0"),
      body("<h1>Hello World</h1>"),
      srv(NULL),
      req(NULL),
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

void Response::setContext(Request *r, Server *s)
{
    req = r;
    srv = s;
}
bool Response::hasReadPermission(const std::string &path) const
{
    if (access(path.c_str(), F_OK) != 0)
        return false;
    if (access(path.c_str(), R_OK) != 0)
        return false;
    return true;
}

bool Response::hasDirPermission(const std::string &path) const
{
    if (access(path.c_str(), F_OK) != 0)
        return false;
    if (access(path.c_str(), R_OK | X_OK) != 0)
        return false;
    return true;
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

void Response::setContentType(const std::string &path)
{
    std::string extension = getFileExtention(path);
    headers["Content-Type"] = getMimeType(extension);
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
    if (extension == "html" || extension == "htm")
        return "text/html";
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
    if (extension == "csv")
        return "text/csv";
    if (extension == "md")
        return "text/markdown";
    if (extension == "jpg" || extension == "jpeg")
        return "image/jpeg";
    if (extension == "png")
        return "image/png";
    if (extension == "gif")
        return "image/gif";
    if (extension == "ico")
        return "image/x-icon";
    if (extension == "svg")
        return "image/svg+xml";
    if (extension == "bmp")
        return "image/bmp";
    if (extension == "webp")
        return "image/webp";
    if (extension == "mp3")
        return "audio/mpeg";
    if (extension == "wav")
        return "audio/wav";
    if (extension == "ogg")
        return "audio/ogg";
    if (extension == "mp4")
        return "video/mp4";
    if (extension == "webm")
        return "video/webm";
    if (extension == "avi")
        return "video/x-msvideo";
    if (extension == "pdf")
        return "application/pdf";
    if (extension == "zip")
        return "application/zip";
    if (extension == "tar")
        return "application/x-tar";
    if (extension == "gz")
        return "application/gzip";
    if (extension == "7z")
        return "application/x-7z-compressed";
    if (extension == "php")
        return "application/x-httpd-php";
    if (extension == "py")
        return "text/x-python";
    if (extension == "c")
        return "text/x-csrc";
    if (extension == "cpp" || extension == "cxx" || extension == "cc")
        return "text/x-c++src";
    if (extension == "h" || extension == "hpp")
        return "text/x-chdr";
    if (extension == "java")
        return "text/x-java-source";
    if (extension == "rb")
        return "text/x-ruby";
    if (extension == "sh")
        return "application/x-sh";
    if (extension == "jsonld")
        return "application/ld+json";

    return "application/octet-stream";
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
    setContext(&req, &ser);
    setHeader("Server", "MyWebServer/1.0");
    if (req.status != 200)
    {
        sendError(req.status, "");
        return;
    }
    validateRequest(req, &ser);
    setVersion(req.version);
    std::cout << "+++++++++++++++++++++++++++++" << std::endl;
    std::cout << req.method << std::endl;
    std::cout << req.loc->root << std::endl;
    std::cout << req.loc->outoIndex << std::endl;
    std::cout << ser.outoIndex << std::endl;
    std::cout << req.loc->locationPath << std::endl;
    std::cout << ser.D_ErrorPages[404] << std::endl;
    std::cout << req.loc->D_ErrorPages[404] << std::endl;
    std::cout << req.fullpath << std::endl;
    std::cout << req.status << std::endl;
    std::cout << req.version << std::endl;
    std::cout << "+++++++++++++++++++++++++++++" << std::endl;

    if (req.method == "GET")
    {
        handleGet(req.fullpath, req, ser);
    }
    else if (req.method == "POST")
    {
        handlePost(req.fullpath, req, ser);
    }
    else
    {
        sendError(405, "");
    }
}

void Response::handlePost(const std::string &path,
                          const Request &req,
                          const Server &srv)
{
    if (existFile(path.c_str()))
    {
        std::string ext = getFileExtention(path);
        
        if (ext == "php" || ext == "py")
        {
            Cgi cgi(req);
            std::string output = cgi.execute(path, path);
            setStatus(200, "");
            setBody(output);
            return;
        }

        std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            sendError(500, "");
            return;
        }
        file.write(req.body.c_str(), req.body.size());
        file.close();

        setStatus(200, "");
        setBody("<h1>Data saved successfully</h1>");
        return;
    }

    if (isDirectory(path.c_str()))
    {
        sendError(405, "");
        return;
    }

    std::ofstream file(path.c_str(), std::ios::out);
    if (!file.is_open())
    {
        sendError(500, "");
        return;
    }
    file.write(req.body.c_str(), req.body.size());
    file.close();

    setStatus(201, "");
    setBody("<h1>Data saved successfully</h1>");
}
void Response::handleDirectory(const std::string &path,
                               const Request &req,
                               const Server &srv)
{
    if (!hasDirPermission(path))
    {
        sendError(403, "");
        return;
    }

    if (path[path.size() - 1] != '/')
    {
        setStatus(301, "");
        headers["Location"] = path + "/";
        headers["Content-Length"] = "0";
        body.clear();
        return;
    }

    const std::vector<std::string> *indexList = NULL;

    if (req.loc && !req.loc->indexFile.empty())
        indexList = &req.loc->indexFile;
    else if (!srv.indexFile.empty())
        indexList = &srv.indexFile;

    if (indexList)
    {
        for (size_t i = 0; i < indexList->size(); ++i)
        {
            std::string fullPath = path + (*indexList)[i];
            if (existFile(fullPath.c_str()))
            {
                servFile(fullPath);
                return;
            }
        }
    }

    if (req.loc->ex)
    {
        if (req.loc->outoIndex)
            generateautoindex(path);
        else
            sendError(403, "");
        return;
    }
    if (srv.outoIndex)
    {
        generateautoindex(path);
        return;
    }

    else
        sendError(403, "");
}
void Response::generateautoindex(const std::string &path)
{
    if (!hasDirPermission(path))
    {
        sendError(403, "");
        return;
    }

    DIR *dir = opendir(path.c_str());
    if (!dir)
    {

        if (access(path.c_str(), F_OK) != 0)
        {
            sendError(404, "");
        }
        else if (access(path.c_str(), R_OK) != 0)
        {
            sendError(403, "");
        }
        else
        {
            sendError(500, "");
        }
        return;
    }

    std::ostringstream html;
    html << "<html><body>";
    html << "<h1>Index of " << path << "</h1>";
    html << "<ul>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        html << "<li><a href=\"" << name;
        if (entry->d_type == DT_DIR)
            html << "/";
        html << "\">" << name << "</a></li>";
    }

    html << "</ul></body></html>";
    closedir(dir);

    setStatus(200, "");
    headers["Content-Type"] = "text/html";
    setBody(html.str());
}
void Response::handleGet(const std::string &path, const Request &req, const Server &srv)
{
    if (existFile(path.c_str()))
    {
        std::string ext = getFileExtention(path);
        if (ext == "py")
        {
            Cgi cgi(req);
            std::string output = cgi.execute(path, path);
            setStatus(200, "");
            setBody(output);
            return;
        }
        servFile(path);
        return;
    }

    if (isDirectory(path.c_str()))
    {
        handleDirectory(path, req, srv);
        return;
    }
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

    if (!hasReadPermission(path))
    {
        sendError(403, "");
        return;
    }

    fileSize = st.st_size;
    setStatus(200, "");
    setContentType(path);

    headers["Content-Length"] = toString(fileSize);
    setHeader("Cache-Control", "no-cache");
    setHeader("Accept-Ranges", "bytes");

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
    setStatus(code, message);

    if (req && req->loc)
    {
        std::map<int, std::string>::iterator it =
            req->loc->D_ErrorPages.find(code);

        if (it != req->loc->D_ErrorPages.end())
        {
            std::string path = req->loc->root + it->second;
            // std::cout << path << std::endl;
            if (existFile(path.c_str()))
            {
                servFile(path);
                return;
            }
        }
    }

    if (srv)
    {
        std::map<int, std::string>::iterator it =
            srv->D_ErrorPages.find(code);

        if (it != srv->D_ErrorPages.end())
        {
            std::string path = srv->root + it->second;
            // std::cout << path << std::endl;
            if (existFile(path.c_str()))
            {
                servFile(path);
                return;
            }
        }
    }

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
