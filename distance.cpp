#include "distance.h"

double energieX(int i, int j, int intersection, Mat& first, Mat&second) {
	//cout << "enrgyX" << endl;
	//cout << i << " " << j << endl;
	//cout << intersection << " " << i + first.cols - intersection << endl;
	//cout << first.cols << " " << first.rows << endl;
	//return norm(Scalar(first.at<Vec3b>(i, j)) - Scalar(second.at<Vec3b>(i + first.cols - intersection, j))) + norm(Scalar(first.at<Vec3b>(i + 1, j)) - Scalar(second.at<Vec3b>(i + first.cols - intersection + 1, j)));
	return norm(Scalar(first.at<Vec3b>(j, i)) - Scalar(second.at<Vec3b>(j, i + first.cols - intersection))) + norm(Scalar(first.at<Vec3b>(j, i + 1)) - Scalar(second.at<Vec3b>(j, i + first.cols - intersection + 1)));

}

double energieY(int i, int j, int intersection, Mat& first, Mat&second) {
	//cout<< "enrgyY" << endl;
	//return norm(Scalar(first.at<Vec3b>(i, j)) - Scalar(second.at<Vec3b>(i + first.cols - intersection, j))) + norm(Scalar(first.at<Vec3b>(i, j + 1)) - Scalar(second.at<Vec3b>(i + first.cols - intersection, j + 1)));
	return norm(Scalar(first.at<Vec3b>(j, i)) - Scalar(second.at<Vec3b>(j, i + first.cols - intersection))) + norm(Scalar(first.at<Vec3b>(j+1, i)) - Scalar(second.at<Vec3b>(j, i + first.cols - intersection )));

}

double energieX(int i1, int j1, int i2, int j2, Mat& first, Mat&second) {
	// cout<< "enrgyX" << endl;
	return norm(Scalar(first.at<Vec3b>(i1, j1)) - Scalar(second.at<Vec3b>(i1, j1))) + norm(Scalar(first.at<Vec3b>(i2, j2)) - Scalar(second.at<Vec3b>(i2, j2)));
}

double energieY(int i1, int j1, int i2, int j2, Mat& first, Mat&second)
{
	// cout<< "enrgyY" << endl;
	if (i1 <= 0 || i1 >= first.rows - 1 || i2 <= 0 || i2 >= first.rows - 1 || j1 <= 0 || j1 >= first.cols - 1 || j2 <= 0 || j2 >= first.cols - 1)
	{
		return 0.;
	}
	else {
		Scalar deriveXi1_first = (Scalar(first.at<Vec3b>(i1 + 1, j1)) - Scalar(first.at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar deriveXi2_first = (Scalar(first.at<Vec3b>(i2 + 1, j2)) - Scalar(first.at<Vec3b>(i2 - 1, j2))) / 2;
		Scalar deriveXi1_second = (Scalar(second.at<Vec3b>(i1 + 1, j1)) - Scalar(second.at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar deriveXi2_second = (Scalar(second.at<Vec3b>(i2 + 1, j2)) - Scalar(second.at<Vec3b>(i2 - 1, j2))) / 2;
		Scalar deriveYj1_first = (Scalar(first.at<Vec3b>(i1, j1 + 1)) - Scalar(first.at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar deriveYj2_first = (Scalar(first.at<Vec3b>(i2, j2 + 1)) - Scalar(first.at<Vec3b>(i2, j2 - 1))) / 2;
		Scalar deriveYj1_second = (Scalar(second.at<Vec3b>(i1, j1 + 1)) - Scalar(second.at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar deriveYj2_second = (Scalar(second.at<Vec3b>(i2, j2 + 1)) - Scalar(second.at<Vec3b>(i2, j2 - 1))) / 2;
		//// cout<< "nice" << endl;
		return((norm(deriveXi1_first - deriveXi1_second, NORM_L2SQR) + norm(deriveXi2_first - deriveXi2_second, NORM_L2SQR)) + sqrt(norm(deriveYj1_first - deriveYj1_second, NORM_L2SQR) + norm(deriveYj2_first - deriveYj2_second, NORM_L2SQR)));
	}
}

double energy(int i1, int j1, int i2, int j2, Mat& first, Mat&second, double X_price, double Y_price) {
	// cout<< "enrgy0" << endl;
	return X_price * energieX(i1, j1, i2, j2, first, second) + Y_price * energieY(i1, j1, i2, j2, first, second);
}

double energy0(int i1, int j1, int i2, int j2, Mat& first, Mat&second, double X_price, double Y_price) {
	// cout<< "enrgy1" << endl;
	return X_price * energieX(i1, j1, i2, j2, first, second);
}

double distance(int i, int j, Mat& first, Mat&second) {
	//// cout<< "distance" << endl;
	//cout << first.rows << " " << first.cols << endl;
	//// cout<< i << " " << j << endl;
	//cout << norm(Scalar(first.at<Vec3b>(i, j)) - Scalar(second.at<Vec3b>(i, j))) << endl;
	return norm(Scalar(first.at<Vec3b>(i, j)) - Scalar(second.at<Vec3b>(i, j)));
}
