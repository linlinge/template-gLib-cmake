#pragma once
#include <iostream>
#include <pcl/point_types.h>
#include <pcl/io/ply_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/octree/octree.h>
#include <pcl/point_cloud.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/segmentation/conditional_euclidean_clustering.h>
#include <omp.h>
#include <vector>
#ifndef PointType
#define PointType pcl::PointXYZRGBNormal
#endif
using namespace std;
double ComputeMeanDistance(const pcl::PointCloud<PointType>::ConstPtr cloud);
double ComputeMaxDistance(const pcl::PointCloud<PointType>::ConstPtr cloud);
void TransformPointCloud(pcl::PointCloud<PointType>::Ptr cloud, pcl::PointCloud<PointType>::Ptr cloud_tf,Eigen::Affine3f tf);
vector<double> StatisticNearestDistance(const pcl::PointCloud<PointType>::ConstPtr cloud);
double GetCellSize(pcl::PointCloud<PointType>::Ptr cloud, int level);
double GetBoxMin(pcl::PointCloud<PointType>::Ptr cloud);

/* Algorithm Section */
pcl::PointCloud<PointType>::Ptr LoOP(pcl::PointCloud<PointType>::Ptr cloud_in, int K=45, double thresh=0.8);
void RecoverColor(pcl::PointCloud<PointType>::Ptr cloud_without_color,pcl::PointCloud<PointType>::Ptr cloud_with_color);