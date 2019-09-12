// Copyright 2018 Alexander Salmi asalmi@bu.edu
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::string;
using std::vector;

#define NOCOLLISION -1;

class vector2d {
  double x, y;

 public:
  vector2d() {
    x = y = 0;
  }
  vector2d(double xx, double yy) : x(xx), y(yy) {}

  vector2d operator+(const vector2d& other) const {
    // Adding two 2d vectors
    return vector2d(x + other.x, y + other.y);
  }

  vector2d operator-(const vector2d& other) const {
    // Subtracting two 2d vectors
    return vector2d(x - other.x, y - other.y);
  }

  vector2d operator*(const double & scalar) const {
    // Multiplying a 2d vector with a scalar
    return vector2d(x * scalar, y * scalar);
  }

  double operator*(const vector2d& vec) const {
    // Dot product of two 2d vectors
    return x * vec.x + y * vec.y;
  }

  friend ostream& operator<<(ostream&, const vector2d&);

  friend istream& operator>>(istream&, vector2d&);
};


ostream& operator<<(ostream& os, const vector2d& i) {
  os << "(" << i.x << "," << i.y << ")";
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

  void move(double time);
  double collide_time(const Stone& s);
  void collide(Stone* s);
  bool operator<(const Stone & other) {
    if (name.compare(other.name) < 1) {
      return true;
    } else {
      return false;
    }
  }

  vector2d momentum() const {
    return vel * mass;
  }
  double energy() const {
    return vel * vel * 0.5 * mass;
  }

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

void Stone::move(double time) {
  // Finding new position of stone at t=time, r(t) = p + v*t
  pos = pos + (vel * time);
  return;
}

double Stone::collide_time(const Stone& s) {
  // Calculating a, b, and c in at^2+bt+c=0
  // Assuming r(t)=ri(t)-rj(t),
  // where r(t).pos = ri(t).pos - rj(t).pos,
  // and r(t).vel = ri(t).vel - rj(t).vel
  vector2d rpos = pos - s.pos;
  vector2d rvel = vel - s.vel;

  // a = r(t).vel * r(t).vel [dotproduct]
  double a = rvel * rvel;

  // b = r(t).pos * r(t).vel [dotproduct]
  double b = rpos * rvel * 2;

  // c = r(t).pos * r(t).pos + (Ri + Rj)^2 [dotproduct]
  double c = rpos * rpos - std::pow(radius + s.radius, 2);

  // Finding the discriminant and checking that the roots are not imaginary
  // If not imaginary, return the lowest value of t
  // If there are no real positive valuse of t, return NOCOLLISION to indicate
  // there are no more collisions
  double disc = std::pow(b, 2) - (4 * a * c);
  if (disc < 0) {
    return NOCOLLISION;
  } else if (disc == 0) {
    double t = (0 - b) / (2 * a);
    if (t > 0) {
      return t;
    } else {
      return NOCOLLISION;
    }
  } else {
    double t1 = ((0 - b) + sqrt(disc)) / (2 * a);
    double t2 = ((0 - b) - sqrt(disc)) / (2 * a);
    if (t1 < t2 && t1 >= 0) {
      return t1;
    } else if (t2 >= 0) {
      return t2;
    } else {
      return NOCOLLISION;
    }
  }
}

void Stone::collide(Stone* s) {
  // Initializing velocities, positions, and masses of two stones for better
  // readability in calculations
  vector2d v1 = vel, v2 = (*s).vel, r1 = pos, r2 = (*s).pos;
  double m1 = mass, m2 = (*s).mass;

  // Calculate new velocities using given formulas
  vel = v1 -
        (r1 - r2) * (((v1 - v2) * (r1 - r2)) / ((r1 - r2) * (r1 - r2))) *
        ((2 * m2) / (m1 + m2));
  (*s).vel = v2 -
             (r2 - r1) * (((v2 - v1) * (r2 - r1)) / ((r2 - r1) * (r2 - r1))) *
             ((2 * m1) / (m1 + m2));

  return;
}


class Collision {
 public:
  Stone* one;
  Stone* two;
  double time;

  Collision(double t, Stone* p1, Stone* p2) {
    time = t;
    one = p1;
    two = p2;
  }

  friend std::ostream& operator<<(std::ostream& os, const Collision& i) {
    os << i.one->name << " " << i.two->name << "\n";
    return os;
  }

  bool valid();
};

bool Collision::valid() {
  // If the time on the collision is -1, this means get_next_collision() could
  // not find a future collision, therefore the main loop in main()
  // should be broken by returning false. Else, continue the loop with true
  if (time == -1) {
    return false;
  } else {
    return true;
  }
}

Collision get_next_collision(vector<Stone> * ps) {
  double earliest = NOCOLLISION;
  Collision nextC(-1, &ps->at(0), &ps->at(1));

  // Looping through the stones to find the earliest collision with
  // current pos & vel values
  for (int i = 0; i < (*ps).size(); i++) {
    for (int j = i + 1; j < (*ps).size(); j++) {
      double time = (*ps).at(i).collide_time((*ps).at(j));
      if (time != -1 && (time < earliest || earliest == -1)) {
        earliest = time;
        nextC.one = &ps->at(i);
        nextC.two = &ps->at(j);
      }
    }
  }
  nextC.time = earliest;

  return nextC;
}


void show_stones(vector<Stone> const & stones) {
  // Output info for all of the stones
  double energy{0};
  vector2d momentum;
  for (auto & s : stones) {
    cout << s;
    momentum = momentum +  s.momentum();
    energy += s.energy();
  }
  cout << "energy: " << energy << "\n";
  cout << "momentum: " << momentum << "\n";
}

int main() {
  double overall_time = 0;

  cout << "Please enter the mass, radius, x/y position, x/y velocity\n";
  cout << "and name of each stone\n";
  cout << "When complete, use EOF / Ctrl-D to stop entering\n";

  vector<Stone> stones;
  Stone s;
  while (cin >> s)
    stones.push_back(s);
  sort(stones.begin(), stones.end());

  cout << "\nHere are the initial stones.\n";
  show_stones(stones);

  // Check to make sure that there are enough stones for a collision to occur
  if (stones.size() <= 1) {
    cout << "There are not enough stones for a collision to occur\n";
    return 0;
  }

  cout << "\nHere are the collision events.\n";
  while (true) {
    Collision c = get_next_collision(&stones);
    if (!c.valid())  break;

    double newtime = c.time;
    for (auto & s : stones)
      s.move(newtime);
    overall_time += newtime;
    cout << "\ntime of event: " << overall_time << "\n";
    cout << "colliding " << c;
    c.one->collide(c.two);
    show_stones(stones);
  }
}
