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
typedef pcl::PointXYZRGBA PointType;

int main(int argc,char** argv)
{
	pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
	Eigen::Vector4f centroid;
	
	if (pcl::io::loadPLYFile<PointType>(argv[1], *cloud) == -1) 
	{
		PCL_ERROR("Couldn't read file test_pcd.pcd \n");
		return (-1);
	}
	
    pcl::compute3DCentroid(*cloud, centroid);
	
	
	
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"));
	
	
	for(int i=0;i<cloud->points.size();i++){
		PointType ptmp=cloud->points[i];
		V3 direction(ptmp.x-centroid[0],ptmp.y-centroid[1],ptmp.z-centroid[2]);
		direction=direction/direction.Length()*0.005;
		
		 viewer->addArrow<pcl::PointXYZ>(pcl::PointXYZ(ptmp.x+direction.x,ptmp.y+direction.y,ptmp.z+direction.z), pcl::PointXYZ(ptmp.x,ptmp.y,ptmp.z), 1.0f, 0.0f, 0.0f, false, std::to_string(i), 0);
		//viewer->addArrow<pcl::PointXYZ>(pcl::PointXYZ(1,1,1), pcl::PointXYZ(0,0,0), 1.0f, 0.0f, 0.0f, false, std::to_string(i), 0);
	}
	
	  
	
	// Set background
	// viewer->setBackgroundColor (0.33, 0.97, 0.59); 
	viewer->setBackgroundColor (1.0f, 1.0f, 1.0f);

	//Set multi-color for point cloud
	pcl::visualization::PointCloudColorHandlerRGBField<PointType> multi_color(cloud);  	
	
	//Add the demostration point cloud data
	viewer->addPointCloud<PointType> (cloud, multi_color, "cloud1");

	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "cloud1");

	while(!viewer->wasStopped()){	
		viewer->spin();
		boost::this_thread::sleep (boost::posix_time::microseconds (10));
	}
	return 0;
}