#pragma once
#ifndef FACEFUSION_FUSIONATPIXELS_H
#define FACEFUSION_FUSIONATPIXELS_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

Mat FusionGreaterPixel(Mat src_img_1, Mat src_img_2);
Mat FusionAveragePixel(Mat src_img_1, Mat src_img_2);

#endif