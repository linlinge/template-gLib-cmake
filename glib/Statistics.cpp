#include "Statistics.h"
Statistics::Statistics(vector<float> dat)
{
	dat_=dat;		
	float sum=0;
	float min=INT_MAX;
	float max=-INT_MAX;
	
	// statistics
	for(int i=0;i<dat.size();i++)
	{
		sum+=dat[i];
		min=min<dat[i] ? min:dat[i];
		max=max>dat[i] ? max:dat[i];
	}
	sum_=sum;
	min_=min;
	max_=max;
	mean_=sum_/dat.size();
	
	sum=0;
	for(int i=0;i<dat.size();i++)
	{
		sum+=pow(dat[i]-mean_,2);				
	}
	stdevp_=sqrt(sum/dat.size());
	stdev_=sqrt(sum/(dat.size()-1));
}
double Quantile(vector<int>& dat,double ratio)
{
	vector<int> dat2;
	for(int i=0;i<dat.size();i++) dat2.push_back(dat[i]);
	sort(dat2.begin(),dat2.end());

	double Q_idx=(dat.size()+1)*ratio-1;
    int Q_idx_integer=(int)Q_idx;
    double Q_idx_decimal=Q_idx-Q_idx_integer;
    double Q=dat2[Q_idx_integer]+(dat2[Q_idx_integer+1]-dat2[Q_idx_integer])*Q_idx_decimal;    
    return Q;
}
