
#include "../include/parsing.hpp"

void applyRules(std::vector<tockens> &tocknes){

}


void bracketValidtion(std::vector<tockens> &tockens){
    int count_closed = 0;
    int count_opened = 0;
    tockenIt it = tockens.begin();
    while(it != tockens.end()){
        if (it->mytocken == OPENED_PRACKET)
            count_opened++;
        if (it->mytocken == CLOSED_PRACKET)
            count_closed++;
        it++;
    }  
    if ((count_closed != count_opened) || count_closed == 0 || count_opened == 0)
        throw std::runtime_error("Error in bracket "); 
    count_closed = 0;
    count_opened = 0;
    it = tockens.begin();
    while (it != tockens.end())
    {
        if (!it->val.compare("server")){
            it++;
            while (it != tockens.end())
            {
                
                if (it->mytocken == OPENED_PRACKET)
                    count_opened++;
                if (it->mytocken == CLOSED_PRACKET)
                    count_closed++;
                if (count_closed == count_opened){
                    count_closed = 0;
                    count_opened = 0;
                    it->mytocken = ENDSERV;
                    break;
                }
                it++;
            }
        }
        it++;
    }
    
}