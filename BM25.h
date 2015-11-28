#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <cmath>
#include "Participle.h"
#define MAXNUM 10000
using namespace std;
class BM25 {
private:
    float k1 = 1.5f;
    float b = 0.75f;
    int avgdl=0;
    vector<string> sentences;
    vector<vector<string> > docs;
    vector<map<string,int> > f;
    map<string,double> df;
    map<string,double> idf;
    vector<vector<double> > scores;
    int D=0;
    Participle* instance;
public:
    BM25 (string raw);
    ~BM25 ();
    vector<vector<string>> getDoc();
    vector<string> getSentences();
    double sim(vector<string> sentence,int order);
    vector<vector<double>> simAll();
};
BM25::BM25(string raw){
    instance=Participle::GetInstance();
    int cur,pos;
    cur=pos=0;
    char flagzh[6][4]={"。","，","：","？","！","；"};
    char flagen[6][1]={'.',',',':','?','!',';'};
    for(;cur<raw.size();cur++){
        for (size_t i = 0; i < 6; i++) {
            if (raw[cur]==flagzh[i][0]&&raw[cur+1]==flagzh[i][1]&&cur-pos>2) {
                sentences.push_back(raw.substr(pos,cur-pos));
                pos=cur+3;
                break;
            }else if((raw[cur]==flagen[i][0]||raw[cur]=='\n')&&cur-pos>2){
                sentences.push_back(raw.substr(pos,cur-pos));
                //cout<<raw.substr(pos,cur-pos)<<endl;
                pos=cur+1;
                break;
            }
        }
    }
    for (D=0;D<sentences.size();D++) {
        docs.push_back(instance->getRes(sentences[D]));
        avgdl+=docs[D].size();
        // for (auto its=docs[D].begin();its!=docs[D].end();its++) {
        //     cout<<*its<<" ";
        // }
        // cout<<endl;
    }
    avgdl/=D;
    for (auto sentence : docs)
    {
        map<string,int> tf;
        for (string word : sentence)
        {
            if (tf.find(word)!=tf.end()) {
                tf[word]+=1;
            }else{
                tf[word]=1;
            }
        }
        f.push_back(tf);
        for (auto entry : tf)
        {
            if (df.find(entry.first)!=df.end()) {
                df[entry.first]+=1;
            }else{
                df[entry.first]=1;
            }
        }
    }
    for (auto entry : df)
    {
        string word = entry.first;
        int freq = entry.second;
        idf[word] = log(D - freq + 0.5) - log(freq + 0.5);
        //cout<<word<<" "<<idf[word]<<endl;
    }
}
vector<vector<string>> BM25::getDoc(){
    return docs;
}
vector<string> BM25::getSentences(){
    return sentences;
}
double BM25::sim(vector<string> sentence,int order)
{
    double score = 0;
    for (string word : sentence)
    {
        if (f[order].find(word)==f[order].end()){
            continue;
        }
        int d = docs[order].size();
        int wf = f[order][word];
        score += (idf[word] * wf * (k1 + 1)/(wf + k1 * (1 - b + b * d/avgdl)));
    }
    return score;
}
vector<vector<double>> BM25::simAll()
{
    for (int i = 0; i < docs.size(); i++) {
        vector<double> tmp;
        for (int j = 0; j < D; j++)
        {
            tmp.push_back(sim(docs[i],j));
        }
        scores.push_back(tmp);
    }
    return scores;
}
