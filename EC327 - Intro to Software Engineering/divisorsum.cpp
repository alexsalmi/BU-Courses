// Copyright 2018 EC327 Alex Salmi asalmi@bu.edu
#include <cstdint>
#include <iostream>

int main() {
  int64_t sum, input;

  while (input != 0) {
    sum = 0;
    std::cin >> input;
    if (input != 0) {
      std::cout << input << ": ";
      for (int i = 1; i <= input / 2; i++) {
        if (input % i == 0) {
          if (sum != 0) {
            std::cout << "+";
          }
          std::cout << i;
          sum += i;
        }
      }
      std::cout << " = " << sum << "\n";
    }
  }
  return 0;
}
