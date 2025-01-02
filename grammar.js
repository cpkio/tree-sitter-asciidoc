const common = require('./common/grammar.js')

module.exports = grammar({
    name: 'asciidoc',
    externals: $ => [
        $._txt,
        $._whitespace,
        $._newline,
        $.monospace_marker_start,
        $.monospace_marker_end,
        $.inline_passthrough_marker_start,
        $.inline_passthrough_marker_end,
        $.hardbreak,
        $.block_continuation,
        $.title_marker0,
        $.title_marker1,
        $.title_marker2,
        $.title_marker3,
        $.title_marker4,
        $.title_marker5,
        $.attribute_marker_start,
        $.attribute_marker_end,
        $.attribute_marker_start_neg,
        $.attribute_marker_end_neg,
        $.comment_marker,
        $.emphasis_marker_start,
        $.emphasis_marker_end,
        $.strong_marker_start,
        $.strong_marker_end,
        $.superscript_marker_start,
        $.superscript_marker_end,
        $.subscript_marker_start,
        $.subscript_marker_end,
        $.highlight_marker_start,
        $.highlight_marker_end,
        $.include_marker,
        $.image_marker,
        $.image_inline_marker,
        $.xref_marker,
        $.footnote_marker,
        $.icon_marker,
        $.http_marker,
        $.link_marker,
        $.mailto_marker,
        $.callout_marker_from,
        $.callout_marker_to,
        $.inline_option_block_marker_start,
        $.inline_option_block_marker_end,
        $.crossreference_marker_start,
        $.crossreference_marker_end,
        $.crossreference_splitter,
        $.block_comment,
        $.block_example_separator1,
        $.block_example_separator2,
        $.block_example_separator3,
        $.block_listing_separator,
        $.block_literal_separator,
        $.block_open_separator,
        $.block_sidebar_separator,
        $.block_passthrough_separator,
        $.block_quote_separator,
        $.block_table_separator1,
        $.block_table_separator2,
        $.table_cell_marker,
        $.block_option_marker_start,
        $.block_option_marker_end,
        $.page_break,
        $.thematic_break,
        $.block_title_marker,
        $.emptyline,
        $.list_asterisk_marker,
        $.list_hyphen_marker,
        $._admonition_note_marker,
        $._admonition_tip_marker,
        $._admonition_important_marker,
        $._admonition_caution_marker,
        $._admonition_warning_marker,
        $.list_decimal__marker,
        $._attribute_reference_start_marker,
        $._attribute_reference_end_marker,
        $.tag_marker,
        $.tag_end_marker,
        $.ifdef_marker, // + ifndef
        $.ifeval_marker,
        $.endif_marker,
        $.pass_marker,
        $.stem_marker,
        $.latexmath_marker,
        $.pass_splitter,
    ],
    extras: $ => [],
    conflicts: $ => [],
    precedences: $ => [],

    word: $ => $.kwd,

    rules: {
        document: $ => repeat(
            choice(
                $.title0,
                $.title1,
                $.title2,
                $.title3,
                $.title4,
                $.title5,
                $.block_title,
                $.block_continuation,
                $.paragraph,
                $.admonition,
                $.block_example1,
                $.block_example2,
                $.block_example3,
                $.block_listing,
                $.block_literal,
                $.block_open,
                $.block_sidebar,
                $.block_pass,
                $.block_quote,
                $.block_table,
                $.option_block,
                $.list_asterisk,
                $.list_checkbox,
                $.list_hyphen,
                $.list_decimal,
                $.list_decimal_,
                $.description_list,
                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                $.image_directive,
                $.attribute,
                $.page_break,
                $.thematic_break,
                $.callout,
                $.emptyline,
                $._newline,
            )
        ),

        _not_newline: $ => /[^\r\n]+/,

        _identifier: $ => prec.left(0, /[A-Za-z][-_\w]*/),

        paragraph: $ => prec.right(0, repeat1(
                          choice(
                            seq(
                              $._text,
                              optional($.hardbreak),
                              $._newline
                            ),
                          )
        )),

        _text: $ =>
                prec.left(0, repeat1(
                    choice(
                        $._txt,
                        common.punctuation_without($, [
                            '\\',
                        ]),
                        $._whitespace,
                        $.xref_directive,
                        $.xref_directive_id,
                        $.icon_macro,
                        $.footnote_macro,
                        // $.anchor_macro,
                        $.attribute_reference,
                        $.emphasis,
                        $.strong,
                        $.monospace,
                        $.highlight,
                        $.superscript,
                        $.subscript,
                        $.inline_passthrough,
                        $.replacement,
                        $.escape,
                        $.encoded_symbol,
                        // $.http,
                        // $.http_macro,
                        // $.email,
                        // $.mailto_macro,
                        $.link_macro,
                        $.cross_reference,
                        $.pass_macro,
                        $.image_macro,
                    ),
                )),


        block_title: $ => prec.right(50, seq($.block_title_marker, $._text, $._newline)),

        title0: $ =>
            prec.left(100,
            seq(
                alias($.title_marker0, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),
        title1: $ =>
            prec.left(100,
            seq(
                alias($.title_marker1, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),
        title2: $ =>
            prec.left(100,
            seq(
                alias($.title_marker2, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),
        title3: $ =>
            prec.left(100,
            seq(
                alias($.title_marker3, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),
        title4: $ =>
            prec.left(100,
            seq(
                alias($.title_marker4, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),
        title5: $ =>
            prec.left(100,
            seq(
                alias($.title_marker5, $.title_marker),
                $._whitespace,
                alias($._text, $.title_content),
                $._newline,
            )
        ),

        attribute: $ => choice(
            seq(
                choice($.attribute_marker_start, $.attribute_marker_start_neg),
                $.attr_name,
                $.attribute_marker_end,
                optional(seq($._whitespace, $.attr_value,)),
                $._newline
            ),
            seq(
                $.attribute_marker_start,
                $.attr_name,
                choice($.attribute_marker_end, $.attribute_marker_end_neg),
                optional(seq($._whitespace, $.attr_value,)),
                $._newline
            ),
        ),
        attr_name: $ => $._identifier,
        attr_value: $ => $._text,

        comment: $ => prec.left(10, seq(
          $.comment_marker,
          choice(
            seq(optional($._whitespace), $.tag),
            seq(
              repeat(choice($._txt, $._whitespace, common.punctuation_without($, []) )),
              $._newline
            )
          )
        )),
        tag: $ => seq(
            choice($.tag_marker, $.tag_end_marker),
            alias($._identifier, $.tag_name),
            '[]',
            $._newline
        ),
        ifdef: $ => seq(
            $.ifdef_marker,
            alias($._identifier, $.ifdef_attribute),
            repeat(seq(/[,+]/, alias($._identifier, $.ifdef_attribute))),
            '[',
            optional($._text),
            ']',
            $._newline
        ),
        endif: $ => seq(
            $.endif_marker,
            '[]',
            $._newline
        ),
        ifeval: $ => seq(
            $.ifeval_marker,
            '[',
            alias(repeat(choice(
              $.attribute_reference,
              $._txt,
              $._whitespace,
              common.punctuation_without($, [']'])
            )), $.ifeval_expr),
            ']',
            $._newline
        ),

        admonition: $ =>
            prec.right(
            seq(
                choice(
                    alias(
                       $._admonition_note_marker, $.admonition_note_label
                    ),
                    alias(
                        $._admonition_tip_marker, $.admonition_tip_label
                    ),
                    alias(
                        $._admonition_important_marker, $.admonition_important_label
                    ),
                    alias(
                        $._admonition_caution_marker, $.admonition_caution_label
                    ),
                    alias(
                        $._admonition_warning_marker, $.admonition_warning_label
                    ),
                ),
                $._whitespace,
                $.paragraph,
            )
        ),

        block_example1: $ => seq(
                            $.block_example_separator1,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example2,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_open,
                                $.block_pass,
                                $.block_quote,
                                $.block_sidebar,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_example_separator1,
                            ),
        block_example2: $ => seq(
                            $.block_example_separator2,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_open,
                                $.block_pass,
                                $.block_quote,
                                $.block_sidebar,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_example_separator2,
                            ),
        block_example3: $ => seq(
                            $.block_example_separator3,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example2,
                                $.block_listing,
                                $.block_literal,
                                $.block_open,
                                $.block_pass,
                                $.block_quote,
                                $.block_sidebar,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_example_separator3,
                            ),
        _literal_content: $ => choice(
                                  $.include_directive,
                                  $.ifdef, $.ifeval, $.endif,
                                  $.block_comment,
                                  $.comment,
                                  $.escape,
                                  $._txt,
                                  $._callout,
                                  common.punctuation_without($, []),
                                  $._whitespace,
                                  $._newline,
                              ),
        block_listing: $ => seq(
                                $.block_listing_separator,
                                repeat($._literal_content),
                                $.block_listing_separator,
                            ),
        block_literal: $ => seq(
                                $.block_literal_separator,
                                repeat($._literal_content),
                                $.block_literal_separator,
                            ),
        block_open: $ => seq(
                            $.block_open_separator,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example2,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_pass,
                                $.block_quote,
                                $.block_sidebar,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_open_separator,
                        ),
        block_sidebar: $ => seq(
                            $.block_sidebar_separator,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example2,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_pass,
                                $.block_quote,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_sidebar_separator,
                            ),
        block_pass: $ => seq(
                            $.block_passthrough_separator,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example2,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_sidebar,
                                $.block_quote,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_passthrough_separator,
                         ),
        block_quote: $ => seq(
                            $.block_quote_separator,
                            repeat(choice(
                                $._newline,
                                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                                $.image_directive,
                                $.admonition,
                                $.attribute,
                                $.block_continuation,
                                $.block_example1,
                                $.block_example2,
                                $.block_example3,
                                $.block_listing,
                                $.block_literal,
                                $.block_pass,
                                $.block_sidebar,
                                $.block_table,
                                $.block_title,
                                $.callout,
                                $.description_list,
                                $.emptyline,
                                $.list_asterisk,
                                $.list_checkbox,
                                $.list_decimal,
                                $.list_decimal_,
                                $.list_hyphen,
                                $.option_block,
                                $.paragraph,
                            )),
                            $.block_quote_separator,
                          ),
        block_table: $ => seq(
                            $.block_table_separator1,
                            repeat(choice(
                              $.table_cell,
                              // $.emptyline,
                              $._newline,
                              $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                            )),
                            $.block_table_separator1,
                          ),

        table_horizontal_alignment_operator: $ => choice('<', '>', '^'),
        table_vertical_alignment_operator: $ => choice('.<', '.>', '.^'),
        table_cell_styles: $ => choice('a', 'd', 'e', 'h', 'l', 'm', 's'),
        table_column_span_factor: $ => /\d+/,
        table_row_span_factor: $ => /\.\d+/,
        table_span_operator: $ => '+',
        table_cell_multiplication_factor: $ => /\d+/,
        table_cell_multiplication_operator: $ => '*',
        table_cell: $ => prec.right(seq(
            optional(
                seq(
                    optional(choice(
                        choice(
                            seq(
                                $.table_column_span_factor,
                                $.table_row_span_factor,
                                $.table_span_operator,
                            ),
                            seq(
                                $.table_column_span_factor,
                                $.table_span_operator,
                            ),
                            seq(
                                $.table_row_span_factor,
                                $.table_span_operator,
                            )
                        ),
                        seq(
                            $.table_cell_multiplication_factor,
                            $.table_cell_multiplication_operator,
                        ),
                    )),
                    optional($.table_horizontal_alignment_operator),
                    optional($.table_vertical_alignment_operator),
                    optional($.table_cell_styles)
                )
            ),
            $.table_cell_marker,
            repeat(choice(
                $.include_directive, $.comment, $.block_comment, $.ifdef, $.ifeval, $.endif,
                $.image_directive,
                $._newline,
                $.admonition,
                $.block_continuation,
                $.block_example1,
                $.block_example2,
                $.block_example3,
                $.block_listing,
                $.block_literal,
                $.block_open,
                $.block_pass,
                $.block_quote,
                $.block_sidebar,
                $.block_title,
                $.description_list,
                $.emptyline,
                $.list_asterisk,
                $.list_checkbox,
                $.list_decimal,
                $.list_decimal_,
                $.list_hyphen,
                $.option_block,
                $.paragraph,
            ))
        )),

        option_block: $ => prec.right(50, seq(
            $.block_option_marker_start,
            $._options_block_choice,
            repeat(
                seq(
                    ',',
                    optional($._whitespace),
                    $._options_block_choice,
                )
            ),
            $.block_option_marker_end,
        )),
        _options_block_choice: $ => choice(
            alias($._identifier, $.attribute),
            alias($.option_id, $.id),
            alias($.option_role, $.role),
            alias($.option_parameters, $.parameter),
            alias(
                choice(
                    $._paragraph_options,
                    $._block_options,
                    $._source_block_options,
                    $._table_options,
                    $._list_options
                ), $.parameter_recognized
            ),
            alias(/lines="\d+\.\.\d+"/, $.highlight_lines),
            alias(seq('caption="', $._caption_content, '"'), $.caption),
            alias(seq('reftext="', $._caption_content, '"'), $.reftext),
            alias($._admonition_labels, $.admonition),
            alias($._block_types, $.block_type),
            alias($._section_styles, $.section_style),
            $.option_table,
            $.option_subs,
            $.option_desclist
        ),
        _option_id_short: $ => seq('#', $._identifier),
        _option_id_named: $ => choice(
            seq(
                'id="',
                $._identifier,
                '"'
            ),
            seq(
                'id=',
                $._identifier,
            )
        ),
        option_id: $ => choice(
            $._option_id_short,
            $._option_id_named,
        ),
        _option_role_short: $ => seq('.', $._identifier),
        _option_role_named: $ => choice(
            seq(
                'role="',
                $._identifier,
                '"'
            ),
            seq(
                'role=',
                $._identifier,
            )
        ),
        option_role: $ => choice(
            $._option_role_short,
            $._option_role_named,
        ),
        _option_param_short: $ => seq('%', $._identifier),
        _option_param_named: $ => seq(
                choice('options','opts'),
                '="',
                $._identifier,
                repeat(
                    seq(
                        ',',
                        optional($._whitespace),
                        $._identifier,
                    )
                ),
                '"'
        ),
        option_parameters: $ => choice(
            $._option_param_short,
            $._option_param_named
        ),

        _caption_content: $ => /[^"]*/,
        _format_choice: $ => choice('psv', 'dsv', 'csv', 'tsv'),
        _separator_symbol: $ => /\W/,
        _frame_choice: $ => choice('all', 'ends', 'none', 'sides'),
        _grid_choice: $ => choice('all', 'cols', 'rows', 'none'),
        _stripes_choice: $ => choice('none', 'even', 'odd', 'hover', 'all'),
        _float_choice: $ => choice('left', 'right'),
        _width_value: $ => /\d+%/,
        _cols_option_column: $ => prec.left(choice(
            seq(
                $.table_horizontal_alignment_operator,
                optional($.table_vertical_alignment_operator),
                optional($.table_cell_multiplication_factor),
                optional($.table_cell_styles)
            ),
            seq(
                optional($.table_horizontal_alignment_operator),
                $.table_vertical_alignment_operator,
                optional($.table_cell_multiplication_factor),
                optional($.table_cell_styles)
            ),
            seq(
                optional($.table_horizontal_alignment_operator),
                optional($.table_vertical_alignment_operator),
                $.table_cell_multiplication_factor,
                optional($.table_cell_styles)
            ),
            seq(
                optional($.table_horizontal_alignment_operator),
                optional($.table_vertical_alignment_operator),
                optional($.table_cell_multiplication_factor),
                $.table_cell_styles
            ),
        )),
        _cols_values: $ => seq(
            $._cols_option_column,
            repeat(
                seq(
                    choice(',', ';'),
                    optional($._whitespace),
                    $._cols_option_column,
                )
            )
        ),
        option_table: $ => choice(
            alias(seq('cols="', $._cols_values, '"'), $.cols),
            alias(seq('format="', $._format_choice, '"'), $.format),
            alias(seq('format=', $._format_choice), $.format),
            alias(seq('separator=', $._separator_symbol), $.separator),
            alias(seq('frame="', $._frame_choice, '"'), $.frame),
            alias(seq('frame=', $._frame_choice), $.frame),
            alias(seq('grid="', $._grid_choice, '"'), $.grid),
            alias(seq('grid=', $._grid_choice), $.grid),
            alias(seq('stripes="', $._stripes_choice, '"'), $.stripes),
            alias(seq('stripes=', $._stripes_choice), $.stripes),
            alias(seq('float="', $._float_choice , '"'), $.float),
            alias(seq('float=', $._float_choice), $.float),
            alias(seq('width=', $._width_value), $.width)
        ),

        _subs_choice: $ => choice(/[+-]?none/, /[+-]?normal/, /[+-]?verbatim/, /[+-]?specialchars/, /[+-]?callouts/, /[+-]?quotes/, /[+-]?attributes/, /[+-]?replacements/, /[+-]?macros/, /[+-]?post_replacements/),
        _subs_values: $ => seq(
            $._subs_choice,
            repeat(
                seq(
                    ',',
                    optional($._whitespace),
                    $._subs_choice,
                )
            )
        ),
        option_subs: $ => field('subs', seq('subs="', $._subs_values, '"')),
        option_desclist: $ => choice(
            alias(seq('labelwidth=', /\d+/), $.labelwidth),
            alias(seq('itemwidth=', /\d+/), $.itemwidth)
        ),

        anchor: $ => seq(
            '[',
            repeat1(choice(
              $._option_id_short,
              $._option_role_short,
              $._option_param_short,
            )),
            ']'
        ),

        _icon_title_attr_quoted: $ => seq(
            'title="',
            repeat(choice($._txt, $._whitespace, common.punctuation_without($, ['"']))),
            '"'
        ),
        _icon_link_attr: $ => seq(
            'link=',
            choice($.http, $._filepath),
        ),
        _icon_link_attr_quoted: $ => seq(
            'link="',
            choice($.http, $._filepath),
            '"'
        ),
        _icon_window_choice: $ => choice('_blank', 'read-later'),
        _icon_window_attr: $ => seq(
            'window=',
            $._icon_window_choice,
        ),
        _icon_window_attr_quoted: $ => seq(
            'window="',
            $._icon_window_choice,
            '"'
        ),
        _icon_size_short: $ => choice('1x', '2x', '3x', '4x', '5x', 'fw', 'lg'),
        _icon_size_attr: $ => seq(
            'size=',
            $._icon_size_short
        ),
        _icon_size_attr_quoted: $ => seq(
            'size="',
            $._icon_size_short,
            '"'
        ),
        _icon_rotate_choice: $ => choice('90', '180', '270'),
        _icon_rotate_attr: $ => seq(
            'rotate=',
            $._icon_rotate_choice
        ),
        _icon_rotate_attr_quoted: $ => seq(
            'rotate="',
            $._icon_rotate_choice,
            '"'
        ),
        _icon_flip_choice: $ => choice('horizontal', 'vertical'),
        _icon_flip_attr: $ => seq(
            'flip=',
            $._icon_flip_choice
        ),
        _icon_flip_attr_quoted: $ => seq(
            'flip="',
            $._icon_flip_choice,
            '"'
        ),
        _icon_choice: $ => choice(
            alias($._icon_size_attr, $.size),
            alias($._icon_size_attr_quoted, $.size),
            alias($._icon_rotate_attr, $.rotate),
            alias($._icon_rotate_attr_quoted, $.rotate),
            alias($._icon_flip_attr, $.flip),
            alias($._icon_flip_attr_quoted, $.flip),
            alias($._option_role_named, $.role),
            alias($._icon_title_attr_quoted, $.title),
            alias($._icon_link_attr, $.link),
            alias($._icon_link_attr_quoted, $.link),
            alias($._icon_window_attr, $.window),
            alias($._icon_window_attr_quoted, $.window),
        ),
        icon_macro: $ => seq(
            $.icon_marker,
            alias($._identifier, $.icon_name),
            $.inline_option_block_marker_start,
            optional(choice(
              alias($._icon_size_short, $.size),
              $._icon_choice
            )),
            repeat(
                seq(
                    ',',
                    optional($._whitespace),
                    $._icon_choice,
                )
            ),
            $.inline_option_block_marker_end
        ),
        footnote_macro: $ => seq(
            $.footnote_marker,
            optional(alias($._identifier, $.footnote_group)),
            $.inline_option_block_marker_start,
            optional(alias($._text, $.footnote)),
            $.inline_option_block_marker_end
        ),

        xref_directive: $ =>
            prec.left(100,
              seq(
                  $.xref_marker,
                  $._antora_resource,
                  $.inline_option_block_marker_start,
                  alias(repeat(choice(
                    $._txt,
                    $._whitespace,
                    $.attribute_reference,
                  )), $.linktext),
                  $.inline_option_block_marker_end
              ),
            ),

        xref_directive_id: $ =>
            prec.left(110,
              seq(
                  $.xref_marker,
                  $._antora_resource,
                  alias(/#[A-Za-z][-\w]*/, $.id),
                  $.inline_option_block_marker_start,
                  alias(repeat1(choice(
                    $._txt,
                    $._whitespace,
                    $.attribute_reference,
                  )), $.linktext),
                  $.inline_option_block_marker_end
              ),
            ),

        image_macro: $ => prec.left(10,
              seq(
                  $.image_inline_marker,
                  $._antora_resource,
                  $.inline_option_block_marker_start,
                  $.inline_option_block_marker_end,
              )),

        image_directive: $ => prec.left(100,
              seq(
                  $.image_marker,
                  $._antora_resource,
                  $.inline_option_block_marker_start,
                  $.inline_option_block_marker_end,
              )),

        include_directive: $ => prec.left(100,
              seq(
                  $.include_marker,
                  $._antora_resource,
                  $.inline_option_block_marker_start,
                  optional($._whitespace),
                  optional($._include_params),
                  optional($._whitespace),
                  $.inline_option_block_marker_end,
              )),

        _include_params: $ => prec.left(100,
                seq(
                    $._includes_choice,
                    repeat(
                        seq(
                            ',',
                            optional($._whitespace),
                            $._includes_choice,
                        )
                    )
                )),

        _includes_choice: $ => choice(
            alias(seq('tag=', alias($._identifier, $.include_tag)), $.tags),
            alias(seq('tag="', alias($._identifier, $.include_tag), '"'), $.tags),
            alias(seq(
                'tags="',
                alias($._identifier, $.include_tag),
                repeat(
                    seq(
                        ';',
                        optional($._whitespace),
                        alias($._identifier, $.include_tag),
                    )
                ),
                '"'
            ), $.tags),
            alias(/leveloffset=\+?\d+/, $.leveloffset),
            alias(/lines="\d+\.\.\d+"/, $.lines),
            alias(/indent=\d+/, $.indent),
            alias(/encoding=\w+/, $.encoding),
            alias(/encoding=\w+/, $.encoding),
            alias(/opts=optional/, $.optional),
            alias(/opts="optional"/, $.optional),
            alias($._any_param, $.parameter),
        ),

        _any_param: $ => choice(
                          seq(
                            alias($._identifier, $.parameter_name),
                            '=',
                            alias($._identifier, $.parameter_value),
                          ),
                          seq(
                            alias($._identifier, $.parameter_name),
                            '="',
                            alias($._identifier, $.parameter_value),
                            repeat(
                                seq(
                                    ';',
                                    optional($._whitespace),
                                    alias($._identifier, $.parameter_value),
                                )
                            ),
                          '"')
                        ),

        _filepath: $ => /(([\d\w\-~_%]+|\.\.|\.)\/)*([\d\w\-~_%]+\.)+\w{1,6}/,

        _antora_resource: $ =>
            choice(
                $.attribute_reference,
                alias($._filepath, $.file_path),
                seq(
                    seq(alias(choice('page', 'image', 'partial', 'example', 'attachment'), $.antora_resource_family), '$'),
                    alias($._filepath, $.file_path)
                ),
                seq(
                    seq(alias(choice(choice($._identifier, $.attribute_reference), $.attribute_reference), $.antora_resource_module), ':'),
                    seq(alias(choice('page', 'image', 'partial', 'example', 'attachment'), $.antora_resource_family), '$'),
                    alias($._filepath, $.file_path)
                ),
                seq(
                    optional(seq(alias(choice($._identifier, $.attribute_reference), $.antora_resource_version), '@')),
                    seq(alias(choice(choice($._identifier, $.attribute_reference), $.attribute_reference), $.antora_resource_component), ':'),
                    seq(alias(choice(choice($._identifier, $.attribute_reference), $.attribute_reference), $.antora_resource_module), ':'),
                    seq(alias(choice('page', 'image', 'partial', 'example', 'attachment'), $.antora_resource_family), '$'),
                    alias($._filepath, $.file_path)
                ),
            ),

        attribute_reference: $ => seq(
            $._attribute_reference_start_marker,
            optional(choice(
                'counter:',
                'set:',
                '!set:',
                'set!:',
            )),
            $._identifier,
            $._attribute_reference_end_marker
        ),
        replacement: $ => prec(5, seq(
            $._attribute_reference_start_marker,
            choice(
                'blank',
                'empty',
                'sp',
                'nbsp',
                'zwsp',
                'wj',
                'apos',
                'quot',
                'lsquo',
                'rsquo',
                'ldquo',
                'rdquo',
                'deg',
                'plus',
                'brvbar',
                'vbar',
                'amp',
                'lt',
                'gt',
                'startsb',
                'endsb',
                'caret',
                'asterisk',
                'tilde',
                'backslash',
                'backtick',
                'two-colons',
                'two-semicolons',
                'cpp',
                'pp',
            ),
            $._attribute_reference_end_marker
        )),

        _in_emphasis: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.strong,
            $.monospace,
            $.highlight,
            $.superscript,
            $.subscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        emphasis: $ => prec.left(10,
            seq(
                $.emphasis_marker_start,
                seq(
                    $._in_emphasis,
                    repeat(
                        seq(
                            $._newline,
                            $._in_emphasis
                        )
                    )
                ),
                $.emphasis_marker_end)
        ),

        _in_strong: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.emphasis,
            $.monospace,
            $.highlight,
            $.superscript,
            $.subscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        strong: $ => prec.left(10,
            seq(
                $.strong_marker_start,
                seq(
                    $._in_strong,
                    repeat(
                        seq(
                            $._newline,
                            $._in_strong
                        )
                    )
                ),
                $.strong_marker_end)
        ),

        _in_monospace: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.strong,
            $.emphasis,
            $.highlight,
            $.superscript,
            $.subscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        monospace: $ => prec.left(10,
            seq(
                $.monospace_marker_start,
                seq(
                    $._in_monospace,
                    repeat(
                        seq(
                            $._newline,
                            $._in_monospace
                        )
                    )
                ),
                $.monospace_marker_end)
        ),

        _in_highlight: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.strong,
            $.emphasis,
            $.monospace,
            $.superscript,
            $.subscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        highlight: $ => prec.left(10,
            seq(
                $.highlight_marker_start,
                seq(
                    $._in_highlight,
                    repeat(
                        seq(
                            $._newline,
                            $._in_highlight
                        )
                    )
                ),
                $.highlight_marker_end)
        ),

        _in_superscript: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.strong,
            $.emphasis,
            $.monospace,
            $.highlight,
            $.subscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        superscript: $ => prec.left(10,
            seq(
                $.superscript_marker_start,
                seq(
                    $._in_superscript,
                    repeat(
                        seq(
                            $._newline,
                            $._in_superscript
                        )
                    )
                ),
                $.superscript_marker_end)
        ),

        _in_subscript: $ => repeat1(choice(
            $._txt,
            $._whitespace,
            common.punctuation_without($, []),
            $.strong,
            $.emphasis,
            $.monospace,
            $.highlight,
            $.superscript,
            $.inline_passthrough,
            $.attribute_reference,
            $.replacement,
        )),

        subscript: $ => prec.left(10,
            seq(
                $.subscript_marker_start,
                seq(
                    $._in_subscript,
                    repeat(
                        seq(
                            $._newline,
                            $._in_subscript
                        )
                    )
                ),
                $.subscript_marker_end)
        ),

        inline_passthrough: $ => prec.left(10,
            seq(
                $.inline_passthrough_marker_start,
                  repeat1(
                      /[^+]+/,
                  ),
                $.inline_passthrough_marker_end)
        ),

        escape: $ => /\\\S/,

        list_asterisk: $ => seq(alias(/\*+\u0020+/, $.list_asterisk_marker), $.paragraph),
        list_hyphen: $ => seq(alias(/-\u0020+/, $.list_hyphen_marker), $.paragraph),

        list_checkbox: $ => seq(alias(/\u0020*\*{1,6}\u0020\[[*x]?\]\u0020+/, $.list_box), $.paragraph),
        list_decimal: $ => seq(alias(/\u0020*\d+\.\u0020+/, $.list_marker), $.paragraph),
        list_decimal_: $ => seq(alias(/\.+\u0020+/, $.list_marker), $.paragraph),

        _description_list_marker: $ => choice('::', ':::', '::::', ';;'),

        description_list: $ => prec.right(10, seq(
            alias(seq($._text, $._description_list_marker), $.description_list_marker),
            repeat1(
                choice(
                    $._newline,
                    $.paragraph,
                    $.list_asterisk,
                    $.list_checkbox,
                    $.list_hyphen,
                    $.list_decimal,
                    $.list_decimal_,
                )
            ),
        )),

        http: $ => /https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}([-a-zA-Z0-9()@:%_\+.~#?&//=]*)/,

        // http_macro: $ => prec.left(10, seq($.http, '[', alias(optional($._text), $.linktext), ']')),

        email: $ => /[a-zA-Z0-9.!#$%&’*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-]+/,

        // mailto_macro: $ => prec.left(10, seq(
        //     'mailto:',
        //     $.email,
        //     '[',
        //     alias(optional($._text), $.linktext),
        //     ']'
        //
        // )),

        _link_macro_params: $ => choice(
            alias($._option_role_named, $.role),
            alias($._icon_window_attr, $.window),
            alias($._icon_window_attr_quoted, $.window),
            alias($._option_param_named, $.options)
        ),

        link_macro: $ =>
          seq(
              $.link_marker,
              choice(
                $.http,
                $.email,
                $._filepath
              ),
              $.inline_option_block_marker_start,
              optional(
                seq(
                  choice(
                  alias(repeat(choice($._txt, $._whitespace, common.punctuation_without($, ['"', '=', ',']) )), $.linktext),
                  alias(seq(
                        '"',
                        repeat(choice($._txt, $._whitespace, common.punctuation_without($, ['"']))),
                        '"',
                  ), $.linktext),
                  $._link_macro_params
                ),
                repeat(
                    seq(
                        ',',
                        optional($._whitespace),
                        $._link_macro_params,
                    )
                ),
              )),
              $.inline_option_block_marker_end,
          ),

        cross_reference: $ => seq(
            $.crossreference_marker_start,
            optional('#'),
            alias($._identifier, $.id),
            optional(
                seq(
                    $.crossreference_splitter,
                    alias(repeat1(choice($._txt, $._whitespace)), $.linktext),
                )
            ),
            $.crossreference_marker_end,
        ),

        pass_macro: $ => seq(
            $.pass_marker,
            optional(seq(
                alias($._pass_macro_options, $.substitution),
                repeat(seq(
                    token(prec(10, ',')),
                    alias($._pass_macro_options, $.substitution),
                )),
            )),
            '[',
            // repeat(choice(
            //     $._txt,
            //     $._whitespace,
            //     common.punctuation_without($, [ '[', ']' ])
            // )),
            ']',
        ),

        _pass_macro_options: $ => choice(
            'a', 'attributes',
            'c', 'specialchars',
            'm', 'macros',
            'n', 'normal',
            'p', 'post_replacements',
            'r', 'replacements',
            'v', 'verbatim',
        ),
        _block_types: $ => choice(
            'example',
            'listing',
            'literal',
            'pass',
            'quote',
            'sidebar',
            'source',
            'stem',
            'verse'
        ),
        _admonition_labels: $ => choice(
            'NOTE',
            'TIP',
            'IMPORTANT',
            'CAUTION',
            'WARNING'
        ),
        _section_styles: $ => choice(
            'abstract',
            'acknowledgments',
            'appendix',
            'bibliography',
            'colophon',
            'dedication',
            'glossary',
            'index',
            'preface'
        ),
        _paragraph_align: $ => choice(
            '.text-left',
            '.text-center',
            '.text-right',
        ),
        _paragraph_options: $ => choice(
            '%hardbreaks',
        ),
        _block_options: $ => choice(
            '%collapsible',
            '%open',
        ),
        _source_block_options: $ => choice(
            '%linenums',
        ),
        _table_options: $ => choice(
            '%header',
            '%noheader',
            '%footer',
            '%autowidth'
        ),
        _list_options: $ => choice('%reversed'),

        _callout: $ => seq(
            $._whitespace,
            alias(
                $.callout_marker_from,
                $.callout_mark
            ),
        ),

        callout: $ => seq(
            alias(
                $.callout_marker_to,
                $.callout_mark
            ),
            $._whitespace,
            $.paragraph
        ),
        encoded_symbol: $ => /&#\d+;/,

        kwd: $ => /[A-Za-z]+:+/
    },
})
