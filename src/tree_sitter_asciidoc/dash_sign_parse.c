// vim:sw=2
/*

[x] OPEN BLOCK

[x] BLOCK LISTING

[x] unnumbered list

*/
if (
  (valid_symbols[T_BLOCK_LISTING_SEPARATOR] ||
  valid_symbols[T_BLOCK_OPEN_SEPARATOR] ||
  valid_symbols[T_THEMATIC_BREAK]) &&
  scanner->lookahead == '-' &&
  is_newline(scanner->lookbehind->stack[0])
) {
  consume(scanner, -1);

  if (match_before(scanner, STR_BLOCK_LISTING_SEPARATOR,
                   strlen(STR_BLOCK_LISTING_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)) {
    scanner->advance(scanner);
    context[V_LISTING] = !(context[V_LISTING]);
    lexer->result_symbol = T_BLOCK_LISTING_SEPARATOR;
    return true;
  }

  if (match_before(scanner, STR_BLOCK_OPEN_SEPARATOR,
                   strlen(STR_BLOCK_OPEN_SEPARATOR), match_newline) &&
      is_newline(scanner->lookahead)) {
    scanner->advance(scanner);
    lexer->result_symbol = T_BLOCK_OPEN_SEPARATOR;
    return true;
  }

  if (match_before(scanner, STR_THEMATIC_BREAK1, strlen(STR_THEMATIC_BREAK1),
                   match_newline) &&
      is_newline(scanner->lookahead)) {
    // scanner->skip(scanner);
    lexer->result_symbol = T_THEMATIC_BREAK;
    return true;
  }
}
