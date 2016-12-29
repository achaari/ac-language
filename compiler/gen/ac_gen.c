

#include "accmpl.h"

static const char * __keywords[] = {
                "while",          "virtual",              "var",           "typeof",           "switch",           "static",           "sizeof",           "sealed",           "return",
               "public",        "protected",          "private",           "object",            "local",               "is",         "internal",               "in",               "if",
                 "goto",           "global",         "function",          "foreach",              "for",           "extern",           "extend",             "else",               "do",
              "default",         "continue",            "const",             "case",            "break",               "as" };


static const char * __tokens[] = {
         "~",     "}",    "||",    "|=",     "|",     "{",     "^",     "@",     "?",   ">>=",    ">>",    ">=",     ">",    "=>",    "==",     "=",    "<=",   "<<=",    "<<",     "<",
         ";",    "::",     ":",    "/=",     "/",     ".",    "->",    "-=",    "--",     "-",     ",",    "+=",    "++",     "+",    "*=",     "*",     ")",     "(",    "&=",    "&&",
         "&",     "$",    "!=",     "!" };


#ifndef AC_DEBUG
static const int __prcex[] = {
       535,    16,    27,    44,    67,   208,   249,   256,   273,   322,   324,   360,   425,   467,   482,   506,    26,    32,     5,     1,    26,    36,     1,     7,     4,
         5,     3,    43,    27,     2,    13,    19,     7,    13,     2,    41,    32,    30,     7,    13,     4,    32,    20,     3,    66,    22,    20,    17,    32,    37,
         1,    62,    36,    36,     1,    62,    17,    36,    36,    32,    30,     5,    34,    20,     7,     1,     3,   207,     9,     1,     2,    89,    22,    17,    32,
        37,     7,     5,    32,    36,     7,     4,     2,    88,    22,    25,     7,     4,     4,     2,   102,    22,     0,    32,    37,     7,     5,    32,    36,     7,
         4,     4,     2,   119,    22,    26,     7,     4,    22,     0,    32,    37,     7,     5,    32,    36,    32,    20,     4,     2,   155,    22,     4,    32,    37,
         7,     5,    32,    36,    32,     5,     1,   154,    36,     1,     2,   146,    22,    27,    32,    22,     7,     4,    32,     1,     6,    22,    30,    12,    32,
        22,     7,     4,     5,     4,     2,   182,    22,    22,    32,    37,     7,    12,     1,   171,    36,    20,     7,     5,    32,    20,     1,   179,    36,    36,
         7,     5,    32,    36,     7,     4,     4,     2,   205,    22,    21,    32,    37,     7,     5,    22,    16,     7,     5,     2,   200,    32,    22,     7,     5,
        32,    36,     7,     4,     4,     7,    12,     3,   248,     2,   218,    37,     2,    32,    28,     7,     5,     4,     2,   230,    37,     6,    43,    29,    33,
        40,    35,     0,     7,    11,     2,   238,    27,     2,    14,    24,     7,     6,     2,   248,    32,     8,     7,     5,    32,    22,     7,     5,     3,   255,
         1,   255,    17,    35,    21,     3,   272,    17,     2,   272,    32,    37,    34,    36,     1,   270,     7,     5,    35,    30,    32,    36,     3,   321,     2,
       310,    32,    37,     1,   290,    36,    36,     1,   290,    17,    36,    36,    32,    30,     5,    32,    36,     2,   301,    32,     5,     7,     4,    32,     1,
         4,    32,    13,    32,    37,     7,     9,    32,    36,     4,     7,     6,     1,   321,     2,   321,    32,    25,     7,     7,     5,     3,   323,     3,   359,
        14,     2,   333,    32,     7,     7,     8,     4,     2,   346,    27,     2,     3,     6,    32,    37,     7,     6,    32,    36,     4,     2,   355,    32,    37,
         7,     9,    32,    36,     4,    33,    41,     7,     7,     3,   424,     7,    10,     1,   376,     2,   376,    37,     4,    26,    21,    22,    25,     7,     7,
         5,    38,     2,    32,    28,     2,   386,    22,    32,     7,     6,     2,   401,    37,     9,    15,    31,    27,    34,    23,    38,     3,    17,     9,     7,
         5,     2,   424,    37,    17,     2,    39,     4,    40,    14,    11,    16,    10,    18,    12,    19,    42,    33,    29,    35,    24,     6,     7,     5,     3,
       466,    34,    20,     2,   434,    32,    22,    17,     4,     2,   443,    27,     2,    31,    28,    32,    20,     4,     2,   451,    22,    18,    17,    32,    20,
         4,     2,   460,    22,     8,     8,     5,    32,    20,     4,     9,     2,     7,     5,    32,    20,     3,   481,    17,     2,   475,    22,    32,     7,     6,
         2,   481,    32,    15,     7,     5,     3,   505,    28,     9,     9,    11,    10,    15,    23,     7,     5,     1,    29,     2,   503,    22,     2,     7,    13,
        32,    20,     4,     7,     3,     3,   534,    22,    12,    17,     2,   520,    32,    22,     1,   520,     7,     6,    35,    30,    32,     5,     1,   534,     7,
        14,     2,   533,    32,     1,    32,    20,     6,     5,     3,   542,     9,     2,     9,    15,     9,     3,     3 };

#else

/************* AC-PROCs PROTOTYPE *************/
static int __exec_statement_block(p_accmpl_ cmplhndp);
static int __exec_variable_declaration(p_accmpl_ cmplhndp);
static int __exec_function_definition(p_accmpl_ cmplhndp);
static int __exec_block_statement(p_accmpl_ cmplhndp);
static int __exec_expression(p_accmpl_ cmplhndp);
static int __exec_type_name(p_accmpl_ cmplhndp);
static int __exec_variable_name(p_accmpl_ cmplhndp);
static int __exec_lamda_definition(p_accmpl_ cmplhndp);
static int __exec_expressions(p_accmpl_ cmplhndp);
static int __exec_variable_expression(p_accmpl_ cmplhndp);
static int __exec_unit_expression(p_accmpl_ cmplhndp);
static int __exec_expression_statement(p_accmpl_ cmplhndp);
static int __exec_variable_definition(p_accmpl_ cmplhndp);
static int __exec_object_member(p_accmpl_ cmplhndp);
static int __exec_object_definition(p_accmpl_ cmplhndp);
static int __exec_main(p_accmpl_ cmplhndp);

/************* AC-PROCs DEFINITION ************/
static int __exec_statement_block(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_statement_block, "statement_block");

    CHECK_SYMBOL('{') 
    BEG_PROCSEQ 
        BREAK_PROCSEQ_IF_CHECK_SYMBOL('}') 
        EXEC_PROC(block_statement) 
        RECALL_PROCSEQ 
    END_PROCSEQ

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_variable_declaration(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_variable_declaration, "variable_declaration");

    CHECK_MULTI_KEYWORD("local", "global")
    EXEC_PROC(variable_definition) 
    IF_CHECK_SYMBOL(',') 
        EXEC_PROC(variable_definition) 
        ACCEPT_PROC 
    END_IF
    CHECK_SYMBOL(';') 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_function_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_function_definition, "function_definition");

    CHECK_KEYWORD("function") 
    GET_IDENT 
    CHECK_SYMBOL('(') 
    BEG_PROCSEQ 
        BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
        BEG_PROCSEQ 
            GET_IDENT 
            BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
            CHECK_SYMBOL(',') 
            RECALL_PROCSEQ 
        END_PROCSEQ
    END_PROCSEQ
    ACCEPT_PROC_IF_CHECK_SYMBOL(';') 
    EXEC_PROC(statement_block) 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_block_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_block_statement, "block_statement");

    ACCEPT_PROC_IF_EXEC_PROC(statement_block) 
    IF_CHECK_KEYWORD("if") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(')') 
        EXEC_PROC(block_statement) 
        IF_CHECK_KEYWORD("else") 
            EXEC_PROC(block_statement) 
        END_IF
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("while") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(')') 
        EXEC_PROC(block_statement) 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("do") 
        EXEC_PROC(block_statement) 
        CHECK_KEYWORD("while") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(')') 
        CHECK_SYMBOL(';') 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("switch") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(')') 
        CHECK_SYMBOL('{') 
        BEG_PROCSEQ 
            BREAK_PROCSEQ_IF_CHECK_SYMBOL('}') 
            IF_CHECK_KEYWORD("default") 
                CHECK_SYMBOL(':') 
                EXEC_PROC(block_statement) 
                CHECK_SYMBOL('}') 
                RECALL_BREAK 
            END_IF
            CHECK_KEYWORD("case") 
            CHECK_LITERAL 
            CHECK_SYMBOL(':') 
            EXEC_PROC(block_statement) 
            RECALL_PROCSEQ 
        END_PROCSEQ
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("for") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression_statement) 
        BEG_PROCSEQ 
            BREAK_PROCSEQ_IF_CHECK_SYMBOL(';') 
            EXEC_PROC(expression) 
            CHECK_SYMBOL(';') 
        END_PROCSEQ
        BEG_PROCSEQ 
            BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
            EXEC_PROC(expression) 
            CHECK_SYMBOL(')') 
        END_PROCSEQ
        EXEC_PROC(block_statement) 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("foreach") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expression) 
        CHECK_KEYWORD("in") 
        EXEC_PROC(expression) 
        IF_CHECK_SYMBOL(':') 
            EXEC_PROC(expression) 
        END_IF
        CHECK_SYMBOL(')') 
        EXEC_PROC(block_statement) 
        ACCEPT_PROC 
    END_IF
    EXEC_PROC(expression_statement) 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_expression, "expression");

    IF_CHECK_MULTI_TOKEN("++", "--")
        EXEC_PROC(expression) 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_MULTI_TOKEN("!", "-", "+", "&", "*", "~")
        EXEC_PROC(unit_expression) 
    END_IF
    IF_CHECK_MULTI_KEYWORD("is", "extend")
        EXEC_PROC(type_name) 
    END_IF
    IF_CHECK_SYMBOL('?') 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(':') 
        EXEC_PROC(expression) 
    END_IF

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_type_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_type_name, "type_name");

    BEG_PROCSEQ 
        GET_IDENT 
        RECALL_PROCSEQ_IF_CHECK_TOKEN("::") 
    END_PROCSEQ

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_variable_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_variable_name, "variable_name");

    GET_IDENT 
    IF_CHECK_SYMBOL('(') 
        ACCEPT_PROC_IF_CHECK_SYMBOL(')') 
        BEG_PROCSEQ 
            EXEC_PROC(expression) 
            RECALL_PROCSEQ_IF_CHECK_SYMBOL(',') 
        END_PROCSEQ
        CHECK_SYMBOL(')') 
    END_IF

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_lamda_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_lamda_definition, "lamda_definition");

    IF_CHECK_SYMBOL('(') 
        BEG_PROCSEQ 
            BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
            BEG_PROCSEQ 
                GET_IDENT 
                BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                CHECK_SYMBOL(',') 
                RECALL_PROCSEQ 
            END_PROCSEQ
        END_PROCSEQ
        CHECK_SYMBOL(')') 
        IF_CHECK_SYMBOL('{') 
            EXEC_PROC(block_statement) 
            CHECK_SYMBOL('}') 
            ACCEPT_PROC 
        END_IF
        CHECK_TOKEN("=>") 
        CHECK_SYMBOL('(') 
        EXEC_PROC(expressions) 
        CHECK_SYMBOL(')') 
        ACCEPT_PROC 
    END_IF
    EXEC_PROC(type_name) 
    BEG_PROCSEQ 
        IF_CHECK_SYMBOL('.') 
            EXEC_PROC(variable_name) 
            RECALL_PROCSEQ 
        END_IF
    END_PROCSEQ

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_expressions(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_expressions, "expressions");


    return(__end_proc(cmplhndp, &procp));
}

static int __exec_variable_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_variable_expression, "variable_expression");

    ACCEPT_PROC_IF_CHECK_LITERAL 
    IF_CHECK_SYMBOL('@') 
        EXEC_PROC(lamda_definition) 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_MULTI_KEYWORD("typeof", "sizeof")
        CHECK_SYMBOL('(') 
        EXEC_PROC(type_name) 
        CHECK_SYMBOL(')') 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_SYMBOL('(') 
        EXEC_PROC(expressions) 
        CHECK_SYMBOL(')') 
        ACCEPT_PROC 
    END_IF
    OPT_CHECK_SYMBOL('$') 
    EXEC_PROC(variable_name) 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_unit_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_unit_expression, "unit_expression");

    EXEC_PROC(variable_expression) 
    BEG_PROCSEQ 
        IF_CHECK_MULTI_TOKEN("->", "::", ":", ".")
            EXEC_PROC(variable_name) 
            RECALL_PROCSEQ 
        END_IF
    END_PROCSEQ
    OPT_CHECK_MULTI_TOKEN("++", "--")
    IF_CHECK_KEYWORD("as") 
        EXEC_PROC(type_name) 
    END_IF
    IF_CHECK_MULTI_TOKEN("=", "+=", "-=", "*=", "/=", "&=", "|=", "<<=", ">>=")
        EXEC_PROC(expression) 
    END_IF
    IF_CHECK_MULTI_TOKEN("||", "&&", "|", "&", "==", ">=", "<=", ">>", "<<", ">", "<", "!=", "+", "-", "*", "/", "^")
        EXEC_PROC(expression) 
    END_IF

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_expression_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_expression_statement, "expression_statement");

    ACCEPT_PROC_IF_CHECK_SYMBOL(';') 
    IF_CHECK_SYMBOL(':') 
        GET_IDENT 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_MULTI_KEYWORD("break", "continue")
        CHECK_SYMBOL(';') 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("goto") 
        GET_IDENT 
        CHECK_SYMBOL(';') 
        ACCEPT_PROC 
    END_IF
    IF_CHECK_KEYWORD("return") 
        OPT_EXEC_PROC(expression) 
        CHECK_SYMBOL(';') 
        ACCEPT_PROC 
    END_IF
    ACCEPT_PROC_IF_EXEC_PROC(variable_declaration) 
    EXEC_PROC(expression) 
    CHECK_SYMBOL(';') 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_variable_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_variable_definition, "variable_definition");

    GET_IDENT 
    IF_CHECK_KEYWORD("as") 
        EXEC_PROC(type_name) 
    END_IF
    IF_CHECK_SYMBOL('=') 
        EXEC_PROC(expression) 
    END_IF

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_object_member(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_object_member, "object_member");

    OPT_CHECK_MULTI_KEYWORD("public", "private", "protected", "internal", "extern", "sealed", "static", "virtual", "const")
    IF_CHECK_KEYWORD("var") 
        EXEC_PROC(variable_definition) 
        CHECK_SYMBOL(';') 
        ACCEPT_PROC 
    END_IF
    EXEC_PROC(function_definition) 

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_object_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_object_definition, "object_definition");

    CHECK_KEYWORD("object") 
    GET_IDENT 
    IF_CHECK_SYMBOL(':') 
        BEG_PROCSEQ 
            EXEC_PROC(type_name) 
            RECALL_PROCSEQ_IF_CHECK_SYMBOL(',') 
        END_PROCSEQ
    END_IF
    CHECK_SYMBOL('{') 
    BEG_PROCSEQ 
        EXEC_PROC(object_member) 
        IF_CHECK_SYMBOL('}') 
            CHECK_SYMBOL(';') 
            RECALL_BREAK 
        END_IF
        RECALL_PROCSEQ 
    END_PROCSEQ

    return(__end_proc(cmplhndp, &procp));
}

static int __exec_main(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_main, "main");

    ACCEPT_PROC_IF_EXEC_PROC(variable_declaration) 
    ACCEPT_PROC_IF_EXEC_PROC(object_definition) 
    ACCEPT_PROC_IF_EXEC_PROC(function_definition) 

    return(__end_proc(cmplhndp, &procp));
}
#endif


/*********** MAIN COMPILER FUNCTION ***********/
int __accmpl_exec_module()
{
    p_accmpl_ cmplhndp = __ac_new_compiler();

    __ac_compl_set_keyword(cmplhndp, __keywords, sizeof(__keywords)/sizeof(char*));


#ifndef AC_DEBUG
    __ac_compl_exec(cmplhndp, __prcex, sizeof(__prcex)/sizeof(int));

#else
    EXEC_PROC(main);
#endif

    return(__ac_exit_compiler(&cmplhndp));
}
