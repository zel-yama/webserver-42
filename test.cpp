
#include <ctime>
#include <iostream>

int main(){

    time_t va =  time(NULL);
    struct tm tim = *localtime(&va);
    std::cout <<tim.tm_sec << tim.tm_hour <<  tim.tm_min << std::endl;
    std::cout << va << std::endl;
    std::cout << ctime(&va) << std::endl;
    std::string s = "eeee";
    printf("%s\n", s.c_str());
}