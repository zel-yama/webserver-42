


#include "../include/parsing.hpp"


std::string removeSpaces(std::string &str){

  
	size_t end = str.size() ;
	size_t start = 0;
	while(start < end && std::isspace((unsigned char)str[start]))
		start++;
	while(start < end && std::isspace((unsigned char)str[end-1]))
		end--;
	return str.substr(start, (end - start));
}

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

std::vector<tockens> tokeniezer(doubleStr &v){
	std::vector<tockens> tockenV;
	tockens tock;
	doubleIter it = v.begin();
	std::string var;
	TYPE enumValue = VALUE;
	std::string val;
	while(it != v.end()){
		var = it->first;
		if (var.compare("location"))
			enumValue = LOCATIONPATH;
		val = it->second;
		if (!val.empty())//?? just check empty case
			pushTockens(VARIABLE, tockenV, var);
		if (it->second.empty())//  if second empty it means val is var case { firt things in line it means it var i handl it using this case 
			val = var;
		tockenValue(val, tockenV, enumValue);
		it++;
	}
  
	return tockenV;
}
void checkTockens(tockens &tockn){

	if (tockn.val.empty() || tockn.val.size() == 0)
		return;
	
	if (!tockn.val.compare("server_name"))
		tockn.mytocken = SERVERNAME;
	else if (!tockn.val.compare("root"))
		tockn.mytocken = ROOT;	
	else if(!tockn.val.compare("return"))
		tockn.mytocken = RETURN;
	else if (!tockn.val.compare("autoindex"))
		tockn.mytocken = AUTOINDEX;
	else if (!tockn.val.compare("client_max_body_size"))
		tockn.mytocken = BODYMAX;
	else if (!tockn.val.compare("limit_except"))
		tockn.mytocken = METHODS;
	else if (!tockn.val.compare("index"))
		tockn.mytocken = INDEX;
	else if (!tockn.val.compare("server"))
		tockn.mytocken = SERVER;
	else if (!tockn.val.compare("listen"))
		tockn.mytocken = LISTEN;
	else if (!tockn.val.compare("error_page"))
		tockn.mytocken = ERRORPAGE;
else if (!tockn.val.compare("location"))
		tockn.mytocken = LOCATION;
	else 
		throw std::runtime_error( "in valid tockenz {" + tockn.val + "} give valid one ");

}
// validtion of tocknes format 
void validatoinVarFormat( tockens &mytockens){
	mytockens.val =  removeSpaces(mytockens.val);
	if (!mytockens.val.empty() && mytockens.mytocken == 0){
		if (mytockens.val[mytockens.val.size()- 1] == ';'){
	   
			mytockens.val =  mytockens.val.substr(0, mytockens.val.size()-1);
		}
		else{
			throw std::runtime_error("invald var name ");
		}
		mytockens.val =  removeSpaces(mytockens.val);
	}
   	mytockens.val =  removeSpaces(mytockens.val);
	
	if (mytockens.mytocken == 1){
		
	    checkTockens(mytockens);
	}

}
