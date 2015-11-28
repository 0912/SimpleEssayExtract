#include <cstdio>
#include <scws/scws.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <string>
#include "locker.h"

#include "Participle.h"
using namespace std;
int main()
{
    char text[100] = "hello javascript英语爱你dom,我是你的老公hello dom";
    Participle* instance=Participle::GetInstance();
    auto s=instance->getRes(text);
    for (auto it=s.begin();it!=s.end();it++) {
        cout<<*it<<endl;
    }
    auto s2=instance->getTopRes(text);
    printf("\n");
    for (auto it=s2.begin();it!=s2.end();it++) {
        cout<<*it<<endl;
    }
    return 0;
}
