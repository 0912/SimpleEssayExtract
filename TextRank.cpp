#include <cstdio>
#include <scws/scws.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include "Participle.h"
using namespace std;
#define MAXWORDS 10000
struct CmpByValue {
  bool operator()(const pair<string,double> & lhs, const pair<string,double> & rhs){
      return lhs.second > rhs.second;
  }
};
class TextRank {
private:
    vector<string> text;//记录已分词,构造函数记录
    map<string,set<string> > m;//string记不重复分词,set记相邻5个不重复word
    int max_iter=200;//最大迭代200次求rank
    double diff=0.001;//精度
    double d=0.85;//阻尼系数
public:
    TextRank (vector<string> t):text(t){};
    vector<pair<string,double>> process(){
        int n=text.size();
        int cnt=0;
        for (int i = 0; i < n; i++) {
            set<string> tmpSet;
            string curString=text[i];
            if (m.find(curString)!=m.end()) {
                tmpSet=m[curString];
            }
            for (int j = 1; j < 5; j++) {
                if (i-j>=0) {
                    tmpSet.insert(text[i-j]);
                }
                if (i+j<=n-1) {
                    tmpSet.insert(text[i+j]);
                }
            }
            m[curString]=tmpSet;
        }
        map<string,double> score;
        for (auto it=m.begin();it!=m.end();it++) {
            score[it->first]=1;
        }
        for (size_t i = 0; i < max_iter; i++) {
            map<string,double> tmpScore;
            double maxdiff=0;
            for (auto it:m) {
                tmpScore[it.first]=1-d;
                set<string> tmpSet=it.second;
                for(auto itother:tmpSet){
                    int ss=m[itother].size();
                    if (it.first==itother||ss==0) {
                        continue;
                    }
                    tmpScore[it.first]=tmpScore[it.first]+d/ss*score[itother];
                }
                maxdiff = max(maxdiff, abs(tmpScore[it.first]-score[it.first]));
            }
            score=tmpScore;
            if (maxdiff<=diff) {
                break;
            }
        }
        vector<pair<string,double>> vecs;//结果
        for (auto it = score.begin();it !=score.end();it ++) {
            vecs.push_back(make_pair(it->first,it->second));
        }
        sort(vecs.begin(),vecs.end(),CmpByValue());
        return vecs;
    }
    ~TextRank (){};
};
int main()
{
    char text[10000] = "北京时间11月26日，ESPN记者DaveMcMenamin报道，多处消息源称，客场99-103不敌猛龙之后，骑士召开了一场只有球员参加的内部会议。勒布朗-詹姆斯和詹姆斯-琼斯提到了球队本赛季不稳定的表现。今天输给猛龙是骑士本赛季第4场败仗，而且4场失利均是在客场，这让球队的话语权领袖们质疑球队的韧性。第三节比赛还剩3分46秒至第四节比赛还剩2分01秒期间，骑士被对手打了一波16-31的比分，被对手完成逆转。更糟糕的是骑士的防守，他们第四节送给对手3次加罚机会（下半场6次），赔上犯规都防不住对方进球。如果要犯规，我们需要让它们奏效，”骑士内线特里斯坦-汤普森说，我不是说要伤害任何人，但是你不能给人送加罚。这是不能接受的。这是我们内线的事情，这是我、K-Love（凯文-乐福）和安迪（安德森-瓦莱乔）的事情。当我们打得软，这是我们（内线）的问题。骑士第四节的进攻同样糟糕，全队18投5中，直到最后35.2秒无关紧要的时间，JR-史密斯才连续命中了2记三分球。主帅大卫-布拉特赛后没有追究球队的任何错误，他提到球队缺少了4名关键轮换球员，凯里-欧文、伊曼-香珀特、季莫费-莫兹戈夫和马修-戴拉维多瓦。我认为我们累了，因为很明显的原因，非常人手短缺，布拉特说，我认为我们的球队打得很努力，我认为球队精力耗尽了。我认为疲劳是很大一部分原因，真的。然而，詹姆斯却拒绝主帅相对更加温和的立场，他在接受采访时表示疲劳和伤病都不是接口。今天输球之后，骑士战绩为11胜4负，仍排在东部榜首，猛龙10胜6负位居东部第二，东部目前有6支球队胜率达到了五成以上。随着东部整体实力的增强，詹姆斯和琼斯试图给球队注入紧迫感，打得更加努力，他们之前就在中场休息期间发表过说话。这是我们整个赛季表现的缩影，”琼斯告诉ESPN，“我们不稳定，我们没有打出球队需要的身体强队和紧迫感。对我们来说，整个赛季就是用来变得更好，琼斯说，现在还太早去想自己做到了什么，或者达到了无法继续提高的层次。所以，我们球队现在是牢靠的，但必须变得更好。我们不是一支追求平庸的球队，仅仅是牢靠还不够好。尽管詹姆斯可能不同意主帅给球员放行的言论，但他后来表现球队被教得很好。琼斯也指出，目前困扰球队的问题是在球员身上。专注和心态，我们偶尔展示了出来，但这是我们需要持续表现的东西，琼斯说，教练、赛程，其他事情都无关。这是我们球员的决定，每天都需要去这么做。为了提高球队紧迫感，更好进入状态，骑士之前就取消了赛前的亮相仪式，并比对手提前来到球场等待跳球。这需要从跳球延续到离开球馆大门，特别是对我来说，汤普森说，当我们打得软弱，我把责任担在自己身上。这是我的工作，这是我在这里的原因。打出身体对抗，让对手变得困难，抢篮板，设置扎实的掩护，成为油漆区的一股力量。所以，这完全在我，我需要在内线打得更有对抗，保护篮筐。我们需要更好地照顾彼此，我们必须打得更好，”詹姆斯说，“而且我们会的。但我不认为球队今晚有提高。";
    Participle* instance=Participle::GetInstance();
    auto s=instance->getRes(text);
    TextRank* t=new TextRank(s);
    auto its=t->process();
    int i=0;
    for (auto it=its.begin();it!=its.end();it++) {
        if (i++>10) {
            break;
        }
        cout<<it->first<<":"<<it->second<<endl;
    }
    return 0;
}
