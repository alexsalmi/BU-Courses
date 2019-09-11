// Author: Alexander Salmi
// BUID: UXXXXXXXX

// your #includes here
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

struct Point {
	int x, y;
	Point() {}
	Point(int x_val, int y_val) : x(x_val), y(y_val) {}
	string toString() {return "(" + to_string(x) + ',' + to_string(y) + ")";}
};

// A utility function to compute the distance between two points
float pairDist(Point p1, Point p2)
{
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) );
}

pair<Point, Point> findClosestPair(Point pts[], int n) {
	// If n=3, compare the 3 points and return the smallest
	if(n==3){
		float dist1 = pairDist(pts[0], pts[1]);
		float dist2 = pairDist(pts[1], pts[2]);
		float dist3 = pairDist(pts[2], pts[0]);

		if(dist1 <= dist2 && dist1 <= dist3)
			return make_pair(pts[0], pts[1]);
		else if(dist2 <= dist1 && dist2 <= dist3)
			return make_pair(pts[1], pts[2]);
		else
			return make_pair(pts[2], pts[0]);
	}
	// If n=2, return the two points
	if(n==2){
		return make_pair(pts[0], pts[1]);
	}
	int i, j;

	// Sort the points by x coordinate
	sort(pts, pts + n, [](auto &a, auto &b) {
    	return a.x < b.x;});

	// Create two subarrays for the two halves
	Point ptsL[n/2];
	Point ptsR[n/2];
	for(i=0; i<n/2; i++){
		ptsL[i] = pts[i];
		ptsR[i] = pts[n/2+i];
	}

	// Recursively sort the two halves
	pair<Point, Point> L = findClosestPair(ptsL, n/2);
	pair<Point, Point> R = findClosestPair(ptsR, n/2);

	// Set d to be the shorter of the distances found from the two halves
	float d = min(pairDist(L.first, L.second), pairDist(R.first, R.second));

	// Create a vector containing all the points that are within d from the center point, eliminating the ones further away
	std::vector<Point> newPts;
	int x = pts[n/2].x;
	for(i=n; i>=0; i--){
		if(pts[i].x > x-d || pts[i].x < x+d)
			newPts.push_back(pts[i]);
	}

	// Sort the new vector by y coordinate
	sort(newPts.begin(), newPts.end(), [](auto &a, auto &b) {
    	return a.y < b.y;});

	// Find the closest two remaining points, comparing each point with the subsequent 7
	float pairD, closest = pairDist(newPts[0], newPts[1]);
	pair<Point, Point> M = make_pair(newPts[0], newPts[1]);
	for(i=0; i<newPts.size()-2; i++){
		for(j=i+1; j<i+7; j++){
			pairD = pairDist(newPts[i], newPts[j]);
			if(pairD < closest){
				M = make_pair(newPts[i], newPts[j]);
				closest = pairD;
			}
			if(j+1 == newPts.size())
				break;
		}
	}

	// Find and return the shortest distance out of L, R, and M
	float distL = pairDist(L.first, L.second);
	float distR = pairDist(R.first, R.second);
	float distM = pairDist(M.first, M.second);
	if(distL <= distR && distL <= distM)
		return L;
	else if(distR <= distL && distR <= distM)
		return R;
	else
		return M;
	 // modify the return value appropriately in your implementation
}

/*
	Auxillary/utility functions, if you need to implement any, go here.
*/
