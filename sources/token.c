#include "compile.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct {
    const char *operator_string;
    enum TokenType ttype;
} operators[] = {
    { "implements", TTIMPLEMENTS },
    { "instanceof", TTINSTANCEOF },
    { "protected", TTPROTECTED },
    { "interface", TTINTERFACE },
    { "function", TTFUNCTION },
    { "debugger", TTDEBUGGER },
    { "continue", TTCONTINUE },
    { "private", TTPRIVATE },
    { "package", TTPACKAGE },
    { "finally", TTFINALLY },
    { "extends", TTEXTENDS },
    { "default", TTDEFAULT },
    { "static", TTSTATIC },
    { "public", TTPUBLIC },
    { "typeof", TTTYPEOF },
    { "switch", TTSWITCH },
    { "return", TTRETURN },
    { "import", TTIMPORT },
    { "export", TTEXPORT },
    { "delete", TTDELETE },
    { "while", TTWHILE },
    { "throw", TTTHROW },
    { "super", TTSUPER },
    { "false", TTFALSE },
    { "const", TTCONST },
    { "class", TTCLASS },
    { "catch", TTCATCH },
    { "break", TTBREAK },
    { "yield", TTYIELD },
    { "with", TTWITH },
    { "void", TTVOID },
    { "true", TTTRUE },
    { "this", TTTHIS },
    { "null", TTNULL },
    { "enum", TTENUM },
    { "else", TTELSE },
    { "case", TTCASE },
    { "let", TTLET },
    { "var", TTVAR },
    { "try", TTTRY },
    { "new", TTNEW },
    { "for", TTFOR },
    { "as", TTAS },
    { "in", TTIN },
    { "if", TTIF },
    { "do", TTDO },
    { "===", TTIDENT },
    { "!==", TTNOTIDENT },
    { ">>>", TTBITSHRZERO },
    { "==", TTEQ },
    { "!=", TTNOTEQ },
    { "+=", TTPLUSASSIGN },
    { "-=", TTMINUSASSIGN },
    { "/=", TTDIVIDEASSIGN },
    { "*=", TTMULTIPLYASSIGN },
    { "%=", TTMODULOASSIGN },
    { ">>", TTBITSHR },
    { "<<", TTBITSHL },
    { "&&", TTAND },
    { "||", TTOR },
    { "<=", TTLESSEQ },
    { ">=", TTGREATEREQ },
    { "=", TTASSIGN },
    { "+", TTPLUS },
    { "-", TTMINUS },
    { "/", TTDIVIDE },
    { "*", TTMULTIPLY },
    { "%", TTMODULO },
    { "&", TTBITAND },
    { "|", TTBITOR },
    { "^", TTBITXOR },
    { "~", TTBITNOT },
    { "!", TTBANG },
    { "?", TTCONDITIONAL },
    { "<", TTLESS },
    { ">", TTGREATER },
    { ".", TTDOT },
    { ";", TTSEMICOLON },
    { "(", TTOPENPAREN },
    { ")", TTCLOSEPAREN },
    { "{", TTOPENBRACE },
    { "}", TTCLOSEBRACE },
    { "[", TTOPENBRACKET },
    { "]", TTCLOSEBRACKET },
    { ":", TTCOLON },
    { ",", TTCOMMA },
}; 

/**Gets a keyword or operator out of the current string.
 *
 * Searches longer keywords first as some keywords/operators are substrings of
 * each other.  Writes the enum TokenType of the keyword/operator to the
 * parameter "ttype".
 *
 * Returns NULL if could not find anything.
 */
const char *get_keyword_or_operator(const char *begin, enum TokenType *ttype)
{
    if (strncmp(begin, "//", 2) == 0 || strncmp(begin, "/*", 2) == 0) {
        *ttype = TTNONE;
        return NULL;
    }

    const size_t NUM_OPERATORS = sizeof operators / sizeof operators[0];
    for (int i = 0; i < NUM_OPERATORS; i++) {
        const char *to_compare = operators[i].operator_string;
        if (strncmp(to_compare, begin, strlen(to_compare)) == 0) {
            *ttype = operators[i].ttype;
            return &begin[strlen(to_compare)];
        }
    }

    *ttype = TTNONE;
    return NULL;
}

const struct {
    const char *keyword;
    enum TokenType ttype;
} keywords[] = {
    // always reserved words
    { "break", TTBREAK },
    { "case", TTCASE },
    { "catch", TTCATCH },
    { "class", TTCLASS },
    { "const", TTCONST },
    { "continue", TTCONTINUE },
    { "debugger", TTDEBUGGER },
    { "default", TTDEFAULT },
    { "delete", TTDELETE },
    { "do", TTDO },
    { "else", TTELSE },
    { "enum", TTENUM },
    { "export", TTEXPORT },
    { "extends", TTEXTENDS },
    { "false", TTFALSE },
    { "finally", TTFINALLY },
    { "for", TTFOR },
    { "function", TTFUNCTION },
    { "if", TTIF },
    { "import", TTIMPORT },
    { "in", TTIN },
    { "instanceof", TTINSTANCEOF },
    { "new", TTNEW },
    { "null", TTNULL },
    { "return", TTRETURN },
    { "super", TTSUPER },
    { "switch", TTSWITCH },
    { "this", TTTHIS },
    { "throw", TTTHROW },
    { "true", TTTRUE },
    { "try", TTTRY },
    { "typeof", TTTYPEOF },
    { "var", TTVAR },
    { "void", TTVOID },
    { "while", TTWHILE },
    { "with", TTWITH },
    // TODO: strict mode only; handle this later
    { "as", TTAS },
    { "implements", TTIMPLEMENTS },
    { "interface", TTINTERFACE },
    { "let", TTLET },
    { "package", TTPACKAGE },
    { "private", TTPRIVATE },
    { "protected", TTPROTECTED },
    { "public", TTPUBLIC },
    { "static", TTSTATIC },
    { "yield", TTYIELD },
};

/**Gets the token type of word if word is a keyword, else TTNONE.
 */
enum TokenType get_keyword_type(struct StringView word)
{
    const size_t NUM_KEYWORDS = sizeof keywords / sizeof keywords[0];

    for (int i = 0; i < NUM_KEYWORDS; i++) {
        // see if the word in question is the keyword in question
        if (word.length != strlen(keywords[i].keyword))
            continue;

        if (strncmp(word.data, keywords[i].keyword, word.length) == 0) {
            return keywords[i].ttype;
        }
    }

    return TTNONE;
}

bool is_identifier_first_char(char c)
{
    return isalpha(c) || c == '_' || c == '$';
}

bool is_identifier_char(char c)
{
    return isdigit(c) || isalpha(c) || c == '_' || c == '$';
}

/**Returns the end of a valid identifier
 */
const char *traverse_identifier(const char *string)
{
    assert(is_identifier_first_char(*string) && "should have checked the first char of an identifier");

    for (; *string != '\0' && is_identifier_char(*string); ++string)
        ;

    return string;
}

/**Returns the end of a valid digit literal
 */
const char *traverse_digit_literal(const char *string)
{
    if (!isdigit(*string)) {
        assert(0 && "shouldn't have made it here bro");
        return string;
    }

    for (; *string != '\0' && isdigit(*string); ++string)
        ;

    return string;
}

/**Traverses until the end of a single quoted string
 * 
 * NULL if malformed.
 */
const char *traverse_single_quoted_string(const char *string)
{
    assert(*string == '\'' && "should only be called when starting a single quoted string");

    string++; // ignore opening '

    while (*string != '\'') {
        if (*string == '\n')
            return NULL;
        else if (*string == '\\') {
            if (*(++string) == '\n')
                return NULL;
        }

        string++;
    }

    return string;
}

/**Traverses until the end of a single quoted string
 * 
 * NULL if malformed.
 */
const char *traverse_double_quoted_string(const char *string)
{
    assert(*string == '"' && "should only be called when starting a double quoted string");

    string++; // ignore opening "

    while (*string != '"') {
        if (*string == '\n')
            return NULL;
        else if (*string == '\\') {
            if (*(++string) == '\n')
                return NULL;
        }

        string++;
    }

    return string;
}

const char *traverse_line_comment(const char *string) {
    assert(strncmp(string, "//", 2) == 0 && "should have checked line comment starts with //");

    string += strlen("//");

    while (*string != '\0' && *string != '\n')
        ++string;

    return string;
}

const char *traverse_block_comment(const char *string, size_t *line) {
    assert(line != NULL);
    assert(strncmp(string, "/*", 2) == 0 && "should have checked block comment starts with /*");

    string += strlen("/*");

    for (; *string != '\0'; ++string) {
        if (*string == '*') {
            if (*(++string) == '/')
                return ++string;
        } else if (*string == '\n') {
            ++*line;
        }
    }

    return string;
}

int tokenise_file(const char *contents, struct Token **tokens, size_t *tokens_written)
{
    size_t i = 0;
    enum TokenType ttype = TTNONE;
    const char *end = NULL;
    size_t line = 1;

    size_t capacity = 2048;
    *tokens = calloc(sizeof **tokens, capacity);

    while (*contents != '\0') {
        if (i >= capacity) {
            *tokens = realloc(*tokens, sizeof **tokens * (capacity *= 2));
            memset(&(*tokens)[capacity / 2], 0, sizeof **tokens * (capacity / 2));
        }

        end = NULL;
        if ((end = get_keyword_or_operator(contents, &ttype)), end != NULL) {
            (*tokens)[i++].type = ttype;
            contents = end;
        } else if (is_identifier_first_char(*contents)) {
            end = traverse_identifier(contents);
            struct StringView id = { .data = contents, .length = end - contents };
            contents = end;

            (*tokens)[i++] = (struct Token) {
                .type = TTIDENTIFIER,
                .view = id,
            };
        } else if (isspace(*contents)) {
            // TODO this doesn't always work
            if (*contents == '\n') ++line;
            while (isspace(*(++contents)))
                if (*contents == '\n') ++line;
        } else if (isdigit(*contents)) {
            end = traverse_digit_literal(contents);
            (*tokens)[i++] = (struct Token) {
                .type = TTNUMLITERAL,
                .view = { .data = contents, .length = end - contents },
            };
            contents = end;
        } else if (strncmp(contents, "//", 2) == 0) {
            // a line comment
            contents = traverse_line_comment(contents);
        } else if (strncmp(contents, "/*", 2) == 0) {
            // a block comment
            contents = traverse_block_comment(contents, &line);
        } else if (*contents == '\'') {
            // single-quoted string
            end = traverse_single_quoted_string(contents);
            if (end == NULL) return EXIT_FAILURE;
            (*tokens)[i++] = (struct Token) {
                .type = TTSINGLESTRING,
                .view = { .data = contents, .length = end - contents },
            };
        } else if (*contents == '"') {
            // double-quoted string
            end = traverse_double_quoted_string(contents);
            if (end == NULL) return EXIT_FAILURE;
            (*tokens)[i++] = (struct Token) {
                .type = TTSINGLESTRING,
                .view = { .data = contents, .length = end - contents },
            };
        } else {
            assert(0 && "unreachable");
        }
    }

    if (tokens_written != NULL)
        *tokens_written = i;

    return EXIT_SUCCESS;
}
