#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include "image.h"

Mat
poisson_blending(Mat &img1, Mat &img2, Rect ROI, int posX, int posY);