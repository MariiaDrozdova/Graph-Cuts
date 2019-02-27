#pragma once
#include "image.h"
#include <iostream>
#include <fstream>
double energieX(int i, int j, int intersection, Mat& first, Mat&second);
double energieY(int i, int j, int intersection, Mat& first, Mat&second);

double energieX(int i1, int j1, int i2, int j2, Mat& first, Mat&second);
double energieY(int i1, int j1, int i2, int j2, Mat& first, Mat&second);
	

double energy0(int i1, int j1, int i2, int j2, Mat& first, Mat&second, double X_price, double Y_price);
double energy(int i1, int j1, int i2, int j2, Mat& first, Mat&second, double X_price, double Y_price);

double distance(int i, int j, Mat& first, Mat&second);
