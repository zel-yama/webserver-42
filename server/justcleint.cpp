
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sockfd;
    sockaddr_in server_addr;

    // 1. Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // 2. Server address (localhost = 127.0.0.1)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(15);

    // Convert "127.0.0.1" to binary form
    if (inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    // 3. Connect
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Connected to server.\n";

    // 4. Send HTTP request
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    write(sockfd, request.c_str(), request.size());

    // 5. Read response
    char buffer[4096];
    int n;

    std::cout << "----- Server Response -----\n";
    while ((n = read(sockfd, buffer, sizeof(buffer)-1)) > 0)
    {
        buffer[n] = '\0';
        std::cout << buffer;
    }
    std::cout << "\n---------------------------\n";

    // 6. Close
    close(sockfd);

    return 0;
}
