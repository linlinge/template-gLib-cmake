#include "StringExtend.h"
void Str2Vec(string str, string delimiters, vector<int>& dat)
{
   dat.clear();
   vector<string> tokens;
   string::size_type lastPos=str.find_first_not_of(delimiters,0);
   string::size_type pos=str.find_first_of(delimiters,lastPos);
   while(string::npos!=pos || string::npos !=lastPos){
       tokens.push_back(str.substr(lastPos,pos-lastPos));
        lastPos=str.find_first_not_of(delimiters,pos);
        pos=str.find_first_of(delimiters,lastPos);
   }

    for(int i=0;i<tokens.size();i++){
        dat.push_back(stoi(tokens[i]));
    }
}

void StrSplit(string str, string delimiters, vector<string>& tokens)
{
   string::size_type lastPos=str.find_first_not_of(delimiters,0);
   string::size_type pos=str.find_first_of(delimiters,lastPos);
   while(string::npos!=pos || string::npos !=lastPos){
       tokens.push_back(str.substr(lastPos,pos-lastPos));
        lastPos=str.find_first_not_of(delimiters,pos);
        pos=str.find_first_of(delimiters,lastPos);
   }
}

void StrPrint(vector<int>& dat)
{
    for(int i=0;i<dat.size();i++)
        cout<<dat[i]<<" ";
    cout<<endl;
}

int get_num_in_string(string str)
{
	int rst=0;
	for(int i=0;i<str.size();i++){		
		if(str[i]>='0' && str[i]<='9'){
			rst=rst*10+(str[i]-'0');
		}
	}
	return rst;
}
