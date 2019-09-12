// Copyright 2018 Alexander Salmi asalmi@bu.edu
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// allowed includes
// tuple, utility, vector, map, set, unordered_map,
// unordered_set, algorithm

using std::cin;
using std::cout;
using std::string;

/* Blokus command glossary

Notes:
 - the first tile is numbered 100, and each new
   one is assigned the next available integer

 - the upper left corner is 0,0

 - first index is row, second index is column


COMMANDS

QUIT: exit the game
>quit
Goodbye

RESET: start the game from beginning
>reset
game reset

CREATE: create a new tile

>create 4
>..*.
>.**.
>.*..
>....
created tile 100


SHOW ALL TILES: show a tile inventory (unordered.)

>show tiles
tile inventory
102
*
101
**
**
100
***
**.
...


SHOW A TILE: show a tile by ID number
>show 100
***
**.
**.

PLAY A TILE: place a tile on the board at row and column numbers

>play 101 4 5
played 101

>play 99 0 0
99 not played

ROTATE A TILE: rotate a tile 90 degrees counterclockwise
>rotate 100
rotate 100 complete
**.
*..
*..

FLIP A TILE sideways (left becomes right)
>fliplr 100
fliplr 100 complete
**.
.*.
.*.

FLIP A TILE vertically (up becomes down)
>flipud 100
flipud 100 complete
.*.
.*.
**.

RESIZE: make the board bigger or smaller. When smaller,
        played pieces fall off the board. Show the new board.

>resize 5

- end of command glossary -
*/


typedef int TileID;

class Tile {
  // common interface. required.
 public:
  std::vector<string> tilebox;
  std::vector<std::vector<string>> alternates;
  int size;
  int TileId;
  Tile(std::vector<string> box, int n, TileID id) {
    tilebox = box;
    size = n;
    TileId = id;

    std::vector<string> currenttilebox;

    for (int i = 0; i < 3; i++) {
      switch (i) {
      case 0:
        currenttilebox = tilebox;
        break;
      case 1:
        currenttilebox = flipudtile(tilebox);
        break;
      case 2:
        currenttilebox = fliplrtile(tilebox);
        break;
      }
      alternates.push_back(currenttilebox);
      for (int i = 0; i < 3; i++) {
        currenttilebox = rotatetile(currenttilebox);
        alternates.push_back(currenttilebox);
      }
    }
  }
  void show() const;  // print out tile in tilebox format
  void rotate();
  void flipud();
  void fliplr();
  std::vector<string> rotatetile(std::vector<string> in);
  std::vector<string> flipudtile(std::vector<string> in);
  std::vector<string> fliplrtile(std::vector<string> in);
};


std::vector<string> topleft(std::vector<string> tile) {
  int size = tile.size();
  int n = tile.size() - 1;
  bool isiempty = true;
  bool isjempty = true;
  while (isiempty) {
    for (int j = 0; j < tile.size(); j++) {
      if (tile.at(0).at(j) == '*') {
        isiempty = false;
      }
    }
    if (isiempty) {
      tile.push_back(tile.at(0));
      tile.erase(tile.begin());
    }
  }
  string newline;
  while (isjempty) {
    for (int i = 0; i < tile.size(); i++) {
      if (tile.at(i).at(0) == '*') {
        isjempty = false;
      }
    }
    if (isjempty) {
      newline = "";
      for (int i = 0; i < tile.size(); i++) {
        newline = tile.at(i) + '.';
        for (int j = 0; j < newline.length() - 1; j++) {
          tile.at(i).at(j) = newline.at(j + 1);
        }
      }
    }
  }

  return tile;
}

void Tile::show() const {
  for (int i = 0; i < tilebox.size(); i++) {
    for (int j = 0; j < tilebox.size(); j++) {
      cout << tilebox.at(i).at(j);
    }
    cout << "\n";
  }
}

std::vector<string> Tile::rotatetile(std::vector<string> in) {
  std::vector<string> out = in;
  int size = in.size();
  int n = in.size() - 1;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out.at(i).at(j) = in.at(j).at(n - i);
    }
  }

  return topleft(out);
}

void Tile::rotate() {
  Tile::tilebox = rotatetile(tilebox);
}

std::vector<string> Tile::flipudtile(std::vector<string> in) {
  std::vector<string> out = in;
  int size = in.size();
  int n = in.size() - 1;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out.at(i).at(j) = in.at(n - i).at(j);
    }
  }

  return topleft(out);
}

void Tile::flipud() {
  tilebox = flipudtile(tilebox);
}

std::vector<string> Tile::fliplrtile(std::vector<string> in) {
  std::vector<string> out = in;
  int size = in.size();
  int n = in.size() - 1;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out.at(i).at(j) = in.at(i).at(n - j);
    }
  }

  return topleft(out);
}

void Tile::fliplr() {
  tilebox = fliplrtile(tilebox);
}

class Blokus {
  // common interface. required.

// collection of Tiles

 public:
  std::unordered_map<TileID, Tile> Tiles;
  TileID nextID = 100;
  std::vector<string> Board;
  std::unordered_map<int, std::pair<TileID, std::pair<int, int>>> Moves;
  int nextMove = 0;

  Tile* find_tile(TileID);
  void create_piece() {
    // read in the size
    // read in strings
    // make a Tile
    // store it in a collection of Tiles
    int size;
    cin >> size;
    std::vector<string> layout;
    string row;
    for (int i = 0; i < size; i++) {
      cin >> row;
      layout.push_back(row);
    }


    int imax = 0, jmax = 0, imin = size, jmin = size, weight = 0;
    std::pair<int, int> firstblock;
    bool firstfound = false;
    std::vector<std::pair<int, int>> checked;
    for (int i = 0; i < size; i++) {
      if (layout.at(i).length() != size) {
        cout << "invalid tile\n";
        return;
      }
      for (int j = 0; j < size; j++) {
        if (layout.at(i).at(j) != '*' && layout.at(i).at(j) != '.') {
          cout << "invalid tile\n";
          return;
        } else if (layout.at(i).at(j) == '*') {
          if (i > imax) {
            imax = i;
          }
          if (i < imin) {
            imin = i;
          }
          if (j > jmax) {
            jmax = j;
          }
          if (j < jmin) {
            jmin = j;
          }
          weight++;
          if (!firstfound) {
            firstblock = std::make_pair(i, j);
            checked.push_back(firstblock);
            firstfound = true;
          }
        }
      }
    }

    checked =
      check_disconnect(layout, firstblock.first, firstblock.second, checked);
    if (checked.size() != weight) {
      cout << "disconnected tile discarded\n";
      return;
    }

    int newsize;
    if ((jmax - jmin + 1) > (imax - imin + 1)) {
      newsize = (jmax - jmin + 1);
    } else {
      newsize = (imax - imin + 1);
    }

    std::vector<string> templayout = topleft(layout);
    std::vector<string> newlayout;
    for (int i = 0; i < newsize; i++) {
      newlayout.push_back(templayout.at(i).substr(0, newsize));
    }
    layout = newlayout;

    std::unordered_map<TileID, Tile>::iterator it = Tiles.begin();
    for ( it = Tiles.begin(); it != Tiles.end(); it++ ) {
      if (it -> second.tilebox == layout) {
        cout << "duplicate of " << it -> first << " discarded\n";
        return;
      }
      for (int i = 0; i < it -> second.alternates.size(); i++) {
        if (it -> second.alternates.at(i) == layout) {
          cout << "duplicate of " << it -> first << " discarded\n";
          return;
        }
      }
    }

    Tile newTile = Tile(layout, size, nextID);
    Tiles.insert(std::pair<TileID, Tile>(nextID, newTile));
    cout << "created tile " << nextID << "\n";
    nextID = nextID + 1;
  }

  void reset();
  void show_tiles() const;
  void show_board() const;
  void play_tile(TileID, int, int);
  void set_size(int);
  std::vector<std::pair<int, int>> check_disconnect(std::vector<string>,
                                int, int, std::vector<std::pair<int, int>>);
  bool play(TileID, int, int, bool);
};

std::vector<std::pair<int, int>>
                              Blokus::check_disconnect(
                                std::vector<string> layout, int i, int j,
std::vector<std::pair<int, int>> checked) {
  if (i + 1 < layout.size()) {
    if (layout.at(i + 1).at(j) == '*') {
      if (std::find(checked.begin(), checked.end(),
                    std::pair<int, int>(i + 1, j)) == checked.end()) {
        checked.push_back(std::pair<int, int>(i + 1, j));
        checked = check_disconnect(layout, i + 1, j, checked);
      }
    }
  }
  if (j + 1 < layout.size()) {
    if (layout.at(i).at(j + 1) == '*') {
      if (std::find(checked.begin(), checked.end(),
                    std::pair<int, int>(i, j + 1)) == checked.end()) {
        checked.push_back(std::pair<int, int>(i, j + 1));
        checked = check_disconnect(layout, i, j + 1, checked);
      }
    }
  }
  if (i - 1 >= 0) {
    if (layout.at(i - 1).at(j) == '*') {
      if (std::find(checked.begin(), checked.end(),
                    std::pair<int, int>(i - 1, j)) == checked.end()) {
        checked.push_back(std::pair<int, int>(i - 1, j));
        checked = check_disconnect(layout, i - 1, j, checked);
      }
    }
  }
  if (j - 1 >= 0) {
    if (layout.at(i).at(j - 1) == '*') {
      if (std::find(checked.begin(), checked.end(),
                    std::pair<int, int>(i, j - 1)) == checked.end()) {
        checked.push_back(std::pair<int, int>(i, j - 1));
        checked = check_disconnect(layout, i, j - 1, checked);
      }
    }
  }
  return checked;
}

Tile* Blokus::find_tile(TileID id) {
  Tile* ptr = &(Tiles.find(id) -> second);
  return ptr;
}

void Blokus::reset() {
  Tiles = {};
  nextID = 100;
  Board = {};
  Moves = {};
  cout << "game reset\n";
}

void Blokus::show_tiles() const {
  cout << "tile inventory\n";
  for ( auto it = Tiles.begin(); it != Tiles.end(); it++ ) {
    cout << it -> first << "\n";
    for ( int i = 0; i < it -> second.tilebox.size(); i++ ) {
      cout << it -> second.tilebox.at(i) << "\n";
    }
  }
}

void Blokus::show_board() const {
  for (int i = 0; i < Board.size(); i++) {
    cout << Board.at(i) << "\n";
  }
}

bool Blokus::play(TileID id, int x, int y, bool fromreset) {
  std::vector<string> tilebox = Tiles.find(id) -> second.tilebox;
  if (id < 100 || id >= nextID) {
    if (fromreset) {
      cout << id << " not played\n";
    }
    return false;
  }

  for (int i = x; i <= x + tilebox.size() - 1; i++) {
    for (int j = y; j <= y + tilebox.size() - 1; j++) {
      if (tilebox.at(i - x).at(j - y) == '*') {
        if (i >= Board.size() || j >= Board.size()) {
          if (!fromreset) {
            cout << id << " not played\n";
          }
          return false;
        }
        if (Board.at(i).at(j) == '*') {
          if (!fromreset) {
            cout << id << " not played\n";
          }
          return false;
        }
      }
    }
  }
  for (int i = x; i <= x + tilebox.size() - 1; i++) {
    for (int j = y; j <= y + tilebox.size() - 1; j++) {
      if (tilebox.at(i - x).at(j - y) == '*') {
        if (Board.at(i).at(j) == '.') {
          Board.at(i).at(j) = tilebox.at(i - x).at(j - y);
        }
      }
    }
  }
  return true;
}

void Blokus::play_tile(TileID id, int x, int y) {
  if (play(id, x, y, false)) {
    std::pair<int, int> coords(x, y);
    std::pair<TileID, std::pair<int, int>> info(id, coords);

    Moves.insert(std::pair<int, std::pair<TileID,
                 std::pair<int, int>>>(nextMove, info));
    nextMove++;

    cout << "played " << id << "\n";
  }

  return;
}

void Blokus::set_size(int size) {
  Board = {};
  string line = "";
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      line = line + '.';
    }
    Board.push_back(line);
    line = "";
  }
  bool success = true;
  std::unordered_map<int, std::pair<TileID, std::pair<int, int>>> newMoves;
  for ( auto it = Moves.begin(); it != Moves.end(); it++ ) {
    success = play(it ->second.first, it->second.second.first,
                   it->second.second.second, true);
    if (success) {
      std::pair<int, int> coords(it->second.second.first,
                                 it->second.second.second);
      std::pair<TileID, std::pair<int, int>> info(it->second.first, coords);
      newMoves.insert(std::pair<int, std::pair<TileID,
                      std::pair<int, int>>>(it->first, info));
    }
  }
  Moves = newMoves;
  return;
}

// MAIN. Do not change the below.


int main() {
  string command;
  Blokus b;

  while (true) {
    cin >> command;
    if (command == "quit")  {
      break;
    } else if (command == "//") {
      getline(cin, command);
    } else if (command == "board") {
      b.show_board();
    } else if (command == "create") {
      b.create_piece();
    } else if (command == "reset") {
      b.reset();
    } else if (command == "show") {
      string arg;
      cin >> arg;
      if (arg == "tiles") {
        b.show_tiles();
      } else {
        auto g = b.find_tile(std::stoi(arg));
        g->show();
      }
    } else if (command == "resize") {
      int newsize;
      cin >> newsize;
      b.set_size(newsize);
      b.show_board();
    } else if (command == "play") {
      TileID id;
      int row, col;
      cin >> id >> row >> col;
      b.play_tile(id, row, col);
    } else if (command == "rotate") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->rotate();  // equivalent (*g).rotate(); // wrong *g.rotate();
      cout << "rotated " << id << "\n";
      g->show();
    } else if (command == "fliplr") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->fliplr();
      cout << "fliplr " << id << "\n";
      g->show();
    } else if (command == "flipud") {
      TileID id;
      cin >> id;
      auto g = b.find_tile(id);
      g->flipud();
      cout << "flipud " << id << "\n";
      g->show();
    } else {
      cout << "command not understood.\n";
    }
  }
  cout << "Goodbye\n";
  return 0;
}
