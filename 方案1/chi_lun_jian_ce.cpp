/*=====================================================================================
*                      chi_lun_jian_ce.c --
*	                   Copyleft! 2017 William Yu
*          Some rights reserved��CC(creativecommons.org)BY-NC-SA
*                      Copyleft! 2017 William Yu
*      ��Ȩ�������У���ѭCC(creativecommons.org)BY-NC-SAЭ����Ȩ��ʽʹ��
*
* Filename                : chi_lun_jian_ce
* Programmer(s)           : William Yu
* Description             : (OpenCV2.4.9+VS2013)
                            �˴���Դ�ڲ��� http ://blog.csdn.net/a_jia_17/article/details/60340158
                            �ò��Ĳ�ȡ�ķ���Ϊ��ȡ�����������ģ�����ֶַ�Բ�����г��ֶ�Բ��ȡʣ����������Ϊ����    
                            �ò��Ļ������������ѧϰ�ı�ע�������ˡ���򷨡�������˼�ʱ�����Ż�                     
                            ���ڸò��Ļ����ϸĽ�����һ�ֳ��ֳ����ķ��������ּ�⣨��ʴ + ͹����                      

* Reference               : http ://blog.csdn.net/a_jia_17/article/details/60340158
* Modification History	  : ver1.0, 2017.02.06, William Yu
*                           ver1.1, 2017.02.21, William Yu add notes
=====================================================================================*/
#include <opencv2/opencv.hpp>      
#include <opencv2/imgproc/imgproc.hpp>   
#include <opencv2/highgui/highgui.hpp>   
#include <math.h>  
#include <iostream>  

using namespace cv;
using namespace std;

//-----------------------------------���---------------------------------------    
//��򷨺�������
int otsuThreshold(IplImage *frame);
//-----------------------------------���---------------------------------------    

int main()
{
	//-----------------------------------���---------------------------------------  
	//��ʱ�Ż�
	int64 st1, et1;
	st1 = cvGetTickCount();
	//-----------------------------------���---------------------------------------    



		//-----------------------------------��ͼ���Ԥ����---------------------------------------    
		//      ������ͼ��Ķ��룬�ҶȻ�����ֵ�����˲�     
		//-----------------------------------------------------------------------------------------------      
		//��1������ԭʼͼ��Mat��������       
		Mat srcImage = imread("����.jpg", 1);  //����Ŀ¼��Ӧ����һ����Ϊ����.jpg���ز�ͼ    
		Mat grayImage, thresholdImage;//��ʱ������Ŀ��ͼ�Ķ���    

		//��2����ʾԭʼͼ    
		namedWindow("��1.ԭʼͼ��", WINDOW_NORMAL);
		imshow("��1.ԭʼͼ��", srcImage);

		//��3����ȡ����Ȥ��  
		//-----------------------------------���---------------------------------------  
		//ʹ��cvsetImageROI����
		//cvsetImageROI(src��cvRect(x, y, width, height))
		//����x��yΪROI�������㣬width��heightΪ��͸ߣ�
		//��src��ȡ����Ȥ������ٶ�src����ͼ����ʱֻ�����ȡ�ĸ���Ȥ���д�����ͼ����ʱҪע����һ�㡣
		//ʹ�øú���ʱҪ��ͨ��cvResetImageROI()�����ͷ�ROI�������ͼ����д���ʱֻ��ROI������жԱȡ�
		//-----------------------------------���---------------------------------------    
		Rect ROIim(300, 100, 600, 600);    //��������ͼƬ�޸�
		Mat ROIsrc;
		srcImage(ROIim).copyTo(ROIsrc);
		namedWindow("��2.roiimage��", WINDOW_NORMAL);
		imshow("��2.roiimage��", ROIsrc);

		//��4��תΪ�Ҷ�ͼ������ͼ��ƽ��    
		cvtColor(ROIsrc, grayImage, CV_BGR2GRAY);//ת����Ե�����ͼΪ�Ҷ�ͼ     
		namedWindow("��3.�Ҷ�ͼ��", WINDOW_NORMAL);
		imshow("��3.�Ҷ�ͼ��", grayImage);

		//��5��ͼ��Ķ�ֵ�������ô�� 
		//-----------------------------------�Ķ�---------------------------------------    
		IplImage * frame_gray;
		frame_gray = &IplImage(grayImage);
		int threshold1 = otsuThreshold(frame_gray);           //���ô�򷨺���
		threshold(grayImage, thresholdImage, threshold1, 255, CV_THRESH_BINARY_INV);   
		namedWindow("��4.��ֵͼ��", WINDOW_NORMAL);
		imshow("��4.��ֵͼ��", thresholdImage);
		//-----------------------------------�Ķ�---------------------------------------   

		//��6����ֵ�˲�  
		medianBlur(thresholdImage, thresholdImage, 3);
		namedWindow("��5.�˲�������ͼ��", WINDOW_NORMAL);
		imshow("��5.�˲�������ͼ��", thresholdImage);



		//-----------------------------------��ͼ��ָ---------------------------------------    
		//      ��������Ե��⣬������ȡ
		//-----------------------------------------------------------------------------------------------  
		
		//��1����ȡ����  
		vector<vector<Point>> contours;
		findContours(thresholdImage,            //ͼ��    
			contours,               //������     
			CV_RETR_EXTERNAL,       //��ȡ�����ķ����������ȡ��Χ������    
			CV_CHAIN_APPROX_NONE    //�������Ƶķ��������ﲻ���ƣ���ȡȫ��������  
			);
		//������������  
		cout << "������" << contours.size() << "��" << "\n";
		for (int i = 0; i<contours.size(); i++)
		{
			cout << "��������" << contours[i].size() << endl;
		}
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

		//��2���������� 
		Mat ContoursIm(thresholdImage.size(), CV_8U, Scalar(255));//����ԭͼͬ��С�Ļ���  
		drawContours(ContoursIm, //����  
			contours, //��������ָ�룩  
			-1,       //�����ȼ������ﻭ������������  
			Scalar(0),//��ɫ  
			-1);       //�ߴ֣�>=0ʱ�����ߴ֣�-1ʱ����CV_FILLED������  
		namedWindow("��6.ContoursIm��", WINDOW_NORMAL);
		imshow("��6.ContoursIm��", ContoursIm);



		//-----------------------------------�����Բ�����---------------------------------------    
		//      �������ݶ�Բ���ݸ�Բֱ��������    
		//----------------------------------------------------------------------------------------------- 

		//��1����ȡԲ��  
		//����������  
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], true);
		}
		//��������������  
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			cout << "��������:" << mc[i] << endl;
		//	cout << "pointPloyTest�ݸ�Բֱ��:" << pointPolygonTest(contours[i], mc[i], true) * 2 << "\n" << endl;
		}

		//��2���ݸ�Բ���ݶ�Բֱ��  
		double rmax, rmin, r, r2;
		for (int i = 0; i<contours.size(); i++)
		{
			for (int j = 0; j<contours[i].size(); j++)
			{
				r2 = pow(contours[i][j].x - mc[i].x, 2) + pow(contours[i][j].y - mc[i].y, 2);
				r = sqrt(r2);
				if (j == 0)
				{
					rmax = rmin = r;
				}
				if (rmax <= r)
				{
					rmax = r;
				}
				if (rmin >= r)
				{
					rmin = r;
				}
			}
			cout << "�ݸ�Բֱ����" << rmin * 2 << endl;
			cout << "�ݶ�Բֱ����" << rmax * 2 << "\n" << endl;
		}

		//��3����������  
		//��ȡ�ֳ�  
		Mat thresholdIm = ContoursIm.clone();
		circle(thresholdIm, mc[0], (rmax + rmin) / 2, Scalar(255), -1);
		namedWindow("��7.��Բ��", WINDOW_NORMAL);
		imshow("��7.��Բ��", thresholdIm);
		//�˲�  
		threshold(thresholdIm, thresholdIm, 0, 255, CV_THRESH_BINARY_INV);
		medianBlur(thresholdIm, thresholdIm, 3);
		namedWindow("��8.��ֵͼ2��", WINDOW_NORMAL);
		imshow("��8.��ֵͼ2��", thresholdIm);
		//��ȡ�ֳ�����  
		vector<vector<Point>> contour;
		findContours(thresholdIm, contour,
			CV_RETR_LIST,       //��ȡ�����ķ������������������������ּ���    
			CV_CHAIN_APPROX_NONE    //�������Ƶķ��������ﲻ���ƣ���ȡȫ��������  
			);
		//ȥ���������߹��̵�����  
		int csmin = 10;
		int csmax = 200;
		vector<vector<Point>>::const_iterator itcs = contour.begin();
		while (itcs != contour.end())
		{
			if (itcs->size() < csmin || itcs->size() > csmax)
				itcs = contour.erase(itcs);
			else
				++itcs;
		}
		//��������
		Mat ContourIm(thresholdIm.size(), CV_8U, Scalar(255));//����ԭͼͬ��С�Ļ���  
		drawContours(ContourIm, contour,
			-1,       //�����ȼ������ﻭ������������  
			Scalar(0),//��ɫ  
			2);       //�ߴ֣�>=0ʱ�����ߴ֣�-1ʱ����CV_FILLED������  
		namedWindow("��9.ContourIm��", WINDOW_NORMAL);
		imshow("��9.ContourIm��", ContourIm);

		//�������  
		cout << "������" << contour.size()<<"\n" << endl;

	//-----------------------------------���---------------------------------------    
	//ʱ����ʾ
	et1 = cvGetTickCount();
	cout << "times cost:" << (et1 - st1) / (double)cvGetTickFrequency() / 1000.0 << "milliseconds\n\n";
	//-----------------------------------���---------------------------------------    
	waitKey(0);
	return 0;
}


//-----------------------------------���---------------------------------------    
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
//-----------------------------------���---------------------------------------    
