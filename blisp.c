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
#include "minimal_UTF8/utf8.h"
#include "minimal_UTF8/utf8.c"
//@+node:caminhante.20210508220407.3: *3* /macros
#define ALLOC(N,S) (typeof(S)*)calloc(N,sizeof(S))
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
//@+node:caminhante.20210509235504.1: *4* struct ParserState
struct ParserState {
  size_t in_len;
  char *in;
  size_t *used_bytes;
  size_t *last_read;
};
//@+node:caminhante.20210509181849.1: ** /warnings and errors
//@+node:caminhante.20210509181919.1: *3* void warning (msg, expr)
// [ string `msg`, struct Atom * `expr` (expression that may be associated with the warning)
// -> show a warning message using `msg` as the content,
  // [ expr != NULL
  // -> print `expr` on the terminal as a detail
  // | I ] ]
void warning (const char *msg, struct Atom *expr) {
  printf("\nWarning: %s",msg);
  // [ expr != NULL -> print `expr` on the terminal as a detail
  // | I ]
  // if (expr != NULL) { printf("\nDetail: "); print_expr(expr); }
}
//@+node:caminhante.20210509181932.1: *3* void error (msg, expr)
//@+node:caminhante.20210509180617.1: ** /input
// #TODO modify the input functions to operate with a `struct ParserState`
//@+node:caminhante.20210508234212.1: *3* size_t replenish_bytes (in_len, in, used_bytes, spare_bytes)
// [ char array `in`
// , positive integer `used_bytes` (number of bytes that were actually read into `in`)
// , positive integer `spare_bytes` -> moves those spare bytes to the beginning of `in`
// -> reads more bytes in the space immediately after the moved ones respecting the remaining
   // space, and returns the number of bytes currently at `in` (that's it, spare bytes +
   // recently read bytes) ]
size_t replenish_bytes (size_t in_len, char in[static in_len], size_t used_bytes, size_t spare_bytes) {
  // [ spare_bytes == 0 -> I
  // | spare_bytes > 0
  // -> copy `spare_bytes` bytes from `in[used_bytes-spare_bytes-1]` to `in[0]` ]
  if (spare_bytes > 0) {
    memmove(&in[0], &in[used_bytes-spare_bytes], spare_bytes);
  }
  // [ try to read `in_len-spare_bytes` bytes from stdin to `in[spare_bytes]` ]
  size_t recently_read_bytes = read(STDIN_FILENO, &in[spare_bytes], in_len-spare_bytes);
  // [ returns how many bytes are now occupying the array ]
  return spare_bytes+recently_read_bytes;
}
//@+node:caminhante.20210508233816.1: *3* struct uchar _getchar (in_len, in, used_bytes, last_read)
// [ a valid UTF8 sequence between `in[*last_read]` and `in[*used_bytes]`
// -> returns a valid struct uchar
// | a UTF8 sequence of length larger than the rest of `in` not yet parsed
// -> replenish_bytes at in, *last_read = 0 and retry
// | a invalid UTF8 sequence
// -> a "invalid UTF8 sequence" warning, increment *last_read and try again ]
struct uchar _getchar (size_t in_len, char in[static in_len], size_t *used_bytes, size_t *last_read) {
  try_again:
  // [ `*last_read < *used_bytes` -> try to extract a `struct uchar`
  // | else -> replenish_bytes at in, *last_read = 0 and retry ]
  if (*last_read < *used_bytes) {
    // [ a valid UTF8 sequence with length <= `*used_bytes-*last_read` -> extract a `struct uchar`
    // | a invalid UTF8 sequence -> issue a warning, increment `*last_read` and retry
    // | else -> replenish_bytes at in, *last_read = 0 and retry ]
    size_t a = uchar_bytes(&in[*last_read]);
    // a invalid UTF8 sequence?
    if (a == 0) {
      // [ issue a warning, increment `*last_read` and retry ]
      warning("Invalid UTF8 byte sequence",NULL);
      *last_read += 1;
      goto try_again;
    // a valid UTF8 sequence with length <= `*used_bytes-*last_read`?
    } else if ( a <= (*used_bytes - *last_read) ) {
      // [ extract a `struct uchar` from `&in[*last_read]` and returns it ]
      struct uchar b = next_uchar(&in[*last_read]);
      *last_read += b.bytes;
      return b;
    // there is a valid UTF8 sequence but its length is shorter than the available rest of `in`
    } else {
      // [ replenish_bytes at in, *last_read = 0 and retry ]
      size_t a = replenish_bytes(in_len,in,*used_bytes,*last_read);
      *used_bytes = a;
      *last_read = 0;
      goto try_again;
    }
  } else {
    // [ replenish_bytes at in, *last_read = 0 and retry ]
    size_t a = replenish_bytes(in_len,in,*used_bytes,*last_read);
    *used_bytes = a;
    *last_read = 0;
    goto try_again;
  }
  // [ inacessible -> returns a invalid 0-filled `struct uchar` ]
  return (struct uchar){0};
}
//@+node:caminhante.20210510000539.1: ** /memory
//@+node:caminhante.20210510000737.1: *3* struct Atom * new_atom (type)
//@+node:caminhante.20210510000758.1: *3* struct Atom * new_number (num)
//@+node:caminhante.20210510000849.1: *3* struct Atom * new_string (str)
//@+node:caminhante.20210510000645.1: *3* struct List * new_list (head, tail)
//@+node:caminhante.20210510000921.1: *3* struct List * new_lambda (arguments, tail)
//@+node:caminhante.20210509235359.1: ** /tokens
//@+node:caminhante.20210509235424.1: *3* struct uchar * skip_whitespace (parser_state)
struct uchar * skip_whitespace (struct ParserState *parser_state) {
  return NULL;
}
//@+node:caminhante.20210509235655.1: *3* bool read_literal (parser_state, lit)
bool read_literal (struct ParserState *parser_state, struct uchar *lit) {
  return false;
}
//@+node:caminhante.20210509235709.1: *3* size_t read_number (parser_state)
size_t read_number (struct ParserState *parser_state) {
  return 0;
}
//@+node:caminhante.20210509235726.1: *3* struct uchar * read_id (parser_state)
struct uchar * read_id (struct ParserState *parser_state) {
  return NULL;
}
//@+node:caminhante.20210509235740.1: *3* struct uchar * read_string (parser_state)
struct uchar * read_string (struct ParserState *parser_state) {
  return NULL;
}
//@+node:caminhante.20210510000229.1: ** /parser
//@+node:caminhante.20210510000234.1: *3* struct List * parse_list (parser_state)
//@+node:caminhante.20210510000432.1: *3* struct List * parse_lambda (parser_state)
//@+node:caminhante.20210510000456.1: *3* struct Table * parse_table (parser_state)
//@+node:caminhante.20210510000517.1: *3* struct Atom * parse_atom (parser_state)
//@+node:caminhante.20210510001052.1: ** /type operations
//@+node:caminhante.20210510001059.1: *3* bool is_equal (atom1, atom2)
//@+node:caminhante.20210510001320.1: *3* size_t list_size (list)
//@+node:caminhante.20210510001154.1: *3* /table operations
//@+node:caminhante.20210510001128.1: *4* struct Atom * lookup_table (key)
//@+node:caminhante.20210510001207.1: *4* structt Table * add_to_table (key, value)
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
//@+node:caminhante.20210508221052.1: *3* static bool repl (in_len, in)
// [ syntatically valid sequence of B Lisp tokens forming an expression
// -> expression evaluation and returns true
// | else
// -> something went wrong and returns false ]
static bool repl (size_t in_len, char in[static in_len]) {
  // size_t used_bytes = 0;
  // size_t last_read = 0;
  return true;
}
//@+node:caminhante.20210508220650.1: *3* static bool _finalize ()
// [ I -> prepares the interpreter to exit ]
static bool _finalize () {
  exiting = true;
  return true;
}
//@+node:caminhante.20210508221001.1: ** int main (argc, argv)
int main (int argc, char **argv) {
  char incoming[INPUT_SIZE];
  if (_initialize()) {
    while (!exiting) {
      prompt();
      if (!repl(sizeof(incoming),incoming)) { puts("Error"); }}
  }
  puts("");
  return 0;
}
//@-others
//@-leo
