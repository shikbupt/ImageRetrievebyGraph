#pragma once
#include <cv.h>

class CEDDFeature
{
public:
	CEDDFeature();
	~CEDDFeature();

public:
	int extract(IplImage* image);
	int extractRegion(IplImage* image,int top, int buttom, int left, int right, float * feature);

public:
	double T0;
	double T1;
	double T2;
	double T3;
	bool Compact;
	double qCEDD[144];
	int IntCEDD[144];
};
