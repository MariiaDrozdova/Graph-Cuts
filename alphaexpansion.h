#include "image.h"
#include "distance.h"
#include <iostream>
#include <fstream>
#include "maxflow/graph.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "labelings.h"
#include "openfiles.h"
#include "blending.h"

struct alpha {
	Mat real_labels;
	Mat* labels;
	Mat original;
};


double alpha_step(int k, Mat* original, vector<Mat> the_vector, Mat& labels, Mat* real_labels);

double alpha_step2(int k, Mat* original, vector<Mat> the_vector, Mat& labels, Mat* real_labels);

alpha alphamarche(String s);
alpha alphamarcheOrder(String s, Mat* labels);