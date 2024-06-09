const PUNCTUATION_CHARACTERS_REGEX = '!-/:-@\\[-`\\{-~'
// prettier-ignore
const PUNCTUATION_CHARACTERS_ARRAY = [
    '!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<',
    '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
];

// Returns a rule that matches all characters that count as punctuation inside markdown, besides
// a list of excluded punctuation characters. Calling this function with a empty list as the second
// argument returns a rule that matches all punctuation.
function punctuation_without($, chars) {
    return seq(
        choice(...PUNCTUATION_CHARACTERS_ARRAY.filter(c => !chars.includes(c))),
    )
}

exports.punctuation_without = punctuation_without
exports.PUNCTUATION_CHARACTERS_REGEX = PUNCTUATION_CHARACTERS_REGEX
