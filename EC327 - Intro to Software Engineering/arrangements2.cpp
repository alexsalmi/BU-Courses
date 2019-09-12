// Copyright 2018 Alexander Salmi asalmi@bu.edu
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "timer.h"

using std::vector;
using std::string;
using std::cout;
using std::sort;


class Arrangements {
 private:
  // your private data
  int maxSize;
  string guests;
  vector<double> panel_counts;
  vector<double> dinner_counts;

 public:
  Arrangements() {
    guests = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    maxSize = 52;
    panel_counts.push_back(0);
    panel_counts.push_back(1);
    panel_counts.push_back(2);
    dinner_counts.push_back(0);
    dinner_counts.push_back(1);
    dinner_counts.push_back(2);
    dinner_counts.push_back(6);
    dinner_counts.push_back(9);
  }
  explicit Arrangements(string thenames) {
    guests = thenames;
    maxSize = thenames.length();
    panel_counts.push_back(0);
    panel_counts.push_back(1);
    panel_counts.push_back(2);
    dinner_counts.push_back(0);
    dinner_counts.push_back(1);
    dinner_counts.push_back(2);
    dinner_counts.push_back(6);
    dinner_counts.push_back(9);
  }
  // see  http://en.cppreference.com/w/cpp/language/explicit

  vector<string> panel_shuffles(int n) {
    vector<string> v, res1, res2;
    string s;
    if (n > maxSize) {
      throw n;
    }
    string guestlist = guests.substr(0, n);

    if (n == 1) {
      v.push_back(guestlist);
      return v;
    }
    if (n == 2) {
      v.push_back(guestlist);
      v.push_back(guestlist.substr(1, 1) + guestlist.substr(0, 1));
      return v;
    }
    if (n > 2) {
      string v1 = guestlist.substr(n - 1, 1);
      res1 = panel_shuffles(n - 1);
      for (int i = 0; i < res1.size(); i++) {
        s = res1.at(i) + v1;
        v.push_back(s);
      }
      string v2 = guestlist.substr(n - 1, 1) + guestlist.substr(n - 2, 1);
      res2 = panel_shuffles(n - 2);
      for (int i = 0; i < res2.size(); i++) {
        s = res2.at(i) + v2;
        v.push_back(s);
      }
    }

    return v;
  }

  vector<string> dinner_shuffles(int n) {
    vector<string> v, res1, res2;
    string s;
    if (n > maxSize) {
      throw n;
    }
    string guestlist = guests.substr(0, n);

    string v1 = guestlist.substr(n - 1, 1);
    string v2 = guestlist.substr(0, 1);
    res1 = panel_shuffles(n - 1);
    for (int i = 0; i < res1.size(); i++) {
      s = res1.at(i) + v1;
      v.push_back(s);
      if (res1.at(i).substr(0, 1) == v2) {
        s = v1 + res1.at(i).substr(1, n - 2) + v2;
        v.push_back(s);
      }
    }
    string v3 = guestlist.substr(n - 1, 1) + guestlist.substr(n - 2, 1);
    res2 = panel_shuffles(n - 2);
    for (int i = 0; i < res2.size(); i++) {
      s = res2.at(i) + v3;
      v.push_back(s);
    }

    s = v1 + guestlist.substr(0, n - 1);
    v.push_back(s);

    s = guestlist.substr(1, n - 1) + v2;
    v.push_back(s);

    return v;
  }

  double panel_count(int n) {
    if (panel_counts.size() > n) {
      return panel_counts.at(n);
    }

    for (int i = panel_counts.size(); i < n + 1; i++) {
      panel_counts.push_back(panel_counts.at(i - 1) + panel_counts.at(i - 2));
    }
    return panel_counts.at(n);
  }
  double dinner_count(int n) {
    if (dinner_counts.size() > n) {
      return dinner_counts.at(n);
    }

    for (int i = dinner_counts.size(); i < n + 1; i++) {
      dinner_counts.push_back(dinner_counts.at(i - 1) +
      	(dinner_counts.at(i - 2) - 2));
    }
    return dinner_counts.at(n);
  }

  // Solution goes here.
};


// TESTING: leave this line and below as is.

void show_result(vector<string> v) {
  sort(v.begin(), v.end());
  for (auto c : v)
    cout << c << "\n";
  cout << "\n";
}

void show_partial_result(string testname, vector<string> res, int n) {
  if (res.empty()) return;

  sort(res.begin(), res.end());

  std::vector<uint64_t> locs{0, res.size() / 3,
                             2 * res.size() / 3, res.size() - 1};
  std::cout << "\n" << testname << " " << n << "\n";
  for (auto i : locs) {
    std::cout << " res(" << i
              << ") = " << res.at(i) << "\n";
  }
}


const int COUNTLIM = 100;
const int COUNTLIM_SMALL = 30;

void standard_tests();
void alternate_tests();

int main(int argc, char const ** argv) {
  if (argc > 1)
    alternate_tests();
  else
    standard_tests();
}

void standard_tests() {
  // tests to be run for full credit, including performance.

  cout.precision(15);
  // Basic test
  Arrangements standard;

  cout << "\nPanel Shuffles for 4 panelists.\n";
  show_result(standard.panel_shuffles(4));

  cout << "\nDinner Shuffles for 4 guests.\n";
  show_result(standard.dinner_shuffles(4));

  // Test other names
  Arrangements numbers("123456789");
  Arrangements symbols("!@#$%^&*()_+");

  std::array<Arrangements*, 3> v{&standard, &numbers, &symbols};

  cout << "\nPanel Shuffles for 6 panelists, 3 sets of names.\n";
  for (auto arr : v)
    show_result(arr->panel_shuffles(6));

  cout << "\nDinner Shuffles for 6 guests, 3 sets of names.\n";
  for (auto arr : v)
    show_result(arr->dinner_shuffles(6));

  // Count tests
  Arrangements large(string(COUNTLIM, 'a'));

  cout << "\nPanel Shuffle Count Table \n";
  cout << "     N  panel(N)\n";
  for (int n = 1; n < COUNTLIM; n++) {
    cout << std::setw(6) << n << " "
         << std::setw(6) << large.panel_count(n) << "\n";
  }

  cout << "\nDinner Shuffle Count Table \n";
  cout << "     N  dinner(N)\n";
  for (int n = 1; n < COUNTLIM; n++) {
    cout << std::setw(6) << n << " "
         << std::setw(6) << large.dinner_count(n) << "\n";
  }

  Timer t_panel("panel");


  cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";
  int n = 4;

  while (t_panel.time() < 0.5)  {
    t_panel.start();
    vector<string> res = standard.panel_shuffles(n);
    t_panel.stop();
    show_partial_result("panel", res, n);
    n++;
  }
  Timer t_dinner("dinner timing");

  n = 4;
  cout << "\nHow many dinner shuffles can be created in 0.5 seconds?\n";

  while (t_dinner.time() < 0.5)  {
    t_dinner.start();
    vector<string> res = standard.dinner_shuffles(n);
    t_dinner.stop();
    show_partial_result("dinner", res, n);
    n++;
  }

  // Error checking
  Arrangements small("abcd");
  cout << "\nError Handling Tests\n";

  try {
    small.panel_count(5);
  } catch (int n) {
    cout << n;
  };
  try {
    small.panel_shuffles(6);
  } catch (int n) {
    cout << n;
  };
  try {
    small.dinner_count(7);
  } catch (int n) {
    cout << n;
  };
  try {
    small.dinner_shuffles(89);
  } catch (int n) {
    cout << n;
  };
  try {
    large.dinner_shuffles(122);
  } catch (int n) {
    cout << n;
  };
  try {
    numbers.dinner_shuffles(9);
  } catch (int n) {
    cout << n;
  };
  try {
    numbers.dinner_shuffles(10);
  } catch (int n) {
    cout << n;
  };
  cout << "\n";
}

void alternate_tests() {
  cout.precision(15);
  // Basic test
  Arrangements standard;

  cout << "\nPanel Shuffles for 4 panelists.\n";
  show_result(standard.panel_shuffles(4));

  cout << "\nPanel Shuffles for 10 panelists.\n";
  show_result(standard.panel_shuffles(10));

  // Count tests
  cout << "\nPanel Shuffle Count Table \n";
  cout << "     N  panel(N)\n";
  for (int n = 1; n < COUNTLIM_SMALL; n++) {
    cout << std::setw(6) << n << " "
         << std::setw(6) << standard.panel_count(n) << "\n";
  }
}
