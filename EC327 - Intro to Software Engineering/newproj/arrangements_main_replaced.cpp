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

 public:
  vector<double> panel_counts;
  vector<double> dinner_counts;
  vector<vector<string>> panel_arrs;

  // more stuff here. data and methods

  Arrangements();
  explicit Arrangements(string thenames);
  vector<string> panel_shuffles(int n);
  vector<string> dinner_shuffles(int n);
  double panel_count(int n);
  double dinner_count(int n);

  // more methods if you want
};

// Methods and constructors defined here:
Arrangements::Arrangements() {
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
  panel_arrs.push_back({});
  panel_arrs.push_back({"a"});
  panel_arrs.push_back({"ab", "ba"});
}

Arrangements::Arrangements(string thenames) {
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
  panel_arrs.push_back({});
  panel_arrs.push_back({guests.substr(0, 1)});
  panel_arrs.push_back({guests.substr(0, 2),
                        guests.substr(1, 1) + guests.substr(0, 1)
                       });
}

vector<string> Arrangements::panel_shuffles(int n) {
  vector<string> v, res1, res2;
  string s;
  if (n > maxSize) {
    throw n;
  }
  string guestlist = guests.substr(0, n);

  if (n < panel_arrs.size()) {
    return panel_arrs.at(n);
  } else {
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

  panel_arrs.push_back(v);
  return v;
}

vector<string> Arrangements::dinner_shuffles(int n) {
  vector<string> v, res1, res2;
  string s;
  if (n > maxSize) {
    throw n;
  }
  if (n == 2) {
    return {guests.substr(0, 2), guests.substr(1, 1) + guests.substr(0, 1)};
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

double Arrangements::panel_count(int n) {
  if (n > maxSize) {
    throw n;
  }

  if (panel_counts.size() > n) {
    return panel_counts.at(n);
  }

  for (int i = panel_counts.size(); i < n + 1; i++) {
    double sum = panel_counts.at(i - 1) + panel_counts.at(i - 2);
    panel_counts.push_back(sum);
  }
  return panel_counts.at(n);
}

double Arrangements::dinner_count(int n) {
  if (n > maxSize) {
    throw n;
  }

  if (dinner_counts.size() > n) {
    return dinner_counts.at(n);
  }

  for (int i = dinner_counts.size(); i < n + 1; i++) {
    double sum = dinner_counts.at(i - 1) + (dinner_counts.at(i - 2) - 2);
    dinner_counts.push_back(sum);
  }
  return dinner_counts.at(n);
}


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
    std::cout << " res.at(" << i
              << ") = " << res.at(i) << "\n";
  }
}


const int COUNTLIM = 100;
const int COUNTLIM_SMALL = 30;

void standard_tests();
void alternate_tests();
void interactive_main();

int main(int argc, char const ** argv) {
  if (argc > 1 and string(*(argv+1)) == "alt")
    alternate_tests();
  else if (argc > 1 and (string(*(argv+1)) == string("int")))
    interactive_main();
  else
    standard_tests();
}

// tests to be run for full credit, including performance.
void standard_tests() {
  int n;

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

  Timer t_pc("panel count", true);
  n = 1;
  cout << "\nPanel Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  panel(N)\n";

  while (n < COUNTLIM and t_pc.time() < 0.1) {
    t_pc.start();
    double pc = large.panel_count(n);
    t_pc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << pc << "\n";
    n++;
  }


  Timer t_dc("dinner count", true);
  n = 1;
  cout << "\nDinner Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  dinner(N)\n";

  while (n < COUNTLIM and t_dc.time() < 0.1) {
    t_dc.start();
    double dc = large.dinner_count(n);
    t_dc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << dc << "\n";
    n++;
  }

  Timer t_panel("panel", true);
  n = 4;
  cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";

  while (t_panel.time() < 0.5 and n <= 30)  {
    double last = t_panel.time();
    t_panel.start();
    vector<string> res = standard.panel_shuffles(n);
    t_panel.stop();
    show_partial_result("panel", res, n);
    cout << "time " << t_panel.time() - last << "\n";
    n++;
  }

  int largest_panel = n - 1;

  Timer t_dinner("dinner timing", true);
  n = 4;
  cout << "\nHow many dinner shuffles can be created in 0.5 seconds?\n";

  while (t_dinner.time() < 0.5 and n <= 30)  {
    double last = t_dinner.time();
    t_dinner.start();
    vector<string> res = standard.dinner_shuffles(n);
    t_dinner.stop();
    show_partial_result("dinner", res, n);
    cout << "time " << t_dinner.time() - last << "\n";
    n++;
  }
  cout << "\nLargest panel shuffles performed: "
       << largest_panel << "\n";
  cout << "\nLargest dinner shuffles performed: " << n - 1 << "\n";

  // Error checking
  Arrangements small("abcd");
  cout << "\nError Handling Tests\n";

  try {
    small.panel_count(5);
  } catch (int n) {
    cout << n;
  }
  try {
    small.panel_shuffles(6);
  } catch (int n) {
    cout << n;
  }
  try {
    small.dinner_count(7);
  } catch (int n) {
    cout << n;
  }
  try {
    small.dinner_shuffles(89);
  } catch (int n) {
    cout << n;
  }
  try {
    large.dinner_shuffles(122);
  } catch (int n) {
    cout << n;
  }
  try {
    numbers.dinner_shuffles(9);
  } catch (int n) {
    cout << n;
  }
  try {
    numbers.dinner_shuffles(10);
  } catch (int n) {
    cout << n;
  }
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

  int n = 1;

  // Count tests
  Timer t_pc("panel count", true);
  cout << "\nPanel Shuffle Count Table (0.1 seconds)\n";
  cout << "     N  panel(N)\n";
  while (n < 52 and t_pc.time() < 0.1) {
    t_pc.start();
    double pc = standard.panel_count(n);
    t_pc.stop();
    cout << std::setw(6) << n << " "
         << std::setw(6) << pc << "\n";
    n++;
  }

  cout << "\nHow many panel shuffles can be created in 0.5 seconds?\n";
  n = 4;

  Timer t_panel("panel", true);

  while (t_panel.time() < 0.5)  {
    t_panel.start();
    vector<string> res = standard.panel_shuffles(n);
    t_panel.stop();
    show_partial_result("panel", res, n);
    n++;
  }
  cout << "\nLargest panel shuffles performed: "
       << n - 1 << "\n";
}


void interactive_main() {
  std::string asktype, symbols;
  int number;
  cout << "Type quit to exit.\n";
  cout << "Commands:\npc names n\nps names n\ndc names n\nds names n\n";
  cout.precision(15);

  while (true) {
    std::cin >> asktype;
    if (asktype == "quit") break;
    std::cin >> symbols;
    Arrangements h(symbols);
    std::cin >> number;
    if (asktype == "pc") {
      std::cout << "panel_count(" << number <<  ") = ";
      std::cout << h.panel_count(number) << "\n";
    } else if (asktype == "ps") {
      std::cout << "panel_shuffles(" << number <<  ") = ";
      for (auto e : h.panel_shuffles(number) )
        std::cout << e << " ";
      std::cout << "\n";
    } else if (asktype == "dc") {
      std::cout << "dinner_count(" << number << ") = ";
      std::cout << h.dinner_count(number) << "\n";
    } else if (asktype == "ds") {
      std::cout << "dinner_shuffles(" << number <<  ") = ";
      for (auto e : h.dinner_shuffles(number))
        std::cout << e << " ";
      std::cout << "\n";
    }
  }
}
