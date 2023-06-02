#ifndef COMPILE_H
#define COMPILE_H

#include <stdbool.h>
#include <stdlib.h>

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
    TTTEMPLATESTRING, // TODO
    // regex
    TTREGEXP, // TODO
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
    TTCOMMA,
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

int tokenise_file(const char *contents, struct Token **tokens, size_t *tokens_written);

enum ExpressionType {
    ETADDITION,
    ETADDITIONASSIGN,
    ETASSIGN,
    ETASYNCFUNCTION,
    ETASYNCGENFUNCTION,
    ETAWAIT,
    ETBITAND,
    ETBITANDASSIGN,
    ETBITNOT,
    ETBITOR,
    ETBITORASSIGN,
    ETBITXOR,
    ETBITXORASSIGN,
    ETCLASS,
    ETCOMMA,
    ETTERNARY,
    ETDECREMENT,
    ETDELETE,
    ETDESTRUCTUREASSIGN,
    ETDIVISION,
    ETDIVISIONASSIGN,
    ETEQUAL,
    ETEXPONENT,
    ETEXPONENTASSIGN,
    ETFUNCTION,
    ETGENFUNCTION,
    ETGREATER,
    ETGREATEREQUAL,
    ETGROUP, // parentheses, e.g. 3 * (2 / 4)
    ETIMPORTMETA,
    ETIMPORT,
    ETIN,
    ETINCREMENT,
    ETINEQUAL,
    ETINSTANCEOF,
    ETLEFTSHIFT,
    ETLEFTSHIFTASSIGN,
    ETLESS,
    ETLESSEQUAL,
    ETLOGICAND,
    ETLOGICOR,
    ETLOGICORASSIGN,
    ETMULTIPLY,
    ETMULTIPLYASSIGN,
    ETNEW,
    ETNEWTARGET,
    ETNULL,
    ETNULLCOALESCEASSIGN,
    ETNULLCOALESCE,
    ETNUMERICLITERAL,
    ETOBJECTINIT,
    ETOPTIONALCHAIN,
    ETPROPERTYACCESS,
    ETREMAINDER,
    ETREMAINDERASSIGN,
    ETRIGHTSHIFT,
    ETRIGHTSHIFTASSIGN,
    ETSPREAD,
    ETSTRICTEQUAL,
    ETSTRICTINEQUAL,
    ETSTRINGLITERAL,
    ETSUBTRACT,
    ETSUBTRACTASSIGN,
    ETSUPER,
    ETTHIS,
    ETTYPEOF,
    ETUNARYNEGATE,
    ETUNARYPLUS,
    ETUNSIGNEDRIGHTSHIFT,
    ETUNSIGNEDRIGHTSHIFTASSIGN,
    ETVOID,
    ETYIELD,
    ETGENYIELD,
};

struct etAddition                 {};
struct etAdditionAssign           {};
struct etAssign                   {};
struct etAsyncFunction            {};
struct etAsyncGenFunction         {};
struct etAwait                    {};
struct etBitAnd                   {};
struct etBitAndAssign             {};
struct etBitNot                   {};
struct etBitOr                    {};
struct etBitOrAssign              {};
struct etBitXor                   {};
struct etBitXorAssign             {};
struct etClass                    {};
struct etComma                    {};
struct etTernary                  {};
struct etDecrement                {};
struct etDelete                   {};
struct etDestructureAssign        {};
struct etDivision                 {};
struct etDivisionAssign           {};
struct etEqual                    {};
struct etExponent                 {};
struct etExponentAssign           {};
struct etFunction                 {};
struct etGenFunction              {};
struct etGreater                  {};
struct etGreaterEqual             {};
struct etGroup                    {};
struct etImportMeta               {};
struct etImport                   {};
struct etIn                       {};
struct etIncrement                {};
struct etInequal                  {};
struct etInstanceof               {};
struct etLeftShift                {};
struct etLeftShiftAssign          {};
struct etLess                     {};
struct etLessEqual                {};
struct etLogicAnd                 {};
struct etLogicOr                  {};
struct etLogicOrAssign            {};
struct etMultiply                 {};
struct etMultiplyAssign           {};
struct etNew                      {};
struct etNewTarget                {};
struct etNull                     {};
struct etNullCoalesceAssign       {};
struct etNullCoalesce             {};
struct etNumericLiteral           { int value; };
struct etObjectInit               {};
struct etOptionalChain            {};
struct etPropertyAccess           {};
struct etRemainder                {};
struct etRemainderAssign          {};
struct etRightShift               {};
struct etRightShiftAssign         {};
struct etSpread                   {};
struct etStrictEqual              {};
struct etStrictInequal            {};
struct etStringLiteral            { char *value; size_t length; };
struct etSubtract                 {};
struct etSubtractAssign           {};
struct etSuper                    {};
struct etThis                     {};
struct etTypeof                   {};
struct etUnaryNegate              {};
struct etUnaryPlus                {};
struct etUnsignedRightShift       {};
struct etUnsignedRightShiftAssign {};
struct etVoid                     {};
struct etYield                    {};
struct etGenYield                 {};

struct Expression {
    enum ExpressionType etype;
    union {
        struct etAddition                 et_addition;
        struct etAdditionAssign           et_addition_assign;
        struct etAssign                   et_assign;
        struct etAsyncFunction            et_async_function;
        struct etAsyncGenFunction         et_async_gen_function;
        struct etAwait                    et_await;
        struct etBitAnd                   et_bit_and;
        struct etBitAndAssign             et_bit_and_assign;
        struct etBitNot                   et_bit_not;
        struct etBitOr                    et_bit_or;
        struct etBitOrAssign              et_bit_or_assign;
        struct etBitXor                   et_bit_xor;
        struct etBitXorAssign             et_bit_xor_assign;
        struct etClass                    et_class;
        struct etComma                    et_comma;
        struct etTernary                  et_ternary;
        struct etDecrement                et_decrement;
        struct etDelete                   et_delete;
        struct etDestructureAssign        et_destructure_assign;
        struct etDivision                 et_division;
        struct etDivisionAssign           et_division_assign;
        struct etEqual                    et_equal;
        struct etExponent                 et_exponent;
        struct etExponentAssign           et_exponent_assign;
        struct etFunction                 et_function;
        struct etGenFunction              et_gen_function;
        struct etGreater                  et_greater;
        struct etGreaterEqual             et_greater_equal;
        struct etGroup                    et_group;
        struct etImportMeta               et_import_meta;
        struct etImport                   et_import;
        struct etIn                       et_in;
        struct etIncrement                et_increment;
        struct etInequal                  et_inequal;
        struct etInstanceof               et_instanceof;
        struct etLeftShift                et_left_shift;
        struct etLeftShiftAssign          et_left_shift_assign;
        struct etLess                     et_less;
        struct etLessEqual                et_less_equal;
        struct etLogicAnd                 et_logic_and;
        struct etLogicOr                  et_logic_or;
        struct etLogicOrAssign            et_logic_or_assign;
        struct etMultiply                 et_multiply;
        struct etMultiplyAssign           et_multiply_assign;
        struct etNew                      et_new;
        struct etNewTarget                et_new_target;
        struct etNull                     et_null;
        struct etNullCoalesceAssign       et_null_coalesce_assign;
        struct etNullCoalesce             et_null_coalesce;
        struct etNumericLiteral           et_numeric_literal;
        struct etObjectInit               et_object_init;
        struct etOptionalChain            et_optional_chain;
        struct etPropertyAccess           et_property_access;
        struct etRemainder                et_remainder;
        struct etRemainderAssign          et_remainder_assign;
        struct etRightShift               et_right_shift;
        struct etRightShiftAssign         et_right_shift_assign;
        struct etSpread                   et_spread;
        struct etStrictEqual              et_strict_equal;
        struct etStrictInequal            et_strict_inequal;
        struct etSubtract                 et_subtract;
        struct etSubtractAssign           et_subtract_assign;
        struct etSuper                    et_super;
        struct etThis                     et_this;
        struct etTypeof                   et_typeof;
        struct etUnaryNegate              et_unary_negate;
        struct etUnaryPlus                et_unary_plus;
        struct etUnsignedRightShift       et_unsigned_right_shift;
        struct etUnsignedRightShiftAssign et_unsigned_right_shift_assign;
        struct etVoid                     et_void;
        struct etYield                    et_yield;
        struct etGenYield                 et_gen_yield;
    };
};

enum StatementOrDeclarationType {
    SDNONE = 0, // used for signalling
    SDASYNCFUNCTION,
    SDASYNCGENFUNCTION,
    SDBLOCK,
    SDBREAK,
    SDCLASS,
    SDCONST,
    SDCONTINUE,
    SDDEBUGGER,
    SDDOWHILE,
    SDEMPTY,
    SDEXPORT,
    SDEXPRSTATEMENT,
    SDFOR,
    SDFORAWAITOF,
    SDFORIN,
    SDFOROF,
    SDFUNCTION,
    SDGENFUNCTION,
    SDIFELSE,
    SDIMPORT,
    SDLABEL,
    SDLET,
    SDRETURN,
    SDSWITCH,
    SDTHROW,
    SDTRYCATCH,
    SDVAR,
    SDWHILE,
};

struct Type {};

struct FunctionParameter {
    struct StringView name;
    struct Type type;
};

struct sdAsyncFunction    { struct FunctionParameter *parameters; size_t num_parameters; struct StatementOrDeclaration *statements; size_t num_statements; };
struct sdAsyncGenFunction { struct FunctionParameter *parameters; size_t num_parameters; struct StatementOrDeclaration *statements; size_t num_statements; };
struct sdBlock            { };
struct sdBreak            { };
struct sdClass            { };
struct sdConst            { struct StringView name; bool initialised; struct Expression initialiser; };
struct sdContinue         { };
struct sdDebugger         { };
struct sdDoWhile          { };
struct sdEmpty            { };
struct sdExport           { };
struct sdExprStatement    { };
struct sdFor              { };
struct sdForAwaitOf       { };
struct sdForIn            { };
struct sdForOf            { };
struct sdFunction         { struct FunctionParameter *parameters; size_t num_parameters; struct StatementOrDeclaration *statements; size_t num_statements; };
struct sdGenFunction      { struct FunctionParameter *parameters; size_t num_parameters; struct StatementOrDeclaration *statements; size_t num_statements; };
struct sdIfElse           { };
struct sdImport           { };
struct sdLabel            { };
struct sdLet              { struct StringView name; bool initialised; struct Expression initialiser; };
struct sdReturn           { };
struct sdSwitch           { };
struct sdThrow            { };
struct sdTryCatch         { };
struct sdVar              { struct StringView name; bool initialised; struct Expression initialiser; };
struct sdWhile            { };

struct StatementOrDeclaration {
    enum StatementOrDeclarationType sdtype;
    union {
        struct sdAsyncFunction      sd_async_function;
        struct sdAsyncGenFunction   sd_async_gen_function;
        struct sdBlock              sd_block;
        struct sdBreak              sd_break;
        struct sdClass              sd_class;
        struct sdConst              sd_const;
        struct sdContinue           sd_continue;
        struct sdDebugger           sd_debugger;
        struct sdDoWhile            sd_do_while;
        struct sdEmpty              sd_empty;
        struct sdExport             sd_export;
        struct sdExprStatement      sd_expr_statement;
        struct sdFor                sd_for;
        struct sdForAwaitOf         sd_for_await_of;
        struct sdForIn              sd_for_in;
        struct sdForOf              sd_for_of;
        struct sdFunction           sd_function;
        struct sdGenFunction        sd_gen_function;
        struct sdIfElse             sd_if_else;
        struct sdImport             sd_import;
        struct sdLabel              sd_label;
        struct sdLet                sd_let;
        struct sdReturn             sd_return;
        struct sdSwitch             sd_switch;
        struct sdThrow              sd_throw;
        struct sdTryCatch           sd_try_catch;
        struct sdVar                sd_var;
        struct sdWhile              sd_while;
    };
};

int parse_tokens(const struct Token *tokens, size_t num_tokens, struct StatementOrDeclaration *out);

#endif // COMPILE_H
