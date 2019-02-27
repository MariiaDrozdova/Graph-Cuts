#include "alphaexpansion.h"


double X_price = 0.1;
double Y_price = 0.1;
double val_fixe = (7 * norm(Scalar(255, 255, 255) - Scalar(0, 0, 0)));


void gauss(Mat*original, vector<Mat> the_vector, Mat& labels) {
	cout << "poisson-blending" << endl << "Wait..." << endl;
	Mat spiled; 
	original->copyTo(spiled);
	int fixed = 7;
	Mat looked = Mat::zeros(labels.rows, labels.cols, CV_32SC1);
	double r = 10;
	IplImage* or = new IplImage(*original);
	IplImage or1 ;
	vector<IplImage> the_vec;
	Mat img1, img2;
	Mat gr1, gr2;
	for (int i = 2*fixed; i < labels.cols-2*fixed-1; i++) {
		for (int j = 2*fixed; j < labels.rows-2*fixed-1; j++) {
			gr1 = *original;

			if (labels.at<int>(j, i) != labels.at<int>(j+1, i+1)) {
				int vector_p = labels.at<int>(j, i);
				if (vector_p == -1) {
					vector_p = 0;
				}
				int vector_q = labels.at<int>(j + 1, i + 1);
				if (vector_q == -1) {
					vector_q = 0;
				}
				gr2 = the_vector.at(vector_p).clone();
				gr1.convertTo(img1, CV_64FC3);
				gr2.convertTo(img2, CV_64FC3);
				int shift = fixed % 3;
				Rect A = Rect(i, j,  fixed, fixed);
				Mat result = poisson_blending(img1, img2, A, i - shift, j - shift);
				result.convertTo(result, CV_8UC1);
				Rect rc2 = Rect(i - shift, j - shift, fixed, fixed);
				Mat roimat = gr2(rc2);
				result.copyTo(roimat);
				roimat.copyTo(gr1(cv::Rect(i, j, roimat.cols, roimat.rows)));
				//j = j + fixed - shift;
			}
		}
	}
}

double vrai_energy(Mat& image, Mat& labels, Mat& real_labels) {
	double d = 0;
	double v = 0;

	for (int i = 0; i < labels.cols; i++) {
		for (int j = 0; j < labels.rows; j++) {
			double dd = 0;
			if (labels.at<int>(j, i) != real_labels.at<int>(j, i) && labels.at<int>(j, i) != -1) {
				dd = val_fixe;
			} 
			d = d + dd;
			if (j > 0) {
				// << "j "<< endl;
				v = v + energy(j - 1, i, j, i, image, image, X_price, Y_price);
			}
			if (i > 0) {
				//cout << "i " << endl;
				v = v + energy(j, i - 1, j, i, image, image, X_price, Y_price);
			}

		}
	}

	return (d+v);
}

double alpha_step(int k, Mat* original, vector<Mat> the_vector, Mat& labels, Mat* real_labels) {
	int rows = labels.rows;
	int cols = labels.cols;
	Graph<double, double, double> alpha_g(2 * rows * cols, 4 * rows * cols);
	cout << "graph creating for " << k << endl;
	int new_nodes = 0;
	alpha_g.add_node(3 * rows*cols);
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < cols; i++)
		

		{

			double curr_price = 0;// //INFINITY;
			double alpha_price = 0;//0;
			if (labels.at<int>(j, i) != -1)
			{
				if (labels.at<int>(j, i) != k) {
					//		cout << "in1" << endl;
					int q = labels.at<int>(j, i);
					alpha_price = val_fixe;

				}
				else {
					//			cout << "in2" << endl;
					alpha_price = 0;//distance(j, i, *original, the_vector.at(q));

				}
				if (labels.at<int>(j, i) == k) {
					curr_price = val_fixe;
				}
				else {
					curr_price = 0;
				}
			}
			//	cout << "adding" << endl;
			alpha_g.add_tweights(i + j * cols, curr_price, alpha_price);
		}

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < cols; i++)
		{
			Mat* inp = real_labels;
			if (i > 0)
			{
				double cost = 0;
				cost = X_price * energieX(j, i - 1, j, i, *original, the_vector.at(k)) + X_price * energieY(j, i - 1, j, i, *original, the_vector.at(k));
				int q = -1;
				int p = -1;
				Mat* im_q = &(the_vector.at(inp->at<int>(j, i)));
				Mat* im_k = &(the_vector.at(k));
				Mat* im_p = &(the_vector.at(inp->at<int>(j, i - 1)));

				if (inp->at<int>(j, i) == inp->at<int>(j, i - 1)) {
					q = inp->at<int>(j, i);

					cost = energy(j, i - 1, j, i, *im_q, *im_k, X_price, Y_price);
					alpha_g.add_edge((i - 1) + j * cols, i + j * cols, cost, cost);
				}

				if (inp->at<int>(j, i) != inp->at<int>(j, i - 1)) {

					q = inp->at<int>(j, i);
					p = inp->at<int>(j, i - 1);
					cost = energy(j, i - 1, j, i, *im_q, *im_k, X_price, Y_price); ;
					alpha_g.add_edge((i - 1) + j * cols, rows*cols + new_nodes, cost, cost);

					cost = energy(j, i - 1, j, i, *im_q, *im_k, X_price, Y_price);
					alpha_g.add_edge(rows*cols + new_nodes, i + j * cols, cost, cost);

					cost = energy(j, i - 1, j, i, *im_q, *im_k, X_price, Y_price);
					alpha_g.add_tweights(rows*cols + new_nodes, 0, cost);
					new_nodes++;
				}
			}
			if (j > 0)
			{
				double cost = 0;
				cost = X_price * energieX(j - 1, i, j, i, *original, the_vector.at(k)) + X_price * energieY(j - 1, i, j, i, *original, the_vector.at(k));
				Mat* im_q = &(the_vector.at(inp->at<int>(j, i)));
				Mat* im_k = &(the_vector.at(k));
				Mat* im_p = &(the_vector.at(inp->at<int>(j - 1, i)));
				int q = -1;
				int p = -1;

				if (inp->at<int>(j, i) == inp->at<int>(j - 1, i)) {
					q = inp->at<int>(j, i);
					cost = energy(j - 1, i, j, i, *im_q, *im_k, X_price, Y_price);
					alpha_g.add_edge((i)+(j - 1) * cols, i + (j)* cols, cost, cost);
				}

				if (inp->at<int>(j, i) != inp->at<int>(j - 1, i)) {
					q = inp->at<int>(j, i);
					p = inp->at<int>(j - 1, i);
					cost = energy(j - 1, i, j, i, *im_p, *im_k, X_price, Y_price);
					alpha_g.add_edge((i)+(j - 1) * cols, rows*cols + new_nodes, cost, cost);

					cost = energy(j - 1, i, j, i, *im_q, *im_k, X_price, Y_price);
					alpha_g.add_edge(rows*cols + new_nodes, i + j * cols, cost, cost);

					cost = energy(j - 1, i, j, i, *im_p, *im_q, X_price, Y_price);
					alpha_g.add_tweights(rows*cols + new_nodes, 0, cost);
					new_nodes++;
				}
			}
		}
	double energy = alpha_g.maxflow();

	if (energy != 0) {

		for (int node_id = 0; node_id < rows*cols; node_id++)
		{
			int ii = node_id % cols;
			int jj = node_id / cols;
			if (alpha_g.what_segment(node_id) != Graph<double, double, double>::SINK)
			{
				if (node_id < rows*cols) {
					real_labels->at<int>(jj, ii) = k;
					(*original).at<Vec3b>(jj, ii) = the_vector.at(k).at<Vec3b>(jj, ii);
				}
				else {
					cout << ii << " " << jj << endl;
				}

			}


		}
		energy = vrai_energy(*original, labels, *real_labels);
		cout << "real energy : " << energy << endl;
	}
	return energy;
}




double alpha_step2(int k, Mat* original, vector<Mat> the_vector, Mat& labels, Mat* real_labels) {
	int rows = labels.rows;
	int cols = labels.cols;
	Graph<double, double, double> alpha_g(rows * cols, 2 * rows * cols);
	cout << "graph creating for " << k << endl;
	int new_nodes = 0;
	alpha_g.add_node(rows*cols);
	for (int j = 0; j < rows; j++)
		for (int i = 0; i < cols; i++)


		{
			double curr_price = 0;
			double alpha_price = 0;
			if (labels.at<int>(j, i) != -1)
			{
				if (labels.at<int>(j, i) != k) {
					alpha_price = val_fixe;
				}
				if (labels.at<int>(j, i) != real_labels->at<int>(j, i)) {
					curr_price = val_fixe;
				}
			}
			alpha_g.add_tweights(i + j * cols, curr_price, alpha_price);
		}

	for (int j = 0; j < rows; j++)
		for (int i = 0; i < cols; i++)
		{
			Mat* inp = real_labels;
			if (i > 0)
			{
				double cost = energy(j, i - 1, j, i, *original, the_vector.at(k), X_price, Y_price);
				alpha_g.add_edge((i - 1) + j * cols, i + j * cols, cost, cost);

			}
			if (j > 0)
			{
				double cost = energy(j - 1, i, j, i, *original, the_vector.at(k), X_price, Y_price);
				alpha_g.add_edge((i)+(j - 1)*cols, i + j * cols, cost, cost);
			}
		}
	double energy = alpha_g.maxflow();
	if (energy != 0) {

		for (int node_id = 0; node_id < rows*cols; node_id++)
		{
			int ii = node_id % cols;
			int jj = node_id / cols;
			if (alpha_g.what_segment(node_id) != Graph<double, double, double>::SINK)
			{
				if (node_id < rows*cols) {
					real_labels->at<int>(jj, ii) = k;
					(*original).at<Vec3b>(jj, ii) = the_vector.at(k).at<Vec3b>(jj, ii);
				}
				else {
					cout << ii << " " << jj << endl;
				}

			}


		}
	}
	return energy;
}


alpha alphamarche(String s) {
	//imread("meandremi1.jpg");
	vector<Mat> the_vector = open(s);
	Image<Vec3b> original = the_vector[0].clone();
	int number_pictures = the_vector.size();
	int rows = the_vector[0].rows;
	int cols = the_vector[0].cols;
	Mat real_labels = Mat::zeros(rows, cols, CV_32SC1);
	Mat* labels = new Mat((-1) * Mat::ones(rows, cols, CV_32SC1));


	interactive_labeling(the_vector, labels);
	imshow("YourLabels", intToColor(*labels));
	waitKey();

	int start = 0;
	int fin = 0;
	int new_nodes = 0;
	bool flag = true;
	double dif = 5;
	double energy_memory = 0;

	do {
		for (int k = 0; k < number_pictures; k++) {
			double energy = alpha_step(k, &original, the_vector, *labels, &real_labels);


			if (k == the_vector.size() - 1) {
				fin = energy;
				dif = fin - energy_memory;
				energy_memory = fin;
			}			
		}

	} while ((abs(dif)) >= 0.5);
	alpha finish;
		
	imshow("original", original);
	imshow("labels", intToColor(real_labels));
	waitKey();
	cout << "Do you want to do blending? Y/n" << endl;
	char c;
	cin >> c;
	if (c == 'Y') {
		gauss(&original, the_vector, real_labels);
	}
	finish.labels = labels;
	finish.real_labels = real_labels;
	finish.original = original;
	return finish;
}


alpha alphamarcheOrder(String s, Mat* labels) {
	//imread("meandremi1.jpg");
	vector<Mat> the_vector = open(s);
	Image<Vec3b> original = the_vector[0].clone();
	int number_pictures = the_vector.size();
	int rows = the_vector[0].rows;
	int cols = the_vector[0].cols;
	Mat real_labels = Mat::zeros(rows, cols, CV_32SC1);
	std::vector<int> myvector;

	// set some values:
	for (int i = 0; i<number_pictures; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

													  // using built-in random generator:
	std::random_shuffle(myvector.begin(), myvector.end());
	// print out content:
	std::cout << "myvector contains:";
	for (int kk = 0; kk < number_pictures; kk++)
		std::cout << ' ' << myvector[kk];

	std::cout << '\n';
	int start = 0;
	int fin = 0;
	int new_nodes = 0;
	bool flag = true;
	double dif = 5;
	double energy_memory = 0;
	
	do {
		for (int kk = 0; kk < number_pictures; kk++) {
			int k = myvector.at(kk);
			double energy = alpha_step(k, &original, the_vector, *labels, &real_labels);
			cout << k << endl;

			if (kk == the_vector.size() - 1) {
				fin = energy;
				dif = fin - energy_memory;
				energy_memory = fin;
			}
		}

	} while ((abs(dif)) >= 0.5);
	alpha finish;

	imshow("original", original);
	imshow("labels", intToColor(real_labels));
	waitKey();
	cout << "Do you want to do blending? Y/n" << endl;
	char c;
	cin >> c;
	if (c == 'Y') {
		gauss(&original, the_vector, real_labels);
	}
	finish.labels = labels;
	finish.real_labels = real_labels;
	finish.original = original;
	return finish;
}