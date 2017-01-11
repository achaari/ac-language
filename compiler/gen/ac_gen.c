

#include "acstd.h"
#include "accmplpub.h"

static const char * __keywords[] = {
                "while",             "void",          "virtual",              "var",           "typeof",          "typedef",           "switch",           "struct",           "static",
               "sizeof",           "sealed",           "return",           "public",        "protected",          "private",           "object",            "local",               "is",
             "internal",              "int",               "in",               "if",             "goto",           "global",         "function",          "foreach",              "for",
                "float",           "extern",           "extend",             "else",           "double",               "do",          "default",         "continue",            "const",
                 "char",             "case",            "break",               "as",           "C_CODE" };


static const char * __tokens[] = {
         "~",     "}",    "||",    "|=",     "|",     "{",     "^",     "]",     "[",     "@",     "?",   ">>=",    ">>",    ">=",     ">",    "=>",    "==",     "=",    "<=",   "<<=",
        "<<",     "<",     ";",    "::",     ":",    "/=",     "/",     ".",    "->",    "-=",    "--",     "-",     ",",    "+=",    "++",     "+",    "*=",     "*",     ")",     "(",
        "&=",    "&&",     "&",     "$",    "!=",     "!" };


#ifndef AC_DEBUG
static const int __prcex[] = {
       557,    34,    36,    53,    84,   142,   181,   196,   213,   259,   295,   357,   362,   372,   388,   398,   412,   445,   469,   489,   504,   528,   581,   588,   604,
       613,   616,   619,   640,   658,   694,   701,   720,   728,    35,     5,    52,    39,     2,    16,    23,     9,    19,     3,    50,    44,    32,     9,    19,     6,
        44,    22,     5,    83,    34,    24,    29,    44,    39,     3,    70,    29,     2,    70,     3,    70,    44,    32,    29,     7,    44,    38,    46,    22,    44,
         5,     4,     0,     9,     4,     1,    44,     1,     5,   141,    11,    11,    21,     6,    21,    13,     0,    14,    32,    15,     6,    16,    26,    17,    25,
        18,     3,   110,    39,     2,    38,    34,    44,    22,     6,     3,   118,    34,    22,    29,    44,    22,     6,     3,   127,    34,    11,    10,     5,    44,
        22,     6,     3,   139,    44,     5,     2,   138,    48,     1,     9,     4,     7,     6,     9,    12,     5,   180,     3,   152,    49,     2,    34,    30,     9,
         5,     6,    50,     6,    45,    31,    35,    42,    37,     0,     9,    10,     3,   170,    39,     2,    17,    29,     9,     6,     3,   180,    44,    10,     9,
         5,    44,    24,     9,     5,     5,   195,    41,     5,     1,    19,    31,    27,    36,    29,     4,   195,    44,    23,    29,     5,   212,    29,     3,   212,
        44,    39,    46,    38,     2,   210,     9,     5,    47,    32,    44,    38,     5,   258,     3,   250,    44,    39,     2,   230,    48,    38,     2,   230,    29,
        48,    38,    44,    32,     7,    44,    38,     3,   241,    44,     5,     9,     4,    44,     1,     6,    44,    15,    44,    39,     9,     5,    44,    38,     6,
         9,     6,     4,   258,    44,    27,     9,     7,     5,   294,    26,     3,   268,    44,     9,     9,     8,     6,     3,   281,    39,     2,     4,     9,    44,
        39,     9,     6,    44,    38,     6,     3,   290,    44,    39,     9,     5,    44,    38,     6,    45,    43,     9,     7,     5,   356,     9,     9,     4,   308,
        49,     4,    28,    23,    24,    27,     9,     7,    50,     2,    34,    30,     3,   318,    34,    39,     9,     6,     3,   333,    49,     9,    17,    33,    29,
        36,    25,    40,     3,    19,    11,     9,     5,     3,   356,    49,    17,     2,    41,     4,    42,    16,    13,    18,    12,    20,    14,    21,    44,    35,
        31,    37,    26,     6,     9,     5,     5,   361,    29,    44,    24,     5,   371,    46,    22,    11,     2,     9,     5,    44,    22,     5,   387,    44,    39,
         9,     5,    44,    38,     9,     4,     3,   387,    34,    30,     9,     4,     5,   397,    44,    39,     9,     5,    44,    38,     9,     4,     5,   411,     9,
         4,    34,     0,    44,    39,     9,     5,    44,    38,    44,    22,     5,   444,    44,    39,     9,     5,    44,    38,    44,     5,     2,   444,    48,     1,
         3,   436,    34,    33,    44,    24,     9,     4,    44,     1,     8,    34,    37,    24,    44,    24,     9,     4,     7,     5,   468,    44,    39,     9,    12,
         2,   458,    48,    22,     9,     5,    44,    22,     2,   466,    48,    38,     9,     5,    44,    38,     9,     4,     5,   488,    44,    39,     9,     5,    34,
        20,     9,     5,     3,   484,    44,    24,     9,     5,    44,    38,     9,     4,     5,   503,    29,     3,   497,    34,    39,     9,     6,     3,   503,    44,
        17,     9,     5,     5,   527,    40,     9,    12,    14,    13,    18,    28,    10,     8,     2,    35,     3,   525,    34,     3,     9,    19,    44,    22,     6,
         9,     3,     5,   556,    34,    15,    29,     3,   542,    44,    24,     2,   542,     9,     6,    47,    32,    44,     5,     2,   556,     9,    20,     3,   555,
        44,     1,    44,    22,     8,     7,     5,   580,    11,     2,    11,    21,    11,     3,     3,   580,    34,    28,    34,    40,    44,     5,     4,     0,     9,
        22,     1,    44,     1,     6,     5,   587,    11,    29,    16,     5,    33,     5,   603,    41,     5,     1,    19,    31,    27,    36,     3,   602,    34,     7,
        29,     6,    29,     5,   612,     9,    23,     4,     0,    44,    37,     1,     5,   615,    24,     5,   618,    24,     5,   639,     2,   624,    47,    37,    29,
         4,   633,    44,     8,     9,    25,    44,     7,     3,   639,    44,    17,     9,    25,     5,   657,    35,     8,    35,    35,     9,    23,     9,    27,     4,
       655,    44,    32,     9,    27,    44,    22,     5,   693,    35,     8,    35,    35,     9,    24,    29,    44,    39,     3,   683,     9,    24,    29,     2,   683,
         3,   683,    44,    32,     9,    24,    29,     7,    44,    38,    46,    22,    44,     5,     9,    26,    44,     1,     5,   700,     3,   700,     9,    24,    30,
         5,   719,    44,    39,    44,    37,     2,   711,    33,     9,    31,    44,    38,    44,    39,    10,    30,    44,    38,     5,   727,     9,    24,     9,    31,
        44,    22,     5,   731,    11,    32,     5 };

#else

#include "accmpldefs.h"


/************* AC-PROCs PROTOTYPE *************/
static int __exec_statement_block(p_accmpl_ cmplhndp);
static int __exec_variable_declaration(p_accmpl_ cmplhndp);
static int __exec_function_definition(p_accmpl_ cmplhndp);
static int __exec_block_statement(p_accmpl_ cmplhndp);
static int __exec_expression(p_accmpl_ cmplhndp);
static int __exec_type_name(p_accmpl_ cmplhndp);
static int __exec_variable_name(p_accmpl_ cmplhndp);
static int __exec_lamda_definition(p_accmpl_ cmplhndp);
static int __exec_variable_expression(p_accmpl_ cmplhndp);
static int __exec_unit_expression(p_accmpl_ cmplhndp);
static int __exec_label_definition(p_accmpl_ cmplhndp);
static int __exec_expression_statement(p_accmpl_ cmplhndp);
static int __exec_keyword_if(p_accmpl_ cmplhndp);
static int __exec_keyword_while(p_accmpl_ cmplhndp);
static int __exec_keyword_do(p_accmpl_ cmplhndp);
static int __exec_keyword_switch(p_accmpl_ cmplhndp);
static int __exec_keyword_for(p_accmpl_ cmplhndp);
static int __exec_keyword_foreach(p_accmpl_ cmplhndp);
static int __exec_variable_definition(p_accmpl_ cmplhndp);
static int __exec_object_member(p_accmpl_ cmplhndp);
static int __exec_object_definition(p_accmpl_ cmplhndp);
static int __exec_main(p_accmpl_ cmplhndp);
static int __exec_c_code_block(p_accmpl_ cmplhndp);
static int __exec_base_type(p_accmpl_ cmplhndp);
static int __exec_data_type(p_accmpl_ cmplhndp);
static int __exec_c_expressions(p_accmpl_ cmplhndp);
static int __exec_c_codeblock(p_accmpl_ cmplhndp);
static int __exec_c_variable_dec(p_accmpl_ cmplhndp);
static int __exec_c_declaration(p_accmpl_ cmplhndp);
static int __exec_c_function(p_accmpl_ cmplhndp);
static int __exec_sig_fct_args(p_accmpl_ cmplhndp);
static int __exec_fct_ptr_id(p_accmpl_ cmplhndp);
static int __exec_function_ptr(p_accmpl_ cmplhndp);
static int __exec_keyword_typedef(p_accmpl_ cmplhndp);

/************* AC-PROCs DEFINITION ************/
static int __exec_statement_block(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "statement_block", AC_TYPE_PROC);


    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_declaration(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "variable_declaration", AC_TYPE_PROC);

    __ac_exec_step(KEYWORD("local", "global")
               AND EXECPROC(variable_definition))

    if (__ac_check_step(SYMBOL(','))) { 
        __ac_exec_step(EXECPROC(variable_definition))

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_function_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "function_definition", AC_TYPE_PROC);

    __ac_exec_step(KEYWORD("function")
               AND IDENT
               AND SYMBOL('('))

    if (__ac_check_step(IDENT)) { 

        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {

            if (__ac_check_step(SYMBOL(','))) { 
                __ac_exec_step(IDENT)
                continue;
            }
            break;
        }
        __ac_exec_step(END_PROCSEQ);
    }
    __ac_exec_step(SYMBOL(')'))
    if (__ac_check_step(SYMBOL(';'))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(SYMBOL('{'))

    while (__ac_check_step(EXECPROC(block_statement))) { 
        ;
    }
    __ac_exec_step(SYMBOL('}'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_block_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "block_statement", AC_TYPE_PROC);

    if (__ac_check_step(EXECPROC(label_definition)
                     OR EXECKEYWORD(KEY(if), KEY(while), KEY(do), KEY(switch), KEY(for), KEY(foreach)))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    if (__ac_check_step(KEYWORD("break", "continue"))) { 
        __ac_exec_step(SYMBOL(';'))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(KEYWORD("goto"))) { 
        __ac_exec_step(IDENT
                   AND SYMBOL(';'))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(KEYWORD("return"))) { 
        __ac_try_step(EXECPROC(expression)) 
        __ac_exec_step(SYMBOL(';'))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(SYMBOL('{'))) { 

        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL('}'))) { 
                break;
            }

            __ac_exec_step(EXECPROC(block_statement))
        }
        __ac_exec_step(END_PROCSEQ);

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_exec_step(EXECPROC(expression_statement))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "expression", AC_TYPE_PROC);


    if (__ac_check_step(TOKEN("++", "--"))) { 
        __ac_exec_step(EXECPROC(expression))

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_try_step(TOKEN("!", "-", "+", "&", "*", "~")) 
    __ac_exec_step(EXECPROC(unit_expression))

    if (__ac_check_step(KEYWORD("is", "extend"))) { 
        __ac_exec_step(EXECPROC(type_name))
    }

    if (__ac_check_step(SYMBOL('?'))) { 
        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(':')
                   AND EXECPROC(expression))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_type_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "type_name", AC_TYPE_PROC);

    if (__ac_check_step(KEYWORD("void", "int", "double", "float", "char"))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(IDENT)

    while (__ac_check_step(TOKEN("::"))) { 
        __ac_exec_step(IDENT)
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_name(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "variable_name", AC_TYPE_PROC);

    __ac_exec_step(IDENT)

    if (__ac_check_step(SYMBOL('('))) { 
        if (__ac_check_step(SYMBOL(')'))) {
            return(__ac_end_proc(cmplhndp, &procp));
        }


        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            __ac_exec_step(EXECPROC(expression))
            if (__ac_check_step(SYMBOL(','))) { 
                continue;
            }

            break;
        }
        __ac_exec_step(END_PROCSEQ);
        __ac_exec_step(SYMBOL(')'))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_lamda_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "lamda_definition", AC_TYPE_PROC);


    if (__ac_check_step(SYMBOL('('))) { 

        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            if (__ac_check_step(SYMBOL(')'))) { 
                break;
            }


            __ac_exec_step(BEG_PROCSEQ);
            while (__ac_pocess_next(cmplhndp)) {
                __ac_exec_step(IDENT)
                if (__ac_check_step(SYMBOL(')'))) { 
                    break;
                }

                __ac_exec_step(SYMBOL(','))
            }
            __ac_exec_step(END_PROCSEQ);
            break;
        }
        __ac_exec_step(END_PROCSEQ);
        __ac_exec_step(SYMBOL(')'))

        if (__ac_check_step(SYMBOL('{'))) { 
            __ac_exec_step(EXECPROC(block_statement)
                       AND SYMBOL('}'))

            return(__ac_end_proc(cmplhndp, &procp));
        }
        __ac_exec_step(TOKEN("=>")
                   AND SYMBOL('(')
                   AND EXECPROC(expression)
                   AND SYMBOL(')'))

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_exec_step(EXECPROC(type_name))

    while (__ac_check_step(SYMBOL('.'))) { 
        __ac_exec_step(EXECPROC(variable_name))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "variable_expression", AC_TYPE_PROC);

    if (__ac_check_step(LITERAL)) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    if (__ac_check_step(SYMBOL('@'))) { 
        __ac_exec_step(EXECPROC(lamda_definition))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(KEYWORD("typeof", "sizeof"))) { 
        __ac_exec_step(SYMBOL('(')
                   AND EXECPROC(type_name)
                   AND SYMBOL(')'))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    if (__ac_check_step(SYMBOL('('))) { 
        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(')'))

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_try_step(SYMBOL('$')) 
    __ac_exec_step(EXECPROC(variable_name))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_unit_expression(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "unit_expression", AC_TYPE_PROC);

    __ac_exec_step(EXECPROC(variable_expression))

    while (__ac_check_step(TOKEN("->", "::", ":", "."))) { 
        __ac_exec_step(EXECPROC(variable_name))
    }
    __ac_try_step(TOKEN("++", "--")) 

    if (__ac_check_step(KEYWORD("as"))) { 
        __ac_exec_step(EXECPROC(type_name))
    }

    if (__ac_check_step(TOKEN("=", "+=", "-=", "*=", "/=", "&=", "|=", "<<=", ">>="))) { 
        __ac_exec_step(EXECPROC(expression))
    }

    if (__ac_check_step(TOKEN("||", "&&", "|", "&", "==", ">=", "<=", ">>", "<<", ">", "<", "!=", "+", "-", "*", "/", "^"))) { 
        __ac_exec_step(EXECPROC(expression))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_label_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "label_definition", AC_TYPE_PROC);

    __ac_exec_step(IDENT
               AND SYMBOL(':'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_expression_statement(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "expression_statement", AC_TYPE_PROC);

    if (__ac_check_step(SYMBOL(';')
                     OR EXECPROC(variable_declaration))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(EXECPROC(expression)
               AND SYMBOL(';'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_if(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "if", AC_TYPE_KEYWORD);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression)
               AND SYMBOL(')')
               AND EXECPROC(block_statement))

    if (__ac_check_step(KEYWORD("else"))) { 
        __ac_exec_step(EXECPROC(block_statement))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_while(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "while", AC_TYPE_KEYWORD);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression)
               AND SYMBOL(')')
               AND EXECPROC(block_statement))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_do(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "do", AC_TYPE_KEYWORD);

    __ac_exec_step(EXECPROC(block_statement)
               AND KEYWORD("while")
               AND SYMBOL('(')
               AND EXECPROC(expression)
               AND SYMBOL(')')
               AND SYMBOL(';'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_switch(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "switch", AC_TYPE_KEYWORD);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression)
               AND SYMBOL(')')
               AND SYMBOL('{'))

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL('}'))) { 
            break;
        }


        if (__ac_check_step(KEYWORD("default"))) { 
            __ac_exec_step(SYMBOL(':')
                       AND EXECPROC(block_statement)
                       AND SYMBOL('}'))
            break;
        }
        __ac_exec_step(KEYWORD("case")
                   AND LITERAL
                   AND SYMBOL(':')
                   AND EXECPROC(block_statement))
    }
    __ac_exec_step(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_for(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "for", AC_TYPE_KEYWORD);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression_statement))

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL(';'))) { 
            break;
        }

        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(';'))
        break;
    }
    __ac_exec_step(END_PROCSEQ);

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL(')'))) { 
            break;
        }

        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(')'))
        break;
    }
    __ac_exec_step(END_PROCSEQ);
    __ac_exec_step(EXECPROC(block_statement))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_foreach(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "foreach", AC_TYPE_KEYWORD);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression)
               AND KEYWORD("in")
               AND EXECPROC(expression))

    if (__ac_check_step(SYMBOL(':'))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    __ac_exec_step(SYMBOL(')')
               AND EXECPROC(block_statement))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_variable_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "variable_definition", AC_TYPE_PROC);

    __ac_exec_step(IDENT)

    if (__ac_check_step(KEYWORD("as"))) { 
        __ac_exec_step(EXECPROC(type_name))
    }

    if (__ac_check_step(SYMBOL('='))) { 
        __ac_exec_step(EXECPROC(expression))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_object_member(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "object_member", AC_TYPE_PROC);

    __ac_try_step(KEYWORD("public", "private", "protected", "internal", "extern", "sealed", "static", "virtual", "const")) 

    if (__ac_check_step(KEYWORD("var"))) { 
        __ac_exec_step(EXECPROC(variable_definition)
                   AND SYMBOL(';'))

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_exec_step(EXECPROC(function_definition))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_object_definition(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "object_definition", AC_TYPE_PROC);

    __ac_exec_step(KEYWORD("object")
               AND IDENT)

    if (__ac_check_step(SYMBOL(':'))) { 

        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {
            __ac_exec_step(EXECPROC(type_name))
            if (__ac_check_step(SYMBOL(','))) { 
                continue;
            }

            break;
        }
        __ac_exec_step(END_PROCSEQ);
    }
    __ac_exec_step(SYMBOL('{'))

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        __ac_exec_step(EXECPROC(object_member))

        if (__ac_check_step(SYMBOL('}'))) { 
            __ac_exec_step(SYMBOL(';'))
            break;
        }
    }
    __ac_exec_step(END_PROCSEQ);

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_main(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "main", AC_TYPE_MAIN);

    if (__ac_check_step(EXECPROC(variable_declaration)
                     OR EXECPROC(object_definition)
                     OR EXECPROC(function_definition))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    if (__ac_check_step(KEYWORD("extern"))) { 
        __ac_exec_step(KEYWORD("C_CODE")
                   AND SYMBOL('{'))

        while (__ac_check_step(EXECPROC(c_code_block))) { 
            ;
        }
        __ac_exec_step(SYMBOL('}'))

        return(__ac_end_proc(cmplhndp, &procp));
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_code_block(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_code_block", AC_TYPE_PROC);

    if (__ac_check_step(EXECPROC(c_function)
                     OR EXECKEYWORD(KEY(typedef)))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_base_type(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "base_type", AC_TYPE_PROC);

    if (__ac_check_step(KEYWORD("void", "int", "double", "float", "char"))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    if (__ac_check_step(KEYWORD("struct"))) { 
        __ac_exec_step(IDENT)

        return(__ac_end_proc(cmplhndp, &procp));
    }
    __ac_exec_step(IDENT)

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_data_type(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "data_type", AC_TYPE_PROC);

    __ac_exec_step(EXECPROC(base_type))

    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_expressions(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_expressions", AC_TYPE_PROC);

    __ac_exec_step(LITERAL)

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_codeblock(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_codeblock", AC_TYPE_PROC);

    __ac_exec_step(LITERAL)

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_variable_dec(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_variable_dec", AC_TYPE_PROC);


    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(SYMBOL('*'))) { 
            continue;
        }

        break;
    }
    __ac_exec_step(END_PROCSEQ);
    __ac_exec_step(IDENT)

    while (__ac_check_step(SYMBOL('['))) { 
        __ac_exec_step(EXECPROC(c_expressions)
                   AND SYMBOL(']'))
    }

    if (__ac_check_step(SYMBOL('='))) { 
        __ac_exec_step(EXECPROC(c_expressions))
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_declaration(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_declaration", AC_TYPE_PROC);

    __ac_try_step(KEYWORD("static")) 
    __ac_try_step(KEYWORD("const")) 
    __ac_exec_step(EXECPROC(base_type)
               AND EXECPROC(c_variable_dec))

    while (__ac_check_step(SYMBOL(','))) { 
        __ac_exec_step(EXECPROC(c_variable_dec))
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_c_function(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "c_function", AC_TYPE_PROC);

    __ac_try_step(KEYWORD("static")) 
    __ac_try_step(KEYWORD("const")) 
    __ac_exec_step(EXECPROC(data_type)
               AND IDENT
               AND SYMBOL('('))

    if (__ac_check_step(EXECPROC(data_type))) { 
        __ac_exec_step(IDENT)

        __ac_exec_step(BEG_PROCSEQ);
        while (__ac_pocess_next(cmplhndp)) {

            if (__ac_check_step(SYMBOL(','))) { 
                __ac_exec_step(EXECPROC(data_type)
                           AND IDENT)
                continue;
            }
            break;
        }
        __ac_exec_step(END_PROCSEQ);
    }
    __ac_exec_step(SYMBOL(')'))
    if (__ac_check_step(SYMBOL(';'))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }

    __ac_exec_step(SYMBOL('{')
               AND EXECPROC(c_codeblock)
               AND SYMBOL('}'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_sig_fct_args(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "sig_fct_args", AC_TYPE_PROC);


    if (__ac_check_step(EXECPROC(data_type))) { 
        __ac_try_step(IDENT) 
    }

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_fct_ptr_id(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "fct_ptr_id", AC_TYPE_PROC);

    __ac_exec_step(SYMBOL('(')
               AND SYMBOL('*'))

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(IDENT)) { 
            break;
        }

        __ac_exec_step(EXECPROC(fct_ptr_id))
        break;
    }
    __ac_exec_step(END_PROCSEQ);
    __ac_exec_step(SYMBOL(')')
               AND SYMBOL('('))
    __ac_try_step(EXECPROC(sig_fct_args)) 
    __ac_exec_step(SYMBOL(')'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_function_ptr(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "function_ptr", AC_TYPE_PROC);

    __ac_exec_step(EXECPROC(data_type)
               AND EXECPROC(fct_ptr_id)
               AND SYMBOL(';'))

    return(__ac_end_proc(cmplhndp, &procp));
}

static int __exec_keyword_typedef(p_accmpl_ cmplhndp)
{
    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, "typedef", AC_TYPE_KEYWORD);

    if (__ac_check_step(EXECPROC(function_ptr))) {
        return(__ac_end_proc(cmplhndp, &procp));
    }


    return(__ac_end_proc(cmplhndp, &procp));
}
#endif

/*********** MAIN COMPILER FUNCTION ***********/
int __accmpl_exec_module(p_accmpl_ cmplhndp)
{
    if (cmplhndp == NULLP) cmplhndp = __ac_new_compiler();

    __ac_compl_set_keywords(cmplhndp, __keywords, sizeof(__keywords)/sizeof(char*));
    __ac_compl_set_tokens(cmplhndp, __tokens, sizeof(__tokens)/sizeof(char*));


#ifndef AC_DEBUG
    __ac_compl_exec(cmplhndp, __prcex, sizeof(__prcex)/sizeof(int));
#else
    __ac_compl_exec_mainproc(cmplhndp, __exec_main);
#endif

    return(__ac_exit_compiler(&cmplhndp));
}
