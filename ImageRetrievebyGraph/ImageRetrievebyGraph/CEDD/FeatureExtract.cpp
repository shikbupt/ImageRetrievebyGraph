
#include "featureExtract.h"

#include "cv.h"
#include "highgui.h"
#include "CEDDFeature.h"
#include <iostream>
#include <direct.h>

using namespace std;
using namespace cv;

#define PIC_DEBUG_DIR "..\\DEBUG_PIC\\debug%02d.jpg"


CEDDFeature myCEDDFeature;



int extractShapeFeature(string filename, float * feature)
{
	//////////////////////////如果原图像过大，则对其进行压缩/////////////////////////////////////
	IplImage *src = 0;   //源图像指针  
	IplImage *dst = 0;   //目标图像指针  

	CvSize dst_cvsize;   //目标图像尺寸  

	src = cvLoadImage(filename.c_str());  
	string path;
	if (src->height > 310 || src->width > 310)
	{
		if (src->width > src->height)
		{
			dst_cvsize.width = 320;
			dst_cvsize.height = src->height * 320 / src->width;
		}
		else
		{
			dst_cvsize.height = 320; //目标图像的高  
			dst_cvsize.width = src->width * 320 / src->height;  //目标图像的宽 
		}

		dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels); //构造目标图象  

		cvResize(src, dst, CV_INTER_AREA); //缩放源图像到目标图像  

		//string resizeFileName = "resize_" + filename;
		string resizeFileName = filename.substr(filename.find_last_of('\\'));
		path = "..\\resize"; 
		mkdir(path.c_str());
		resizeFileName = path + resizeFileName.insert(resizeFileName.size()-4, "_resize");

		cvSaveImage(resizeFileName.c_str(), dst);
		//image = Mat(dst, 1);
		cvReleaseImage(&dst);

		filename = resizeFileName;

	}

	cvReleaseImage(&src); //释放源图像占用的内存  

	//////////////////////////End of 如果原图像过大，则对其进行压缩/////////////////////////////////////

	Mat image = imread(filename, 1);

	if( image.empty() )
	{
		cout << "\n Durn, couldn't read image filename " << filename << endl;
		return 1;
	}

	Mat mask;
	mask.create( image.size(), CV_8UC1);
	mask.setTo(Scalar::all(GC_PR_FGD));

	Mat bgdModel;
	//	bgdModel.create(cvSize(13*5, 1 ),CV_32FC1);
	Mat fgdModel;
	//	fgdModel.create(cvSize(13*5, 1),CV_32FC1);

	CvRect rect = cvRect(image.cols/20, image.rows/8, 9 * image.cols/10, 3 * image.rows/4);

	// 	int roi = (image.cols/4+ image.rows/4) * (image.cols/4+ image.rows/4);
	// 	
	// 	for (int i = 0; i < image.rows; i++)
	// 	{
	// 		for (int j = 0; j < image.cols; j++)
	// 		{
	// 			if ( ( (i-image.rows/2) * (i-image.rows/2) + (j-image.cols/2) * (j-image.cols/2) )  >   roi)
	// 			{
	// 				mask.data[i*image.cols + j] = GC_BGD;
	// 			}
	// 
	// 		}
	// 	}

	grabCut( image, mask, rect, bgdModel, fgdModel, 3, GC_INIT_WITH_RECT);

	IplImage *saveImg = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
	int height = saveImg->height;
	int width = saveImg->width;

	int top = 0; 
	int buttom = height;
	int left = 0;
	int right = width;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (mask.data[i*mask.cols + j] != GC_PR_FGD )
			{
				saveImg->imageData[i * saveImg->widthStep + j * 3] = 0;
				saveImg->imageData[i * saveImg->widthStep + j * 3 + 1] = 0;
				saveImg->imageData[i * saveImg->widthStep + j * 3 + 2] = 0;
			}
			else
			{
				saveImg->imageData[i * saveImg->widthStep + j * 3] = -1;
				saveImg->imageData[i * saveImg->widthStep + j * 3 + 1] = -1;
				saveImg->imageData[i * saveImg->widthStep + j * 3 + 2] = -1;
			}
		}
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width ; ++j)
		{
			if (saveImg->imageData[i * saveImg->widthStep + j * 3] == -1)
			{
				top = i;
				i = height;
				j = width ;
			}
		}
	}

	for (int i = height-1/*height*/; i >= 0; --i)
	{
		for (int j = width-1/*width*/; j >= 0; --j)
		{
			if (saveImg->imageData[i * saveImg->widthStep + j * 3] == -1)
			{
				buttom = i;
				i = 0;
				j = 0;
			}
		}
	}

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			if (saveImg->imageData[j * saveImg->widthStep + i * 3] == -1)
			{
				left = i;
				i = width;
				j = height;
			}
		}
	}

	//for (int i = width; i >= 0 ; i--)
	for	(int i = width-1; i>=0; --i)
	{
		//for (int j = height; j >= 0; j--)
		for (int j = height-1; j >= 0; --j)
		{
			if (saveImg->imageData[j * saveImg->widthStep + i * 3] == -1)
			{
				right = i;
				i = 0;
				j = 0;
			}
		}
	}

	cvSaveImage(PIC_DEBUG_DIR, saveImg);
	getHistogram(saveImg->imageData,saveImg->widthStep, top , buttom, left, right, feature);
	IplImage * colorImg = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
	myCEDDFeature.extractRegion(colorImg,  top , buttom, left, right, feature);

	if (saveImg)
	{
		cvReleaseImage(&saveImg);
		saveImg = NULL;
	}

	if (colorImg)
	{
		cvReleaseImage(&colorImg);
		colorImg = NULL;
	}
	if (!path.empty()) {
		string temp = "del " + path + "\\*.jpg";
		system(temp.c_str());
	}	
	return 0;

}

void getHistogram(char* image, int widthStep,int top, int buttom, int left, int right, float * feature)
{
	int height = buttom - top + 1;
	int width = right - left +1;

	int i = 0, j = 0;

	double widthBin[10] = {0};
	double heightBin[10] = {0};


	double binWidth = (double) height / (double) 10;
	double oneThirdBinWidth = (double) height / (double) 30;
	double pixelBinWidth = (double) 15 / (double)height;


	double binHeight = (double) width / (double) 10;  //按列分为10个bin，每个bin的宽度
	double oneThirdBinHeight = (double) width / (double) 30;   //三分之一个bin的宽度
	double pixelBinHeight = (double) 15 / (double)width;	//一个像素点对应的bin隶属的变化

	int binBelong, binRemain;

	for(i = top; i <= buttom-1; i++)
	{
		for (j = left; j <= right-1; j++)
		{
			if (image[i * widthStep + j * 3] == -1)
			{
				////////////////////////////////////////////////////////////////
				/*按行统计*/
				binBelong = (i-top) / binWidth;
				binRemain = (double) (i-top) - binWidth * binBelong;

// 				if (binBelong > 9)
// 					char a = 't';

				if (binRemain <  oneThirdBinWidth )
				{
					if (binBelong == 0)
					{
						widthBin[0] += 1;
					}

					if (binBelong > 9)
					{
						widthBin[9] += 1;
					}

					if (binBelong >= 1 && binBelong <= 9)
					{
						double widthBinDivided = pixelBinWidth * binRemain;
						widthBin[binBelong - 1] += (0.5 - widthBinDivided);
						widthBin[binBelong] += (0.5 + widthBinDivided);

					}
				}

				if (binRemain >= oneThirdBinWidth && binRemain <= 2 * oneThirdBinWidth)
				{
					widthBin[binBelong] += 1;
				}

				if (binRemain > 2 * oneThirdBinWidth)
				{
					if (binBelong >= 9)
					{
						widthBin[9] += 1;
					}
					else
					{
						double widthBinDivided = pixelBinWidth * (binWidth - binRemain);
						widthBin[binBelong + 1] += (0.5 - widthBinDivided);
						widthBin[binBelong] += (0.5 + widthBinDivided);

					}
				}

				/*按行统计*/
				///////////////////////////////////////////////////////////////

				////////////////////////////////////////////////////////////////
				/*按列统计*/
				binBelong = (j - left) / binHeight;  //确定像素点所在的bin
				binRemain = (double) (j - left) - binHeight * binBelong;	//在每个bin的第几个像素点

// 				if (binBelong > 9)
// 					char a = 't';

				if (binRemain < oneThirdBinHeight)
				{
					if (binBelong == 0)
					{
						heightBin[0] += 1;
					}

					if (binBelong > 9)
					{
						heightBin[9] += 1;
					}

					if (binBelong >= 1 && binBelong <= 9)
					{
						double heightBinDivided = pixelBinHeight * binRemain;
						heightBin[binBelong - 1] += (0.5 - heightBinDivided);
						heightBin[binBelong] += (0.5 + heightBinDivided);
					}
				}

				if (binRemain >= oneThirdBinHeight && binRemain <= 2 * oneThirdBinHeight)
				{
					heightBin[binBelong] += 1;
				}

				if (binRemain > 2 * oneThirdBinHeight)
				{
					if(binBelong == 9)
						heightBin[9] += 1;
					else
					{
						double heightBinDivided = pixelBinHeight * (binHeight - binRemain);
						heightBin[binBelong + 1] += (0.5 - heightBinDivided);
						heightBin[binBelong] += (0.5 + heightBinDivided);
					}
				}
			}

		}
	}

	///////////////////////////////////////////////////////////////////////////
	/*量化*/
	double sumHight = 0, sumWidth = 0;
	for (int k = 0; k < 10; k++)
	{
		sumHight += heightBin[k];
		sumWidth += widthBin[k];
	}


	float heightProportion = 200 * (float)(buttom - top + 1) / (buttom - top + right - left + 2);
	float widthProportion = 200 - heightProportion;

	for (int k = 0; k < 10; k++)
	{
		feature[k] =(float)(heightBin[k] * heightProportion/sumHight);
		feature[k+10] = (float)(widthBin[k] * widthProportion/sumWidth);
	}
	///////////////////////////////////////////////////////////////////////////

	return;
}
