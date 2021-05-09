//@+leo-ver=5-thin
//@+node:caminhante.20210508215346.1: * @file blisp.c
//@@language c
//@@tabwidth -2
//@+at License notice
//
// Copyright 2021 X Caminhante
//
// This file is part of B Lisp.
//
// B Lisp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// B Lisp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with B Lisp.  If not, see <https://www.gnu.org/licenses/>
//
//@@c
//@+others
//@+node:caminhante.20210508220407.1: ** /preprocessing
//@+node:caminhante.20210508220407.2: *3* /includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "minimal_UTF8/utf.h"
#include "minimal_UTF8/utf.c"
//@+node:caminhante.20210508220407.3: *3* /macros
#define ALLOC(N,S) (typeof(S)*)calloc(N,sizeof(S))
#define ERROR(MSG,A) (puts(MSG), print_expr(A), exit(1))
#define INPUT_SIZE 1024*16
//@+node:caminhante.20210508220407.4: *3* /constants, variables
const int close_char = ')';
bool exiting = false;
//@+node:caminhante.20210508224249.1: *3* /types
//@+node:caminhante.20210508220407.5: *4* enum Type
enum Type {
   NUM,
   STR,
   LIST,
   LAMBDA,
   TABLE
};
//@+node:caminhante.20210508220407.6: *4* struct List
struct List {
   struct Atom *head;
   struct List *tail;
};
//@+node:caminhante.20210508220407.7: *4* struct Atom
struct Atom {
   enum Type type;
   union {
      int num;
      struct uchar *str;
      struct List *list;
      struct Table *table;
   };
};
//@+node:caminhante.20210508220407.8: *4* struct Table
struct Table {
   struct uchar *key;
   struct Atom *value;
   struct Table *next;
};
//@+node:caminhante.20210508220407.9: *3* /prototypes
// static struct Atom* parse (void);
//@+node:caminhante.20210508220645.1: ** /life cycle
//@+node:caminhante.20210508220702.1: *3* static bool _initialize ()
// [ I -> prints a initial text ]
static bool _initialize () {
  puts("Copyright (C) 2021 X Caminhante\n"
       "This program comes with ABSOLUTELY NO WARRANTY; for details type (help warranty)\n"
       "This is free software, and you are welcome to redistribute it \n"
       "under certain conditions; type (help conditions) for details.");
  return true;
}
//@+node:caminhante.20210508221257.1: *3* static void prompt ()
// [ I -> prints a simple prompt, indicating that the interpreter is waiting for instructions ]
static void prompt () {
  printf("> ");
}
//@+node:caminhante.20210508221052.1: *3* static bool repl (size_t in_len, char in[static in_len])
// [ syntatically valid sequence of B Lisp tokens forming an expression ->
  // expression evaluation and true | something went wrong and false ]
static bool repl (size_t in_len, char in[static in_len]) {
  // points to the last byte parsed, 0 if nothing was parsed yet
  size_t last_byte = 0;
  // controls how many bytes were received from the input
  size_t read_bytes = read(STDIN_FILENO, in, in_len);
  //@+others
  //@+node:caminhante.20210508234212.1: *4* size_t replenish_bytes (size_t spare_bytes)
  // [ `spare_bytes` -> moves those spare bytes to the beginning of `in`, reads more bytes in
  // the space immediately after the moved ones respecting the remaining space, and returns the
  // number of bytes currently at `in` (that's it, `spare_bytes` + recently read bytes ]
  size_t replenish_bytes (size_t spare_bytes) {
    // [ spare_bytes == 0 -> I |
      // spare_bytes > 0 -> copy `spare_bytes` bytes from `in[in_len-spare_bytes-1]` to `in[0]` ]
    if (spare_bytes > 0) {}
    // [ try to read `in_len-spare_bytes` bytes from stdin starting at `in[spare_bytes]` ]
    // [ returns `spare_bytes+recently_read_bytes` ]
    return 0;
  }
  //@+node:caminhante.20210508233816.1: *4* struct uchar _getchar ()
  // [ returns a UTF-8 character, and if necessary, reads new bytes from the input ]
  struct uchar _getchar () {
    // [ last_byte < read_bytes -> get next uchar from `in` and increment last_byte |
      // last_byte >= read_bytes -> read more bytes from stdin into `in` and tries again ] 
    if (last_char < occupied) {
      // [ get next uchar from `in` and increment last_byte ]
    } else if (last_char == occupied) {
      // [ read more bytes from stdin into `in` and tries again ]
      read_bytes = replenish_bytes(0);
    }
  }
  //@-others
  return true;
}
//@+node:caminhante.20210508220650.1: *3* static bool _finalize ()
// [ I -> prepares the interpreter to exit ]
static bool _finalize () {
  exiting = true;
  return true;
}
//@+node:caminhante.20210508221001.1: ** int main (int argc, char **argv)
int main (int argc, char **argv) {
  char incoming[INPUT_SIZE];
  if (_initialize()) {
    while (!exiting) {
      prompt();
      if (!repl(sizeof(incoming),incoming)) { puts("Error"); }}
  }
  return 0;
}
//@-others
//@-leo
