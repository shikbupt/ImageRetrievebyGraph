#ifndef GETLBPFEATURE_H
#define GETLBPFEATURE_H

#include <opencv2/opencv.hpp>

#define MAXLINE 64  //图片大小
#define NUM_G 288  //特征维度 是否是288？

bool GetLBPFeature(const cv::Mat& gray_img_64_64, float feature_vector[NUM_G]);

#endif




