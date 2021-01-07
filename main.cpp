#include <iostream>	
#include "PCLExtend.h"
#include <Eigen/Dense>
#include "V3.hpp"
#include <vector>
using namespace std;

int main(int argc,char** argv)
{
	pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);		
	pcl::PointCloud<PointType>::Ptr cloud_out(new pcl::PointCloud<PointType>);
	pcl::io::loadPLYFile<PointType>(argv[1], *cloud); 
	vector<int> idx=LoOP(cloud);
	subtract_points(cloud,idx);
	
    pcl::io::savePLYFileBinary(argv[2],*cloud);		
	return 0;
}