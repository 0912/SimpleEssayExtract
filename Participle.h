#include <cstdio>
#include <scws/scws.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "locker.h"

using namespace std;
static locker lockRes;
static locker lockTopRes;
static locker selfLocker;
class Participle {
public:
    ~Participle(){
        delete instance_;
        scws_free(s);
        delete [] text;
    }
    static Participle* GetInstance(){
        selfLocker.lock();
        if (instance_==NULL) {
            instance_=new Participle();
        }
        selfLocker.unlock();
        return instance_;
    }
    vector<string> getRes(char text[],bool);//分词
    vector<string> getRes(string t,bool);
    vector<string> getTopRes(char text[],bool);//求关键词
    vector<string> getTopRes(string t,bool);
private:
    Participle(){
        if (!(s = scws_new())) {
          printf("ERROR: cann't init the scws!\n");
        }
        scws_set_charset(s, "utf8");
        scws_set_dict(s, "./etc/dict.utf8.xdb", SCWS_XDICT_MEM);
        scws_set_rule(s, "./etc/rules.utf8.ini");
        scws_set_ignore(s, 1);
    }
    static Participle* instance_;
    char text[100000];
    scws_t s;
    vector<string> Res;
    vector<string> topRes;
};
vector<string> Participle::getRes(char text[],bool en=true){
    lockRes.lock();
    scws_res_t res,cur;
    Res.clear();
    scws_send_text(s, text, strlen(text));
    while (res = cur = scws_get_result(s))
    {
        while (cur!=NULL)
        {
            if((en&&cur->attr[0]=='e'&&cur->attr[1]=='n')||strchr(cur->attr,'n')!=NULL){
                char tmp[20];
                memcpy(tmp,text+cur->off,cur->len);
                //printf("%.*s/%s\n",cur->len, text+cur->off, cur->attr);
                //printf("%c\n",cur->attr[0]);
                tmp[cur->len]=0;
                string t=tmp;
                Res.push_back(t);
            }
            cur = cur->next;
        }
    }
    lockRes.unlock();
    scws_free_result(res);
    return Res;
}
vector<string> Participle::getRes(string t,bool en=true){
    lockRes.lock();
    char *text;
    int len = t.size();
    text = (char *)malloc((len)*sizeof(char));
    text=strncpy(text,t.data(),len);
    scws_res_t res,cur;
    Res.clear();
    scws_send_text(s, text, strlen(text));
    while (res = cur = scws_get_result(s))
    {
        while (cur!=NULL)
        {
            if((en&&cur->attr[0]=='e'&&cur->attr[1]=='n')||strchr(cur->attr,'n')!=NULL){
                char tmp[20];
                memcpy(tmp,text+cur->off,cur->len);
                //printf("%.*s/%s\n",cur->len, text+cur->off, cur->attr);
                //printf("%c\n",cur->attr[0]);
                tmp[cur->len]=0;
                string t=tmp;
                Res.push_back(t);
            }
            cur = cur->next;
        }
    }
    scws_free_result(res);
    delete [] text;
    lockRes.unlock();
    return Res;
}
vector<string> Participle::getTopRes(char text[],bool en=true){
    lockTopRes.lock();
    scws_top_t res,cur;
    topRes.clear();
    scws_send_text(s, text, strlen(text));
    res = cur =scws_get_tops(s,10,NULL);
    while(cur!=NULL&&((en&&cur->attr[0]=='e'&&cur->attr[1]=='n')||(strchr(cur->attr,'n')!=NULL)))
    //while(cur!=NULL)
    {
        string t=cur->word;
        topRes.push_back(t);
        cur = cur->next;
    }
    lockTopRes.unlock();//加锁防止冲突
    scws_free_tops(res);
    return topRes;
}
vector<string> Participle::getTopRes(string t,bool en=true){
    lockTopRes.lock();
    char *text;
    int len = t.size();
    text = (char *)malloc((len)*sizeof(char));
    text=strncpy(text,t.data(),len);

    scws_top_t res,cur;
    topRes.clear();
    scws_send_text(s, text, strlen(text));
    res = cur =scws_get_tops(s,10,NULL);
    while(cur!=NULL&&((en&&cur->attr[0]=='e'&&cur->attr[1]=='n')||(strchr(cur->attr,'n')!=NULL)))
    //while(cur!=NULL)
    {
        string t=cur->word;
        topRes.push_back(t);
        cur = cur->next;
    }
    scws_free_tops(res);
    delete [] text;
    lockTopRes.unlock();//加锁防止冲突
    return topRes;
}
Participle* Participle::instance_=NULL;
