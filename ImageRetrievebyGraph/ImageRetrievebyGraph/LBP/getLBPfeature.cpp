#include "getLBPfeature.h"


bool GetLBPFeature( const cv::Mat& gray_img_64_64, float feature_vector[NUM_G] )
{
	uchar s[MAXLINE][MAXLINE];
	//int nl = gray_img_64_64.rows;
	//int nc = gray_img_64_64.cols;
	for (int j=0; j<MAXLINE; ++j) {
		const uchar* data = gray_img_64_64.ptr<uchar>(j);
		for (int i=0; i<MAXLINE; ++i) {
			s[j][i] = data[i];
			
		}
	}
	unsigned char i,j,k;
	unsigned char center,sumb;
	unsigned char  p[MAXLINE+2][MAXLINE+2],edgep[MAXLINE+2][MAXLINE+2];

	for ( i = 0; i < MAXLINE; i ++)
		for ( j = 0; j < MAXLINE; j ++)
			p[i+1][j+1] = s[i][j];
	for ( i = 0; i < MAXLINE+2; i ++)
	{
		p[i][0] = p[0][i] = p[65][i] = p[i][65] = 0;
		edgep[i][0] = edgep[0][i] = edgep[65][i] = edgep[i][65] = 0;
	}

	unsigned char tempa[3];
	for (i = 1; i<MAXLINE+1; i++)   //1~64
		for (j = 1; j<MAXLINE+1; j++)
		{
			for (k=0; k<3; k++)
				tempa[k]=0;
			center=p[i][j];
			if (p[i-1][j]>=center)
				tempa[0]=1;
			if (p[i][j+1]>=center)
				tempa[1]=1;
			if (p[i+1][j-1]>=center)
				tempa[2]=1;


			sumb=tempa[0]+tempa[1]*2+tempa[2]*4;
			edgep[i][j]=sumb;
		}

		//以直方图作为特征  6*6个小格
		int count=0;
		unsigned char hist[8];
		for ( i = 0; i <66; i+=11)
			for ( j = 0; j <66; j+=11)
			{
				for (k=0; k<8; k++)
					hist[k]=0;
				for (int ii=i; ii<i+11 ; ii++)
					for (int jj=j; jj<j+11 ; jj++)
						hist[edgep[ii][jj]]++;     //edge
				for (k=0; k<8; k++)
				{
					feature_vector[count]=hist[k];
					count++;
				}
			}
			return true;
}


