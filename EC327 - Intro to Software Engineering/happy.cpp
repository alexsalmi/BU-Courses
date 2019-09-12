// Copyright 2018 EC327 Alex Salmi asalmi@bu.edu
#include <cstdint>
#include <iostream>

int main() {
  int input, n, i;
  std::cin >> input;

  while (input != 0) {
    n = input;

    while (n != 1 && n != 4) {
      i = 0;
      while (n % 10 != n) {
        i += (n % 10) * (n % 10);
        n = n / 10;
      }
      n = i + (n % 10) * (n % 10);
    }

    if (n == 1) {
      std::cout << input << " is happy\n";
    } else if (n == 4) {
      std::cout << input << " is not happy\n";
    }
    std::cin >> input;
  }
  return 0;
}
