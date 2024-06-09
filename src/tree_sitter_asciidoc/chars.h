#ifndef TREE_SITTER_ASCIIDOC_CHARS_H
#define TREE_SITTER_ASCIIDOC_CHARS_H

#include <tree_sitter/parser.h>

#define CHAR_EOF 0
#define CHAR_NEWLINE 10 // LF
#define CHAR_CARRIAGE_RETURN 13 // CR
#define CHAR_NBSP 160

#define CHAR_EM_DASH 8212

#define CHAR_SPACE ' '
#define CHAR_FORM_FEED '\f'
#define CHAR_TAB '\t'
#define CHAR_VERTICAL_TAB '\v'

#define CHAR_EN_QUAD 8192
#define CHAR_EM_QUAD 8193
#define CHAR_EN_SPACE 8194
#define CHAR_EM_SPACE 8195
#define CHAR_THREE_PER_EM_SPACE 8196
#define CHAR_FOUR_PER_EM_SPACE 8197
#define CHAR_SIX_PER_EM_SPACE 8198
#define CHAR_FIGURE_SPACE 8199
#define CHAR_PUNCTUATION_SPACE 8200
#define CHAR_THIN_SPACE 8201
#define CHAR_HAIR_SPACE 8202

#define CHAR_HIGHLIGHT '#'
#define CHAR_OPTION_START '['
#define CHAR_OPTION_END ']'
#define CHAR_STRONG '*'
#define CHAR_EMPHASIS '_'
#define CHAR_MONOSPACE '`'
#define CHAR_SUPERSCRIPT '^'
#define CHAR_SUBSCRIPT '~'
#define CHAR_INLINE_PASSTHROUGH '+'
#define CHAR_MACRO_SEPARATOR ':' // also '::' for blocks
#define CHAR_CROSSREFERENCE_START '<' // '<<' to start
#define CHAR_CROSSREFERENCE_END '>' // '>>' to end
#define CHAR_ATTRIBUTE_REFERENCE_START '{'
#define CHAR_ATTRIBUTE_REFERENCE_END '}'

#define CHAR_ATTRIBUTE ':'
#define CHAR_ATTRIBUTENEGATION '!'
#define CHAR_SECTIONHEADER '='
#define CHAR_COMMENT '/'
#define CHAR_ESCAPE '\\'

#define CHAR_CELL1 '|'
#define CHAR_CELL2 '!'

static bool is_newline(int32_t c);
static bool is_space(int32_t c);
static bool is_newline_or_space(int32_t c);
static bool is_anything(int32_t c);

static bool is_number(int32_t c);
static bool is_abc_lower(int32_t c);
static bool is_abc_upper(int32_t c);
static bool is_abc(int32_t c);
static bool is_alphanumeric(int32_t c);
static bool is_alphanumeric_dash_underscore(int32_t c);
static bool is_word(int32_t c);

static bool is_inline_markup_start_char(int32_t c);
static bool is_inline_markup_end_char(int32_t c);
static bool is_punctuation(int32_t c);

#endif /* ifndef TREE_SITTER_ASCIIDOC_CHARS_H */
