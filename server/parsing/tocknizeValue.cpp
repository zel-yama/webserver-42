
#include "../include/parsing.hpp"

void tockenValue(std::string &val, std::vector<tockens> &tockV, TYPE value){
	tockens tock;
	size_t i = 0;

	std::string buff  = "";
	while(i < val.size()){
		if (val[i] == '{'){
			pushTokens(OPENED_PRACKET, tockV, "{");
			i++;
		}
		if (val[i] == '}'){
			pushTokens(CLOSED_PRACKET, tockV, "}");
			i++;
		}
		while(i < val.size())
		{
			if (val[i] != '}' && val[i] != '{')
			{
				buff.push_back(val[i]);
				i++;
			}
			else{
				buff = removeSpaces(buff);
				if (!buff.empty())
				{
	
					pushTokens(value, tockV, buff);
					buff.clear();
					break;
				}
				else 
					break;
			}   
		}
		if (!buff.empty())
			pushTokens(VALUE, tockV, buff);
	}
}