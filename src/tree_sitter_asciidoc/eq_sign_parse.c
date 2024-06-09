// vim:sw=2
/*

[x] BLOCK EXAMPLE ==== , ======, ======

[x] Заголовки всех 6 уровней

*/
if (
  (
    valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR1] ||
    valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR2] ||
    valid_symbols[T_BLOCK_EXAMPLE_SEPARATOR3] ||
    valid_symbols[T_TITLE_MARKER0] ||
    valid_symbols[T_TITLE_MARKER1] ||
    valid_symbols[T_TITLE_MARKER2] ||
    valid_symbols[T_TITLE_MARKER3] ||
    valid_symbols[T_TITLE_MARKER4] ||
    valid_symbols[T_TITLE_MARKER5]
  ) &&
  scanner->lookahead == CHAR_SECTIONHEADER &&
  is_newline(scanner->lookbehind->stack[0])
) {
  do {
    scanner->advance(scanner);
  } while (scanner->lookbehind->stack[0] ==
           scanner->lookahead);
  if (scanner->lookahead == CHAR_SECTIONHEADER) {
    scanner->advance(scanner);
  }
  if (match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR3,
                   strlen(STR_BLOCK_EXAMPLE_SEPARATOR3), match_newline) &&
      is_newline(scanner->lookahead)) {
    lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR3;
    return true;
  }
  if (match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR2,
                   strlen(STR_BLOCK_EXAMPLE_SEPARATOR2), match_newline) &&
      is_newline(scanner->lookahead)) {
    lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR2;
    return true;
  }
  if (match_before(scanner, STR_BLOCK_EXAMPLE_SEPARATOR1,
                   strlen(STR_BLOCK_EXAMPLE_SEPARATOR1), match_newline) &&
      is_newline(scanner->lookahead)) {
    lexer->result_symbol = T_BLOCK_EXAMPLE_SEPARATOR1;
    return true;
  }

  if (match_before(scanner, STR_SECTION_HEADER5, strlen(STR_SECTION_HEADER5),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER5;
    return true;
  }
  if (match_before(scanner, STR_SECTION_HEADER4, strlen(STR_SECTION_HEADER4),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER4;
    return true;
  }
  if (match_before(scanner, STR_SECTION_HEADER3, strlen(STR_SECTION_HEADER3),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER3;
    return true;
  }
  if (match_before(scanner, STR_SECTION_HEADER2, strlen(STR_SECTION_HEADER2),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER2;
    return true;
  }
  if (match_before(scanner, STR_SECTION_HEADER1, strlen(STR_SECTION_HEADER1),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER1;
    return true;
  }
  if (match_before(scanner, STR_SECTION_HEADER0, strlen(STR_SECTION_HEADER0),
                   match_newline) &&
      is_space(scanner->lookahead)) {
    lexer->result_symbol = T_TITLE_MARKER0;
    return true;
  }
}
