
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

// validtion of tocknes format 
void validatoinVarFormat( tockens &mytockens){
	mytockens.val =  removeSpaces(mytockens.val);
	if (!mytockens.val.empty() && mytockens.mytocken == 0){
		if (mytockens.val[mytockens.val.size()- 1] == ';'){
	   
			mytockens.val =  mytockens.val.substr(0, mytockens.val.size()-1);
		}
		else{
			throw std::runtime_error("invald var name {" + mytockens.val + "}");
		}
		mytockens.val =  removeSpaces(mytockens.val);
	}
   	mytockens.val =  removeSpaces(mytockens.val);
	
	if (mytockens.mytocken == 1){
		
	    checkTockens(mytockens);
	}

}
