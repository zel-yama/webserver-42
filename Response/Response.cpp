#include "../server/include/Server.hpp"
#include "../server/include/tools.hpp"
#include "Response.hpp"
#include "cgi.hpp"
#include <cctype>
#define CHUNK_SIZE 4096

void validateRequest(Request &req, Server *srv);

Response::Response()
    : statusCode(200),
      statusMessage("OK"),
      LargeFile(false),
      keepStatus(false),  
      version("HTTP/1.0"),
      body(""),
      req(NULL),
      srv(NULL)
{
   
    statusMap[200] = "OK";
    statusMap[201] = "Created";
    statusMap[204] = "No Content";
    statusMap[301] = "Moved Permanently";
    statusMap[302] = "Found";
    statusMap[304] = "Not Modified";
    statusMap[400] = "Bad Request";
    statusMap[403] = "Forbidden";
    statusMap[404] = "Not Found";
    statusMap[405] = "Method Not Allowed";
    statusMap[409] = "Conflict";
    statusMap[413] = "Payload Too Large";
    statusMap[414] = "URI too long";
    statusMap[502] = "Bad Gateway";
    statusMap[500] = "Internal Server Error";
    statusMap[504] = "Gateway Timeout";
    statusMap[501] = "Not Implemented";
}

Response::~Response() {

}

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

void Response::resetCgiFlag()
{
    cgiPending = false;
    cgiReadFd = -1;
    cgiWriteFd = -1;
    cgiPid = -1;
}

static std::string toLower(const std::string &value)
{
    std::string out = value;
    for (size_t i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    return out;
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
    if (extension == "jpg" || extension == "jpeg")
        return "image/jpeg";
    if (extension == "png")
        return "image/png";
    if (extension == "gif")
        return "image/gif";
    if (extension == "ico")
        return "image/x-icon";
    if (extension == "webp")
        return "image/webp";
    if (extension == "mp3")
        return "audio/mpeg";
    if (extension == "mp4")
        return "video/mp4";
    if (extension == "pdf")
        return "application/pdf";
    if (extension == "zip")
        return "application/zip";
    if (extension == "tar")
        return "application/x-tar";
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

    return "application/octet-stream";
}

bool Response::isLargeFile() const
{
    return LargeFile;
}

std::string& Response::getFilePath() 
{
    return  filePath;
}

bool Response::isCgipending() const {return cgiPending;}

int Response::getcgiReadFd() const {return cgiReadFd;}

int Response::getcgiWriteFd() const {return cgiWriteFd;}

pid_t Response::getcgiPid() const {return cgiPid;}

size_t Response::getFileSize() const
{
    return fileSize;
}

void Response::applyCgiResponse(const std::string &cgiOutput)
{
    size_t pos = cgiOutput.find("\r\n\r\n");
    if (pos == std::string::npos)
        pos = cgiOutput.find("\n\n");

    if (pos == std::string::npos)
    {
        setBody(cgiOutput);
        return;
    }

    std::string headersPart = cgiOutput.substr(0, pos);
    size_t offset;
    int parsedStatus = -1;

    if (cgiOutput[pos + 1] == '\r')
        offset = 4;
    else
        offset = 2;

    std::string bodyPart = cgiOutput.substr(pos + offset);
    std::istringstream iss(headersPart);
    std::string line;
    while (std::getline(iss, line))
    {
        if (line.empty() || line == "\r")
            continue;

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value = value.substr(1);
            while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == '\n'))
                value = value.substr(0, value.size() - 1);

            if (toLower(key) == "status")
            {
                std::istringstream status(value);
                int code;
                if (status >> code)
                    parsedStatus = code;
                continue;
            }

            setHeader(key, value);
        }
    }

    if (parsedStatus >= 400)
    {
        sendError(parsedStatus, ""); 
        return;
    }
    if (parsedStatus >= 100)
        setStatus(parsedStatus, "");

    setBody(bodyPart);
}

void Response::processRequest(Request &req, Server &ser)
{
    resetCgiFlag();
    setContext(&req, &ser);
    setHeader("Server", srv->ServerName);

    if (!req.keepalive)
        setHeader("Connection", "close");
    else
        setHeader("Connection", "keep-alive");

    validateRequest(req, &ser);
    setStatus(req.status, "");
    if (req.status != 200)
    {
        if (req.status >= 300 && req.status < 400 && req.headers.find("location") != req.headers.end())
        {
            setStatus(req.status, "");
            setHeader("Location", req.headers["location"]);
            setHeader("Content-Length", "0");
            body.clear();
            return;
        }
        sendError(req.status, "");
        return;
    }

    setVersion(req.version);
    logMethod = req.method;
    logIpAdress = ser.ipAdress;
    logPath = req.path;
    logUserAgent = req.headers["user-agent"];

    if (req.method == "GET")
    {
        handleGet(req.fullpath, req, ser);
    }
    else if (req.method == "POST")
    {
        handlePost(req.fullpath, req, ser);
    }
    else if (req.method == "DELETE")
    {
        handleDelete(req.fullpath, req, ser);
    }
    else
    {
        sendError(405, "");
    }

}

void Response::handleMultipartUpload(const Request &req, const Server &srv)
{

    if (req.multipartData.empty())
    {
        setStatus(400, "");
        setBody("<h1>No multipart data received</h1>");
        return;
    }

    std::string uploadPath = srv.uploadPath;
    if (!req.loc.uploadPath.empty())
        uploadPath = req.loc.uploadPath;

    if (!isDirectory(uploadPath.c_str()))
    {
        if (mkdir(uploadPath.c_str(), 0755) != 0)
        {
            sendError(500, "");
            return;
        }
    }

    std::ostringstream response;
    response << "<h1>File Upload Summary</h1>";
    response << "<ul>";

    for (size_t i = 0; i < req.multipartData.size(); ++i)
    {
        const MultipartPart &part = req.multipartData[i];
        if (!part.filename.empty())
        {
            std::string filePath = uploadPath + "/" + part.filename;
            std::ofstream file(filePath.c_str(), std::ios::binary);

            if (!file.is_open())
            {
                response << "<li><strong>ERROR uploading " << part.filename << "</strong></li>";
                continue;
            }

            file.write(part.content.c_str(), part.content.size());
            file.close();

            response << "<li>File uploaded: <strong>" << part.filename 
                    << "</strong> (" << part.content.size() << " bytes, "
                    << part.contentType << ")</li>";
        }
        else if (!part.content.empty() && !part.name.empty())
        {
            std::string filePath = uploadPath + "/" + part.name + ".txt";
            std::ofstream file(filePath.c_str(), std::ios::binary);

            if (!file.is_open())
            {
                response << "<li><strong>ERROR saving field " << part.name << "</strong></li>";
                continue;
            }

            file.write(part.content.c_str(), part.content.size());
            file.close();

            response << "<li>Field saved: '<strong>" << part.name << ".txt" 
                    << "</strong>' (" << part.content.size() << " bytes)</li>";
        }
        else
        {
            response << "<li>Field '<strong>" << part.name << "</strong>': " 
                    << part.content << "</li>";
        }
    }

    response << "</ul>";
    response << "<p>All files uploaded successfully!</p>";

    setStatus(201, "");
    headers["Content-Type"] = "text/html";
    setBody(response.str());
}

void Response::handlePost(const std::string &path,
                          const Request &req,
                          const Server &srv)
{

    if (!req.multipartData.empty())
    {
        handleMultipartUpload(req, srv);
        return;
    }

    if (existFile(path.c_str()))
    {
        std::string ext = getFileExtention(path);
        if (!ext.empty() && ext[0] != '.')
            ext = "." + ext;

        int cgiEnabled = srv.cgiStatus;
        std::map<std::string, std::string> cgiConfig = srv.cgiConfig;

        if (req.loc.cgiStatus != -1)
            cgiEnabled = req.loc.cgiStatus;
        if (!req.loc.CgiCofing.empty())
            cgiConfig = req.loc.CgiCofing;

        if (cgiEnabled == 1 && cgiConfig.find(ext) != cgiConfig.end())
        {
            Cgi cgi(req);

            std::string cgiPath = cgiConfig[ext];
            Cgihandle  handle = cgi.execute(cgiPath, path);
            if (handle.readFd == -1 || handle.pid == -1)
            {
                sendError(500, "");
                return ;
            }
            cgiPending = true;
            cgiReadFd = handle.readFd;
            cgiWriteFd = handle.writeFd;
            cgiPid = handle.pid;
            return ;
        }

        if (isDirectory(path.c_str()))
        {
            sendError(403, "");
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

        setStatus(201, "");
        headers["Content-Length"] = toString(req.body.size());
        return;
    }
    if (!req.query.empty())
    {
        setStatus(201, "");
        headers["Content-Length"] = toString(req.body.size());
        return ;
    }

    if (isDirectory(path.c_str()))
    {
        sendError(403, "");
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
    headers["Content-Length"] = toString(req.body.size());
}

void Response::handleDirectory(std::string &path,
                               const Request &req,
                               const Server &srv)
{
    if (!hasDirPermission(path))
    {
        sendError(403, "");
        return;
    }

    const std::vector<std::string> *indexList = NULL;
    bool has_index = false;

    if (!req.loc.indexFile.empty())
    {
        indexList = &req.loc.indexFile;
        has_index = true;
    }
    else if (!srv.indexFile.empty())
    {
        indexList = &srv.indexFile;
        has_index = true;
    }

    if (indexList)
    {
        std::string directoryPath = path;
        if (!directoryPath.empty() && directoryPath[directoryPath.size() - 1] != '/')
            directoryPath += "/";

        for (size_t i = 0; i < indexList->size(); ++i)
        {
            std::string fullPath = directoryPath + (*indexList)[i];
            if (existFile(fullPath.c_str()))
            {
                servFile(fullPath);
                return;
            }
        }

        if (has_index)
        {
            sendError(404, "");
            return;
        }
    }

    if (req.loc.ex)
    {
        if (req.loc.outoIndex)
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


void Response::handleGet(std::string &path, const Request &req, const Server &srv)
{

    if (existFile(path.c_str()))
    {
        std::string ext = getFileExtention(path);
        if (!ext.empty() && ext[0] != '.')
            ext = "." + ext;

        int cgiEnabled = srv.cgiStatus;
        std::map<std::string, std::string> cgiConfig = srv.cgiConfig;

        if (req.loc.cgiStatus != -1)
            cgiEnabled = req.loc.cgiStatus;
        if (!req.loc.CgiCofing.empty())
            cgiConfig = req.loc.CgiCofing;
        if (cgiEnabled == 1 && cgiConfig.find(ext) != cgiConfig.end())
        {
            Cgi cgi(req);

            std::string cgiPath = cgiConfig[ext];
            Cgihandle  handle = cgi.execute(cgiPath, path);
            if (handle.readFd == -1 || handle.pid == -1)
            {
                sendError(500, "");
                return ;
            }
            cgiPending = true;
            cgiReadFd = handle.readFd;
            cgiWriteFd = handle.writeFd;
            cgiPid = handle.pid;
            return ;
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
    if (!keepStatus)
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
    setStatus(code, message);

    if (req)
    {
        std::map<int, std::string>::iterator it =
            req->loc.D_ErrorPages.find(code);

        if (it != req->loc.D_ErrorPages.end())
        {
            std::string path = req->loc.root + it->second;
            if (existFile(path.c_str()))
            {
                keepStatus = true;
                servFile(path);
                keepStatus = false;
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
            if (existFile(path.c_str()))
            {
                keepStatus = true;
                servFile(path);
                keepStatus = false;
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

void Response::handleDelete(const std::string &path,
                            const Request &req,
                            const Server &srv)
{
    (void)req;
    (void)srv;
    if (!existFile(path.c_str()))
    {
        sendError(404, "");
        return;
    }

    if (isDirectory(path.c_str()))
    {
        sendError(409, "");
        return;
    }

    if (access(path.c_str(), W_OK) != 0)
    {
        sendError(403, "");
        return;
    }
    std::string ext = getFileExtention(path);
    if (!ext.empty() && ext[0] != '.')
        ext = "."  + ext;

    int cgiEnabled = srv.cgiStatus;
    std::map<std::string, std::string> cgiConfig = srv.cgiConfig;

    if(req.loc.cgiStatus != -1)
        cgiEnabled = req.loc.cgiStatus;
    if (!req.loc.CgiCofing.empty())
        cgiConfig = req.loc.CgiCofing;

    if (cgiEnabled == 1 && cgiConfig.find(ext) != cgiConfig.end())
    {
        Cgi cgi(req);
        
        std::string cgiPath = cgiConfig[ext];
        Cgihandle handle = cgi.execute(cgiPath, path);
        if (handle.readFd == -1 || handle.pid == -1)
        {
            sendError(500, "");
            return ;
        }
        cgiPending = true;
        cgiReadFd = handle.readFd;
        cgiWriteFd = handle.writeFd;
        cgiPid = handle.pid;
        return ;
    }

    if (std::remove(path.c_str()) != 0)
    {
        sendError(500, "");
        return;
    }
    
    setStatus(204, "");
    body.clear();
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

    if (statusCode != 204 && statusCode != 304)
    {
        response << body;
    }
    std::cout <<  logIpAdress << "--";  
    __displayTime();
    std::cout << " \"" << logMethod << " " << logPath << " " << version << "\" " << statusCode << " " << headers["Content-Length"] << " \"-\" " << logUserAgent << std::endl; 
    return response.str();
}
