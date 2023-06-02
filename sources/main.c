#include "compile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

struct Arguments {
    bool strict;
    const char *file;
};

enum OptionIndex {
    OISTRICT = 0,
    OIMAX = 1,
};

const static struct option options[] = {
    [OISTRICT] = { "strict", no_argument, NULL, 0 },
    [OIMAX] = {0},
};

static int load_file(const char *name, char **out_data);
static void print_token(const struct Token *token);
static void print_usage(void);

int main(int argc, const char *argv[])
{
    struct Arguments arguments = {0};

    while (true) {
        int index, c;

        if ((c = getopt_long(argc, (char *const *)argv, "", options, &index)) == -1)
            break;

        assert(c == 0);

        switch (index) {
        case OISTRICT:
            arguments.strict = true;
            break;
        default:
            assert(0 && "unreachable");
        }
    }

    const size_t EXPECTED_POSITIONAL_ARGS = 1;

    if (optind + EXPECTED_POSITIONAL_ARGS != argc) {
        print_usage();
        return EXIT_FAILURE;
    } else {
        printf("%susing strict mode\n", arguments.strict ? "" : "not ");
        arguments.file = argv[optind];
    }

    char *to_read = NULL;

    if (load_file(arguments.file, &to_read) != EXIT_SUCCESS) {
        fprintf(stderr, "could not load file\n");
    }

    struct Token *tokens = NULL;
    size_t num_tokens;
    if (tokenise_file(to_read, &tokens, &num_tokens) != EXIT_SUCCESS) {
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

void print_usage()
{
    printf("Usage: compile [--strict] file\n");
}

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
    [TTCOMMA] = ",",
};

static const struct {
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
    case TTCOMMA:
    case TTDOT:
        printf("%s\n", token_type_strings[token->type]);
        break;
    default:
        assert(0 && "unreachable");
    }
}
