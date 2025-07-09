#ifndef TREE_SITTER_ASCIIDOC_SCANNER_H
#define TREE_SITTER_ASCIIDOC_SCANNER_H

#define STACK_SIZE 12

#include <tree_sitter/parser.h>
#include "marker_types.h"

#define EDGELIMIT 64

typedef struct Edge {
    struct Node* to;
} Edge;

typedef struct Node {
    size_t size;
    char glyph;
    bool root;
    enum MarkerType token;
    Edge* children[EDGELIMIT];
} Node;

typedef struct ADOCScanner ADOCScanner;

typedef struct {
  int32_t stack[STACK_SIZE];
  unsigned long long int mask;
} lookbehind_s;

struct ADOCScanner {
  TSLexer* lexer;
  const bool* valid_symbols;

  uint64_t mask;
  int32_t lookahead;
  lookbehind_s* lookbehind;
  Node* root;

  void (*advance)(ADOCScanner* scanner);
  void (*skip)(ADOCScanner* scanner);

  unsigned (*serialize)(ADOCScanner* scanner, char* buffer);
  void (*deserialize)(ADOCScanner* scanner, const char* buffer, unsigned length);
  bool (*scan)(ADOCScanner* scanner);
  bool (*eof)(TSLexer const* TSLexer);
};

static ADOCScanner* adoc_scanner_create();
static void adoc_scanner_destroy(ADOCScanner* scanner);

static void shift(ADOCScanner* scanner, int32_t value);
static bool match_before(ADOCScanner* scanner, char* value, size_t length, bool (*func_ptr)(int32_t*, size_t) );

static void adoc_scanner_advance(ADOCScanner* scanner);
static void adoc_scanner_skip(ADOCScanner* scanner);

static unsigned adoc_scanner_serialize(ADOCScanner* scanner, char* buffer);
static void adoc_scanner_deserialize(ADOCScanner* scanner, const char* buffer, unsigned length);

static bool adoc_scanner_scan(ADOCScanner* scanner);

Node* createNode(char glyph);

bool isLeaf(const Node* node);

Node* findNode(int32_t glyph, const Node* start);

Node* addNode(char glyph, Node* here);

void buildSuffixTree(const char* s, enum MarkerType token, Node* root);

enum MarkerType matchTok(char* s, const Node* root);
enum MarkerType matchGlyph(int32_t s, const Node* root, ADOCScanner* scanner);

#endif // ifndef TREE_SITTER_ASCIIDOC_SCANNER_H
