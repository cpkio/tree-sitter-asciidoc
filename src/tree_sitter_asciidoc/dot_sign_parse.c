// vim:sw=2
/*

[x] BLOCK LITERAL

[x] BLOCK TITLE

[ ] NUMBERED LIST VARIATION

*/
if (
  (valid_symbols[T_BLOCK_LITERAL_SEPARATOR] || valid_symbols[T_BLOCK_TITLE_MARKER]) &&
  scanner->lookahead == '.' && is_newline(scanner->lookbehind->stack[0])
) {
  consume(scanner, -1);

  if (
    match_before(scanner, STR_BLOCK_LITERAL_SEPARATOR,
                   strlen(STR_BLOCK_LITERAL_SEPARATOR), match_newline) &&
    is_newline(scanner->lookahead)
  ) {
    // scanner->advance(scanner);
    context[V_LITERAL] = !(context[V_LITERAL]);
    lexer->result_symbol = T_BLOCK_LITERAL_SEPARATOR;
    return true;
  }

  if (
    !is_space(scanner->lookahead) && !is_number(scanner->lookahead)
  ) {
    lexer->result_symbol = T_BLOCK_TITLE_MARKER;
    return true;
  }

  // if ( is_space(scanner->lookahead) ) {
  //   lexer->result_symbol = T_LIST_DECIMAL__MARKER;
  //   return true;
  // }

}
