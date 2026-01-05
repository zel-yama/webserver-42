#include <iostream>
#include "Response.hpp"

int main()
{
    Response res;

    res.setStatus(200, "OK");
    res.setHeader("Content-Type", "text/html");
    res.setBody("<h1>Hello WebServ</h1>");

    std::string output = res.build();

    std::cout << output;

    return 0;
}
