typedef void *p_accmpl_;
typedef void *p_accmpl_proc_;
typedef int (*__exec)(p_accmpl_ cmplhndp);

extern int __ac_init_proc(p_accmpl_ cmplhndp, __exec prcfctp, const char *procname);
extern int __ac_compl_set_keywords(p_accmpl_ cmplhndp, char **keywords, int count);
extern int __ac_compl_set_tokens(p_accmpl_ cmplhndp, char **tokens, int count);


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
       537,    17,    28,    45,    68,   209,   250,   257,   274,   323,   325,   361,   426,   431,   469,   484,   508,    27,    32,     5,     1,    27,    36,     1,     7,
         4,     5,     3,    44,    27,     2,    13,    19,     7,    14,     2,    42,    32,    30,     7,    14,     4,    32,    20,     3,    67,    22,    20,    17,    32,
        37,     1,    63,    36,    36,     1,    63,    17,    36,    36,    32,    30,     5,    34,    20,     7,     1,     3,   208,     9,     1,     2,    90,    22,    17,
        32,    37,     7,     5,    32,    36,     7,     4,     2,    89,    22,    25,     7,     4,     4,     2,   103,    22,     0,    32,    37,     7,     5,    32,    36,
         7,     4,     4,     2,   120,    22,    26,     7,     4,    22,     0,    32,    37,     7,     5,    32,    36,    32,    20,     4,     2,   156,    22,     4,    32,
        37,     7,     5,    32,    36,    32,     5,     1,   155,    36,     1,     2,   147,    22,    27,    32,    22,     7,     4,    32,     1,     6,    22,    30,    12,
        32,    22,     7,     4,     5,     4,     2,   183,    22,    22,    32,    37,     7,    13,     1,   172,    36,    20,     7,     5,    32,    20,     1,   180,    36,
        36,     7,     5,    32,    36,     7,     4,     4,     2,   206,    22,    21,    32,    37,     7,     5,    22,    16,     7,     5,     2,   201,    32,    22,     7,
         5,    32,    36,     7,     4,     4,     7,    13,     3,   249,     2,   219,    37,     2,    32,    28,     7,     5,     4,     2,   231,    37,     6,    43,    29,
        33,    40,    35,     0,     7,    11,     2,   239,    27,     2,    14,    24,     7,     6,     2,   249,    32,     8,     7,     5,    32,    22,     7,     5,     3,
       256,     1,   256,    17,    35,    21,     3,   273,    17,     2,   273,    32,    37,    34,    36,     1,   271,     7,     5,    35,    30,    32,    36,     3,   322,
         2,   311,    32,    37,     1,   291,    36,    36,     1,   291,    17,    36,    36,    32,    30,     5,    32,    36,     2,   302,    32,     5,     7,     4,    32,
         1,     4,    32,    13,    32,    37,     7,     9,    32,    36,     4,     7,     6,     1,   322,     2,   322,    32,    25,     7,     7,     5,     3,   324,     3,
       360,    14,     2,   334,    32,     7,     7,     8,     4,     2,   347,    27,     2,     3,     6,    32,    37,     7,     6,    32,    36,     4,     2,   356,    32,
        37,     7,     9,    32,    36,     4,    33,    41,     7,     7,     3,   425,     7,    10,     1,   377,     2,   377,    37,     4,    26,    21,    22,    25,     7,
         7,     5,    38,     2,    32,    28,     2,   387,    22,    32,     7,     6,     2,   402,    37,     9,    15,    31,    27,    34,    23,    38,     3,    17,     9,
         7,     5,     2,   425,    37,    17,     2,    39,     4,    40,    14,    11,    16,    10,    18,    12,    19,    42,    33,    29,    35,    24,     6,     7,     5,
         3,   430,    17,    32,    22,     3,   468,    34,    20,     9,    12,     2,   445,    27,     2,    31,    28,    32,    20,     4,     2,   453,    22,    18,    17,
        32,    20,     4,     2,   462,    22,     8,     8,     5,    32,    20,     4,     9,     2,     7,     5,    32,    20,     3,   483,    17,     2,   477,    22,    32,
         7,     6,     2,   483,    32,    15,     7,     5,     3,   507,    28,     9,     9,    11,    10,    15,    23,     7,     5,     1,    29,     2,   505,    22,     2,
         7,    14,    32,    20,     4,     7,     3,     3,   536,    22,    12,    17,     2,   522,    32,    22,     1,   522,     7,     6,    35,    30,    32,     5,     1,
       536,     7,    15,     2,   535,    32,     1,    32,    20,     6,     5,     3,   544,     9,     2,     9,    16,     9,     3,     3 };

#else
extern int __ac_compl_exec_mainproc(p_accmpl_ cmplhndp, __exec prcfctp);
extern int __ac_process_step(p_accmpl_ cmplhndp, int checkstepb, int step, ...);
extern int __ac_end_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);
extern int __ac_stop_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);

#define __ac_proc_step(stat, mand) __ac_process_step(cmplhndp, mandb, AC_CMPL_STAT_##stat)
#define __ac_check_step(stat)      __ac_proc_step(stat, FALSE)
#define __ac_exec_step(stat)       if(! __ac_proc_step(stat, TRUE)) { return(__ac_stop_proc(cmplhndp, &proc)); }
#define __ac_try_step(stat)        __ac_proc_step(stat, FALSE);


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
static int __exec_label_definition(p_accmpl_ cmplhndp);
static int __exec_expression_statement(p_accmpl_ cmplhndp);
static int __exec_variable_definition(p_accmpl_ cmplhndp);
static int __exec_object_member(p_accmpl_ cmplhndp);
static int __exec_object_definition(p_accmpl_ cmplhndp);
static int __exec_main(p_accmpl_ cmplhndp);

/************* AC-PROCs DEFINITION ************/
static int __exec_statement_block(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_statement_block, "statement_block");

    __ac_exec_step(SYMBOL('{')) 
    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL('}'))) { 
            break;
        }

        __ac_exec_step(EXECPROC(__exec_block_statement)) 
    }
    __ac_exec_stat(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_declaration(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_variable_declaration, "variable_declaration");

    __ac_exec_step(ONE_KEYWORD("local", "global")
    __ac_exec_step(EXECPROC(__exec_variable_definition)) 
    if (__ac_check_step(SYMBOL(','))) { 
        __ac_exec_step(EXECPROC(__exec_variable_definition)) 
        __ac_exec_step(ACCEPT)
    }

    __ac_exec_step(SYMBOL(';')) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_function_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_function_definition, "function_definition");

    __ac_exec_step(KEYWORD("function"))
    __ac_exec_step(IDENT) 
    __ac_exec_step(SYMBOL('(')) 
    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL(')'))) { 
            break;
        }

        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            __ac_exec_step(IDENT) 
            if (__ac_check_step(SYMBOL(')'))) { 
                break;
            }

            __ac_exec_step(SYMBOL(',')) 
        }
        __ac_exec_stat(END_PROCSEQ);

    }
    __ac_exec_stat(END_PROCSEQ);

    if (__ac_check_step(SYMBOL(';'))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(EXECPROC(__exec_statement_block)) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_block_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_block_statement, "block_statement");

    if (__ac_check_step(EXECPROC(__exec_statement_block))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(KEYWORD("if"))) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(EXECPROC(__exec_block_statement)) 
        if (__ac_check_step(KEYWORD("else"))) {
            __ac_exec_step(EXECPROC(__exec_block_statement)) 
        }

        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("while"))) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(EXECPROC(__exec_block_statement)) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("do"))) {
        __ac_exec_step(EXECPROC(__exec_block_statement)) 
        __ac_exec_step(KEYWORD("while"))
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(SYMBOL(';')) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("switch"))) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(SYMBOL('{')) 
        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL('}'))) { 
                break;
            }

            if (__ac_check_step(KEYWORD("default"))) {
                __ac_exec_step(SYMBOL(':')) 
                __ac_exec_step(EXECPROC(__exec_block_statement)) 
                __ac_exec_step(SYMBOL('}')) 
                break;
            }

            __ac_exec_step(KEYWORD("case"))
            __ac_exec_step(LITERAL) 
            __ac_exec_step(SYMBOL(':')) 
            __ac_exec_step(EXECPROC(__exec_block_statement)) 
        }
        __ac_exec_stat(END_PROCSEQ);

        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("for"))) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression_statement)) 
        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL(';'))) { 
                break;
            }

            __ac_exec_step(EXECPROC(__exec_expression)) 
            __ac_exec_step(SYMBOL(';')) 
            break;
        }
        __ac_exec_stat(END_PROCSEQ);

        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL(')'))) { 
                break;
            }

            __ac_exec_step(EXECPROC(__exec_expression)) 
            __ac_exec_step(SYMBOL(')')) 
            break;
        }
        __ac_exec_stat(END_PROCSEQ);

        __ac_exec_step(EXECPROC(__exec_block_statement)) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("foreach"))) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(KEYWORD("in"))
        __ac_exec_step(EXECPROC(__exec_expression)) 
        if (__ac_check_step(SYMBOL(':'))) { 
            __ac_exec_step(EXECPROC(__exec_expression)) 
        }

        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(EXECPROC(__exec_block_statement)) 
        __ac_exec_step(ACCEPT)
    }

    __ac_exec_step(EXECPROC(__exec_expression_statement)) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_expression, "expression");

    if (__ac_check_step(ONE_TOKEN("++", "--")) {
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(ONE_TOKEN("!", "-", "+", "&", "*", "~")) {
        __ac_exec_step(EXECPROC(__exec_unit_expression)) 
    }

    if (__ac_check_step(ONE_KEYWORD("is", "extend")) {
        __ac_exec_step(EXECPROC(__exec_type_name)) 
    }

    if (__ac_check_step(SYMBOL('?'))) { 
        __ac_exec_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(':')) 
        __ac_exec_step(EXECPROC(__exec_expression)) 
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_type_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_type_name, "type_name");

    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        __ac_exec_step(IDENT) 
        if (__ac_check_step(TOKEN("::"))) { 
            continue;
        }

        break;
    }
    __ac_exec_stat(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_variable_name, "variable_name");

    __ac_exec_step(IDENT) 
    if (__ac_check_step(SYMBOL('('))) { 
        if (__ac_check_step(SYMBOL(')'))) { 
            return(__ac_end_proc(cmplhndp, &procp));
        }

        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            __ac_exec_step(EXECPROC(__exec_expression)) 
            if (__ac_check_step(SYMBOL(','))) { 
                continue;
            }

            break;
        }
        __ac_exec_stat(END_PROCSEQ);

        __ac_exec_step(SYMBOL(')')) 
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_lamda_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_lamda_definition, "lamda_definition");

    if (__ac_check_step(SYMBOL('('))) { 
        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL(')'))) { 
                break;
            }

            __ac_exec_stat(BEG_PROCSEQ);
            while (__ac_pocess_next(cmplhndp)) {
                __ac_exec_step(IDENT) 
                if (__ac_check_step(SYMBOL(')'))) { 
                    break;
                }

                __ac_exec_step(SYMBOL(',')) 
            }
            __ac_exec_stat(END_PROCSEQ);

        }
        __ac_exec_stat(END_PROCSEQ);

        __ac_exec_step(SYMBOL(')')) 
        if (__ac_check_step(SYMBOL('{'))) { 
            __ac_exec_step(EXECPROC(__exec_block_statement)) 
            __ac_exec_step(SYMBOL('}')) 
            __ac_exec_step(ACCEPT)
        }

        __ac_exec_step(TOKEN("=>")) 
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_expressions)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(ACCEPT)
    }

    __ac_exec_step(EXECPROC(__exec_type_name)) 
    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL('.'))) { 
            __ac_exec_step(EXECPROC(__exec_variable_name)) 
            continue;
        }

    }
    __ac_exec_stat(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_expressions(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_expressions, "expressions");

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_variable_expression, "variable_expression");

    if (__ac_check_step(LITERAL)) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(SYMBOL('@'))) { 
        __ac_exec_step(EXECPROC(__exec_lamda_definition)) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(ONE_KEYWORD("typeof", "sizeof")) {
        __ac_exec_step(SYMBOL('(')) 
        __ac_exec_step(EXECPROC(__exec_type_name)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(SYMBOL('('))) { 
        __ac_exec_step(EXECPROC(__exec_expressions)) 
        __ac_exec_step(SYMBOL(')')) 
        __ac_exec_step(ACCEPT)
    }

    __ac_try_step(SYMBOL('$')) 
    __ac_exec_step(EXECPROC(__exec_variable_name)) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_unit_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_unit_expression, "unit_expression");

    __ac_exec_step(EXECPROC(__exec_variable_expression)) 
    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(ONE_TOKEN("->", "::", ":", ".")) {
            __ac_exec_step(EXECPROC(__exec_variable_name)) 
            continue;
        }

    }
    __ac_exec_stat(END_PROCSEQ);

    __ac_try_step(ONE_TOKEN("++", "--")
    if (__ac_check_step(KEYWORD("as"))) {
        __ac_exec_step(EXECPROC(__exec_type_name)) 
    }

    if (__ac_check_step(ONE_TOKEN("=", "+=", "-=", "*=", "/=", "&=", "|=", "<<=", ">>=")) {
        __ac_exec_step(EXECPROC(__exec_expression)) 
    }

    if (__ac_check_step(ONE_TOKEN("||", "&&", "|", "&", "==", ">=", "<=", ">>", "<<", ">", "<", "!=", "+", "-", "*", "/", "^")) {
        __ac_exec_step(EXECPROC(__exec_expression)) 
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_label_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_label_definition, "label_definition");

    __ac_exec_step(IDENT) 
    __ac_exec_step(SYMBOL(':')) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_expression_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_expression_statement, "expression_statement");

    if (__ac_check_step(SYMBOL(';'))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(EXECPROC(__exec_label_definition))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(ONE_KEYWORD("break", "continue")) {
        __ac_exec_step(SYMBOL(';')) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("goto"))) {
        __ac_exec_step(IDENT) 
        __ac_exec_step(SYMBOL(';')) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(KEYWORD("return"))) {
        __ac_try_step(EXECPROC(__exec_expression)) 
        __ac_exec_step(SYMBOL(';')) 
        __ac_exec_step(ACCEPT)
    }

    if (__ac_check_step(EXECPROC(__exec_variable_declaration))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(EXECPROC(__exec_expression)) 
    __ac_exec_step(SYMBOL(';')) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_variable_definition, "variable_definition");

    __ac_exec_step(IDENT) 
    if (__ac_check_step(KEYWORD("as"))) {
        __ac_exec_step(EXECPROC(__exec_type_name)) 
    }

    if (__ac_check_step(SYMBOL('='))) { 
        __ac_exec_step(EXECPROC(__exec_expression)) 
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_object_member(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_object_member, "object_member");

    __ac_try_step(ONE_KEYWORD("public", "private", "protected", "internal", "extern", "sealed", "static", "virtual", "const")
    if (__ac_check_step(KEYWORD("var"))) {
        __ac_exec_step(EXECPROC(__exec_variable_definition)) 
        __ac_exec_step(SYMBOL(';')) 
        __ac_exec_step(ACCEPT)
    }

    __ac_exec_step(EXECPROC(__exec_function_definition)) 
    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_object_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_object_definition, "object_definition");

    __ac_exec_step(KEYWORD("object"))
    __ac_exec_step(IDENT) 
    if (__ac_check_step(SYMBOL(':'))) { 
        __ac_exec_stat(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            __ac_exec_step(EXECPROC(__exec_type_name)) 
            if (__ac_check_step(SYMBOL(','))) { 
                continue;
            }

            break;
        }
        __ac_exec_stat(END_PROCSEQ);

    }

    __ac_exec_step(SYMBOL('{')) 
    __ac_exec_stat(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        __ac_exec_step(EXECPROC(__exec_object_member)) 
        if (__ac_check_step(SYMBOL('}'))) { 
            __ac_exec_step(SYMBOL(';')) 
            break;
        }

    }
    __ac_exec_stat(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_main(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_main, "main");

    if (__ac_check_step(EXECPROC(__exec_variable_declaration))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(EXECPROC(__exec_object_definition))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(EXECPROC(__exec_function_definition))) { 
        return(__ac_end_proc(cmplhndp, &procp));
    }

    return(__ac_end_proc(cmplhndp, &procp));
}
#endif

/*********** MAIN COMPILER FUNCTION ***********/
int __accmpl_exec_module()
{
    p_accmpl_ cmplhndp = __ac_new_compiler();

    __ac_compl_set_keywords(cmplhndp, __keywords, sizeof(__keywords)/sizeof(char*));
    __ac_compl_set_tokens(cmplhndp, __tokens, sizeof(__tokens)/sizeof(char*));


#ifndef AC_DEBUG
    __ac_compl_exec(cmplhndp, __prcex, sizeof(__prcex)/sizeof(int));
#else
    __ac_compl_exec_mainproc(cmplhndp, __exec_main);
#endif

    return(__ac_exit_compiler(&cmplhndp));
}
