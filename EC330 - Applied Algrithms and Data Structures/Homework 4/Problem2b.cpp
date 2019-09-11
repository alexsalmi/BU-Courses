#include "closest.h"

int main() {

	Point pts[] = {{2, 3}, {15, 30}, {40, 25}, {6, 1}, {12, 8}, {3, 4}};
	int n = sizeof(pts) / sizeof(pts[0]);
	pair<Point, Point> p = findClosestPair(pts, n);

	cout << "The closest pair of points is: " << p.first.toString() << " and " << p.second.toString() << endl;
	cout << "The distance betwee them is: " << to_string(pairDist(p.first, p.second)) << endl;

	return 0;
}
