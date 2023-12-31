#include "tree_sitter/parser.h"
#include <stdio.h>

enum TokenType {
    TokenTypeEOF,
};

void* tree_sitter_asciidoc_external_scanner_create()
{
    return NULL;
}

void tree_sitter_asciidoc_external_scanner_destroy(void* payload)
{
    // ...
}

unsigned tree_sitter_asciidoc_external_scanner_serialize(
    void* payload,
    char* buffer)
{
    return 0;
}

void tree_sitter_asciidoc_external_scanner_deserialize(
    void* payload,
    const char* buffer,
    unsigned length)
{
    // ...
}

bool tree_sitter_asciidoc_external_scanner_scan(
    void* payload,
    TSLexer* lexer,
    const bool* valid_symbols)
{
    if (lexer->eof(lexer)) {
        if (valid_symbols[TokenTypeEOF]) {
            lexer->result_symbol = TokenTypeEOF;
            return true;
        }
    }
    return false;
}
