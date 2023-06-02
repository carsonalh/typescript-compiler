#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType {
    // used for signalling
    TTNONE = 0,
    // keywords
    TTBREAK,
    TTCASE,
    TTCATCH,
    TTCLASS,
    TTCONST,
    TTCONTINUE,
    TTDEBUGGER,
    TTDEFAULT,
    TTDELETE,
    TTDO,
    TTELSE,
    TTENUM,
    TTEXPORT,
    TTEXTENDS,
    TTFALSE,
    TTFINALLY,
    TTFOR,
    TTFUNCTION,
    TTIF,
    TTIMPORT,
    TTIN,
    TTINSTANCEOF,
    TTNEW,
    TTNULL,
    TTRETURN,
    TTSUPER,
    TTSWITCH,
    TTTHIS,
    TTTHROW,
    TTTRUE,
    TTTRY,
    TTTYPEOF,
    TTVAR,
    TTVOID,
    TTWHILE,
    TTWITH,
    // strict mode reserved words
    TTAS,
    TTIMPLEMENTS,
    TTINTERFACE,
    TTLET,
    TTPACKAGE,
    TTPRIVATE,
    TTPROTECTED,
    TTPUBLIC,
    TTSTATIC,
    TTYIELD,
    // strings
    TTSINGLESTRING,
    TTDOUBLESTRING,
    TTTEMPLATESTRING,
    // arbitrary identifiers
    TTIDENTIFIER,
    // operators
    TTIDENT,            // ===
    TTNOTIDENT,         // !==
    TTEQ,               // ==
    TTNOTEQ,            // !=
    TTASSIGN,           // =
    TTPLUS,             // +
    TTMINUS,            // -
    TTDIVIDE,           // /
    TTMULTIPLY,         // *
    TTMODULO,           // %
    TTPLUSASSIGN,       // +=
    TTMINUSASSIGN,      // -=
    TTDIVIDEASSIGN,     // /=
    TTMULTIPLYASSIGN,   // *=
    TTMODULOASSIGN,     // %=
    TTBITAND,           // &
    TTBITOR,            // |
    TTBITXOR,           // ^
    TTBITNOT,           // ~
    TTBITSHRZERO,       // >>>
    TTBITSHR,           // >>
    TTBITSHL,           // <<
    TTAND,              // &&
    TTOR,               // ||
    TTBANG,             // !
    TTCONDITIONAL,      // ?
    TTLESS,             // <
    TTGREATER,          // >
    TTLESSEQ,           // <=
    TTGREATEREQ,        // >=
    TTDOT,              // .
    // literals
    TTNUMLITERAL,
    // syntax
    TTSEMICOLON,
    TTOPENPAREN,
    TTCLOSEPAREN,
    TTOPENBRACE,
    TTCLOSEBRACE,
    TTOPENBRACKET,
    TTCLOSEBRACKET,
    TTCOLON,
};

const char *token_type_strings[] = {
    [TTBREAK] = "break",
    [TTCASE] = "case",
    [TTCATCH] = "catch",
    [TTCLASS] = "class",
    [TTCONST] = "const",
    [TTCONTINUE] = "continue",
    [TTDEBUGGER] = "debugger",
    [TTDEFAULT] = "default",
    [TTDELETE] = "delete",
    [TTDO] = "do",
    [TTELSE] = "else",
    [TTENUM] = "enum",
    [TTEXPORT] = "export",
    [TTEXTENDS] = "extends",
    [TTFALSE] = "false",
    [TTFINALLY] = "finally",
    [TTFOR] = "for",
    [TTFUNCTION] = "function",
    [TTIF] = "if",
    [TTIMPORT] = "import",
    [TTIN] = "in",
    [TTINSTANCEOF] = "instanceof",
    [TTNEW] = "new",
    [TTNULL] = "null",
    [TTRETURN] = "return",
    [TTSUPER] = "super",
    [TTSWITCH] = "switch",
    [TTTHIS] = "this",
    [TTTHROW] = "throw",
    [TTTRUE] = "true",
    [TTTRY] = "try",
    [TTTYPEOF] = "typeof",
    [TTVAR] = "var",
    [TTVOID] = "void",
    [TTWHILE] = "while",
    [TTWITH] = "with",
    // strict mode keywords
    [TTAS] = "as",
    [TTIMPLEMENTS] = "implements",
    [TTINTERFACE] = "interface",
    [TTLET] = "let",
    [TTPACKAGE] = "package",
    [TTPRIVATE] = "private",
    [TTPROTECTED] = "protected",
    [TTPUBLIC] = "public",
    [TTSTATIC] = "static",
    [TTYIELD] = "yield",
    // operators
    [TTIDENT] = "===",
    [TTNOTIDENT] = "!==",
    [TTEQ] = "==",
    [TTNOTEQ] = "!=",
    [TTASSIGN] = "=",
    [TTPLUS] = "+",
    [TTMINUS] = "-",
    [TTDIVIDE] = "/",
    [TTMULTIPLY] = "*",
    [TTMODULO] = "%",
    [TTPLUSASSIGN] = "+=",
    [TTMINUSASSIGN] = "-=",
    [TTDIVIDEASSIGN] = "/=",
    [TTMULTIPLYASSIGN] = "*=",
    [TTMODULOASSIGN] = "%=",
    [TTBITAND] = "&",
    [TTBITOR] = "|",
    [TTBITXOR] = "^",
    [TTBITNOT] = "~",
    [TTBITSHRZERO] = ">>>",
    [TTBITSHR] = ">>",
    [TTBITSHL] = "<<",
    [TTAND] = "&&",
    [TTOR] = "||",
    [TTBANG] = "!",
    [TTCONDITIONAL] = "?",
    [TTLESS] = "<",
    [TTGREATER] = ">",
    [TTLESSEQ] = "<=",
    [TTGREATEREQ] = ">=",
    [TTDOT] = ".",
    // syntax
    [TTSEMICOLON] = ";",
    [TTOPENPAREN] = "(",
    [TTCLOSEPAREN] = ")",
    [TTOPENBRACE] = "{",
    [TTCLOSEBRACE] = "}",
    [TTOPENBRACKET] = "[",
    [TTCLOSEBRACKET] = "]",
    [TTCOLON] = ":",
};

/**A string view that does not own a string.
 */
struct StringView {
    const char *data;
    size_t length;
};

struct Token {
    enum TokenType type;
    struct StringView view;
};

int load_file(const char *name, char **out_data);

const struct {
    const char *operator_string;
    enum TokenType ttype;
} operators[] = {
    { "===", TTIDENT },
    { "!==", TTNOTIDENT },
    { "==", TTEQ },
    { "!=", TTNOTEQ },
    { "=", TTASSIGN },
    { "+", TTPLUS },
    { "-", TTMINUS },
    { "/", TTDIVIDE },
    { "*", TTMULTIPLY },
    { "%", TTMODULO },
    { "+=", TTPLUSASSIGN },
    { "-=", TTMINUSASSIGN },
    { "/=", TTDIVIDEASSIGN },
    { "*=", TTMULTIPLYASSIGN },
    { "%=", TTMODULOASSIGN },
    { "&", TTBITAND },
    { "|", TTBITOR },
    { "^", TTBITXOR },
    { "~", TTBITNOT },
    { ">>>", TTBITSHRZERO },
    { ">>", TTBITSHR },
    { "<<", TTBITSHL },
    { "&&", TTAND },
    { "||", TTOR },
    { "!", TTBANG },
    { "?", TTCONDITIONAL },
    { "<", TTLESS },
    { ">", TTGREATER },
    { "<=", TTLESSEQ },
    { ">=", TTGREATEREQ },
    { ";", TTSEMICOLON },
    { "(", TTOPENPAREN },
    { ")", TTCLOSEPAREN },
    { "{", TTOPENBRACE },
    { "}", TTCLOSEBRACE },
    { "[", TTOPENBRACKET },
    { "]", TTCLOSEBRACKET },
    { ":", TTCOLON },
}; 

/**All the operators that have length 3
 */
const char *operator3[] = {
    "===",
    "!==",
    ">>>",
};

bool is_operator3(const char *begin)
{
    const size_t LEN_OPERATOR3 = sizeof operator3 / sizeof operator3[0];
    for (int i = 0; i < LEN_OPERATOR3; i++) {
        if (strncmp(begin, operator3[i], 3) == 0) {
            return true;
        }
    }
    return false;
}

enum TokenType dereference_operator3(const char *begin)
{
    const size_t NUM_OPERATORS = sizeof operators / sizeof operators[0];
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (strncmp(operators[i].operator_string, begin, 3) == 0) {
            return operators[i].ttype;
        }
    }

    return TTNONE;
}

/**All the operators with length 2
 */
const char *operator2[] = {
    "==",
    "!=",
    "&&",
    "||",
    "++",
    "--",
    ">=",
    "<=",
    "=>",
    "?.",
    "??",
    ">>",
    "<<",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
};

bool is_operator2(const char *begin)
{
    const size_t LEN_OPERATOR2 = sizeof operator2 / sizeof operator2[0];
    for (int i = 0; i < LEN_OPERATOR2; i++) {
        if (strncmp(begin, operator2[i], 2) == 0) {
            return true;
        }
    }
    return false;
}

enum TokenType dereference_operator2(const char *begin)
{
    const size_t NUM_OPERATORS = sizeof operators / sizeof operators[0];
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (strncmp(operators[i].operator_string, begin, 2) == 0) {
            return operators[i].ttype;
        }
    }

    return TTNONE;
}

/**All the operators with length 1
 */
const char *operator1[] = {
    "&",
    "|",
    "^",
    "~",
    "=",
    "+",
    "-",
    "/",
    "*",
    "%",
    "!",
    "?",
    "<",
    ">",
    ".",
};

bool is_operator1(const char *begin) {
    const size_t LEN_OPERATOR1 = sizeof operator1 / sizeof operator1[0];
    for (int i = 0; i < LEN_OPERATOR1; i++) {
        if (*begin == *operator1[i])
            return true;
    }

    return false;
}

enum TokenType dereference_operator1(const char *begin)
{
    const size_t NUM_OPERATORS = sizeof operators / sizeof operators[0];
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (strncmp(operators[i].operator_string, begin, 1) == 0) {
            return operators[i].ttype;
        }
    }

    return TTNONE;
}

const char *syntax_tokens[] = {
    ";",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
    ":",
};

bool is_syntax_token(const char *begin)
{
    const size_t NUM_SYNTAX_TOKENS = sizeof syntax_tokens / sizeof syntax_tokens[0];
    for (int i = 0; i < NUM_SYNTAX_TOKENS; i++) {
        if (*begin == *syntax_tokens[i])
            return true;
    }
    return false;
}

enum TokenType dereference_syntax_token(const char *begin)
{
    const size_t NUM_OPERATORS = sizeof operators / sizeof operators[0];
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (*operators[i].operator_string == *begin)
            return operators[i].ttype;
    }

    return TTNONE;
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

/**Returns the end of a valid identifier
 */
const char *traverse_identifier(const char *string)
{
    assert(isalpha(*string) && "should have checked the first char of an identifier");

    for (; *string != '\0' && (isalpha(*string) || isdigit(*string)); ++string)
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

const char *traverse_block_comment(const char *string) {
    assert(strncmp(string, "/*", 2) == 0 && "should have checked block comment starts with /*");

    string += strlen("/*");

    for (; *string != '\0'; ++string) {
        if (*string == '*') {
            if (*(++string) == '/')
                return ++string;
        }
    }

    return string;
}

int tokenise_file(const char *contents, struct Token tokens[], size_t max_tokens, size_t *tokens_written)
{
    size_t i = 0;

    memset(tokens, 0, sizeof tokens[0] * max_tokens);
    while (*contents != '\0') {
        if (i >= max_tokens) {
            return EXIT_FAILURE;
        }

        if (isalpha(*contents)) {
            const char *end = traverse_identifier(contents);
            struct StringView id = { .data = contents, .length = end - contents };
            contents = end;
            enum TokenType ttype = get_keyword_type(id);

            if (ttype == TTNONE) {
                tokens[i].type = TTIDENTIFIER;
                tokens[i].view = id;
            } else {
                tokens[i].type = ttype;
            }
            i++;
        } else if (isspace(*contents)) {
            while (isspace(*(++contents)))
                ;
        } else if (isdigit(*contents)) {
            const char *end = traverse_digit_literal(contents);
            tokens[i++] = (struct Token) {
                .type = TTNUMLITERAL,
                .view = { .data = contents, .length = end - contents },
            };
            contents = end;
        } else if (strncmp(contents, "//", 2) == 0) {
            // a line comment
            contents = traverse_line_comment(contents);
        } else if (strncmp(contents, "/*", 2) == 0) {
            // a block comment
            contents = traverse_block_comment(contents);
        } else if (*contents == '\'') {
            // single-quoted string
            const char *end = traverse_single_quoted_string(contents);
            if (end == NULL) return EXIT_FAILURE;
            tokens[i++] = (struct Token) {
                .type = TTSINGLESTRING,
                .view = { .data = contents, .length = end - contents },
            };
        } else if (*contents == '"') {
            // double-quoted string
            const char *end = traverse_double_quoted_string(contents);
            if (end == NULL) return EXIT_FAILURE;
            tokens[i++] = (struct Token) {
                .type = TTSINGLESTRING,
                .view = { .data = contents, .length = end - contents },
            };
        } else if (is_operator3(contents)) {
            enum TokenType ttype = dereference_operator3(contents);
            tokens[i++] = (struct Token) { .type = ttype };
            contents += 3;
        } else if (is_operator2(contents)) {
            enum TokenType ttype = dereference_operator2(contents);
            tokens[i++] = (struct Token) { .type = ttype };
            contents += 2;
        } else if (is_operator1(contents)) {
            enum TokenType ttype = dereference_operator1(contents);
            tokens[i++] = (struct Token) { .type = ttype };
            contents += 1;
        } else if (is_syntax_token(contents)) {
            enum TokenType ttype = dereference_syntax_token(contents);
            tokens[i++] = (struct Token) { .type = ttype };
            contents++;
        } else {
            assert(0 && "unreachable");
        }
    }

    if (tokens_written != NULL)
        *tokens_written = i;

    return EXIT_SUCCESS;
}

void print_token(const struct Token *token)
{
    const size_t NUM_KEYWORDS = sizeof keywords / sizeof keywords[0];
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (keywords[i].ttype == token->type) {
            printf("keyword %s\n", keywords[i].keyword);
            return;
        }
    }

    switch (token->type) {
    case TTNONE:
        printf("(none)\n");
        break;
        break;
    case TTIDENTIFIER:
        printf("identifier %.*s\n", (int)token->view.length, token->view.data);
        break;
    case TTNUMLITERAL:
        printf("numeric literal %.*s\n", (int)token->view.length, token->view.data);
        break;
    case TTBREAK:
    case TTCASE:
    case TTCATCH:
    case TTCLASS:
    case TTCONST:
    case TTCONTINUE:
    case TTDEBUGGER:
    case TTDEFAULT:
    case TTDELETE:
    case TTDO:
    case TTELSE:
    case TTENUM:
    case TTEXPORT:
    case TTEXTENDS:
    case TTFALSE:
    case TTFINALLY:
    case TTFOR:
    case TTFUNCTION:
    case TTIF:
    case TTIMPORT:
    case TTIN:
    case TTINSTANCEOF:
    case TTNEW:
    case TTNULL:
    case TTRETURN:
    case TTSUPER:
    case TTSWITCH:
    case TTTHIS:
    case TTTHROW:
    case TTTRUE:
    case TTTRY:
    case TTTYPEOF:
    case TTVAR:
    case TTVOID:
    case TTWHILE:
    case TTWITH:
    case TTAS:
    case TTIMPLEMENTS:
    case TTINTERFACE:
    case TTLET:
    case TTPACKAGE:
    case TTPRIVATE:
    case TTPROTECTED:
    case TTPUBLIC:
    case TTSTATIC:
    case TTYIELD:
    case TTIDENT:
    case TTNOTIDENT:
    case TTEQ:
    case TTNOTEQ:
    case TTASSIGN:
    case TTPLUS:
    case TTMINUS:
    case TTDIVIDE:
    case TTMULTIPLY:
    case TTMODULO:
    case TTPLUSASSIGN:
    case TTMINUSASSIGN:
    case TTDIVIDEASSIGN:
    case TTMULTIPLYASSIGN:
    case TTMODULOASSIGN:
    case TTBITAND:
    case TTBITOR:
    case TTBITXOR:
    case TTBITNOT:
    case TTBITSHRZERO:
    case TTBITSHR:
    case TTBITSHL:
    case TTAND:
    case TTOR:
    case TTBANG:
    case TTCONDITIONAL:
    case TTLESS:
    case TTGREATER:
    case TTLESSEQ:
    case TTGREATEREQ:
    case TTSEMICOLON:
    case TTOPENPAREN:
    case TTCLOSEPAREN:
    case TTOPENBRACE:
    case TTCLOSEBRACE:
    case TTOPENBRACKET:
    case TTCLOSEBRACKET:
    case TTCOLON:
    case TTDOT:
        printf("%s\n", token_type_strings[token->type]);
        break;
    default:
        fprintf(stderr, "default not handled\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: compile [file]\n");
        return EXIT_FAILURE;
    }

    char *to_read = NULL;

    if (load_file(argv[1], &to_read) != EXIT_SUCCESS) {
        fprintf(stderr, "could not load file\n");
    }

    #define MAX_TOKENS 64
    struct Token tokens[MAX_TOKENS];
    size_t num_tokens;
    if (tokenise_file(to_read, tokens, MAX_TOKENS, &num_tokens) != EXIT_SUCCESS) {
        fprintf(stderr, "failure to tokenise\n");
    }

    printf("Got a list of tokens:\n");
    for (int i = 0; i < num_tokens; i++) {
        print_token(&tokens[i]);
    }

    free(to_read);

    return EXIT_SUCCESS;
}

int load_file(const char *name, char **out_data)
{
    FILE *f;

    if ((f = fopen(name, "rt")) == NULL) {
        return EXIT_FAILURE;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *contents = malloc(sizeof *contents * (size + 1));

    fread(contents, sizeof *contents, size, f);
    contents[size] = '\0';

    *out_data = contents;

    return EXIT_SUCCESS;
}
