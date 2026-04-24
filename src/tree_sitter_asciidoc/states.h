#define CTX_SECTION1         1ULL << 63ULL // 10000000                                                                = Section
#define CTX_SECTION2         1ULL << 62ULL // 01000000                                                                == Section
#define CTX_SECTION3         1ULL << 61ULL // 00100000                                                                === Section
#define CTX_SECTION4         1ULL << 60ULL // 00010000                                                                ==== Section
#define CTX_SECTION5         1ULL << 59ULL // 00001000                                                                ===== Section
#define CTX_SECTION6         1ULL << 58ULL // 00000100                                                                ====== Section
#define CTX_ATTRIBUTE        1ULL << 57ULL // 00000010                                                                :my-attribute:
#define CTX_OPTIONBLOCK      1ULL << 56ULL // 00000001                                                                [#id], [%header] etc

#define CTX_OPTIONINLINE     1ULL << 55ULL //          10000000 00000000                                              []text; по-моему там особенность в том, что допускаются только короткие варианты: [.role#id%option]#some#
#define CTX_MACROPARAMETER   1ULL << 54ULL //          01000000 00000000                                              text[]
#define CTX_OPTIONSEPARATOR  1ULL << 53ULL //          00100000 00000000                                              `,` в [], <<,>> и возможно других контекстах
#define CTX_TABLE1BLOCK      1ULL << 52ULL //          00010000 00000000                                              |===
#define CTX_TABLE1CELL       1ULL << 51ULL //          00001000 00000000                                              `|`
#define CTX_TABLE2BLOCK      1ULL << 50ULL //          00000100 00000000                                              !=== я пока не поддержваю ,=== и :=== как виды таблиц
#define CTX_TABLE2CELL       1ULL << 49ULL //          00000010 00000000                                              `!`
#define CTX_EXAMPLE1BLOCK    1ULL << 48ULL //          00000001 00000000                                              ====
#define CTX_EXAMPLE2BLOCK    1ULL << 47ULL //          00000000 10000000                                              =====
#define CTX_EXAMPLE3BLOCK    1ULL << 46ULL //          00000000 01000000                                              ======
#define CTX_LISTINGBLOCK     1ULL << 45ULL //          00000000 00100000                                              ----
#define CTX_LITERALBLOCK     1ULL << 44ULL //          00000000 00010000                                              ....
#define CTX_OPENBLOCK        1ULL << 43ULL //          00000000 00001000                                              --
#define CTX_SIDEBARBLOCK     1ULL << 42ULL //          00000000 00000100                                              ****
#define CTX_PASSBLOCK        1ULL << 41ULL //          00000000 00000010                                              ++++
#define CTX_QUOTEBLOCK       1ULL << 40ULL //          00000000 00000001                                              ____

#define CTX_PASSTHROUGH      1ULL << 00ULL //                                                                00000001 + +
#define CTX_HIGHLIGHT        1ULL << 01ULL //                                                                00000010 # #
#define CTX_SUBSCRIPT        1ULL << 02ULL //                                                                00000100 ~ ~
#define CTX_SUPERSCRIPT      1ULL << 03ULL //                                                                00001000 ^ ^
#define CTX_MONOSPACE        1ULL << 04ULL //                                                                00010000 ` `
#define CTX_EMPHASIS         1ULL << 05ULL //                                                                00100000 _ _
#define CTX_STRONG           1ULL << 06ULL //                                                                01000000 * *
#define CTX_COMMENT          1ULL << 07ULL //                                                                10000000 //

#define CTX_UNORDEREDLIST    1ULL << 08ULL //                                                       00000001
#define CTX_ORDEREDLIST      1ULL << 09ULL //                                                       00000010
#define CTX_DESCRIPTIONLIST  1ULL << 10ULL //                                                       00000100
#define CTX_COMMENTBLOCK     1ULL << 11ULL //                                                       00001000          ////
#define CTX_CROSSREFERENCE   1ULL << 12ULL //                                                       00010000          <<#identifier,text>>
#define CTX_XREFMACRO        1ULL << 13ULL //                                                       00100000          == также устанавливается (?) для inlcude::, image::
#define CTX_CONDITIONAL      1ULL << 14ULL //                                                       01000000          ifdef, ifndef, ifeval, endif
#define CTX_ANCHORMACRO      1ULL << 15ULL //                                                       10000000          anchor:text[] == [#anchortext]

#define CTX_PASSTHROUGH_U    1ULL << 16ULL //                                              00000001                   ++ ++ unconstrained
#define CTX_HIGHLIGHT_U      1ULL << 17ULL //                                              00000010                   ## ##
#define CTX_SUBSCRIPT_U      1ULL << 18ULL //                                              00000100                   ~~ ~~
#define CTX_SUPERSCRIPT_U    1ULL << 19ULL //                                              00001000                   ^^ ^^
#define CTX_MONOSPACE_U      1ULL << 20ULL //                                              00010000                   `` ``
#define CTX_EMPHASIS_U       1ULL << 21ULL //                                              00100000                   __ __
#define CTX_STRONG_U         1ULL << 22ULL //                                              01000000                   ** **
#define CTX_UNKNOWN1         1ULL << 23ULL //                                              10000000

#define CTX_PASSMACRO        1ULL << 24ULL //                            00000000 00000000                            pass:
#define CTX_ICONMACRO        1ULL << 25ULL //                            00000000 00000000                            icon:
#define CTX_LINKMACRO        1ULL << 26ULL //                            00000000 00000000                            link:
#define CTX_MAILTOMACRO      1ULL << 27ULL //                            00000000 00000000                            mailto:
#define CTX_MATHMACRO        1ULL << 28ULL //                            00000000 00000000                            stem:, latexmath:
#define CTX_FOOTNOTEMACRO    1ULL << 29ULL //                            00000000 00000000                            footnote:
#define CTX_IMAGEMACRO       1ULL << 30ULL //                            00000000 00000000                            icon:
#define CTX_AUDIOMACRO       1ULL << 31ULL //                            00000000 00000000                            audio::

#define CTX_VIDEOMACRO       1ULL << 32ULL //                            00000000 00000000                            video::
#define CTX_INCLUDEMACRO     1ULL << 33ULL //                            00000000 00000000                            include::
#define CTX_MARKEREXPLICIT   1ULL << 34ULL //                            00000000 00000000                            ((   ))
#define CTX_MARKERIMPLICIT   1ULL << 35ULL //                            00000000 00000000                            ((( )))
#define CTX_UNKNOWN12        1ULL << 36ULL //                            00000000 00000000
#define CTX_UNKNOWN11        1ULL << 37ULL //                            00000000 00000000
#define CTX_UNKNOWN10        1ULL << 38ULL //                            00000000 00000000
#define CTX_UNKNOWN9         1ULL << 39ULL //                            00000000 00000000

#define CTX_MASK            (scanner->mask)
#define GET_BIT(bitIndex)   ((CTX_MASK  &   (bitIndex)) > 0)
#define SET_BIT(bitIndex)    (CTX_MASK  |=  (bitIndex))
#define TOGGLE_BIT(bitIndex) (CTX_MASK  ^=  (bitIndex))
#define CLEAR_BIT(bitIndex)  (CTX_MASK  &= ~(bitIndex))

#define NOT_BLOCK !( GET_BIT(CTX_TABLE1BLOCK) || GET_BIT(CTX_TABLE1BLOCK) || GET_BIT(CTX_TABLE2BLOCK) || GET_BIT(CTX_EXAMPLE1BLOCK) || GET_BIT(CTX_EXAMPLE2BLOCK) || GET_BIT(CTX_EXAMPLE3BLOCK) || GET_BIT(CTX_LISTINGBLOCK) || GET_BIT(CTX_LITERALBLOCK) || GET_BIT(CTX_OPENBLOCK) || GET_BIT(CTX_SIDEBARBLOCK) || GET_BIT(CTX_PASSBLOCK) || GET_BIT(CTX_QUOTEBLOCK) || GET_BIT(CTX_COMMENTBLOCK) || GET_BIT(CTX_COMMENT) )

#define NOT_PLAIN !( GET_BIT(CTX_LISTINGBLOCK) || GET_BIT(CTX_LITERALBLOCK) || GET_BIT(CTX_PASSBLOCK) || GET_BIT(CTX_COMMENTBLOCK) || GET_BIT(CTX_COMMENT) )

#define IS_MACRO ( GET_BIT(CTX_ANCHORMACRO) || GET_BIT(CTX_PASSMACRO) || GET_BIT(CTX_ICONMACRO) || GET_BIT(CTX_LINKMACRO) || GET_BIT(CTX_MAILTOMACRO) || GET_BIT(CTX_MATHMACRO) || GET_BIT(CTX_FOOTNOTEMACRO) || GET_BIT(CTX_IMAGEMACRO) || GET_BIT(CTX_AUDIOMACRO) || GET_BIT(CTX_VIDEOMACRO) )
