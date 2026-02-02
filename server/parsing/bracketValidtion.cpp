
#include "../include/parsing.hpp"
void accept(tockens tokn, TYPE tokn1){
    if (tokn.mytocken != tokn1)
        throw std::runtime_error("this is <" + tokn.val +  "> unexpected tokens ");
}

void applyRules(std::vector<tockens> &tocknes){

    tockenIt it = tocknes.begin();
    while ((it + 1) != tocknes.end()){
            if (it->mytocken == SERVER)
                accept(*(it + 1), OPENED_PRACKET);
            if (it->mytocken == LOCATIONPATH)
                accept(*(it + 1), OPENED_PRACKET);
            if (it->mytocken == LOCATION)
                accept(*(it +1), LOCATIONPATH);
            if (it->mytocken > 7)
                accept(*(it + 1), VALUE);
            it++;
    }
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
    applyRules(tockens);
}