// Author: Alexander Salmi
// BUID: UXXXXXXX
// LATE DAY SUBMISSION

// your #includes here
#include <cmath> //Used for min() and max()
#include <vector> //Used for vector operations
#include <unordered_map> //for part (b), to store points that are on same line
#include <iostream>
using namespace std;

struct Point {
	int x, y;
	Point() {}
	Point(int x_val, int y_val) : x(x_val), y(y_val) {}
};

// Struct to store lines for part (b). I used this as a key for my unordered maps.
// I read up on how to make a struct a key for a map, including how to create a hash function, from the following link:
// http://www.techiedelight.com/use-struct-key-std-unordered_map-cpp/
template<typename T1, typename T2>
struct Line {
	T1 slope;
	T2 yInt;
	Line() {}
	Line(double slopeVal, double yIntVal) : slope(slopeVal), yInt(yIntVal) {}

	bool operator==(const Line &x) const{
		return slope == x.slope && yInt == x.yInt;
	}
};

struct hash_fn{
	template<class T1, class T2>
	std::size_t operator() (const Line<T1, T2> &line) const{
		std::size_t h1 = std::hash<T1>()(line.slope);
		std::size_t h2 = std::hash<T2>()(line.yInt);
		return h1*h2;
	}

};

/*
The idea behind this algorithm is to find indeces i1 and i2
for vectors v1 and v2 such that half of the overall elements come
before the indices, and half come after. That way, the median becomes
the largest value in the first half of values, or the average of the
largest value in the first half and the smallest value in the second half.
This means we halve to find iterators such that the last element in v1 in the
first half is smaller than the first element in v2 in the second half, and vice versa
O(min(n,m))
*/
float findMedian(vector<int>& v1, vector<int>& v2) {
	// This algorithm only works if v1 is smaller than v2
	if(v2.size() < v1.size()){
		return findMedian(v2, v1);
	}

	int s1 = v1.size(), s2 = v2.size();
	int minI = 0, maxI = s1, i1, i2, firstHalfSmallest;

	while(minI <= maxI){
		i1 = (minI + maxI) / 2;
		i2 = ((s1 + s2 + 1)/ 2) - i1;

		// If the last value from v1 in the first half is larger
		// than the first value from v2 in the second half, we need to
		// move the iterator
		if(v2[i2-1] > v1[i1] && i1<s1 && i2>0)
			minI = i1 + 1;

		// If the last value from v2 in the first half is larger
		// than the first value from v1 in the second half, we need to
		// move the iterator
		else if(v1[i1-1] > v2[i2] && i2<s2 && i1>0)
			maxI = i1 - 1;

		// If this is reached, we have found the correct two halves, such
		// that they are split equally, and all the values in the first half
		// are less than all the values in the second half
		else{
			// If i1 is 0, this means none of the values from v1 are in the
			// first half, so the median is the largest element from v2 in
			// the first half
			if(i1 == 0)
				firstHalfSmallest = v2[i2-1];

			// If i2 is 0, none of the values from v2 are in the first half,
			// so the median is the largest element from v1 in the first half
			else if(i2 == 0)
				firstHalfSmallest = v1[i1-1];

			// Otherwise, the median will be the largest value in the first half,
			// which is the max of the largest values form both vectors in the first
			// half
			else
				firstHalfSmallest = max(v1[i1-1], v2[i2-1]);

			break;
		}
	}

	// If the total number of values is odd, return the median(last element in
	// the first half)
	if((s1+s2)%2 == 1){
		return firstHalfSmallest;
	}
	// If none of the values from v1 are in the first half, return the average
	// of the last element from the first half and the first element from the
	// second half (smallest value of v2 in second half)
	else if(i1 == s1)
		return (firstHalfSmallest + v2[i2]) / 2.0;

	// If none of the values from v2 are in the first half, return the average
	// of the last element from the first half and the first element from the
	// second half (smallest value of v1 in second half)
	else if(i2 == s2)
		return (firstHalfSmallest + v1[i1]) / 2.0;

	// Otherwise, return the average of the last element from the first half
	// and the first element of the second half, which will be the min of the
	// smallest value from ech vector in the second half
	else
		return (firstHalfSmallest + min(v1[i1], v2[i2])) / 2.0;
}


/*
	Find the maximum number of points that lie one the same line on a 2D plane.
*/
// This algorithm finds the line two points are on, and saves the points in a hash table
vector<Point> maxCollinearPoints(vector<Point>& pts) {
	unordered_map<Line<double, double>, int, hash_fn> LineCounts;
	unordered_map<Line<double, double>, std::vector<Point>, hash_fn> LinePoints;
	int currentMax = 0;
	Line<double, double> maxLine;
	double slope, yInt;

	// Store all points in the hashtable, as well as the count of how many points are on each line
	for(int i=0; i<pts.size(); i++){
		for(int j=i+1; j<pts.size(); j++){
			if(pts[j].y - pts[i].y == 0)
				slope = 0;
			else
				slope = (pts[j].x - pts[i].x)/(pts[j].y - pts[i].y);
			yInt = pts[i].y - slope*pts[i].x;
			Line<double, double> newLine (slope, yInt);
			LineCounts[newLine]++;
			LinePoints[newLine].push_back(pts[i]);
			LinePoints[newLine].push_back(pts[j]);
			if(LineCounts[newLine] > currentMax){
				maxLine = newLine;
				currentMax = LineCounts[newLine];
			}
		}
	}

	//Remove all duplicate points
	std::vector<Point> maxPoints = LinePoints[maxLine];
	for(int i=maxPoints.size(); i>0; i--){
		for(int j=i-1; j>=0; j--){
			if(maxPoints[i].x == maxPoints[j].x && maxPoints[i].y == maxPoints[j].y){
				maxPoints.erase(maxPoints.begin() + j);
				i=i-1;
			}
		}
	}

	cout << "There are " << currentMax << " points \n";

	return maxPoints;
}
