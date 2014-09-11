#pragma once
#ifndef FACEFUSION_FUSION_LAPLACIAN_H
#define FACEFUSION_FUSION_LAPLACIAN_H

#include "opencv2/opencv.hpp"
using namespace cv;

class LaplacianBlending {
private:
	Mat_<Vec3f> left;
	Mat_<Vec3f> right;
	Mat_<float> blendMask;

	vector<Mat_<Vec3f> > leftLapPyr,rightLapPyr,resultLapPyr;//Laplacian Pyramids
	Mat leftHighestLevel, rightHighestLevel, resultHighestLevel;
	vector<Mat_<Vec3f> > maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB

	int levels;

	void buildPyramids() {
		buildLaplacianPyramid(left,leftLapPyr,leftHighestLevel);
		buildLaplacianPyramid(right,rightLapPyr,rightHighestLevel);
		//	buildGaussianPyramid();
	}

	void buildGaussianPyramid() {//金字塔内容为每一层的掩模
		assert(leftLapPyr.size()>0);

		maskGaussianPyramid.clear();
		Mat currentImg;
		cvtColor(blendMask, currentImg, CV_GRAY2BGR); //为处理BGR3通道。store color img of blend mask into maskGaussianPyramid
		maskGaussianPyramid.push_back(currentImg); //0-level

		currentImg = blendMask;
		for (int i=1; i<levels+1; i++) {
			Mat _down;
			if (leftLapPyr.size() > i)
				pyrDown(currentImg, _down, leftLapPyr[i].size());
			else
				pyrDown(currentImg, _down, leftHighestLevel.size()); //lowest level

			Mat down;
			cvtColor(_down, down, CV_GRAY2BGR);
			maskGaussianPyramid.push_back(down);//add color blend mask into mask Pyramid
			currentImg = _down;
		}
	}

	void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& HighestLevel) {
		lapPyr.clear();
		Mat currentImg = img;
		for (int l=0; l<levels; l++) {
			Mat down,up;
			pyrDown(currentImg, down);
			pyrUp(down, up,currentImg.size());
			Mat lap = currentImg - up;  //保存的是差值金字塔
			lapPyr.push_back(lap);
			currentImg = down;
		}
		currentImg.copyTo(HighestLevel);
	}

	Mat_<Vec3f> reconstructImgFromLapPyramid() {
		//将左右laplacian图像拼成的resultLapPyr金字塔中每一层
		//从上到下插值放大并相加，即得blend图像结果
		Mat currentImg = resultHighestLevel;
		for (int i=levels-1; i>=0; i--) {
			Mat up;

			pyrUp(currentImg, up, resultLapPyr[i].size());
			currentImg = up + resultLapPyr[i];  //差值金字塔
		}
		return currentImg;
	}

	//决定融合策略
	void blendLapPyrs() {
#if 0
		//获得每层金字塔中直接用左右两图Laplacian变换拼成的图像resultLapPyr
		// A.mul(B) 为对应位置元素相乘
		resultHighestLevel = leftHighestLevel.mul(maskGaussianPyramid.back()) +
			rightHighestLevel.mul(Scalar(1.0,1.0,1.0) - maskGaussianPyramid.back()); // 选取左边一半，右边一半
		for (int i=0; i<levels; i++) {
			Mat A = leftLapPyr[i].mul(maskGaussianPyramid[i]);
			Mat antiMask = Scalar(1.0,1.0,1.0) - maskGaussianPyramid[i];
			Mat B = rightLapPyr[i].mul(antiMask);
			Mat_<Vec3f> blendedLevel = A + B;

			resultLapPyr.push_back(blendedLevel);
		}
#else
		// 缩小图取平均
		resultHighestLevel = leftHighestLevel/2 + rightHighestLevel/2;
		for (int i = 0; i < levels; i++)
		{
			Mat A = (leftLapPyr[i] > rightLapPyr[i])/255;   //左边大为1/255，右边大为0；除255是为将blendedLevel值范围限制在0~1
			//	cout << A.channels() << endl;
			//	cout << A.depth() << endl;
			Mat B;
			A.convertTo(B, CV_32FC3);
			//	cout << A;
			//	cvtColor(A, A, CV_GRAY2BGR);
			//	Mat C = leftLapPyr[0].mul(B); //+ rightLapPyr[0].mul(Scalar(1,1,1)-B);
			Mat_<Vec3f> blendedLevel = leftLapPyr[i].mul(B) + rightLapPyr[i].mul(Scalar(1,1,1)-B);  //取大，混合
			resultLapPyr.push_back(blendedLevel);
		}


#endif
	}

public:
	LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _blendMask, int _levels)://construct function, used in LaplacianBlending lb(l,r,m,4);
	  left(_left),right(_right),blendMask(_blendMask),levels(_levels)
	  {
		  assert(_left.size() == _right.size());
		  assert(_left.size() == _blendMask.size());
		  buildPyramids();	//construct Laplacian Pyramid and Gaussian Pyramid
		  blendLapPyrs();	//blend left & right Pyramids into one Pyramid
	  };

	  Mat_<Vec3f> blend() {
		  return reconstructImgFromLapPyramid();//reconstruct Image from Laplacian Pyramid
	  }
};

//Mat_<Vec3f> LaplacianBlend(const Mat_<Vec3f>& l, const Mat_<Vec3f>& r, const Mat_<float>& m) {
//	LaplacianBlending lb(l,r,m,4);
//	return lb.blend();
//}

#endif