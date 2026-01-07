
#include "../include/parsing.hpp"

void tockenValue(std::string &val, std::vector<tockens> &tockV, TYPE value){
	tockens tock;
	size_t i = 0;
	bool flagO = false;
	std::string buff  = "";
	bool flagC = false; 
	while(i < val.size()){
		if (val[i] == '{'){
			pushTockens(OPENED_PRACKET, tockV, "{");
			i++;
		}
		if (val[i] == '}'){
			pushTockens(CLOSED_PRACKET, tockV, "}");
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
	
					pushTockens(value, tockV, buff);
					buff.clear();
					break;
				}
				else 
					break;
			}   
		}
		if (!buff.empty())
			pushTockens(VALUE, tockV, buff);
	}
}