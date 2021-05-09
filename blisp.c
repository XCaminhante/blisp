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
[  ]
static bool repl (size_t in_len, char in[static in_len]) {
  return true;
}
//@+node:caminhante.20210508220650.1: *3* static bool _finalize ()
static bool _finalize () {
  exiting = true;
  return true;
}
//@+node:caminhante.20210508221001.1: *3* int main (int argc, char **argv)
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
