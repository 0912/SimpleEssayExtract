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
#include "BM25.h"
using namespace std;
#define MAXWORDS 10000
struct CmpByValue {
  bool operator()(const pair<string,double> & lhs, const pair<string,double> & rhs){
      return lhs.second > rhs.second;
  }
};
class TextSummary {
private:
    vector<vector<double>> weight;
    vector<vector<string>> docs;
    vector<string> sentences;
    vector<pair<string,double>> vecs;//结果
    vector<double> vertex;
    vector<double> weight_sum;
    int max_iter=200;//最大迭代200次求rank
    double min_diff=0.001;//精度
    double d=0.85;//阻尼系数
    int D;
public:
    TextSummary (vector<vector<string>>,vector<string>,vector<vector<double>>);
    ~TextSummary(){};
    vector<pair<string,double>> getTopSentence();
    double sumall(vector<double>);
};
TextSummary::TextSummary(vector<vector<string>> docs,vector<string> s,vector<vector<double>> w)
{
    this->docs = docs;
    this->sentences = s;
    this->D = docs.size();
    this->weight=w;
}
vector<pair<string,double>> TextSummary::getTopSentence()
{
    int cnt = 0;
    for (;cnt<D;cnt++)
    {
        weight_sum.push_back(sumall(weight[cnt]) - weight[cnt][cnt]); // 减掉自己，自己跟自己肯定最相似
        vertex.push_back(1.0);
    }
    for (int _ = 0; _ < max_iter; ++_)
    {
        vector<double> m;
        double max_diff = 0;
        for (int i = 0; i < D; ++i)
        {
            m.push_back(1 - d);
            for (int j = 0; j < D; ++j)
            {
                if (j == i || weight_sum[j] == 0) continue;
                m[i] += (d * weight[j][i] / weight_sum[j] * vertex[j]);
            }
            double diff = abs(m[i] - vertex[i]);
            if (diff > max_diff)
            {
                max_diff = diff;
            }
        }
        vertex = m;
        if (max_diff <= min_diff) break;
    }
    for (int i=0;i<vertex.size();i++) {
        vecs.push_back(make_pair(sentences[i],vertex[i]));
    }
    sort(vecs.begin(),vecs.end(),CmpByValue());
    for (auto it=vecs.begin();it!=vecs.end();it++) {
        cout<<it->first<<":"<<it->second<<endl;
    }
    return vecs;
}
double TextSummary::sumall(vector<double> arr)
{
    double total = 0;
    for (double v : arr)
    {
        total += v;
    }
    return total;
}
int main()
{
    string ss="北京时间11月26日，ESPN记者DaveMcMenamin报道，多处消息源称，客场99-103不敌猛龙之后，骑士召开了一场只有球员参加的内部会议。勒布朗-詹姆斯和詹姆斯-琼斯提到了球队本赛季不稳定的表现。今天输给猛龙是骑士本赛季第4场败仗，而且4场失利均是在客场，这让球队的话语权领袖们质疑球队的韧性。第三节比赛还剩3分46秒至第四节比赛还剩2分01秒期间，骑士被对手打了一波16-31的比分，被对手完成逆转。更糟糕的是骑士的防守，他们第四节送给对手3次加罚机会（下半场6次），赔上犯规都防不住对方进球。如果要犯规，我们需要让它们奏效，”骑士内线特里斯坦-汤普森说，我不是说要伤害任何人，但是你不能给人送加罚。这是不能接受的。这是我们内线的事情，这是我、K-Love（凯文-乐福）和安迪（安德森-瓦莱乔）的事情。当我们打得软，这是我们（内线）的问题。骑士第四节的进攻同样糟糕，全队18投5中，直到最后35.2秒无关紧要的时间，JR-史密斯才连续命中了2记三分球。主帅大卫-布拉特赛后没有追究球队的任何错误，他提到球队缺少了4名关键轮换球员，凯里-欧文、伊曼-香珀特、季莫费-莫兹戈夫和马修-戴拉维多瓦。我认为我们累了，因为很明显的原因，非常人手短缺，布拉特说，我认为我们的球队打得很努力，我认为球队精力耗尽了。我认为疲劳是很大一部分原因，真的。然而，詹姆斯却拒绝主帅相对更加温和的立场，他在接受采访时表示疲劳和伤病都不是接口。今天输球之后，骑士战绩为11胜4负，仍排在东部榜首，猛龙10胜6负位居东部第二，东部目前有6支球队胜率达到了五成以上。随着东部整体实力的增强，詹姆斯和琼斯试图给球队注入紧迫感，打得更加努力，他们之前就在中场休息期间发表过说话。这是我们整个赛季表现的缩影，”琼斯告诉ESPN，“我们不稳定，我们没有打出球队需要的身体强队和紧迫感。对我们来说，整个赛季就是用来变得更好，琼斯说，现在还太早去想自己做到了什么，或者达到了无法继续提高的层次。所以，我们球队现在是牢靠的，但必须变得更好。我们不是一支追求平庸的球队，仅仅是牢靠还不够好。尽管詹姆斯可能不同意主帅给球员放行的言论，但他后来表现球队被教得很好。琼斯也指出，目前困扰球队的问题是在球员身上。专注和心态，我们偶尔展示了出来，但这是我们需要持续表现的东西，琼斯说，教练、赛程，其他事情都无关。这是我们球员的决定，每天都需要去这么做。为了提高球队紧迫感，更好进入状态，骑士之前就取消了赛前的亮相仪式，并比对手提前来到球场等待跳球。这需要从跳球延续到离开球馆大门，特别是对我来说，汤普森说，当我们打得软弱，我把责任担在自己身上。这是我的工作，这是我在这里的原因。打出身体对抗，让对手变得困难，抢篮板，设置扎实的掩护，成为油漆区的一股力量。所以，这完全在我，我需要在内线打得更有对抗，保护篮筐。我们需要更好地照顾彼此，我们必须打得更好，”詹姆斯说，“而且我们会的。但我不认为球队今晚有提高。";
    BM25* p=new BM25(ss);
    auto w=p->simAll();
    auto docs=p->getDoc();
    auto sen=p->getSentences();
    TextSummary* ptr=new TextSummary(docs,sen,w);
    ptr->getTopSentence();
    return 0;
}
