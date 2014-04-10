#pragma once
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using cv::Mat;

int extractShapeFeature(string filename, float * feature);
void getHistogram(char* image,int widthStep, int top, int buttom, int left, int right, float * feature);