#include "labelings.h"

Mat* test_labeling(Mat& image) {
	int cols = image.cols;
	int rows = image.rows;
	int k_alpha = 0;
	Mat* labels = new Mat(Mat::zeros(rows, cols, CV_32SC1));
	for (int i = 0; i < labels->cols; i++) {
		for (int j = 0; j < labels->rows; j++) {
			if (j >   3 * labels->rows / 4) {
				k_alpha = 1;
			}
			else if (((i < labels->cols / 2) || (i > 7 * labels->cols / 8)) || ((j < labels->rows / 8) || (j > 7 * labels->rows / 8))) {
				k_alpha = 2;
			}
			else {
				k_alpha = 0;
			}
			labels->at<int>(j, i) = k_alpha;
		}
	}
	return labels;
}


int drawing = 0;
int old_x = 0;
int old_y = 0;
String window = "analyse";


struct MouseParams
{
	Mat img;
	Point pt;
	Scalar color;
	Mat my_labels;
	int int_col;
	Mat original;
};

Scalar intColor(int param) {
	if (param == -1) {
		return  Scalar(0, 71, 20);
	}
	int value = param;
	int c = 4;
	int g = int((value % c) * (255 / c));
	int b = int(((value + int(c / 3)) % c) * (255 / c));
	int r = int(((value + int(2 * c / 3)) % c) * (255 / c));
	return Scalar(r, b, g);
}

Image<Vec3b> intToColor(Mat& image) {
	//cout << "intToColorfonction" << endl;

	//cout << image.at<Scalar>(0, 0) << ' ' << endl;
	//cout << image.at<int>(0, 0) << ' ' << endl;
	int rows = image.rows;
	int cols = image.cols;
	Image<Vec3b> J(cols, rows, 16);
	if (3 == 3) {
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				
				//	cout << i << " " << j << endl;
				int value = image.at<int>(j, i) * 10000;
				if (value == -1) {
					J(i, j) = Vec3b(0, 71, 19);
					//cout << "negative" << endl;
				}
				//	cout << i << " " << j << endl;
				int c = value % (255 * 255 * 255);
				int g = c % (255);
				int b = (c - g) % (255 * 255);
				int r = (c - g - b * 255) % (255 * 255 * 255);
				J(i, j) = Vec3b(r, b, g);

			}
		}
	}
	return(J);
}

void interective_drawing(int event, int x, int y, int, void* param) {
	//cout << "interective_drawing" << endl;
	//cout << drawing << endl;
	int radius = 40;
	MouseParams* mp = (MouseParams*)param;
	Mat* imgptr = (Mat*)&(mp->img);
	Mat* m_labels = (Mat*)&(mp->my_labels);
	Mat* orig = (Mat*)&(mp->original);
	Scalar color = intColor(mp->int_col);
	//cout << mp->int_col << " " << event << " " << endl;
	if (mp->int_col > 20) {
		waitKey(1);
		//cout << "end of interective_drawing" << endl;
		return;
	}


	if (event == EVENT_LBUTTONDOWN)
	{
		//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		if (drawing == 0) {
			drawing = 1;
			old_x = x;
			old_y = y;
			//cout << "nochanges" << endl;
		}
		else {
			drawing = 0;
			//cout << "changes " << drawing << endl;

		}
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		waitKey(1);
		//cout << "end of interective_drawing" << endl;
		return;
	}
	//if (event != 1) return;     // only draw on lmouse down
	//Mat & img = (*(Mat*)imgptr); // first cast, then deref
	Point pt1 = Point(x, y);


	//imshow("Display window", *(Mat*)imgptr);
	int rows = mp->img.rows;
	int cols = mp->img.cols;



	if (drawing == 1) {
		//circle((*(Mat*)imgptr), pt1, 1, Scalar(255, 0, 0), 10, 8, 0);
		line((*(Mat*)imgptr), Point(x, y), Point(old_x, old_y), color, radius);
		line((*(Mat*)m_labels), Point(x, y), Point(old_x, old_y), mp->int_col, radius);
		line((*(Mat*)orig), Point(x, y), Point(old_x, old_y), color, radius);
		old_x = x;
		old_y = y;
	}
	//cout << "COUT " << mp->int_col << endl;
	imshow("orig", (*(Mat*)orig));
	imshow(window, (*(Mat*)imgptr));
	//imshow("Fin labels", intToColor((*(Mat*)m_labels), 4));
	waitKey(1);
	//cout << "end of interective_drawing" << endl;
	return;
}

void interactive_labeling(vector<Mat> images, Mat* labels)
{
	Mat original = images[0];
	int number = images.size();
	cout << "Number of images " << number << endl;
	cout << "You can choose a desired part in the window <<Analyse>> by drawing. " << number << endl;
	cout << "To draw push left button of a mouse. To stop push it again." <<  endl << "To choose a part of the next image, push Space" << endl;
	MouseParams mps;
	mps.original = original.clone();
	mps.my_labels = *labels;
	mps.int_col = 0;
	for (int i = 0; i < number; i++) {
		cout << i << endl;
		mps.int_col = i;
		mps.color = intColor(i);
		//cout << i << "WHAT " << endl;
		mps.img = images[i].clone();
		imshow(window, images[i]);
		setMouseCallback(window, interective_drawing, &mps);
		if (i < number) {
			waitKey();
			destroyAllWindows();
		}

	}
}

static Image<Vec3b> intToColor2(Mat& image) {
	int rows = image.rows;
	int cols = image.cols;
	Image<Vec3b> J(cols, rows, 16);
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			if (image.at<int>(j, i) == 0) {
				J(i, j) = Vec3b(0, 255, 0);
			}
			if (image.at<int>(j, i) == 1) {
				J(i, j) = Vec3b(0, 0, 255);
			}
			if (image.at<int>(j, i) == 2) {
				J(i, j) = Vec3b(255, 0, 0);
			}
			if (image.at<int>(j, i) == 3) {
				J(i, j) = Vec3b(0, 100, 100);
			}
			if (image.at<int>(j, i) == 4) {
				J(i, j) = Vec3b(100, 100, 0);
			}
			if (image.at<int>(j, i) == 5) {
				J(i, j) = Vec3b(100, 0, 100);
			}
			if (image.at<int>(j, i) == 6) {
				J(i, j) = Vec3b(200, 0, 100);
			}
			if (image.at<int>(j, i) == 7) {
				J(i, j) = Vec3b(100, 0, 200);
			}
			if (image.at<int>(j, i) == 8) {
				J(i, j) = Vec3b(100, 200, 100);
			}
			else {
				int l = image.at<int>(j, i);
				J(i, j) = Vec3b(l * 1000 % 225, l * 319 % 225, l * 19 % 225);
			}
		}
	}
	
	return(J);
}
