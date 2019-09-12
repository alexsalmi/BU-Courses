// class for euclidean vectors in 2 dimensions
//
// shows how operator overload and friend methods work
//
#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;

class vector2d {
 double x,y;
 public:

 vector2d() {
    x=y=0;
 }
 vector2d(double xx, double yy) :x(xx) , y(yy) {}

  vector2d operator+(const vector2d& other) const {
    return vector2d(x+other.x,y+other.y);
  }

  friend ostream& operator<<(ostream&, const vector2d&);

  friend istream& operator>>(istream&, vector2d&);
};


ostream& operator<<(ostream& os, const vector2d& i) {
  os << "< " << i.x << " , " << i.y << " >";
  return os;
}

std::istream& operator>>(std::istream& is, vector2d& i) {
  cin >> i.x;
  cin >> i.y;
  return is;
}

int main()
{

    vector2d v;
    vector2d p(3,1.2);
    cin >> v;
    cout << v << p << "\n";

    vector2d res;
    res = v+p;

    cout << v << "\n";
    cout << res << "\n";
    return 0;
}