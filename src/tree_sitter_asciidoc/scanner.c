// vim:tw=0:sw=4:virtualedit=all
#include <stdio.h>
#include <string.h>

#include "chars.c"
#include "markers.h"
#include "tokens.h"
#include "tree_sitter_asciidoc/marker_types.h"
#include "scanner.h"

static ADOCScanner* adoc_scanner_create()
{
    ADOCScanner* scanner = calloc(1, sizeof(ADOCScanner));

    scanner->advance = adoc_scanner_advance;
    scanner->skip = adoc_scanner_skip;

    scanner->lookbehind = calloc(1, sizeof(lookbehind_s));

    scanner->serialize = adoc_scanner_serialize;
    scanner->deserialize = adoc_scanner_deserialize;
    scanner->scan = adoc_scanner_scan;

    scanner->root = createNode('\0');
    scanner->root->root = true;

    buildSuffixTree(STR_MONOSPACE,                    M_MONOSPACE,                                    scanner->root);

    buildSuffixTree(STR_MONOSPACE_U,                  M_MONOSPACE_U,                                  scanner->root);

    buildSuffixTree(STR_INLINE_PASSTHROUGH,           M_INLINE_PASSTHROUGH_HARDBREAK_CONTINUATION,    scanner->root);

    buildSuffixTree(STR_INLINE_PASSTHROUGH_U,         M_INLINE_PASSTHROUGH_U,                         scanner->root);

    buildSuffixTree(STR_HARDBREAK,                    M_INLINE_PASSTHROUGH_HARDBREAK_CONTINUATION,    scanner->root);
    buildSuffixTree(STR_BLOCK_CONTINUATION,           M_INLINE_PASSTHROUGH_HARDBREAK_CONTINUATION,    scanner->root);
    buildSuffixTree(STR_SECTION_HEADER0,              M_TITLE0,                                       scanner->root);
    buildSuffixTree(STR_SECTION_HEADER1,              M_TITLE1,                                       scanner->root);
    buildSuffixTree(STR_SECTION_HEADER2,              M_TITLE2,                                       scanner->root);
    buildSuffixTree(STR_ATTRIBUTE_MARKER,             M_ATTRIBUTE_MARKER,                             scanner->root);

    buildSuffixTree(STR_ATTRIBUTE_MARKER_START_NEG,   M_ATTRIBUTE_MARKER_START_NEG,                   scanner->root);
    buildSuffixTree(STR_ATTRIBUTE_MARKER_END_NEG,     M_ATTRIBUTE_MARKER_END_NEG,                     scanner->root);
    buildSuffixTree(STR_COMMENT_MARKER,               M_COMMENT_MARKER,                               scanner->root);
    buildSuffixTree(STR_EMPHASIS,                     M_EMPHASIS,                                     scanner->root);

    buildSuffixTree(STR_EMPHASIS_U,                   M_EMPHASIS_U,                                   scanner->root);

    buildSuffixTree(STR_STRONG,                       M_STRONG_UNNUMBERED_ASTERISK1,                  scanner->root);

    buildSuffixTree(STR_STRONG_U,                     M_STRONG_U,                                     scanner->root);

    buildSuffixTree(STR_SUPERSCRIPT,                  M_SUPERSCRIPT,                                  scanner->root);

    buildSuffixTree(STR_SUPERSCRIPT_U,                M_SUPERSCRIPT_U,                                scanner->root);

    buildSuffixTree(STR_SUBSCRIPT,                    M_SUBSCRIPT,                                    scanner->root);

    buildSuffixTree(STR_SUBSCRIPT_U,                  M_SUBSCRIPT_U,                                  scanner->root);

    buildSuffixTree(STR_HIGHLIGHT,                    M_HIGHLIGHT,                                    scanner->root);

    buildSuffixTree(STR_HIGHLIGHT_U,                  M_HIGHLIGHT_U,                                  scanner->root);


    buildSuffixTree(STR_INCLUDE,                      M_INCLUDE,                                      scanner->root);
    buildSuffixTree(STR_IMAGE,                        M_IMAGE,                                        scanner->root);
    buildSuffixTree(STR_IMAGE_INLINE,                 M_IMAGE_INLINE,                                 scanner->root);
    buildSuffixTree(STR_XREF,                         M_XREF,                                         scanner->root);
    buildSuffixTree(STR_FOOTNOTE,                     M_FOOTNOTE,                                     scanner->root);
    buildSuffixTree(STR_ICON,                         M_ICON,                                         scanner->root);
    buildSuffixTree(STR_LINK,                         M_LINK,                                         scanner->root);
    buildSuffixTree(STR_MAILTO,                       M_MAILTO,                                       scanner->root);

    buildSuffixTree(STR_CALLOUT,                      M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT1,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT2,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT3,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT4,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT5,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT6,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT7,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT8,                     M_CALLOUT,                                      scanner->root);
    buildSuffixTree(STR_CALLOUT9,                     M_CALLOUT,                                      scanner->root);

    buildSuffixTree(STR_OPTION_START,                 M_OPTION_START,                                 scanner->root);
    buildSuffixTree(STR_OPTION_END,                   M_OPTION_END,                                   scanner->root);
    buildSuffixTree(STR_CROSSREFERENCE_START,         M_CROSSREFERENCE_START,                         scanner->root);
    buildSuffixTree(STR_CROSSREFERENCE_END,           M_CROSSREFERENCE_END,                           scanner->root);
    buildSuffixTree(STR_BLOCK_COMMENT_MARKER,         M_BLOCK_COMMENT_MARKER,                         scanner->root);
    buildSuffixTree(STR_BLOCK_EXAMPLE_SEPARATOR1,     M_TITLE3_BLOCK_EXAMPLE_SEPARATOR1,              scanner->root);
    buildSuffixTree(STR_BLOCK_EXAMPLE_SEPARATOR2,     M_TITLE4_BLOCK_EXAMPLE_SEPARATOR2,              scanner->root);
    buildSuffixTree(STR_BLOCK_EXAMPLE_SEPARATOR3,     M_TITLE5_BLOCK_EXAMPLE_SEPARATOR3,              scanner->root);
    buildSuffixTree(STR_BLOCK_LISTING_SEPARATOR,      M_BLOCK_LISTING_SEPARATOR,                      scanner->root);
    buildSuffixTree(STR_BLOCK_LITERAL_SEPARATOR,      M_BLOCK_LITERAL_SEPARATOR_NUMBERED4,            scanner->root);
    buildSuffixTree(STR_BLOCK_OPEN_SEPARATOR,         M_BLOCK_OPEN_SEPARATOR,                         scanner->root);
    buildSuffixTree(STR_BLOCK_SIDEBAR_SEPARATOR,      M_BLOCK_SIDEBAR_SEPARATOR_UNNUMBERED_ASTERISK4, scanner->root);
    buildSuffixTree(STR_BLOCK_PASSTHROUGH_SEPARATOR,  M_BLOCK_PASSTHROUGH_SEPARATOR,                  scanner->root);
    buildSuffixTree(STR_BLOCK_QUOTE_SEPARATOR,        M_BLOCK_QUOTE_SEPARATOR,                        scanner->root);
    buildSuffixTree(STR_BLOCK_TABLE_SEPARATOR1,       M_BLOCK_TABLE_SEPARATOR1,                       scanner->root);
    buildSuffixTree(STR_BLOCK_TABLE_SEPARATOR2,       M_BLOCK_TABLE_SEPARATOR2,                       scanner->root);

    buildSuffixTree(STR_TABLE_CELL_MARKER1,           M_TABLE_CELL_MARKER1,                           scanner->root);
    buildSuffixTree(STR_TABLE_CELL_MARKER2,           M_TABLE_CELL_MARKER2,                           scanner->root);

    buildSuffixTree(STR_PAGE_BREAK,                   M_PAGE_BREAK,                                   scanner->root);
    buildSuffixTree(STR_THEMATIC_BREAK1,              M_THEMATIC_BREAK,                               scanner->root);
    buildSuffixTree(STR_THEMATIC_BREAK2,              M_THEMATIC_BREAK_UNNUMBERED_ASTERISK3,          scanner->root);
    buildSuffixTree(STR_BLOCK_TITLE,                  M_BLOCK_TITLE_LIST_NUMBERED1,                   scanner->root);

    buildSuffixTree(STR_LIST_UNNUMBERED_ASTERISK2,    M_LIST_UNNUMBERED_ASTERISK,                     scanner->root);
    buildSuffixTree(STR_LIST_UNNUMBERED_ASTERISK5,    M_LIST_UNNUMBERED_ASTERISK,                     scanner->root);

    buildSuffixTree(STR_LIST_HYPHEN,                  M_LIST_UNNUMBERED_HYPHEN,                       scanner->root);

    buildSuffixTree(STR_LIST_NUMBERED2,               M_LIST_NUMBERED,                                scanner->root);
    buildSuffixTree(STR_LIST_NUMBERED3,               M_LIST_NUMBERED,                                scanner->root);
    buildSuffixTree(STR_LIST_NUMBERED5,               M_LIST_NUMBERED,                                scanner->root);

    buildSuffixTree(STR_ADMONITION_NOTE,              M_ADMONITION_NOTE,                              scanner->root);
    buildSuffixTree(STR_ADMONITION_TIP,               M_ADMONITION_TIP,                               scanner->root);
    buildSuffixTree(STR_ADMONITION_IMPORTANT,         M_ADMONITION_IMPORTANT,                         scanner->root);
    buildSuffixTree(STR_ADMONITION_CAUTION,           M_ADMONITION_CAUTION,                           scanner->root);
    buildSuffixTree(STR_ADMONITION_WARNING,           M_ADMONITION_WARNING,                           scanner->root);


    buildSuffixTree(STR_ATTRIBUTE_REFERENCE_START,    M_ATTRIBUTE_REFERENCE_START,                    scanner->root);
    buildSuffixTree(STR_ATTRIBUTE_REFERENCE_END,      M_ATTRIBUTE_REFERENCE_END,                      scanner->root);
    buildSuffixTree(STR_TAG,                          M_TAG,                                          scanner->root);
    buildSuffixTree(STR_TAGEND,                       M_TAGEND,                                       scanner->root);
    buildSuffixTree(STR_IFDEF,                        M_IFDEF,                                        scanner->root);
    buildSuffixTree(STR_IFNDEF,                       M_IFDEF,                                        scanner->root);
    buildSuffixTree(STR_IFEVAL,                       M_IFEVAL,                                       scanner->root);
    buildSuffixTree(STR_IFEND,                        M_IFEND,                                        scanner->root);

    buildSuffixTree(STR_PASS,                         M_PASS,                                         scanner->root);
    buildSuffixTree(STR_STEM,                         M_STEM,                                         scanner->root);
    buildSuffixTree(STR_LATEXMATH,                    M_LATEXMATH,                                    scanner->root);

    buildSuffixTree(STR_DESCRIPTION_LIST1,            M_DESCRIPTION_LIST,                             scanner->root);
    buildSuffixTree(STR_DESCRIPTION_LIST2,            M_DESCRIPTION_LIST,                             scanner->root);
    buildSuffixTree(STR_DESCRIPTION_LIST3,            M_DESCRIPTION_LIST,                             scanner->root);
    buildSuffixTree(STR_DESCRIPTION_LIST4,            M_DESCRIPTION_LIST,                             scanner->root);
    buildSuffixTree(STR_ESCAPE,                       M_ESCAPE,                                       scanner->root);

    return scanner;
}

static void adoc_scanner_destroy(ADOCScanner* scanner)
{
    free(scanner->root);
    free(scanner->lookbehind);
    free(scanner);
}

static void shift(ADOCScanner* scanner, int32_t value)
{
    int32_t* items = scanner->lookbehind->stack;
    memmove(&items[1], &items[0], (STACK_SIZE-1)*sizeof(int32_t));
    items[0] = value;
}

static void see(ADOCScanner* scanner)
{
    int32_t* items = scanner->lookbehind->stack;
    printf(
        "[0]..[11] of `stack` | %c‧%c‧%c‧%c‧%c‧%c‧%c‧%c‧%c‧%c‧%c‧%c\n"
        "                     | %d‧%d‧%d‧%d‧%d‧%d‧%d‧%d‧%d‧%d‧%d‧%d\n\n",
        items[0],
        items[1],
        items[2],
        items[3],
        items[4],
        items[5],
        items[6],
        items[7],
        items[8],
        items[9],
        items[10],
        items[11],

        items[0],
        items[1],
        items[2],
        items[3],
        items[4],
        items[5],
        items[6],
        items[7],
        items[8],
        items[9],
        items[10],
        items[11]
    );
}

static bool match_before(ADOCScanner* scanner, char* value, size_t length, bool (*func_ptr)(int32_t*, size_t) )
{
    bool match = true;
    int32_t* items = scanner->lookbehind->stack;
    for (size_t i = 0; i < length && match; i++) {
        match = (value[length - i - 1] == items[i]);
    }
    bool match_end = func_ptr(items, length);
    return match && match_end;
}

static void adoc_scanner_advance(ADOCScanner* scanner)
{
    TSLexer* lexer = scanner->lexer;
    shift(scanner, lexer->lookahead);
    lexer->advance(lexer, false);
    scanner->lookahead = lexer->lookahead;
}

static void adoc_scanner_skip(ADOCScanner* scanner)
{
    TSLexer* lexer = scanner->lexer;
    shift(scanner, scanner->lookahead);
    lexer->advance(lexer, true);
    scanner->lookahead = lexer->lookahead;
}

static unsigned adoc_scanner_serialize(ADOCScanner* scanner, char* buffer)
{
    memcpy(buffer, scanner->lookbehind, sizeof(lookbehind_s));
    return sizeof(lookbehind_s);
}

static void adoc_scanner_deserialize(ADOCScanner* scanner, const char* buffer, unsigned length)
{
    if (buffer != NULL) {
        memcpy(scanner->lookbehind, (void*)buffer, sizeof(lookbehind_s));
    }
}



Node* createNode(char glyph)
{
    Node* node = malloc(sizeof(Node));
    node->token = M_NONE;
    node->size = 0;
    node->glyph = glyph;
    for (size_t i = 0; i < EDGELIMIT; i++) {
        node->children[i] = NULL;
    }
    return node;
}

bool isLeaf(const Node* node)
{
    return node->children[0] == NULL;
}

void listNode(const Node* node)
{
    printf("list: node address is (%p)\n", node);
    printf("  list: node char is `%c`\n", node->glyph);
    printf("  list: node token is `%d`\n", node->token);
    printf("  list: node children size `%i`\n", node->size);
    if (node->size) {
        for (size_t i = 0; i < node->size; i++) {
            printf("  list: node [%d] is char `%c`(%zu)\n",
                   i,
                   node->children[i]->to->glyph,
                   node->children[i]->to->glyph
                  );
        }
    }
}

Node* findNode(int32_t glyph, const Node* start)
{
    for (size_t e = 0; e < start->size; e++) {
        Edge* edge = start->children[e];
        Node* n = edge->to;
        if (glyph == (int32_t)(n->glyph)) {
            return n;
        }
    }
    return NULL;
}

Node* addNode(char glyph, Node* here)
{
    Edge* newEdge = malloc(sizeof(Edge));
    newEdge->to = createNode(glyph);

    here->size++;
    here->children[here->size-1] = newEdge;

    return here->children[here->size-1]->to;
}


void buildSuffixTree(const char* s, enum MarkerType token, Node* root)
{
    if (strlen(s) == 0) {
        root->token = token;
        return;
    }

    Node* n = findNode(s[0], root);

    if (n == NULL) {
        Node* f = addNode(s[0], root);
        void* substr = calloc(20, sizeof(char));
        memcpy(substr, s + 1, strlen(s) - 1);
        buildSuffixTree(substr, token, f);
    }
    else {
        void* substr = calloc(20, sizeof(char));
        memcpy(substr, s + 1, strlen(s) - 1);
        buildSuffixTree(substr, token, n);
    }
}

enum MarkerType matchTok(char* s, const Node* root)
{
    if (strlen(s) == 0) {
        return root->token;
    }

    if (isLeaf(root)) {
        return M_NONE;
    }

    Node* n = findNode(s[0], root);

    if (n == NULL) {
        return M_NONE;
    }

    if (n) {
        void* substr = calloc(20, sizeof(char));
        memcpy(substr, s + 1, strlen(s) - 1);
        return matchTok(substr, n);
    }
}

enum MarkerType matchGlyph(int32_t s, const Node* root, ADOCScanner* scanner)
{
    if (isLeaf(root)) {
        return root->token;
    }
    Node* n = findNode(s, root);

    if (n) {
        scanner->advance(scanner);
        return matchGlyph(scanner->lookahead, n, scanner);
    }

    if (n == NULL) {
        if (root->token != M_NONE) {
            return root->token;
        }
        if (root->root) {
            scanner->advance(scanner);
            return M_NONE;
        }
    }

}





bool m_newline(int32_t* value, size_t position)
{
    return is_newline(*(value + position));
}
bool m_space(int32_t* value, size_t position)
{
    return is_space(*(value + position));
}
bool m_newline_or_space(int32_t* value, size_t position)
{
    return is_newline(*(value + position)) || is_space(*(value + position));
}
bool m_any(int32_t* value, size_t position)
{
    return true;
}
bool m_emptyline(int32_t* value, size_t position)
{
    return is_newline(*(value + position)) &&
           is_newline(*(value + position + 1));
}

static bool adoc_scanner_scan(ADOCScanner* scanner)
{
    TSLexer* lexer = scanner->lexer;
    const bool* valid_symbols = scanner->valid_symbols;
    bool (*match_newline)(int32_t*, size_t) = &m_newline;
    bool (*match_space)(int32_t*, size_t) = &m_space;
    bool (*match_newline_or_space)(int32_t*, size_t) = &m_newline_or_space;
    bool (*match_any)(int32_t*, size_t) = &m_any;
    bool (*match_emptyline)(int32_t*, size_t) = &m_emptyline;

    if (
        valid_symbols[T_WHITESPACE] &&
        is_space(scanner->lookahead)
    ) {
        while (is_space(scanner->lookahead)) {
            scanner->advance(scanner);
        }
        lexer->result_symbol = T_WHITESPACE;
        return true;
    }

    if (
        valid_symbols[T_EMPTYLINE] &&
        is_newline(scanner->lookahead) &&
        is_newline(scanner->lookbehind->stack[0])
    ) {
        if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
        if (scanner->lookahead == CHAR_NEWLINE) {
            scanner->advance(scanner);
            lexer->result_symbol = T_EMPTYLINE;
            return true;
        }
    }
    if (
        valid_symbols[T_NEWLINE] &&
        is_newline(scanner->lookahead) &&
        is_newline(scanner->lookbehind->stack[0])
    ) {
        if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
        if (scanner->lookahead == CHAR_NEWLINE) {
            scanner->advance(scanner);
            lexer->result_symbol = T_NEWLINE;
            return true;
        }
    }
    if (
        valid_symbols[T_NEWLINE] &&
        is_newline(scanner->lookahead) &&
        !is_newline(scanner->lookbehind->stack[0])
    ) {
        if (scanner->lookahead == CHAR_CARRIAGE_RETURN) scanner->skip(scanner);
        if (scanner->lookahead == CHAR_NEWLINE || scanner->lookahead == CHAR_EOF) {
            scanner->advance(scanner);
            lexer->result_symbol = T_NEWLINE;
            return true;
        }
    }

    enum MarkerType match;

    if (!scanner->eof && !is_newline_or_space(scanner->lookahead)) {
        match = matchGlyph(scanner->lookahead, scanner->root, scanner);

        int32_t prev = scanner->lookbehind->stack[1];
        switch (match) {
        case M_NONE:
            if (valid_symbols[T_TEXT]) {
                lexer->result_symbol = T_TEXT;
                return true;
            }
            break;
        case M_MONOSPACE:
            if (
                valid_symbols[T_MONOSPACE_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_MONOSPACE_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_MONOSPACE_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_MONOSPACE_MARKER_END;
                return true;
            }
            break;
        case M_MONOSPACE_U:
            if (
                valid_symbols[T_MONOSPACE_UNCONSTRAINED_MARKER_START]
            ) {
                lexer->result_symbol = T_MONOSPACE_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_MONOSPACE_UNCONSTRAINED_MARKER_END]
            ) {
                lexer->result_symbol = T_MONOSPACE_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_INLINE_PASSTHROUGH_HARDBREAK_CONTINUATION:
            if (
                valid_symbols[T_HARDBREAK] &&
                match_before(scanner, STR_HARDBREAK, strlen(STR_HARDBREAK), match_space) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_HARDBREAK;
                return true;
            }
            if (
                valid_symbols[T_BLOCK_CONTINUATION] &&
                match_before(scanner, STR_BLOCK_CONTINUATION, strlen(STR_BLOCK_CONTINUATION), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_CONTINUATION;
                return true;
            }
            if (
                valid_symbols[T_INLINE_PASSTHROUGH_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_INLINE_PASSTHROUGH_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_INLINE_PASSTHROUGH_MARKER_END;
                return true;
            }
            break;
        case M_INLINE_PASSTHROUGH_U:
            if (
                valid_symbols[T_INLINE_PASSTHROUGH_UNCONSTRAINED_MARKER_START]
            ) {
                lexer->result_symbol = T_INLINE_PASSTHROUGH_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_INLINE_PASSTHROUGH_UNCONSTRAINED_MARKER_END]
            ) {
                lexer->result_symbol = T_INLINE_PASSTHROUGH_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_TITLE0:
            if (
                valid_symbols[T_TITLE_MARKER0]
            )
            {
                lexer->result_symbol = T_TITLE_MARKER0;
                return true;
            }
            break;
        case M_TITLE1:
            if (
                valid_symbols[T_TITLE_MARKER1]
            )
            {
                lexer->result_symbol = T_TITLE_MARKER1;
                return true;
            }
            break;
        case M_TITLE2:
            if (
                valid_symbols[T_TITLE_MARKER2]
            )
            {
                lexer->result_symbol = T_TITLE_MARKER2;
                return true;
            }
            break;
        case M_ATTRIBUTE_MARKER:
            if (
                valid_symbols[T_ATTRIBUTE_MARKER_START] &&
                match_before(scanner, STR_ATTRIBUTE_MARKER, strlen(STR_ATTRIBUTE_MARKER), match_newline)
            ) {
                lexer->result_symbol = T_ATTRIBUTE_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_ATTRIBUTE_MARKER_END] &&
                is_newline_or_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ATTRIBUTE_MARKER_END;
                return true;
            }
            break;
        case M_ATTRIBUTE_MARKER_START_NEG:
            if (
                valid_symbols[T_ATTRIBUTE_MARKER_START_NEG] &&
                match_before(scanner, STR_ATTRIBUTE_MARKER_START_NEG, strlen(STR_ATTRIBUTE_MARKER_START_NEG), match_newline)
            ) {
                lexer->result_symbol = T_ATTRIBUTE_MARKER_START_NEG;
                return true;
            }
            break;
        case M_ATTRIBUTE_MARKER_END_NEG:
            if (
                valid_symbols[T_ATTRIBUTE_MARKER_END_NEG] &&
                is_newline_or_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ATTRIBUTE_MARKER_END_NEG;
                return true;
            }
            break;
        case M_COMMENT_MARKER:
            if (
                valid_symbols[T_COMMENT_MARKER] &&
                match_before(scanner, STR_COMMENT_MARKER, strlen(STR_COMMENT_MARKER), match_newline)
            ) {
                lexer->result_symbol = T_COMMENT_MARKER;
                return true;
            }
            break;
        case M_EMPHASIS:
            if (
                valid_symbols[T_EMPHASIS_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_EMPHASIS_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_EMPHASIS_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_EMPHASIS_MARKER_END;
                return true;
            }
            break;
        case M_EMPHASIS_U:
            if (
                valid_symbols[T_EMPHASIS_UNCONSTRAINED_MARKER_START]
            ) {
                lexer->result_symbol = T_EMPHASIS_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_EMPHASIS_UNCONSTRAINED_MARKER_END]
            ) {
                lexer->result_symbol = T_EMPHASIS_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_STRONG_UNNUMBERED_ASTERISK1:
            if (
                valid_symbols[T_STRONG_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_STRONG_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_STRONG_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_STRONG_MARKER_END;
                return true;
            }
            if (
                valid_symbols[T_LIST_ASTERISK_MARKER] &&
                match_before(scanner, STR_LIST_UNNUMBERED_ASTERISK1, strlen(STR_LIST_UNNUMBERED_ASTERISK1), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_ASTERISK_MARKER;
                return true;
            }
            break;
        case M_STRONG_U:
            if (
                valid_symbols[T_STRONG_UNCONSTRAINED_MARKER_START]
            )
            {
                lexer->result_symbol = T_STRONG_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_STRONG_UNCONSTRAINED_MARKER_END]
            )
            {
                lexer->result_symbol = T_STRONG_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_SUPERSCRIPT:
            if (
                valid_symbols[T_SUPERSCRIPT_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_SUPERSCRIPT_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_SUPERSCRIPT_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_SUPERSCRIPT_MARKER_END;
                return true;
            }
            break;
        case M_SUPERSCRIPT_U:
            if (
                valid_symbols[T_SUPERSCRIPT_UNCONSTRAINED_MARKER_START]
            )
            {
                lexer->result_symbol = T_SUPERSCRIPT_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_SUPERSCRIPT_UNCONSTRAINED_MARKER_END]
            )
            {
                lexer->result_symbol = T_SUPERSCRIPT_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_SUBSCRIPT:
            if (
                valid_symbols[T_SUBSCRIPT_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_SUBSCRIPT_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_SUBSCRIPT_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_SUBSCRIPT_MARKER_END;
                return true;
            }
            break;
        case M_SUBSCRIPT_U:
            if (
                valid_symbols[T_SUBSCRIPT_UNCONSTRAINED_MARKER_START]
            )
            {
                lexer->result_symbol = T_SUBSCRIPT_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_SUBSCRIPT_UNCONSTRAINED_MARKER_END]
            )
            {
                lexer->result_symbol = T_SUBSCRIPT_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_HIGHLIGHT:
            if (
                valid_symbols[T_HIGHLIGHT_MARKER_START] &&
                !is_newline_or_space_extended(scanner->lookahead) &&
                (
                    is_newline(prev) ||
                    is_space(prev) ||
                    is_punctuation(prev) ||
                    is_inline_markup_start_char(prev)
                )
            ) {
                lexer->result_symbol = T_HIGHLIGHT_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_HIGHLIGHT_MARKER_END] &&
                !is_space_extended(prev) &&
                (
                    is_newline_or_space_extended(scanner->lookahead) ||
                    is_punctuation(scanner->lookahead)
                )
            ) {
                lexer->result_symbol = T_HIGHLIGHT_MARKER_END;
                return true;
            }
            break;
        case M_HIGHLIGHT_U:
            if (
                valid_symbols[T_HIGHLIGHT_UNCONSTRAINED_MARKER_START]
            )
            {
                lexer->result_symbol = T_HIGHLIGHT_UNCONSTRAINED_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_HIGHLIGHT_UNCONSTRAINED_MARKER_END]
            )
            {
                lexer->result_symbol = T_HIGHLIGHT_UNCONSTRAINED_MARKER_END;
                return true;
            }
            break;
        case M_INCLUDE:
            if (
                valid_symbols[T_INCLUDE_MARKER] &&
                match_before(scanner, STR_INCLUDE, strlen(STR_INCLUDE), match_newline)
            ) {
                lexer->result_symbol = T_INCLUDE_MARKER;
                return true;
            }
            break;
        case M_IMAGE:
            if (
                valid_symbols[T_IMAGE_MARKER] &&
                match_before(scanner, STR_IMAGE, strlen(STR_IMAGE), match_newline)
            )
            {
                lexer->result_symbol = T_IMAGE_MARKER;
                return true;
            }
            break;
        case M_IMAGE_INLINE:
            if (
                valid_symbols[T_IMAGE_INLINE_MARKER]
            )
            {
                lexer->result_symbol = T_IMAGE_INLINE_MARKER;
                return true;
            }
            break;
        case M_XREF:
            if (
                valid_symbols[T_XREF_MARKER]
            )
            {
                lexer->result_symbol = T_XREF_MARKER;
                return true;
            }
            break;
        case M_FOOTNOTE:
            if (
                valid_symbols[T_FOOTNOTE_MARKER]
            )
            {
                lexer->result_symbol = T_FOOTNOTE_MARKER;
                return true;
            }
            break;
        case M_ICON:
            if (
                valid_symbols[T_ICON_MARKER]
            )
            {
                lexer->result_symbol = T_ICON_MARKER;
                return true;
            }
            break;
        case M_LINK:
            if (
                valid_symbols[T_LINK_MARKER]
            )
            {
                lexer->result_symbol = T_LINK_MARKER;
                return true;
            }
            break;
        case M_MAILTO:
            if (
                valid_symbols[T_MAILTO_MARKER]
            )
            {
                lexer->result_symbol = T_MAILTO_MARKER;
                return true;
            }
            break;
        case M_CALLOUT:
            if (
                valid_symbols[T_CALLOUT_MARKER_FROM] &&
                (
                    match_before(scanner, STR_CALLOUT,  strlen(STR_CALLOUT),  match_space) ||
                    match_before(scanner, STR_CALLOUT1, strlen(STR_CALLOUT1), match_space) ||
                    match_before(scanner, STR_CALLOUT2, strlen(STR_CALLOUT2), match_space) ||
                    match_before(scanner, STR_CALLOUT3, strlen(STR_CALLOUT3), match_space) ||
                    match_before(scanner, STR_CALLOUT4, strlen(STR_CALLOUT4), match_space) ||
                    match_before(scanner, STR_CALLOUT5, strlen(STR_CALLOUT5), match_space) ||
                    match_before(scanner, STR_CALLOUT6, strlen(STR_CALLOUT6), match_space) ||
                    match_before(scanner, STR_CALLOUT7, strlen(STR_CALLOUT7), match_space) ||
                    match_before(scanner, STR_CALLOUT8, strlen(STR_CALLOUT8), match_space) ||
                    match_before(scanner, STR_CALLOUT9, strlen(STR_CALLOUT9), match_space)
                ) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_CALLOUT_MARKER_FROM;
                return true;
            }
            if (
                valid_symbols[T_CALLOUT_MARKER_TO] &&
                (
                    match_before(scanner, STR_CALLOUT,  strlen(STR_CALLOUT),  match_newline) ||
                    match_before(scanner, STR_CALLOUT1, strlen(STR_CALLOUT1), match_newline) ||
                    match_before(scanner, STR_CALLOUT2, strlen(STR_CALLOUT2), match_newline) ||
                    match_before(scanner, STR_CALLOUT3, strlen(STR_CALLOUT3), match_newline) ||
                    match_before(scanner, STR_CALLOUT4, strlen(STR_CALLOUT4), match_newline) ||
                    match_before(scanner, STR_CALLOUT5, strlen(STR_CALLOUT5), match_newline) ||
                    match_before(scanner, STR_CALLOUT6, strlen(STR_CALLOUT6), match_newline) ||
                    match_before(scanner, STR_CALLOUT7, strlen(STR_CALLOUT7), match_newline) ||
                    match_before(scanner, STR_CALLOUT8, strlen(STR_CALLOUT8), match_newline) ||
                    match_before(scanner, STR_CALLOUT9, strlen(STR_CALLOUT9), match_newline)
                ) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_CALLOUT_MARKER_TO;
                return true;
            }
            break;
        case M_OPTION_START:
            if (
                valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_START] &&
                !match_before(scanner, STR_OPTION_START, strlen(STR_OPTION_START), match_newline_or_space)
            ) {
                lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_START;
                return true;
            }
            if (
                valid_symbols[T_BLOCK_OPTION_MARKER_START] &&
                match_before(scanner, STR_OPTION_START, strlen(STR_OPTION_START), match_newline)
            ) {
                lexer->result_symbol = T_BLOCK_OPTION_MARKER_START;
                return true;
            }
            break;
        case M_OPTION_END:
            if (
                valid_symbols[T_INLINE_OPTION_BLOCK_MARKER_END]
            ) {
                lexer->result_symbol = T_INLINE_OPTION_BLOCK_MARKER_END;
                return true;
            }
            if (
                valid_symbols[T_BLOCK_OPTION_MARKER_END] &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_OPTION_MARKER_END;
                return true;
            }
            break;
        case M_CROSSREFERENCE_START:
            if (valid_symbols[T_CROSSREFERENCE_MARKER_START])
                lexer->result_symbol = T_CROSSREFERENCE_MARKER_START;
            else lexer->result_symbol = T_TEXT;
            return true;
            break;
        case M_CROSSREFERENCE_END:
            if (valid_symbols[T_CROSSREFERENCE_MARKER_END])
                lexer->result_symbol = T_CROSSREFERENCE_MARKER_END;
            else lexer->result_symbol = T_TEXT;
            return true;
            break;
        case M_BLOCK_COMMENT_MARKER:
            if (
                valid_symbols[T_BLOCK_COMMENT_MARKER] &&
                match_before(scanner, STR_BLOCK_COMMENT_MARKER, strlen(STR_BLOCK_COMMENT_MARKER), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_COMMENT_MARKER;
                return true;
            }
            break;
        case M_TITLE3_BLOCK_EXAMPLE_SEPARATOR1:
            if (
                valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR1] &&
                match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR1, strlen(STR_BLOCK_EXAMPLE_SEPARATOR1), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR1;
                return true;
            }
            if (
                valid_symbols[T_TITLE_MARKER3] &&
                match_before(scanner, STR_SECTION_HEADER3, strlen(STR_SECTION_HEADER3), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_TITLE_MARKER3;
                return true;
            }
            break;
        case M_TITLE4_BLOCK_EXAMPLE_SEPARATOR2:
            if (
                valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR2] &&
                match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR2, strlen(STR_BLOCK_EXAMPLE_SEPARATOR2), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR2;
                return true;
            }
            if (
                valid_symbols[T_TITLE_MARKER4] &&
                match_before(scanner, STR_SECTION_HEADER4, strlen(STR_SECTION_HEADER4), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_TITLE_MARKER4;
                return true;
            }
            break;
        case M_TITLE5_BLOCK_EXAMPLE_SEPARATOR3:
            if (
                valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR3] &&
                match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR3, strlen(STR_BLOCK_EXAMPLE_SEPARATOR3), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR3;
                return true;
            }
            if (
                valid_symbols[T_TITLE_MARKER5] &&
                match_before(scanner, STR_SECTION_HEADER5, strlen(STR_SECTION_HEADER5), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_TITLE_MARKER5;
                return true;
            }
            break;
        case M_BLOCK_LISTING_SEPARATOR:
            if (
                valid_symbols[T_BLOCK_LISTING_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_LISTING_SEPARATOR, strlen(STR_BLOCK_LISTING_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_LISTING_SEPARATOR;
                return true;
            }
            break;
        case M_BLOCK_LITERAL_SEPARATOR_NUMBERED4:
            if (
                valid_symbols[T_BLOCK_LITERAL_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_LITERAL_SEPARATOR, strlen(STR_BLOCK_LITERAL_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_LITERAL_SEPARATOR;
                return true;
            }
            if (
                valid_symbols[T_LIST_NUMBERED] &&
                match_before(scanner, STR_LIST_NUMBERED4, strlen(STR_LIST_NUMBERED4), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_NUMBERED;
                return true;
            }
            break;
        case M_BLOCK_OPEN_SEPARATOR:
            if (
                valid_symbols[T_BLOCK_OPEN_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_OPEN_SEPARATOR, strlen(STR_BLOCK_OPEN_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_OPEN_SEPARATOR;
                return true;
            }
            break;
        case M_BLOCK_SIDEBAR_SEPARATOR_UNNUMBERED_ASTERISK4:
            if (
                valid_symbols[T_BLOCK_SIDEBAR_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_SIDEBAR_SEPARATOR, strlen(STR_BLOCK_SIDEBAR_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_SIDEBAR_SEPARATOR;
                return true;
            }
            if (
                valid_symbols[T_LIST_ASTERISK_MARKER] &&
                match_before(scanner, STR_LIST_UNNUMBERED_ASTERISK4, strlen(STR_LIST_UNNUMBERED_ASTERISK4), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_ASTERISK_MARKER;
                return true;
            }
            break;
        case M_BLOCK_PASSTHROUGH_SEPARATOR:
            if (
                valid_symbols[T_BLOCK_PASSTHROUGH_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_PASSTHROUGH_SEPARATOR, strlen(STR_BLOCK_PASSTHROUGH_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_PASSTHROUGH_SEPARATOR;
                return true;
            }
            break;
        case M_BLOCK_QUOTE_SEPARATOR:
            if (
                valid_symbols[T_BLOCK_QUOTE_SEPARATOR] &&
                match_before(scanner, STR_BLOCK_QUOTE_SEPARATOR, strlen(STR_BLOCK_QUOTE_SEPARATOR), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_QUOTE_SEPARATOR;
                return true;
            }
            break;
        case M_BLOCK_TABLE_SEPARATOR1:
            if (
                valid_symbols[T_BLOCK_TABLE_SEPARATOR1] &&
                match_before(scanner, STR_BLOCK_TABLE_SEPARATOR1, strlen(STR_BLOCK_TABLE_SEPARATOR1), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR1;
                return true;
            }
            break;
        case M_BLOCK_TABLE_SEPARATOR2:
            if (
                valid_symbols[T_BLOCK_TABLE_SEPARATOR2] &&
                match_before(scanner, STR_BLOCK_TABLE_SEPARATOR2, strlen(STR_BLOCK_TABLE_SEPARATOR2), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR2;
                return true;
            }
            break;
        case M_TABLE_CELL_MARKER1:
            if (valid_symbols[T_TABLE_CELL_MARKER1]) {
                lexer->result_symbol = T_TABLE_CELL_MARKER1;
                return true;
            }
            break;
        case M_TABLE_CELL_MARKER2:
            if (valid_symbols[T_TABLE_CELL_MARKER2]) {
                lexer->result_symbol = T_TABLE_CELL_MARKER2;
                return true;
            }
            break;
        case M_PAGE_BREAK:
            if (
                valid_symbols[T_PAGE_BREAK]
            )
            {
                lexer->result_symbol = T_PAGE_BREAK;
                return true;
            }
            break;
        case M_THEMATIC_BREAK:
            if (
                valid_symbols[T_THEMATIC_BREAK]
            )
            {
                lexer->result_symbol = T_THEMATIC_BREAK;
                return true;
            }
            break;
        case M_THEMATIC_BREAK_UNNUMBERED_ASTERISK3:
            if(
                valid_symbols[T_THEMATIC_BREAK] &&
                match_before(scanner, STR_THEMATIC_BREAK2, strlen(STR_THEMATIC_BREAK2), match_newline) &&
                is_newline(scanner->lookahead)
            ) {
                lexer->result_symbol = T_THEMATIC_BREAK;
                return true;
            }
            if(
                valid_symbols[T_LIST_ASTERISK_MARKER] &&
                match_before(scanner, STR_LIST_UNNUMBERED_ASTERISK3, strlen(STR_LIST_UNNUMBERED_ASTERISK3), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_ASTERISK_MARKER;
                return true;
            }
            break;
        case M_BLOCK_TITLE_LIST_NUMBERED1:
            if (
                valid_symbols[T_BLOCK_TITLE_MARKER] &&
                match_before(scanner, STR_BLOCK_TITLE, strlen(STR_BLOCK_TITLE), match_emptyline) &&
                !is_space(scanner->lookahead) &&
                !is_number(scanner->lookahead)
            ) {
                lexer->result_symbol = T_BLOCK_TITLE_MARKER;
                return true;
            }
            if (
                valid_symbols[T_LIST_NUMBERED] &&
                match_before(scanner, STR_LIST_NUMBERED1, strlen(STR_LIST_NUMBERED1), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_NUMBERED;
                return true;
            }
            break;
        case M_LIST_UNNUMBERED_ASTERISK:
            if (
                valid_symbols[T_LIST_ASTERISK_MARKER] &&
                is_space(scanner->lookahead) &&
                (
                    match_before(scanner, STR_LIST_UNNUMBERED_ASTERISK2, strlen(STR_LIST_UNNUMBERED_ASTERISK2), match_newline) ||
                    match_before(scanner, STR_LIST_UNNUMBERED_ASTERISK5, strlen(STR_LIST_UNNUMBERED_ASTERISK5), match_newline)
                )
            ) {
                lexer->result_symbol = T_LIST_ASTERISK_MARKER;
                return true;
            }
            break;
        case M_LIST_UNNUMBERED_HYPHEN:
            if (
                valid_symbols[T_LIST_HYPHEN_MARKER] &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_HYPHEN_MARKER;
                return true;
            }
            break;
        case M_LIST_NUMBERED:
            if (
                valid_symbols[T_LIST_NUMBERED] &&
                (
                    match_before(scanner, STR_LIST_NUMBERED1, strlen(STR_LIST_NUMBERED1), match_newline) ||
                    match_before(scanner, STR_LIST_NUMBERED2, strlen(STR_LIST_NUMBERED2), match_newline) ||
                    match_before(scanner, STR_LIST_NUMBERED3, strlen(STR_LIST_NUMBERED3), match_newline) ||
                    match_before(scanner, STR_LIST_NUMBERED5, strlen(STR_LIST_NUMBERED5), match_newline)
                ) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_LIST_NUMBERED;
                return true;
            }
            if (valid_symbols[T_TEXT]) {
                lexer->result_symbol = T_TEXT;
                return true;
            }
            break;
        case M_ADMONITION_NOTE:
            if (
                valid_symbols[T_ADMONITION_NOTE_MARKER] &&
                match_before(scanner, STR_ADMONITION_NOTE, strlen(STR_ADMONITION_NOTE), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ADMONITION_NOTE_MARKER;
                return true;
            }
            break;
        case M_ADMONITION_TIP:
            if (
                valid_symbols[T_ADMONITION_TIP_MARKER] &&
                match_before(scanner, STR_ADMONITION_TIP, strlen(STR_ADMONITION_TIP), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ADMONITION_TIP_MARKER;
                return true;
            }
            break;
        case M_ADMONITION_IMPORTANT:
            if (
                valid_symbols[T_ADMONITION_IMPORTANT_MARKER] &&
                match_before(scanner, STR_ADMONITION_IMPORTANT, strlen(STR_ADMONITION_IMPORTANT), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ADMONITION_IMPORTANT_MARKER;
                return true;
            }
            break;
        case M_ADMONITION_CAUTION:
            if (
                valid_symbols[T_ADMONITION_CAUTION_MARKER] &&
                match_before(scanner, STR_ADMONITION_CAUTION, strlen(STR_ADMONITION_CAUTION), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ADMONITION_CAUTION_MARKER;
                return true;
            }
            break;
        case M_ADMONITION_WARNING:
            if (
                valid_symbols[T_ADMONITION_WARNING_MARKER] &&
                match_before(scanner, STR_ADMONITION_WARNING, strlen(STR_ADMONITION_WARNING), match_newline) &&
                is_space(scanner->lookahead)
            ) {
                lexer->result_symbol = T_ADMONITION_WARNING_MARKER;
                return true;
            }
            break;
        case M_ATTRIBUTE_REFERENCE_START:
            if (valid_symbols[T_ATTRIBUTE_REFERENCE_START_MARKER]) {
                lexer->result_symbol = T_ATTRIBUTE_REFERENCE_START_MARKER;
                return true;
            }
            break;
        case M_ATTRIBUTE_REFERENCE_END:
            if (valid_symbols[T_ATTRIBUTE_REFERENCE_END_MARKER]) {
                lexer->result_symbol = T_ATTRIBUTE_REFERENCE_END_MARKER;
                return true;
            }
            break;
        case M_TAG:
            if (
                valid_symbols[T_TAG_MARKER]
            )
            {
                lexer->result_symbol = T_TAG_MARKER;
                return true;
            }
            break;
        case M_TAGEND:
            if (
                valid_symbols[T_TAG_END_MARKER]
            )
            {
                lexer->result_symbol = T_TAG_END_MARKER;
                return true;
            }
            break;
        case M_IFDEF:
            if (
                valid_symbols[T_IFDEF_MARKER]
            )
            {
                lexer->result_symbol = T_IFDEF_MARKER;
                return true;
            }
            break;
        case M_IFEVAL:
            if (
                valid_symbols[T_IFEVAL_MARKER]
            )
            {
                lexer->result_symbol = T_IFEVAL_MARKER;
                return true;
            }
            break;
        case M_IFEND:
            if (
                valid_symbols[T_ENDIF_MARKER]
            )
            {
                lexer->result_symbol = T_ENDIF_MARKER;
                return true;
            }
            break;
        case M_PASS:
            if (
                valid_symbols[T_PASS_MARKER]
            )
            {
                lexer->result_symbol = T_PASS_MARKER;
                return true;
            }
            break;
        case M_STEM:
            if (
                valid_symbols[T_STEM_MARKER]
            )
            {
                lexer->result_symbol = T_STEM_MARKER;
                return true;
            }
            break;
        case M_LATEXMATH:
            if (
                valid_symbols[T_LATEXMATH_MARKER]
            )
            {
                lexer->result_symbol = T_LATEXMATH_MARKER;
                return true;
            }
            break;
        case M_DESCRIPTION_LIST:
            if (
                valid_symbols[T_DESCRIPTION_LIST_MARKER]
            ) {
                lexer->result_symbol = T_DESCRIPTION_LIST_MARKER;
                return true;
            }
            break;
        case M_ESCAPE:
            if (
                valid_symbols[T_ESCAPE] &&
                !is_newline_or_space_extended(scanner->lookahead)
            )
            {
                scanner->advance(scanner);
                lexer->result_symbol = T_ESCAPE;
                return true;
            }
            break;
        default:
            break;
        }
    }

    return false;
}
