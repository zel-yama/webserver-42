
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
	else if (!tockn.val.compare("cgiExten"))
		tockn.mytocken = CGIEXTEN;
	else if (!tockn.val.compare("CGI"))
		tockn.mytocken = CGI;
	else if (!tockn.val.compare("uploadPath"))
		tockn.mytocken = UPLOADPATH;
	else if (!tockn.val.compare("upload"))
		tockn.mytocken = UPLOAD;
	else if (!tockn.val.compare("cgiPath"))
		tockn.mytocken = CGIPATH;
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
            methodesHandler(serv.allowedMethods, it->val, 1);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, serv.outoIndex);
            break;
        case INDEX:
            it++;
            methodesHandler(serv.indexFile, it->val, 0);
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
            returnP(it->val, serv.returnP, serv.returnCode);
            break;
        case CGI:
            it++;
            outoIndexHandler(it->val, serv.cgiStatus);
            break;
        case CGIEXTEN:
            it++;
            variableSingleValue(it->val, serv.cgiExten);
            break;
        case CGIPATH:
            it++;
            HandleMapstrings(it->val, serv.cgiConfig);
            break;
        case UPLOAD:
            it++;
            outoIndexHandler(it->val, serv.upload);
            break;
        case UPLOADPATH:
            it++;
            variableSingleValue(it->val, serv.uploadPath);
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
            methodesHandler(local.allowedMethods, it->val, 1);
            break;
        case AUTOINDEX:
            it++;
            outoIndexHandler(it->val, local.outoIndex);
            local.ex = true;
            break;
        case INDEX:
            it++;
            methodesHandler(local.indexFile, it->val, 0);
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
            returnP(it->val,local.returnP, local.returnCode );
            break;
        case CGI:
            it++;
            outoIndexHandler(it->val, local.cgiStatus);
            break;
        case CGIEXTEN:
            it++;
            variableSingleValue(it->val, local.cgiExten);
            break;
        case CGIPATH:
            it++;
            HandleMapstrings(it->val, local.CgiCofing);
            break;
        case UPLOAD:
            it++;
            outoIndexHandler(it->val, local.upload);
            break;
        case UPLOADPATH:
            it++;
            variableSingleValue(it->val, local.uploadPath);
            break;
        default:
            throw std::runtime_error("invalid value {"+ it->val + "} put valid config in location");
            break;
    }
}