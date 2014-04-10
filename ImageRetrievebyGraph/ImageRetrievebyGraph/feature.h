#ifndef FEATURE_H__
#define FEATURE_H__

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <vector>
using std::vector;
using cv::Ptr;
using cv::FeatureDetector;
using cv::DescriptorExtractor;
using cv::KeyPoint;
using cv::Mat;

//特征
class Feature {
public:
	virtual bool detect(const Mat& img, Mat& features) =0 {
		if (img.empty()) {
			return -1;
		}
		return 1;
	};
};


class LBP : public Feature {
public:
	virtual bool detect(const Mat& img, Mat& features);
};


class SIFT : public Feature {
public:
	SIFT() : detector_(new cv::SIFT), descriptorExtractor_(new cv::SIFT) {}

	virtual bool detect(const Mat& img, Mat& features);

private:
	Ptr<FeatureDetector> detector_;
	Ptr<DescriptorExtractor> descriptorExtractor_;
	vector<KeyPoint> keypoint_;
};


class CEDD : public Feature {
public:
	virtual bool detect(const Mat& img, Mat& features);
};


//相似度度量
class Metric {
public:
	virtual float compute(const Mat& feature1, const Mat& feature2)=0;
};

class Euclidean : public Metric {
public:
	virtual float compute(const Mat& feature1, const Mat& feature2);
};
#endif // FEATURE_H__
