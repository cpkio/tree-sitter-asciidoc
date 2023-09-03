const common = require('./common/grammar.js')

const PUNCTUATION_CHARACTERS_REGEX = '!-/:-@\\[-`\\{-~'

const PUNCTUATION_CHARACTERS_ARRAY = [
    '!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<',
    '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
];

module.exports = grammar({
    name: 'asciidoc',
    extras: _ => [],
    inline: $ => [],
    conflicts: $ => [
    ],
    precedences: $ => [],
    externals: $ => [$.eof],

    rules: {
        document: $ => repeat($._block),

        _whitespace: _ => /[  ]+/, // space & non-breaking space

        _newline: _ => /\n|\r\n?/,

        _emptyline: $ => repeat1(seq($._newline, repeat(' '))),

        _block: $ =>
            prec(100,
                choice(
                    $._emptyline,
                    $.title0,
                    $.title1,
                    $.title2,
                    $.title3,
                    $.title4,
                    $.title5,
                    $.comment,
                    $.attribute,
                    $.paragraph,
                    $.list,
                    $.include_directive,
                    $.line_breaks,
                    $.page_breaks,
                    $._titled_block,
                ),
            ),

        _titled_block: $ =>
            seq(
                seq(
                    optional($.block_title),
                    choice(
                        $._admonitions,
                        $.list,
                        $.image,
                        // $.code,
                        $.audio,
                        $.video,
                        // $.description_list,
                        $.table,
                    ),
                ),
            ),

        block_title: _ => seq('.', /.+\n?/),
        title0: $ =>
            seq(
                alias('=', $.title_marker),
                ' ',
                alias(/.*\n?/, $.title_content),
            ),
        // prettier-ignore
        title1: $ => seq(alias('==', $.title_marker), ' ', alias(/.*\n?/, $.title_content)),
        // prettier-ignore
        title2: $ => seq(alias('===', $.title_marker), ' ', alias(/.*\n?/, $.title_content)),
        // prettier-ignore
        title3: $ => seq(alias('====', $.title_marker), ' ', alias(/.*\n?/, $.title_content)),
        // prettier-ignore
        title4: $ => seq(alias('=====', $.title_marker), ' ', alias(/.*\n?/, $.title_content)),
        // prettier-ignore
        title5: $ => seq(alias('======', $.title_marker), ' ', alias(/.*\n?/, $.title_content)),

        attribute: $ =>
            seq(
                $.attr_mark,
                $.attr_name,
                $.attr_mark,
                ' ',
                $.attr_value
            ),
        attr_mark: _ => ':',
        attr_name: _ => choice(/!?[\w\-_]+/, /[\w\-_]+!?/ ),
        attr_value: _ => /.+/,

        // Admonitions
        _admonitions: $ =>
            choice($.note, $.tip, $.important, $.caution, $.warning),

        note: $ => choice($._note_line, $._note_block),
        _note_line: _$ => seq('NOTE: ', /.+\n?/),
        _note_block: _$ => seq('[NOTE]\n', '----\n', repeat(/.+\n/), '----\n'),

        tip: $ => choice($._tip, $._tip_block),
        _tip: _$ => seq('TIP: ', /.+\n?/),
        _tip_block: _$ => seq('[TIP]\n', '----\n', repeat(/.+\n/), '----\n'),

        important: $ => choice($._important, $._important_block),
        _important: _$ => seq('IMPORTANT: ', /.+\n?/),
        _important_block: _$ =>
            seq('[IMPORTANT]\n', '----\n', repeat(/.+\n/), '----\n'),

        caution: $ => choice($._caution, $._caution_block),
        _caution: _$ => seq('CAUTION: ', /.+\n?/),
        _caution_block: _$ =>
            seq('[CAUTION]\n', '----\n', repeat(/.+\n/), '----\n'),

        warning: $ => choice($._warning, $._warning_block),
        _warning: _$ => seq('WARNING: ', /.+\n?/),
        _warning_block: _$ =>
            seq('[WARNING]\n', '----\n', repeat(/.+\n/), '----\n'),

        // list
        // list: $ => prec(50, repeat1($.list_item)),
        list: $ =>
            seq(
                '*',
                ' ',
                prec.right(repeat1($._text)), // Это правило обеспечивает «жадный» захват в токен строк, следующих за строкой с маркером списка
            ),


        // _unordered_list_mark: _ => '*',
        // _ordered_list_mark: _ => choice(/\.+/, /0?\d+\./, /[\w\P{M}]\./),
        // _checklist_mark: _ => /\* \[[\* x]\]/,

        // code: $ =>
        //     seq(
        //         /\[,\s?/,
        //         field('language', $.code_language),
        //         ']\n',
        //         '----\n',
        //         field('content', optional($.code_content)),
        //         '----\n',
        //     ),
        // code_language: _ => /\w+/,
        // code_content: _ => repeat1(/.+\n/),

        comment: $ => seq('// ', /.*/, $._newline),

        line_breaks: _ => seq(/[\-\*]{3}\n\n/),

        page_breaks: _ => seq('<<<\n\n'),

        image: $ =>
            seq(
                'image::',
                field('url', $.audio_url),
                '[',
                field('title', $.audio_title),
                ']\n',
            ),

        table: $ =>
            seq($.table_start, optional($.table_content), $.table_end, '\n'),
        table_start: _ => '|===\n',
        table_content: _ => repeat1(/.+\n?/),
        table_end: _ => '|===\n',

        // description_list: $ => seq(repeat1($.description_list_item), '\n'),
        // description_list_item: $ =>
        //     seq(
        //         alias(/\w+/, $.list_item_name),
        //         ':: ',
        //         alias(/.+/, $.list_item_content),
        //         '\n',
        //     ),

        audio: $ =>
            seq(
                'audio::',
                field('url', optional($.audio_url)),
                '[',
                field('title', optional($.audio_title)),
                ']',
                '\n',
            ),
        audio_url: _ => /[\w.]+/,
        audio_title: _ => /[\w.]+/,

        video: $ =>
            seq(
                'video::',
                field('url', optional($.audio_url)),
                '[',
                field('title', optional($.audio_title)),
                ']',
                '\n',
            ),

        paragraph: $ => prec.right(0, repeat1($._text)),

        _text: $ =>
            seq(
                repeat1(
                    choice(
                        $._word,
                        $._whitespace,
                        common.punctuation_without($, []),
                    ),
                ),
                $._newline
            ),

        _word: _ =>
            new RegExp('[^' + PUNCTUATION_CHARACTERS_REGEX + '  \\t\\n\\r]+'),

        _inline_element: $ =>
            choice(
                $.emphasis,
                $.strong,
                $.monospace,
                $.superscript,
                $.subscript,
                $.replacement,
                $.passthrough,
                $.kbd,
                $.footnote,
                $.links,
                $.xref,
                $.highlight,
            ),

        kbd: $ => seq('kbd:[', optional($.kbd_content), ']'),
        kbd_content: _ => /\w+(\+\w+)?/,

        footnote: $ => seq('footnote:[', optional($.footnote_content), ']'),
        footnote_content: _ => /[\w._]+/,

        links: $ => choice($.url_macro, $.link_macro, $.mailto),
        autolinks: $ => alias(/\w+:\/\/[^\[\n]+/, $.url),

        url_macro: $ =>
            seq($.autolinks, '[', alias(/[^\]]+/, $.url_title), ']'),

        link_macro: $ =>
            seq(
                'link:',
                alias(/[^\[]+/, $.url),
                '[',
                alias(/[^\]]+/, $.url_title),
                ']',
            ),

        mailto: $ =>
            seq(
                'mailto:',
                alias(/[\w\.]+@[\w\.]+/, $.url),
                '[',
                optional(/[^\]]+/),
                ']',
            ),

        xref: $ => choice($._inline_xref, $._xref),

        _inline_xref: $ =>
            seq(
                '<<',
                field('url', $.xref_url),
                optional(seq(',', field('title', $.audio_title))),
                '>>',
            ),
        _xref: $ =>
            seq(
                'xref:',
                field('url', $.xref_url),
                '[',
                field('title', $.audio_title),
                ']',
            ),
        xref_url: _ => /\w+/,

        emphasis: _$ => /_.+_/,
        strong: _$ => /\*.+\*/,
        monospace: _$ => /`.+`/,
        highlight: _ => /#.+#/,
        superscript: _$ => /\^.+\^/,
        subscript: _$ => /~.+~/,

        passthrough: $ => choice($._passthrough_plus, $._passthrough_cmd),
        _passthrough_plus: $ => seq('+++', $.passthrough_content, '+++'),
        _passthrough_cmd: $ => seq('pass:[', $.passthrough_content, ']'),
        passthrough_content: _ => /\w+/,

        replacement: _ =>
            choice(
                '{blank}',
                '{empty}',
                '{sp}',
                '{nbsp}',
                '{zwsp}',
                '{wj}',
                '{apos}',
                '{quot}',
                '{lsquo}',
                '{rsquo}',
                '{ldquo}',
                '{rdquo}',
                '{deg}',
                '{plus}',
                '{brvbar}',
                '{vbar}',
                '{amp}',
                '{lt}',
                '{gt}',
                '{startsb}',
                '{endsb}',
                '{caret}',
                '{asterisk}',
                '{tilde}',
                '{backslash}',
                '{backtick}',
                '{two-colons}',
                '{two-semicolons}',
                '{cpp}',
                '{pp}',
            ),

        include_directive: $ =>
            prec(100,
              seq(
                  'include',
                  '::',
                  $._antora_resource,
                  $._include_params,
                  choice($._newline, $.eof)
              ),
            ),

        _include_params: $ =>
            seq('[',
                repeat(seq($.include_param, ',')),
                optional($.include_param),
                ']'
            ),

        include_param: $ => /[^\],]+/,

        _antora_resource: $ =>
            seq(
                optional(
                    alias(token(seq(/[\w\d\.]+/, '@')), $.antora_resource_version),
                ),
                alias(token(seq(/\w+/, ':')), $.antora_resource_component),
                alias(token(seq(/\w+/, ':')), $.antora_resource_module),
                alias(token(seq(choice('page', 'image', 'partial', 'example', 'attachment'), '$')), $.antora_resource_family),
                alias(token(seq(/\w+/, '.', /\w+/)), $.antora_resource_file)
            ),
    },
})
