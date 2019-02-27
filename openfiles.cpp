#include "openfiles.h";

vector<Mat> open(String s) {
	vector<Mat> the_vector;

	if (s.find("family") >= 0 && s.find("family") < s.size()) {
		cout << "Family" << endl;
		Mat alpha1 = imread("..\\pictures\\family\\family1.jpg");
		Mat alpha2 = imread("..\\pictures\\family\\family2.jpg");
		Mat alpha3 = imread("..\\pictures\\family\\family3.jpg");
		Mat alpha4 = imread("..\\pictures\\family\\family4.jpg");
		Mat alpha5 = imread("..\\pictures\\family\\family5.jpg");
		the_vector.push_back(alpha1);
		the_vector.push_back(alpha2);
		the_vector.push_back(alpha3);
		the_vector.push_back(alpha4);
		the_vector.push_back(alpha5);
		return (the_vector);
	}
	if (s.find("ceremonie") >= 0 && s.find("ceremonie") < s.size()) {
		cout << "Ceremonie photos" << endl;
		Mat alpha1 = imread("..\\pictures\\ceremonie\\meandremi1.jpg");
		Mat alpha2 = imread("..\\pictures\\ceremonie\\meandremi2.jpg");
		Mat alpha3 = imread("..\\pictures\\ceremonie\\meandremi3.jpg");
		the_vector.push_back(alpha1);
		the_vector.push_back(alpha2);
		the_vector.push_back(alpha3);
		return (the_vector);
	}
	if (s.find("cathedral") >= 0 && s.find("cathedral") < s.size()) {
		cout << "Cathedral" << endl;
		Mat alpha1 = imread("..\\pictures\\cathedral\\cathedral1.jpg");
		Mat alpha2 = imread("..\\pictures\\cathedral\\cathedral2.jpg");
		Mat alpha3 = imread("..\\pictures\\cathedral\\cathedral3.jpg");
		Mat alpha4 = imread("..\\pictures\\cathedral\\cathedral4.jpg");
		Mat alpha5 = imread("..\\pictures\\cathedral\\cathedral5.jpg");
		the_vector.push_back(alpha1);
		the_vector.push_back(alpha2);
		the_vector.push_back(alpha3);
		the_vector.push_back(alpha4);
		the_vector.push_back(alpha5);
		return (the_vector);
	}
	if (s.find("cathedral") >= 0 && s.find("cathedral") < s.size()) {
		cout << "Cathedral" << endl;
		Mat alpha1 = imread("..\\pictures\\cathedral\\cathedral1.jpg");
		Mat alpha2 = imread("..\\pictures\\cathedral\\cathedral2.jpg");
		Mat alpha3 = imread("..\\pictures\\cathedral\\cathedral3.jpg");
		Mat alpha4 = imread("..\\pictures\\cathedral\\cathedral4.jpg");
		Mat alpha5 = imread("..\\pictures\\cathedral\\cathedral5.jpg");
		the_vector.push_back(alpha1);
		the_vector.push_back(alpha2);
		the_vector.push_back(alpha3);
		the_vector.push_back(alpha4);
		the_vector.push_back(alpha5);
		return (the_vector);
	}
	if (s.find("test") >= 0 && s.find("test") < s.size()) {
		Mat alpha1 = imread("..\\pictures\\test\\white.png");
		Mat alpha2 = imread("..\\pictures\\test\\yellow.png");
		Mat alpha3 = imread("..\\pictures\\test\\black.png");
		Mat alpha4 = imread("..\\pictures\\test\\red.png");
		the_vector.push_back(alpha1);
		the_vector.push_back(alpha2);
		the_vector.push_back(alpha3);
		the_vector.push_back(alpha4);
		return (the_vector);
	}
	cout << "Error: there is no folder " << s << endl;
}