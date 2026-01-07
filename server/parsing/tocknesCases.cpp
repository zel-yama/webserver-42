
#include "../include/parsing.hpp"


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

void serverCases(tockenIt &it, Server &serv){
    
    if (it->mytocken == CLOSED_PRACKET || it->mytocken == OPENED_PRACKET
        || it->mytocken == SERVER)
        return ;
    switch (it->mytocken)
    {
        case ROOT:
            it++;
            variableSingleValue(it->val, serv.root);
            break;
        case LISTEN: 
            it++;
            insertListenConfig(serv, it->val);
            break;
        case BODYMAX:
            it++;
            bodySizeMax(serv.bodyMaxByte, it->val); 
            break;   
        case METHODS:
            it++;
            methodesHandler(serv.allowedMethods, it->val);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, serv.outoIndex);
            break;
        case INDEX:
            it++;
            methodesHandler(serv.indexFile, it->val);
            break;
        case SERVERNAME:
            it++;
            variableSingleValue(it->val, serv.ServerName);
            break;
        case ERRORPAGE:
            it++;
            methodsIntKey(serv.D_ErrorPages, it->val);
            break;
        case RETURN:
            it++;
            variableSingleValue(it->val, serv.returnP );
            break;
        default:
            throw std::runtime_error("invalid value {"+ it->val + "} put invalid config in server");
            break;
    }
}

void locationCases(tockenIt &it, location &local){
    
      if (it->mytocken == CLOSED_PRACKET || it->mytocken == OPENED_PRACKET)
        return ;
    switch (it->mytocken)
    {
        case ROOT:
            it++;
            variableSingleValue(it->val, local.root);
            break;
        case BODYMAX:
            it++;
            bodySizeMax(local.bodyMaxByte, it->val);
            break;
        case METHODS:
            it++;
            methodesHandler(local.allowedMethods, it->val);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, local.outoIndex);
            break;
        case INDEX:
            it++;
            methodesHandler(local.indexFile, it->val);
            break;
        case ERRORPAGE:
            it++;
            methodsIntKey(local.D_ErrorPages, it->val);
            break;
        case LOCATION:
            it++;
            variableSingleValue(it->val, local.locationPath);
            break;
        case RETURN:
            it++;
            variableSingleValue(it->val, local.returnP);
        default:
            throw std::runtime_error("invalid value {"+ it->val + "} put invalid config in location");
            break;
    }
}