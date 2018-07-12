/******************************************************************************************************/
/********************************         ���ּ�⣨��ʴ+͹����         *******************************/
/***********************************   2017.07.08   by:William Yu  ************************************/
/******************************************************************************************************/
/**** ע ���˴��벿�ֲο�����ֲ�ڲ��� http ://blog.csdn.net/a_jia_17/article/details/60340158       ***/
/****      �ò��Ĳ�ȡ�ķ���Ϊ��ȡ�����������ģ�����ֶַ�Բ�����г��ֶ�Բ��ȡʣ����������Ϊ����     ***/
/****      �˴����ȡ�ķ���Ϊ��ȡ��������ʴ����Ϊ��״����͹����͹��������Ϊ����          **************/
/******************************************************************************************************/

#include <opencv2/OpenCV.hpp>      
#include <opencv2/imgproc/imgproc.hpp>   
#include <opencv2/highgui/highgui.hpp>   
#include <math.h>  
#include <iostream>  
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

//��򷨺�������
int otsuThreshold(IplImage *frame);

int main()
{
	//��ʱ�Ż�
	int64 st1, et1;
	st1 = cvGetTickCount();


		//-----------------------------------��ͼ��Ԥ����---------------------------------------    
		//      ������ͼ��Ķ��룬ѡ�����ҶȻ�����ֵ�����˲�     
		//-----------------------------------------------------------------------------------------------  

		//��1������ԭʼͼ����ʾͼƬ     
		Mat srcImage = imread("����.jpg", 1);      
		namedWindow("��1.ԭʼͼ��", WINDOW_AUTOSIZE);
		imshow("��1.ԭʼͼ��", srcImage);

		//��2����ȡ����Ȥ��  
		Rect ROIim(300, 100, 600, 600);    //��������ͼƬ�޸�
		Mat ROIsrc;
		srcImage(ROIim).copyTo(ROIsrc);
		namedWindow("��2.roiimage��", WINDOW_NORMAL);
		imshow("��2.roiimage��", ROIsrc);

		//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��  
		Mat grayImage;
		cvtColor(ROIsrc, grayImage, CV_BGR2GRAY);
		namedWindow("��3.�Ҷ�ͼ��", WINDOW_NORMAL);
		imshow("��3.�Ҷ�ͼ��", grayImage);

		////��4��ͼ��Ķ�ֵ��
		//Mat thresholdImage;
		//threshold(grayImage, thresholdImage, 126, 255, CV_THRESH_BINARY_INV);   // ��������������ͼƬ�޸ģ��������򷨣� 
		////ע������������Ϊ��ɫΪǰ�����壬��ɫΪ��������, ����Ϊ�˺����ȡ��������Ҫ�����������������ʹ����Ϊǰ��
		//namedWindow("��4.��ֵͼ��", WINDOW_NORMAL);
		//imshow("��4.��ֵͼ��", thresholdImage);
		
		//��4��ͼ��Ĵ�򷨶�ֵ��
		Mat thresholdImage;
		IplImage * frame_gray;
		frame_gray = &IplImage(grayImage);
		int threshold1 = otsuThreshold(frame_gray);           //���ô�򷨺���,�����򷨴����ٶȻ���һЩ��������
		threshold(grayImage, thresholdImage, threshold1, 255, CV_THRESH_BINARY_INV);
		namedWindow("��4.��ֵͼ��", WINDOW_NORMAL);
		imshow("��4.��ֵͼ��", thresholdImage);

		//��5����ֵ�˲�  
		medianBlur(thresholdImage, thresholdImage, 3);
		namedWindow("��5.�˲���", WINDOW_NORMAL);
		imshow("��5.�˲���", thresholdImage);



		//-----------------------------------��ͼ��ָ---------------------------------------    
		//      ��������Ե��⣬������ȡ    
		//-----------------------------------------------------------------------------------------------  
		
		//��1����ȡ����  
		vector<vector<Point>> contours;       //���飬��--->һ������--->������
		//��ȡ����
		findContours(thresholdImage,           
			contours,                  
			CV_RETR_EXTERNAL,       //��ȡ��Χ����
			CV_CHAIN_APPROX_NONE    //��ȡȫ������
			);
		//��ʾ��������  
		cout << "������" << contours.size() << "��" << "\n";
		for (int i = 0; i<contours.size(); i++)
		{
			cout << "��������" << contours[i].size() << endl;
		}

		//��2������ɸѡ 
		long cmin = 2000;           //��������ͼƬ�޸�
		long cmax = 3000;
		vector<vector<Point>>::const_iterator itc = contours.begin();
		while (itc != contours.end())
		{
			if (itc->size() < cmin || itc->size() > cmax)
				itc = contours.erase(itc);
			else
				++itc;
		}
		cout << "��Ч������" << contours.size() << "��" << "\n" << endl;

		//��3���������� 
		Mat ContoursIm(thresholdImage.size(), CV_8U, Scalar(0));//����ԭͼͬ��С�Ļ���//Scalar(0)��������ɫ  
		drawContours(ContoursIm, //����  
			contours, //��������ָ�룩  
			-1,       //�����ȼ������ﻭ������������  
			Scalar(255),//����������ɫ  
			-1);       //�ߴ֣�>=0ʱ�����ߴ֣�-1ʱ����CV_FILLED������  
		namedWindow("��6.ContoursIm��", WINDOW_NORMAL);
		imshow("��6.ContoursIm��", ContoursIm);



		//-----------------------------------����̬����---------------------------------------    
		//      �����������븯ʴ    
		//-----------------------------------------------------------------------------------------------  
		
		//��1����������
		Mat src;
		src = ContoursIm;
		if (!src.data)
		{
			return -1;
		}

		//��2����ʴ����
		Mat dst;  //���帯ʴ���������
		//����ṹԪ����������ͼƬ�޸� 
		Mat element = getStructuringElement(MORPH_ELLIPSE,    //MORPH_ELLIPSE����Բ���ںˣ�
			Size(2 * 16 + 1, 2 * 16 + 1),                  	//ָ���ں˴�С��16���ں˰뾶
			Point(16, 16));                                 //ê��λ�á���ָ��ê��λ�ã���Ĭ��ê�����ں�����λ�á�
		//��ʴ
		erode(src,dst, element);
		namedWindow("��7.Erosion��", WINDOW_NORMAL);
		imshow("��7.Erosion��", dst);

		////��2�����Ͳ���
		//Mat dst;                
		////����ṹԪ����������ͼƬ�޸� 
		//Mat element = getStructuringElement(MORPH_ELLIPSE,   
		//	Size(2 * 16 + 1, 2 * 16 + 1),                
		//	Point(16, 16));                               
		////����
		//dilate(src, dst, element);
		//namedWindow("��7.Erosion��", CV_WINDOW_AUTOSIZE);
		//imshow("��7.Dilation��", dst);


		//-----------------------------------���ٴ�ͼ��ָ---------------------------------------    
		//      ��������Ե��⣬������ȡ������͹��    
		//----------------------------------------------------------------------------------------------- 

		//��1����ȡ����
		vector<vector<Point>> contours1;
		findContours(dst,            //ͼ��    
			contours1,               //������     
			CV_RETR_EXTERNAL,       //��ȡ�����ķ����������ȡ��Χ������    
			CV_CHAIN_APPROX_NONE    //�������Ƶķ��������ﲻ���ƣ���ȡȫ��������  
			);

		//��2��ȡ����0������
		vector<Point> points_01;   //������ά�ȡ�
		points_01 = contours1[0];  //findContours����������vector<vector<Point>>���ͣ���Ҫvector<Point>������͹��

		//��3������͹��
		vector<int> hull;
		convexHull(points_01, hull, 1);

		//��4������������͹��
		Mat ContoursIm_1(dst.size(), CV_8U, Scalar(0));//����ԭͼͬ��С�Ļ���  
		//������
		drawContours(ContoursIm_1, //����  
			contours1, //��������ָ�룩  
			-1,       //�����ȼ������ﻭ������������  
			Scalar(255),//����������ɫ  
			-1);       //�ߴ֣�>=0ʱ�����ߴ֣�-1ʱ����CV_FILLED������
		//��͹��
		int hullcount = (int)hull.size();
		Point poi_01 = points_01[hull[hullcount - 1]];
		for (int i = 0; i < hullcount; i++)
		{
			Point pt = points_01[hull[i]];
			line(ContoursIm_1, poi_01, pt, Scalar(255), 3, CV_AA);
			poi_01 = pt;
		}
		namedWindow("��8.Convex Hull��", WINDOW_NORMAL);
		imshow("��8.Convex Hull��", ContoursIm_1);
		


		//-----------------------------------�����Բ�����---------------------------------------    
		//      ����������    
		//-----------------------------------------------------------------------------------------------  

		//��1���������  
		cout << "������" << hull.size() << "\n" << endl;

	//ʱ����ʾ
	et1 = cvGetTickCount();
	cout << "times cost:" << (et1 - st1) / (double)cvGetTickFrequency() / 1000.0 << "milliseconds\n\n";

	waitKey(0);
	return 0;
}



/************************     ���      **************************/

int otsuThreshold(IplImage *frame)
{
#define GrayScale 256
	int width = frame->width;
	int height = frame->height;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height, threshold = 0;
	uchar* data = (uchar*)frame->imageData;  //ָ���������ݵ�ָ��
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
		}
	}

	//����ÿ������������ͼ���еı���  
	float maxPro = 0.0;
	int kk = 0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
			kk = i;
		}
	}

	//�����Ҷȼ�[0,255]  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i��Ϊ��ֵ
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //��������  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //ǰ������  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}

		u0 = u0tmp / w0;  //������ֵ
		u1 = u1tmp / w1; //ǰ����ֵ


		u = u0tmp + u1tmp;//�ܾ�ֵ�Ķ���
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);//��䷽��
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}