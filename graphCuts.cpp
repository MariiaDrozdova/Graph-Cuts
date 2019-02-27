#include <iostream>
#include <fstream>
#include <stdio.h>
#include "image.h"
#include "distance.h"
#include "unitedimages.h"
#include "labelings.h"
#include "alphaexpansion.h"
#include "openfiles.h"

bool FileExists(const char *fname)
{
	fstream f(fname, ios::in);
	return (!f.is_open());
}

using namespace std;
void shortlongtest(string s = "long.jpg") {
	string st = "..\\pictures\\" + s;
	const char* fname = st.c_str();
	if (FileExists(fname)) {
		cout << "Can't find your file " << "..\\pictures\\" << s << endl;
		return;
	}
	Image<Vec3b> J = imread("..\\pictures\\" + s);
	imshow("J", J);
	cout << "Real length is " << J.cols << endl;
	cout << "How much longer you would like to make new image? Enter a number from 1.1 to 1.9" << endl;
	double coef = 1.3;
	cin >> coef;
	if (coef < 1.1 || coef > 1.9) {
		cout << "default is used: 1.5" << endl;
		coef = 1.5;
	}
	int willingLength = int(coef * J.cols);
	cout << "Willing length is " << willingLength << " = " << coef << " * " << J.cols << endl;
	waitKey();
	Image<Vec3b> J0 = produceLongImage(J, willingLength);
	imshow("2images", J0);
	cout << "press SPACE" << endl;
	waitKey();
}

void shortlongstructuredtest(string s = "nuts6.png") {
	string st = "..\\pictures\\" + s;
	const char* fname = st.c_str();
	if (FileExists(fname)) {
		cout << "Can't find your file " << "..\\pictures\\" << s << endl;
		//return;
	}
	Image<Vec3b> I = imread("..\\pictures\\" + s);
	imshow("StructuredImage", I);
	cout << "Please choose the direction of fusion: 0 for horizontal and 1 for vertical:" << endl;
	int direction = 0;
	cin >> direction;
	cout << "Please, wait..." << endl;
	Image<Vec3b> J = produceLongStructuredImage(I, direction);
	imshow("LongStructuredImageResult", J);
	cout << "press SPACE" << endl;
	waitKey();
}

void photomontage() {
	vector<Mat> ims;
	char c;
	String task;
	cout << "Choose a folder : " << endl;
	cout << "a - for <<ceremonie>>" << endl;
	cout << "b - for <<cathedral>>" << endl;
	cout << "c - for <<family>>" << endl;
	cout << "d - for <<test>>" << endl;
	cout << "default: for returning back" << endl;
	cin >> c;
	if (c == 'a') {
		task = "ceremonie";
	} else 
		if (c == 'b') {
			task = "cathedral";
		} else 
			if (c == 'c') {
				task = "family";
			} else
				if (c == 'd') {
					task = "test";
				}
				else {
					return;
				}
	alpha fin = alphamarche(task);
	imshow("result", fin.original);
	cout << "press SPACE" << endl;
	waitKey();
	alpha new_fin;
	while (true) {
		cout << "Do you want to continue with the chosen labels?(change order of alphas) Y/n" << endl;
		cin >> c;
		if (c == 'Y') {
			
		
			Mat* labels = (fin.labels);
			new_fin = alphamarcheOrder(task, labels);
			imshow("resultOrder", new_fin.original);
			cout << "press SPACE" << endl;
			waitKey();
		}
		else {
			return;
		}
	}
	return;
	
}
int main() {
	cout << "Hello ! ";
	while (true) {

		cout << "Please, choose what you would like to do with help of this program" << endl;
		cout << "Enter:" << endl;
		cout << "A - if you want to make a long photo from the short one containing a lot of small NOT structured objects." << endl;
		cout << "B - if you want to make a long photo from the short one containing a lot of small structured objects." << endl;
		cout << "C - if you want to try photomontage." << endl;
		cout << "D - if you want to EXIT." << endl;
		char c;
		cin >> c;
		if (c == 'A') {
			cout << "If you want to look at the example, press N." << endl << "If you want to make modifications with your own image, add it to the folder <<pictures>> and press its name(name.jpg) here. " << endl;
			string s;
			cin >> s;
			if (s == "N") {
				shortlongtest();
			}
			else {
				shortlongtest(s);
			}
		}
		else if (c == 'B') {
			cout << "If you want to look at the example, press N." << endl << "If you want to make modifications with your own image, add it to the folder <<pictures>> and press its name(name.jpg)  here. " << endl;
			string s;
			cin >> s;
			if (s == "N") {
				shortlongstructuredtest();
			}
			else {
				shortlongstructuredtest(s);
			}
		} else
		if (c == 'C') {
			photomontage();
		} else 
			if (c == 'D') {
				return 0;
			}
		else {
			cout << "Error. Try to choose again." << endl;
		}
		cout << "closing windows" << endl;
		cout << "press SPACE" << endl;
		waitKey();
		destroyAllWindows();
	}
	return 0;
}