// vim:sw=2
/*

*/
if (scanner->lookahead == '|' && is_newline(scanner->lookbehind->stack[0])) {
    scanner->advance(scanner);
    if (scanner->lookahead == '=') { scanner->advance(scanner); } else { return false; }
    if (scanner->lookahead == '=') { scanner->advance(scanner); } else { return false; }
    lexer->result_symbol = T_BLOCK_TABLE_SEPARATOR1;
    return true;
}
