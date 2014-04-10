#include "feature.h"
#include "CEDD/FeatureExtract.h"
#include "LBP/getLBPfeature.h"
#include <math.h>

using namespace cv;

bool LBP::detect(const Mat& img, Mat& features) {
	if(!Feature::detect(img, features)) return -1;
	Mat gray_img_64_64 = Mat(64, 64, img.type());
	resize(img, gray_img_64_64, gray_img_64_64.size(), 0, 0, cv::INTER_LINEAR);
	float feature_vector[NUM_G];
	bool result = GetLBPFeature(gray_img_64_64, feature_vector);
	features.release();
	features = Mat(1, sizeof(feature_vector)/sizeof(float), CV_32F, feature_vector).clone();
	return result;
}

bool ::SIFT::detect(const Mat& img, Mat& features) {
	if(!Feature::detect(img, features)) return -1;
	keypoint_.clear();
	features.release();
	detector_->detect(img, keypoint_);		
	descriptorExtractor_->compute(img, keypoint_, features);
	return 1;

}

bool CEDD::detect(const Mat& img, Mat& features) {
	if(!Feature::detect(img, features)) return -1;
	float CEDDfeature[144] = {0.0};
	imwrite(".\\temp.jpg", img);
	bool result = extractShapeFeature(".\\temp.jpg", CEDDfeature);
	features = Mat(1, sizeof(CEDDfeature)/sizeof(float), CV_32F, CEDDfeature).clone();
	system("del .\\temp.jpg");
	return result;
}

float Euclidean::compute(const Mat& feature1, const Mat& feature2) {
	if (feature1.cols == feature2.cols &&
		feature1.rows == feature2.rows && feature1.rows == 1) {
		float sum = 0;
		for (int j = 0; j<feature1.rows; ++j) {
			const float* data1 = feature1.ptr<float>(j);
			const float* data2 = feature2.ptr<float>(j);
			for (int i = 0; i<feature1.cols; ++i) {
				sum += pow(data1[i]-data2[i], 2);
			}
		}
		return std::sqrt(sum);
	}
	return -1;
}