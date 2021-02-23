#include "VectorExtend.h"
double VectorMean(vector<double>& dat)
{
    double rst=0;
    for(int i=0;i<dat.size();i++)
        rst+=dat[i];
    
    return rst/dat.size();
}

void VectorNormalize(vector<double>& dat)
{
    double dat_min=VectorMinimum(dat);
    double dat_max=VectorMaximum(dat);
    #pragma omp parrallel for
    for(int i=0;i<dat.size();i++){
        dat[i]=(dat[i]-dat_min)/(dat_max-dat_min);
    }    
}

double VectorMinimum(vector<double>& dat)
{
    auto vmin=std::min_element(dat.begin(),dat.end());
    return *vmin;
}
double VectorMaximum(vector<double>& dat)
{
    auto vmax=std::max_element(dat.begin(),dat.end());
    return *vmax;
}
double VectorMaximum(vector<float>& dat)
{
    auto vmax=std::max_element(dat.begin(),dat.end());
    return *vmax;
}
double VectorStd(vector<double>& dat)
{
    double rst=0;
    double dat_mean=VectorMean(dat);
    for(int i=0;i<dat.size();i++){
        rst+=(dat[i]-dat_mean)*(dat[i]-dat_mean);
    }
    return rst/(dat.size()-1);
}

double VectorQuantile(vector<double>& dat,double p)
{
    sort(dat.begin(),dat.end());
    double Q_idx=1+(dat.size()-1)*p;
    int Q_idx_integer=(int)Q_idx;
    double Q_idx_decimal=Q_idx-Q_idx_integer;
    double Q=dat[Q_idx_integer-1]+(dat[Q_idx_integer]-dat[Q_idx_integer-1])*Q_idx_decimal;    
    return Q;
}

int VectorIQR(vector<double>& dat)
{
    double IQR=VectorQuantile(dat,0.75)-VectorQuantile(dat,0.25);
    double thresh=VectorQuantile(dat,0.75)+10*IQR;
    for(int i=0;i<dat.size();i++)
    {
        if(dat[i]>thresh){
            return 1;
        }
    }
    return 0;
}

double VectorSum(vector<double>& dat)
{
    double sum=0;
    for(int i=0;i<dat.size();i++)
    {
        sum+=dat[i];
    }
    return sum;
}
void VectorDelete(vector<int>&rdat,int index)
{
    rdat[index]=rdat[rdat.size()-1];
    rdat.pop_back();
}
void VectorDelete(vector<int>&raw_dat, vector<int>& delete_dat)
{
    if(delete_dat.size()!=0){
        vector<int> target;
        sort(raw_dat.begin(),raw_dat.end());
        sort(delete_dat.begin(),delete_dat.end());
        set_difference(raw_dat.begin(),raw_dat.end(),delete_dat.begin(),delete_dat.end(),std::back_inserter(target));
        raw_dat.clear();
        for(int i=0;i<target.size();i++) raw_dat.push_back(target[i]);
    }
}
void VectorDifference(vector<int>& dat1,vector<int>& dat2,vector<int>& out)
{
    set<int> st1(dat1.begin(),dat1.end());
    set<int> st2(dat2.begin(),dat2.end());    
    set<int> rst;
    set_difference(st1.begin(),st1.end(),st2.begin(),st2.end(),std::back_inserter(out));
}
void VectorUnion(vector<int>& dat1,vector<int>& dat2,vector<int>& out)
{
    sort(dat1.begin(),dat1.end());
    sort(dat2.begin(),dat2.end());
    set_union(dat1.begin(),dat1.end(),dat2.begin(),dat2.end(),std::back_inserter(out));
}
void VectorIntersection(vector<int>& dat1,vector<int>& dat2,vector<int>& out)
{
    sort(dat1.begin(),dat1.end());
    sort(dat2.begin(),dat2.end());
    set_intersection(dat1.begin(),dat1.end(),dat2.begin(),dat2.end(),std::back_inserter(out));
}


int VecFindPos(vector<int>& buf,int dat)
{
    vector<int>::iterator it=find(buf.begin(),buf.end(),dat);
    if(it!=buf.end())
        return it-buf.begin();
    else
        return -1;
}

void VecFindPos(vector<int>& buf1,vector<int>& buf2, vector<int>& out)
{
    for(int i=0;i<buf2.size();i++){
        for(int j=0;j<buf1.size();j++){
            if(buf1[j]==buf2[i])
                out.push_back(j);
        }            
    }
}

void VecPrint(vector<int>& dat)
{
    for(int i=0;i<dat.size();i++)
        cout<<dat[i]<<" ";
    cout<<endl;
}

void VecUnique(vector<int>& dat)
{
    vector<int> v;
    v.insert(v.end(),dat.begin(),dat.end());
    sort(v.begin(),v.end());
    v.erase(unique(v.begin(),v.end()),v.end());
    // VecPrint(v);
}

void VectorWrite(string filename,vector<double>& dat,string mode)
{
    std::ofstream file;
    if (file.bad())
        std::cout << "cannot open file" << std::endl;
    if(mode=="append,row"){
        file.open(filename, ios::out | std::ios::app);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<",";
        file<<dat[dat.size()-1]<<endl;
    }
    else if(mode=="append,column"){
        file.open(filename, ios::out | std::ios::app);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<","<<endl;
        file<<dat[dat.size()-1]<<endl;
    }          
    else if(mode=="cover,row"){
        file.open(filename, ios::out);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<",";
        file<<dat[dat.size()-1]<<endl;
    }
    else if(mode=="cover,column"){
        file.open(filename, ios::out);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<","<<endl;
        file<<dat[dat.size()-1]<<endl;
    }
    else
    {
        cout<<"VectorWrite mode error!"<<endl;
    }
    file.close();
}

void VectorWrite(string filename,vector<float>& dat,string mode)
{
    std::ofstream file;
    if (file.bad())
        std::cout << "cannot open file" << std::endl;
    if(mode=="append,row"){
        file.open(filename, ios::out | std::ios::app);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<",";
        file<<dat[dat.size()-1]<<endl;
    }
    else if(mode=="append,column"){
        file.open(filename, ios::out | std::ios::app);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<","<<endl;
        file<<dat[dat.size()-1]<<endl;
    }          
    else if(mode=="cover,row"){
        file.open(filename, ios::out);
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<",";
        file<<dat[dat.size()-1]<<endl;
    }
    else if(mode=="cover,column"){
        for(int i=0;i<dat.size()-1;i++)
            file<<dat[i]<<","<<endl;
        file<<dat[dat.size()-1]<<endl;
    }
    else
    {
        cout<<"VectorWrite mode error!"<<endl;
    }
    file.close();
}

void VectorInterval(vector<double>& dat,vector<double>& out)
{
    out.resize(dat.size()-1);
    for(int i=1;i<dat.size();i++){
        out[i]=dat[i]-dat[i-1];
    }
}

void VectorInterval(vector<float>& dat,vector<double>& out)
{
    out.resize(dat.size());
    out[0]=0;
    for(int i=1;i<dat.size();i++){
        out[i]=dat[i]-dat[i-1];
    }
}
int VectorFindFirstNot(vector<int>& dictionary, vector<int>& dat)
{
    for(int i=0;i<dat.size();i++){
        vector<int>::iterator it=find(dictionary.begin(),dictionary.end(),dat[i]);        
        if(it==dictionary.end())
            return dat[i];
    }
    return -1;
}
