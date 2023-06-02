#include "compile.h"

#include <assert.h>
#include <ctype.h>

int parse_tokens(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out)
{
    return -1; // TODO
}

/* Each of these returns the end of the parsed sequence.
 */

static const struct Token *               parse_expression(const struct Token *tokens, size_t num_tokens, struct Expression *out);
static const struct Token *          parse_numeric_literal(const struct Token *tokens, size_t num_tokens, struct Expression *out);

static const struct Token *            parse_let_statement(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out);
static const struct Token * parse_statement_or_declaration(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out);

const struct Token *parse_expression(const struct Token *tokens, size_t num_tokens, struct Expression *out)
{
    const struct Token *end = NULL;

    switch (tokens[0].type) {
    case TTNUMLITERAL:
        end = parse_numeric_literal(tokens, num_tokens, out);
        break;
    default:
        assert(0 && "unhandled");
    }

    return end;
}

const struct Token *parse_numeric_literal(const struct Token *tokens, size_t num_tokens, struct Expression *out)
{
    assert(tokens[0].type == TTNUMLITERAL);
    assert(num_tokens == 1);
    assert(out != NULL);

    if (num_tokens == 0) {
        return NULL;
    }

    assert(tokens[0].view.length != 0 && "empty numeric literal");
    assert(tokens[0].view.data[0] != '-' && "negative integers not handled");

    int power = 1, value = 0;
    for (int i = tokens[0].view.length; i >= 0; i--) {
        if (!isdigit(tokens[0].view.data[i])) {
            // TODO error
            return NULL;
        } else {
            value += power * (tokens[0].view.data[i] - '0');
            power *= 10;
        }
    }

    out->etype = ETNUMERICLITERAL;
    out->et_numeric_literal.value = value;

    return &tokens[1];
}

const struct Token *
parse_let_statement(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out)
{
    /*
    only allows format: let <identifier> = <expression>;
    does not allow:
        let x;
        let x: string;
        let a, b = 2;
        let a = 1, b = a;
        let a: string = 'hello world';
    */
    assert(tokens[0].type == TTLET);

    if (num_tokens < 3) {
        // malformed let statement
        return NULL;
    }

    if (tokens[1].type != TTIDENTIFIER) return NULL;
    if (tokens[2].type != TTASSIGN) return NULL;

    const size_t num_parsed_tokens = 3;
    out->sdtype = SDLET;
    parse_expression(&tokens[num_parsed_tokens], num_tokens - num_parsed_tokens, &out->sd_let.initialiser);

    return NULL; // TODO implement this
}

const struct Token *
parse_statement_or_declaration(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out)
{
    if (num_tokens == 0) {
        return NULL;
    }

    switch (tokens[0].type) {
    case TTLET:
        parse_let_statement(tokens, num_tokens, out);
        break;
    default:
        assert(0 && "parse_statement_or_declaration unhandled token type");
    }

    return NULL; // TODO
}
