#include "unitedimages.h"

Image<Vec3b> produceLongImage(Image<Vec3b>& I, int willingLength) {
	
	if (willingLength < I.width()) {
		cout << "Error. You want to make it smaller." << endl;
		return I;
	}
	int intersection = 2*(I.width()) - willingLength;
	int shift = I.width() - intersection;
	Graph<double, double, double> graph(I.height()*intersection, 2 * I.height()*intersection);
	graph.add_node(I.height()*intersection);
	for (int i = 0; i<intersection; i++) {
		for (int j = 0; j<I.height()-1; j++) {
			int i2 = i + I.width() - intersection;
			if (i == 0) {
				graph.add_tweights(i + j * intersection, INFINITY, 0);
			}
			else if (i == intersection - 1) graph.add_tweights(i + j * intersection, 0, INFINITY);
			else graph.add_tweights(i + j * intersection, 0, 0);
			if (i < intersection - 1) {
				//double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j))) + norm(Scalar(I(i + 1, j)) - Scalar(I(i2 + 1, j)));
				double m = energieX(i, j, intersection, I, I);
				graph.add_edge(i + j * intersection, (i + 1) + j * intersection, m, m);
			}
			if (j < I.height() - 1) {
				//double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j))) + norm(Scalar(I(i, j + 1)) - Scalar(I(i2, j + 1)));
				double m = energieY(i, j, intersection, (Mat)I, (Mat)I);
				
				graph.add_edge(i + j * intersection, i + (j + 1)*intersection, m, m);
			}
		}
	}

	double flow = graph.maxflow();
	Image<Vec3b> J(willingLength, I.height(), I.type());
	Image<Vec3b> J2(willingLength, I.height(), I.type());

	for (int i = 0; i < J.width(); i++) {
		for (int j = 0; j < J.height(); j++) {
			if (i < I.width() - intersection) {
				J(i, j) = I(i, j);
				J2(i, j) = I(i, j);
			}
			else if (i >= I.width()) {
				J(i, j) = I(i - I.width() + intersection, j);
				J2(i, j) = J(i, j);
			}
			else {
				J(i, j) = (graph.what_segment((i - I.width() + intersection) + j * intersection) == Graph<double, double, double>::SOURCE) ? I(i, j) : I(i - I.width() + intersection, j);
				J2(i, j) = J(i, j);
				if (i > I.width() - intersection && graph.what_segment((i - I.width() + intersection) + j * intersection) != graph.what_segment((i - 1 - I.width() + intersection) + j * intersection)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
				if (j > 0 && graph.what_segment((i - I.width() + intersection) + j * intersection) != graph.what_segment((i - I.width() + intersection) + (j - 1) *intersection)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
			}
		}
	}
	return J2;

}



struct point {
	int h;
	int v;
	point(int h, int v) {
		this->h = h;
		this->v = v;
	}
	bool operator < (const point& p) const
	{
		if (this->h == p.h) return this->v < p.v;
		else return this->h < p.h;
	}
};


float matchScore(const Image<Vec3b>& I, point placement) {
	int C = 0;
	for (int i = 0; i < I.width() - placement.h; i++) {
		for (int j = 0; j < I.height() - placement.v; j++) {
			C = C + norm(Scalar(I(i, j)) - Scalar(I(i + placement.h, j + placement.v)));
		}
	}
	return C / (float)((I.width() - placement.h)*(I.height() - placement.v));
}

point placementSearch(const Image<Vec3b>& I, float hl, float hr, float vt, float vb) {
	int leftline = hl * I.width(), rightline = hr * I.width();
	int topline = vt * I.height(), bottomline = vb * I.height();
	int h = leftline, v = topline;
	int besth = h, bestv = v;
	float bestC = INFINITY, C = 0;
	for (h = leftline; h < rightline; h++) {
		for (v = topline; v < bottomline; v++) {
			C = matchScore(I, point(h, v));
			if (C < bestC) {
				besth = h;
				bestv = v;
				bestC = C;
				//cout << h << " " << v << " " << bestC << endl;
			}
		}
	}
	return point(besth, bestv);
}


point placementSearch2(const Image<Vec3b>& I, float hl, float hr, float vt, float vb) {
	int leftline = hl * I.width(), rightline = hr * I.width();
	int topline = vt * I.height(), bottomline = vb * I.height();
	int h = leftline, v = topline;
	int besth = h, bestv = v;
	point placement(h, v);
	float bestC = matchScore(I, placement);
	map<point, float> m;
	m[placement] = bestC;
	bool findBetter, jump;
	do {
		jump = false;
		do {
			findBetter = false;
			for (int h2 = h - 6; h2 <= h + 6; h2++) {
				for (int v2 = v - 6; v2 <= v + 6; v2++) {
					cout << h << " " << v << "" << h2 << " " << v2 << endl;
					point placement(h2, v2);
					if (m.find(placement) != m.end() || h2 < leftline || h2 >= rightline || v2 < topline || v2 >= bottomline) { continue; }
					m[placement] = matchScore(I, placement);
					if (m[placement] < bestC) {
						besth = h2;
						bestv = v2;
						bestC = m[placement];
						findBetter = true;
					}
				}
			}
			h = besth; v = bestv;
		} while (findBetter);
		for (int h2 = h - 12; h2 <= h + 12; h2++) {
			for (int v2 = v - 12; v2 <= v + 12; v2++) {
				point placement(h2, v2);
				if (m.find(placement) != m.end() || h2 < leftline || h2 >= rightline || v2 < topline || v2 >= bottomline) continue;
				m[placement] = matchScore(I, placement);
				if (m[placement] < bestC) {
					besth = h2;
					bestv = v2;
					bestC = m[placement];
					jump = true;
				}
			}
		}
		h = besth; v = bestv;
	} while (jump);

	return point(besth, bestv);
}

void fusionH(const Image<Vec3b>& I, Image<Vec3b>& J, Image<Vec3b>& J2, point placement) {
	int overlap_width = I.width() - placement.h, overlap_height = I.height() - placement.v;
	Graph<double, double, double> G(overlap_width * overlap_height, 2 * overlap_width * overlap_height);
	G.add_node(overlap_width * overlap_height);
	for (int i = 0; i < overlap_width; i++) {
		for (int j = 0; j < overlap_height; j++) {
			int i2 = i + placement.h, j2 = j + placement.v;
			if (i == 0) G.add_tweights(i + j * overlap_width, INFINITY, 0);
			else if (i == overlap_width - 1) G.add_tweights(i + j * overlap_width, 0, INFINITY);
			else G.add_tweights(i + j * overlap_width, 0, 0);
			if (i < overlap_width - 1) {
				double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j2))) + norm(Scalar(I(i + 1, j)) - Scalar(I(i2 + 1, j2)));
				G.add_edge(i + j * overlap_width, (i + 1) + j * overlap_width, m, m);
			}
			if (j < overlap_height - 1) {
				double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j2))) + norm(Scalar(I(i, j + 1)) - Scalar(I(i2, j2 + 1)));
				G.add_edge(i + j * overlap_width, i + (j + 1) * overlap_width, m, m);
			}
		}
	}

	double flow = G.maxflow();
	cout << "flow=" << flow << endl;

	Image<Vec3b> K(placement.h + I.width(), overlap_height, I.type());
	Image<Vec3b> K2(placement.h + I.width(), overlap_height, I.type());
	J = K; J2 = K2;

	for (int i = 0; i < J.width(); i++) {
		//cout << i << endl; waitKey();
		for (int j = 0; j < J.height(); j++) {
			if (i < placement.h) {
				J(i, j) = I(i, j + placement.v);
				J2(i, j) = J(i, j);
			}
			else if (i >= I.width()) {
				J(i, j) = I(i - placement.h, j);
				J2(i, j) = J(i, j);
			}
			else {
				J(i, j) = (G.what_segment((i - placement.h) + j * overlap_width) == Graph<double, double, double>::SOURCE) ? I(i, j + placement.v) : I(i - placement.h, j);
				J2(i, j) = J(i, j);
				if (i > placement.h && G.what_segment((i - placement.h) + j * overlap_width) != G.what_segment((i - 1 - placement.h) + j * overlap_width)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
				if (j > 0 && G.what_segment((i - placement.h) + j * overlap_width) != G.what_segment((i - placement.h) + (j - 1) * overlap_width)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
			}
		}
	}
}

void fusionV(const Image<Vec3b>& I, Image<Vec3b>& J, Image<Vec3b>& J2, point placement) {
	int overlap_width = I.width() - placement.h, overlap_height = I.height() - placement.v;
	Graph<double, double, double> G(overlap_width * overlap_height, 2 * overlap_width * overlap_height);
	G.add_node(overlap_width * overlap_height);
	for (int i = 0; i < overlap_width; i++) {
		for (int j = 0; j < overlap_height; j++) {
			int i2 = i + placement.h, j2 = j + placement.v;
			if (j == 0) G.add_tweights(i + j * overlap_width, INFINITY, 0);
			else if (j == overlap_height - 1) G.add_tweights(i + j * overlap_width, 0, INFINITY);
			else G.add_tweights(i + j * overlap_width, 0, 0);
			if (i < overlap_width - 1) {
				double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j2))) + norm(Scalar(I(i + 1, j)) - Scalar(I(i2 + 1, j2)));
				G.add_edge(i + j * overlap_width, (i + 1) + j * overlap_width, m, m);
			}
			if (j < overlap_height - 1) {
				double m = norm(Scalar(I(i, j)) - Scalar(I(i2, j2))) + norm(Scalar(I(i, j + 1)) - Scalar(I(i2, j2 + 1)));
				G.add_edge(i + j * overlap_width, i + (j + 1) * overlap_width, m, m);
			}
		}
	}

	double flow = G.maxflow();
	cout << "flow=" << flow << endl;

	Image<Vec3b> K(overlap_width, placement.v + I.height(), I.type());
	Image<Vec3b> K2(overlap_width, placement.v + I.height(), I.type());
	J = K; J2 = K2;

	for (int i = 0; i < J.width(); i++) {
		//cout << i << endl; waitKey();
		for (int j = 0; j < J.height(); j++) {
			if (j < placement.v) {
				J(i, j) = I(i + placement.h, j);
				J2(i, j) = J(i, j);
			}
			else if (j >= I.height()) {
				J(i, j) = I(i, j - placement.v);
				J2(i, j) = J(i, j);
			}
			else {
				J(i, j) = (G.what_segment(i + (j - placement.v) * overlap_width) == Graph<double, double, double>::SOURCE) ? I(i + placement.h, j) : I(i, j - placement.v);
				J2(i, j) = J(i, j);
				if (j > placement.v && G.what_segment(i + (j - placement.v) * overlap_width) != G.what_segment(i + (j - 1 - placement.v) * overlap_width)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
				if (i > 0 && G.what_segment(i + (j - placement.v) * overlap_width) != G.what_segment((i - 1) + (j - placement.v) * overlap_width)) {
					J(i, j) = Vec3b(0, 255, 0);
				}
			}
		}
	}
}

Image<Vec3b> produceLongStructuredImage(Image<Vec3b>& I, int direction)//direction = 0: horizontal, direction = 1: vertical
{
	Image<Vec3b> J;
	Image<Vec3b> J2;
	if (direction == 0) {
		point placement = placementSearch(I, 0.5, 0.7, 0.0, 0.2);
		cout << "placement, x: " << placement.h << " y: " << placement.v << endl;

		fusionH(I, J, J2, placement);
		return J2;
	}
	else {
		point placement = placementSearch(I, 0.0, 0.2, 0.5, 0.7);
		cout << "placement, x: " << placement.h << " y: " << placement.v << endl;

		fusionV(I, J, J2, placement);
		return J2;
	}
}
