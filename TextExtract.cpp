/*************************************************************************
	> File Name: TextExtract
	> Author: ysg
	> Mail: ysgcreate@gmail.com
	> Created Time: 2015年11月5日 星期四 22时59分58秒
    > 基于行块分布函数的网页正文提取方法的实现
 ************************************************************************/
#include <pcre++.h>
#include <cstring>
#include <string>
#include <vector>
using namespace pcrepp;
class TextExtract {
public:
    TextExtract(string r,int blank=3,int t=106):blankSize(blank),threshold(t){
        this->rawString=r;
    };
    void setthreshold(int value){
        this->threshold=value;
    }
    string parse();
    double enRate(){//获取英文占文章总比率
        int len=res.length();
        int enlen=0;
        for(int i=0;i<len;i++){
            if ((res[i]>=65&&res[i]<=90)||(res[i]>=97&&res[i]<=122)) {
                enlen++;
            }
        }
        return enlen/len;
    }
private:
    string preProcess();
    void split(string preString);
private:
    string rawString;
    string res;
    int sizeLines[10000];
    vector<string> vString;
    int blankSize;
    int maxline;//result line number
    int threshold;
};
string TextExtract::preProcess(){//预处理除杂
    string s=this->rawString;
    //remove script
    Pcre p_script("<script[^>].*>[\\d\\D]*?</script>","gi");
    s = p_script.replace(s, "");
    //remove css
    Pcre p_style("<style.*?>[\\d\\D]*?</style>","ig");
    s = p_style.replace(s, "");
    //remove doctype
    Pcre p_doctype("<!DOCTYPE.*?>","sig");
    s = p_doctype.replace(s, "");
    //remove comment
    Pcre p_comment("<!--[\\d\\D]*?-->","g");
    s = p_comment.replace(s, "");
    //remove html
    Pcre p_html("<.*?>","g");
    s = p_html.replace(s, "");
    return s;
}
void TextExtract::split(string preString) {//进入vector,并计算每行words,每行取下三行字数
    int pos=0;
    int cur=0;
    Pcre p("\\s+");
    while((cur=preString.find('\n',pos))!=string::npos){
        if (cur-pos-1>1) {
            vString.push_back(p.replace((preString.substr(pos,cur-pos)),""));
        }
        pos=cur+1;
    }
    maxline=vString.size();
    for(int i=0;i<maxline-blankSize;i++){
        int wordsNum=0;
        for(int j=i;j<i+blankSize;j++){
            wordsNum+=vString[j].length();
        }
        sizeLines[i]=wordsNum;
    }
}
string TextExtract::parse(){//解析
    string preString=this->preProcess();
    this->split(preString);
    int starts = -1;
    int ends= -1;
    int flagLine[1000];
    int maxFlagLine=0;
    int curFlagLine=0;
    bool boolstart = false, boolend = false;
    for (int i = 0; i < maxline-blankSize; i++) {
        if (sizeLines[i] > threshold && ! boolstart) {
            if (sizeLines[i+1] != 0
                || sizeLines[i+2] != 0
                || sizeLines[i+3] != 0) {
                boolstart = true;
                starts = i;
                continue;
            }
        }
        if (boolstart) {
            if (sizeLines[i] == 0
                || sizeLines[i+1] == 0) {
                ends = i;
                boolend = true;
            }
        }
        if (boolend) {
            for (int ii = starts; ii <= ends; ii++) {
                if (sizeLines[ii] < 5){ continue;}
                flagLine[maxFlagLine++]=ii;
            }
            for (; curFlagLine < maxFlagLine; curFlagLine++) {
                res.append(vString.at(flagLine[curFlagLine]));
                res.push_back('\n');
            }
            boolstart = boolend = false;
        }
    }
    return res;
}
