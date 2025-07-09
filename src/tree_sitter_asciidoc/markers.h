#ifndef TREE_SITTER_ASCIIDOC_MARKERS_H
#define TREE_SITTER_ASCIIDOC_MARKERS_H




#define STR_MONOSPACE "`"

#define STR_MONOSPACE_U "``"

#define STR_INLINE_PASSTHROUGH "+"

#define STR_INLINE_PASSTHROUGH_U "++"

#define STR_HARDBREAK "+"
#define STR_BLOCK_CONTINUATION "+"
#define STR_SECTION_HEADER0 "="
#define STR_SECTION_HEADER1 "=="
#define STR_SECTION_HEADER2 "==="
#define STR_SECTION_HEADER3 "===="
#define STR_SECTION_HEADER4 "====="
#define STR_SECTION_HEADER5 "======"
#define STR_ATTRIBUTE_MARKER ":"

#define STR_ATTRIBUTE_MARKER_START_NEG ":!"
#define STR_ATTRIBUTE_MARKER_END_NEG "!:"
#define STR_COMMENT_MARKER "//"
#define STR_EMPHASIS "_"

#define STR_EMPHASIS_U "__"

#define STR_STRONG "*"

#define STR_STRONG_U "**"

#define STR_SUPERSCRIPT "^"

#define STR_SUPERSCRIPT_U "^^"

#define STR_SUBSCRIPT "~"

#define STR_SUBSCRIPT_U "~~"

#define STR_HIGHLIGHT "#"

#define STR_HIGHLIGHT_U "##"


#define STR_INCLUDE "include::"
#define STR_IMAGE "image::"
#define STR_IMAGE_INLINE "image:"
#define STR_XREF "xref:"
#define STR_FOOTNOTE "footnote:"
#define STR_ICON "icon:"
// #define STR_HTTP "http:"
#define STR_LINK "link:"
#define STR_MAILTO "mailto:"

#define STR_CALLOUT  "<.>"
#define STR_CALLOUT1 "<1>"
#define STR_CALLOUT2 "<2>"
#define STR_CALLOUT3 "<3>"
#define STR_CALLOUT4 "<4>"
#define STR_CALLOUT5 "<5>"
#define STR_CALLOUT6 "<6>"
#define STR_CALLOUT7 "<7>"
#define STR_CALLOUT8 "<8>"
#define STR_CALLOUT9 "<9>"

#define STR_OPTION_START "["
#define STR_OPTION_END "]"
#define STR_CROSSREFERENCE_START "<<"
#define STR_CROSSREFERENCE_END ">>"
#define STR_BLOCK_COMMENT_MARKER "////"
#define STR_BLOCK_EXAMPLE_SEPARATOR1 "===="
#define STR_BLOCK_EXAMPLE_SEPARATOR2 "====="
#define STR_BLOCK_EXAMPLE_SEPARATOR3 "======"
#define STR_BLOCK_LISTING_SEPARATOR "----"
#define STR_BLOCK_LITERAL_SEPARATOR "...."
#define STR_BLOCK_OPEN_SEPARATOR "--"
#define STR_BLOCK_SIDEBAR_SEPARATOR "****"
#define STR_BLOCK_PASSTHROUGH_SEPARATOR "++++"
#define STR_BLOCK_QUOTE_SEPARATOR "____"
#define STR_BLOCK_TABLE_SEPARATOR1 "|==="
#define STR_BLOCK_TABLE_SEPARATOR2 "!==="

#define STR_TABLE_CELL_MARKER1 "|"
#define STR_TABLE_CELL_MARKER2 "!"

#define STR_PAGE_BREAK "<<<"
#define STR_THEMATIC_BREAK1 "---"
#define STR_THEMATIC_BREAK2 "***"
#define STR_BLOCK_TITLE "."

#define STR_LIST_UNNUMBERED_ASTERISK1 "*"
#define STR_LIST_UNNUMBERED_ASTERISK2 "**"
#define STR_LIST_UNNUMBERED_ASTERISK3 "***"
#define STR_LIST_UNNUMBERED_ASTERISK4 "****"
#define STR_LIST_UNNUMBERED_ASTERISK5 "*****"

#define STR_LIST_HYPHEN "-"

#define STR_LIST_NUMBERED1 "."
#define STR_LIST_NUMBERED2 ".."
#define STR_LIST_NUMBERED3 "..."
#define STR_LIST_NUMBERED4 "...."
#define STR_LIST_NUMBERED5 "....."

#define STR_ADMONITION_NOTE "NOTE:"
#define STR_ADMONITION_TIP "TIP:"
#define STR_ADMONITION_IMPORTANT "IMPORTANT:"
#define STR_ADMONITION_CAUTION "CAUTION:"
#define STR_ADMONITION_WARNING "WARNING:"


#define STR_ATTRIBUTE_REFERENCE_START "{"
#define STR_ATTRIBUTE_REFERENCE_END "}"
#define STR_TAG "tag::"
#define STR_TAGEND "end::"
#define STR_IFDEF "ifdef::"
#define STR_IFNDEF "ifndef::"
#define STR_IFEVAL "ifeval::"
#define STR_IFEND "endif::"

#define STR_PASS "pass:"
#define STR_STEM "stem:"
#define STR_LATEXMATH "latexmath:"

#define STR_DESCRIPTION_LIST1 ":: "
#define STR_DESCRIPTION_LIST2 "::: "
#define STR_DESCRIPTION_LIST3 ":::: "
#define STR_DESCRIPTION_LIST4 ";; "

#define STR_ESCAPE "\\"

// #define STR_ANCHOR "anchor:"
// #define STR_BTN "btn:"
// #define STR_KBD "kbd:"
// #define STR_INDEXTERM "indexterm:"

#endif // TREE_SITTER_ASCIIDOC_MARKERS_H
