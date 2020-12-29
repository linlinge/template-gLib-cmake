#include <iostream>	
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <Eigen/Dense>
#include "V3.hpp"
#include <vector>
using namespace std;
typedef pcl::PointXYZRGBA PointType;

int main(int argc,char** argv)
{
	pcl::PointCloud<PointType>::Ptr cloud_without_color(new pcl::PointCloud<PointType>);
	pcl::PointCloud<PointType>::Ptr cloud_with_color(new pcl::PointCloud<PointType>);		
	pcl::io::loadPLYFile<PointType>(argv[1], *cloud_without_color); 
	pcl::io::loadPLYFile<PointType>(argv[2], *cloud_with_color); 
	pcl::search::KdTree<PointType>::Ptr kdtree(new pcl::search::KdTree<PointType>);
	kdtree->setInputCloud(cloud_with_color);

	for(int i=0;i<cloud_without_color->points.size();i++){
		vector<int> idx;
		vector<float> dist;
		kdtree->nearestKSearch(cloud_without_color->points[i],1,idx,dist);

		cloud_with_color->points[i].r=cloud_without_color->points[idx[0]].r;
		cloud_with_color->points[i].g=cloud_without_color->points[idx[0]].g;
		cloud_with_color->points[i].b=cloud_without_color->points[idx[0]].b;
	}	
    pcl::io::savePCDFileBinary("1.ply",*cloud_with_color);		
	return 0;
}