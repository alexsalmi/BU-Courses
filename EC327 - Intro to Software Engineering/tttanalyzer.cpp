// Copyright 2018 EC327 Alex Salmi asalmi@bu.edu
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "movedef.h"

using std::cout;
using std::string;
using std::vector;

char tttresult(string tttboard) {
  int winners = 0, xs = 0, os = 0, hashs = 0;
  char winner, letter;
  if (tttboard.length() != 9) {
    return 'e';
  }
  for (int i = 0; i < 9; i++) {
    if (tttboard[i] == 'o') {
      os += 1;
    } else if (tttboard[i] == 'x') {
      xs += 1;
    } else if (tttboard[i] == '#') {
      hashs += 1;
    } else {
      return 'e';
    }
  }
  if (xs > os + 1 || os > xs) {
    return 'i';
  }
  for (int i = 0; i < 3; i++) {
    letter = tttboard[i];
    if (letter == tttboard[i + 3] && letter == tttboard[i + 6] && letter != '#'
        && winner != letter) {
      winners += 1;
      winner = letter;
    }
    int j = i * 3;
    letter = tttboard[j];
    if (letter == tttboard[j + 1] && letter == tttboard[j + 2] && letter != '#'
        && winner != letter) {
      winners += 1;
      winner = letter;
    }
  }
  if (((tttboard[0] == tttboard[4] && tttboard[0] == tttboard[8])
       || (tttboard[2] == tttboard[4] && tttboard[2] == tttboard[6]))
      && tttboard[4] != '#' && winner != tttboard[4]) {
    winners += 1;
    winner = tttboard[4];
  }
  if (winners == 1) {
    if ((winner == 'x' && os == xs) || (winner == 'o' && xs > os)) {
      return 'i';
    } else {
      return winner;
    }
  } else if (winners == 0 && hashs == 0) {
    return 't';
  } else if (winners == 0 && hashs != 0) {
    return 'c';
  } else if (winners > 1) {
    return 'i';
  }
}

char tttresult(vector<Move> board) {
  string tttboard = "#########";
  for (int i = 0; i < board.size(); i++) {
    int position = 3 * board.at(i).r + board.at(i).c;
    if (board.at(i).r > 2 || board.at(i).c > 2 || tttboard[position] != '#') {
      return 'e';
    }
    tttboard[position] = board.at(i).player;
  }
  return tttresult(tttboard);
}

vector<string> get_all_boards() {
  vector<string> boards;
  string board = "#########";
  boards.push_back(board);

  while (board != "xxxxxxxxx") {
    int i = 8;
    while (board[i] == 'x') {
      i = i - 1;
    }
    if (board[i] == '#') {
      board[i] = 'o';
    } else if (board[i] == 'o') {
      board[i] = 'x';
    }
    for (i = i + 1; i < 9; i++) {
      board[i] = '#';
    }
    boards.push_back(board);
  }
  return boards;
}

// MAIN

int main() {
  std::vector<std::string> boards = get_all_boards();
  std::map<char, int> tally;
  for (int i = 0; i < boards.size(); i++) {
    tally[tttresult(boards.at(i))] += 1;
  }

  cout << "x " << tally['x'] << "\no " << tally['x'] << "\nt " << tally['o'];
  cout << "\ni " << tally['i'] << "\nc " << tally['c'] << "\n";
  return 0;
}
