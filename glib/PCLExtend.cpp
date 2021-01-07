#include "PCLExtend.h"
#define MAX2(A,B) ((A)>(B) ? (A):(B))
#define MAX3(A,B,C) MAX2(MAX2(A,B),C)
#define MIN2(A,B) ((A)<(B) ? (A):(B))
#define MIN3(A,B,C) MIN2(MIN2(A,B),C)
double ComputeMeanDistance(const pcl::PointCloud<PointType>::ConstPtr cloud)
{
	double res = 0.0;
	int n_points = 0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	pcl::search::KdTree<PointType> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i)
	{
		if (!std::isfinite((*cloud)[i].x))
		{
			continue;
		}
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2)
		{
			res += sqrt(sqr_distances[1]);
			++n_points;
		}
	}
	if (n_points != 0)
	{
		res /= n_points;
	}
	return res;
}


double ComputeMaxDistance(const pcl::PointCloud<PointType>::ConstPtr cloud)
{
	double rst = 0.0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	pcl::search::KdTree<PointType> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i)
	{
		if (!std::isfinite((*cloud)[i].x))
		{
			continue;
		}
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2)
		{
			double dist_tmp=sqrt(sqr_distances[1]);
			rst = rst>dist_tmp ? rst:dist_tmp;
		}
	}

	return rst;
}

vector<double> StatisticNearestDistance(const pcl::PointCloud<PointType>::ConstPtr cloud)
{
	int n_points = 0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	vector<double> rst;
	pcl::search::KdTree<PointType> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i)
	{ 
		if (!std::isfinite((*cloud)[i].x))
		{
			continue;
		}
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2)
		{			
			rst.push_back(sqr_distances[1]);
			++n_points;
		}
	}	
	
	return rst;
}

void TransformPointCloud(pcl::PointCloud<PointType>::Ptr cloud, pcl::PointCloud<PointType>::Ptr cloud_tf,Eigen::Affine3f tf)
{
	cloud_tf->points.resize(cloud->points.size());
	
	#pragma omp parallel for
	for(int i=0;i<cloud->points.size();i++)
	{
		Eigen::Vector3f v1(cloud->points[i].x,cloud->points[i].y,cloud->points[i].z);		
		Eigen::Vector3f v2=tf*v1;
		cloud_tf->points[i].x=v2(0,0);
		cloud_tf->points[i].y=v2(1,0); 
		cloud_tf->points[i].z=v2(2,0);
	}
}

double GetCellSize(pcl::PointCloud<PointType>::Ptr cloud, int level)
{
	PointType pmin,pmax;
	pcl::getMinMax3D(*cloud,pmin,pmax);
	double cellsize=MAX3(abs(pmax.x-pmin.x),abs(pmax.y-pmin.y),abs(pmax.z-pmax.z));
	return cellsize/pow(2,level);
}
double GetBoxMin(pcl::PointCloud<PointType>::Ptr cloud)
{
	PointType pmin,pmax;
    pcl::getMinMax3D(*cloud,pmin,pmax);
	double vmin=MIN3(abs(pmax.x-pmin.x),abs(pmax.y-pmin.y),abs(pmax.z-pmin.z));
	return vmin;
}

pcl::PointCloud<PointType>::Ptr LoOP(pcl::PointCloud<PointType>::Ptr cloud_in, int K, double thresh)
{
	pcl::PointCloud<PointType>::Ptr cloud_out(new pcl::PointCloud<PointType>);
	pcl::search::KdTree<PointType>::Ptr kdtree(new pcl::search::KdTree<PointType>);
	kdtree->setInputCloud(cloud_in);
	vector<double> sigma;
	vector<double> plof;
	vector<double> rst_LoOP_;
    double nplof=0;
    // Resize Scores	
	sigma.resize(cloud_in->points.size());
	plof.resize(cloud_in->points.size());
	rst_LoOP_.resize(cloud_in->points.size());

	// Step 01: Calculate sigma
	#pragma omp parallel for
	for(int i=0;i<cloud_in->points.size();i++){
		// find k-nearest neighours
		vector<int> idx(K+1);
		vector<float> dist(K+1);
		kdtree->nearestKSearch (i, K+1, idx, dist);
		// cout<<cloud->points[i]<<endl;
		double sum=0;
		for(int j=1;j<K+1;j++){
			sum+=dist[j];
		}
		sum=sum/K;
		sigma[i]=sqrt(sum);
	}
	
	// Step 02: calculate mean
	double mean=0;
	#pragma omp parallel for
	for (int i = 0; i < cloud_in->points.size(); i++){        
        vector<int> idx(K+1);
		vector<float> dist(K+1);
		kdtree->nearestKSearch (cloud_in->points[i], K+1, idx, dist);
        double sum = 0;
        for (int j = 1; j < K+1; j++)
          sum += sigma[idx[j]];
        sum /= K;
        plof[i] = sigma[i] / sum  - 1.0f;				
        mean += plof[i] * plof[i];
    }
	nplof=sqrt(mean/cloud_in->points.size());	

	// Step 03: caculate score
	#pragma omp parallel for
	for(int i=0;i<cloud_in->points.size();i++){
		double value = plof[i] / (nplof * sqrt(2.0f));
		// rst_.records_[i].item1_=value;

        double dem = 1.0 + 0.278393 * value;
        dem += 0.230389 * value * value;
        dem += 0.000972 * value * value * value;
        dem += 0.078108 * value * value * value * value;
        double op = std::max(0.0, 1.0 - 1.0 / dem);
        rst_LoOP_[i] = op;
	}

	#pragma omp parallel for
    for(int i=0;i<rst_LoOP_.size();i++){
        if(rst_LoOP_[i]<thresh){
			cloud_out->points.push_back(cloud_in->points[i]);
		}                
	}

	// pcl::io::savePLYFileBinary(path_out,*cloud_out);
	return cloud_out;
}

void RecoverColor(pcl::PointCloud<PointType>::Ptr cloud_without_color,pcl::PointCloud<PointType>::Ptr cloud_with_color)
{
	pcl::search::KdTree<PointType>::Ptr kdtree(new pcl::search::KdTree<PointType>);
	kdtree->setInputCloud(cloud_with_color);

	for(int i=0;i<cloud_without_color->points.size();i++){
		vector<int> idx;
		vector<float> dist;
		kdtree->nearestKSearch(cloud_without_color->points[i],1,idx,dist);
		cloud_without_color->points[i].r=cloud_with_color->points[idx[0]].r;
		cloud_without_color->points[i].g=cloud_with_color->points[idx[0]].g;
		cloud_without_color->points[i].b=cloud_with_color->points[idx[0]].b;

		cloud_without_color->points[i].normal_x=cloud_with_color->points[idx[0]].normal_x;
		cloud_without_color->points[i].normal_y=cloud_with_color->points[idx[0]].normal_y;
		cloud_without_color->points[i].normal_z=cloud_with_color->points[idx[0]].normal_z;
	}
}