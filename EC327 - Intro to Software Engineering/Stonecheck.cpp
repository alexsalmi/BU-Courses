// Copyright 2018 Alexander Salmi asalmi@bu.edu
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::string;
using std::vector;

#define NOCOLLISION -1;

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

  vector2d operator-(const vector2d& other) const {
    return vector2d(x-other.x,y-other.y);
  }

  vector2d operator*(const double other) const {
    return vector2d(x*other,y*other);
  }

  double operator*(const vector2d& other) const {
    return (x*other.x)+(y*other.y);
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

class Stone {
 public:
  vector2d pos, vel;
  double mass;
  double radius;
  string name;

  double collide_time(const Stone& s);
  bool operator<(const Stone & other);

  friend std::ostream& operator<<(std::ostream& os, const Stone& i) {
    os << i.name << " m=" << i.mass << " R=" << i.radius
       << " p=" << i.pos << " v=" << i.vel << "\n";
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Stone& i) {
    cin >> i.mass >> i.radius >> i.pos >> i.vel >> i.name;
    return is;
  }
};

double Stone::collide_time(const Stone& s){
  // Calculating a, b, and c in at^2+bt+c=0
  // Assuming r(t)=ri(t)-rj(t),
  // where r(t).pos = ri(t).pos - rj(t).pos, and r(t).vel = ri(t).vel - rj(t).vel
  
  // a = r(t).vel.x^2 + r(t).vel.y^2, or a = r(t).vel * r(t).vel [dotproduct]
  vector2d rpos = pos - s.pos;

  vector2d rvel = vel - s.vel;

  double a = rvel*rvel;
  cout << "a = " << a << "\n";

  // b = 2(r(t).pos.x*r(t).vel.x + r(t).pos.y*r(t).vel.y), or b = r(t).pos * r(t).vel [dotproduct]
  double b = rpos*rvel*2;
  cout << "b = " << b << "\n";

  // c = r(t).pos.x^2 + r(t).pos.y^2 + (Ri + Rj)^2, or c = r(t).pos * r(t).pos + (Ri + Rj)^2 [dotproduct]
  double c = rpos*rpos - std::pow(radius + s.radius, 2);
  cout << "c = " << c << "\n";
  
  // Finding the discriminant and checking that the roots are not imaginary
  // If not imaginary, return the lowest value of t
  double disc = std::pow(b, 2) - (4*a*c);
  cout << "disc = " << disc << "\n";
  if(disc<0){
    return NOCOLLISION;
  }
  else if(disc==0){
    return (0-b)/(2*a);
  }
  else{
    double t1 = ((0-b) + sqrt(disc))/(2*a);
    double t2 = ((0-b) - sqrt(disc))/(2*a);
    if(t1<t2){
      return t1;
    }
    else{
      return t2;
    }
  }
}

bool Stone::operator<(const Stone& other){

  return true;
}

int main() {
  cout << "Please enter the mass, radius, x/y position, x/y velocity\n";
  cout << "and name of each stone\n";
  cout << "When complete, use EOF / Ctrl-D to stop entering\n";
  vector<Stone> stones;
  Stone s1, s2;

  cin >> s1;
  cin >> s2;

  double time = s1.collide_time(s2);

  cout << "t = " << time << "\n";
}