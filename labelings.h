#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "maxflow/graph.h"
#include "image.h"
#pragma once
using namespace std;

Mat* test_labeling(Mat& image);
void interactive_labeling(vector<Mat> images, Mat* labels);
void interective_drawing(int event, int x, int y, int, void* param);
Scalar intColor(int param);
Image<Vec3b> intToColor2(Mat& image);
Image<Vec3b> intToColor(Mat& image);