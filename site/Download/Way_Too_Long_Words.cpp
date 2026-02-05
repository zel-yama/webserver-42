#include <iostream>
#include <vector>


int main()
{
    int n;
    std::cin >> n;
    std::string str;
    std::vector<std::string> v;
    std::string res;
    for (int i = 0; i < n; i++)
    {
        std::cin >> str;
        if (str.size() <= 10)
            v.push_back(str);
        else
        {
            res = str[0] + std::to_string(str.size() - 2) + str[str.size() - 1];
            v.push_back(res);
        }
    }
    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << std::endl;
    }
}