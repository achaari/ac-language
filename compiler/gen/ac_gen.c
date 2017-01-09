

#include "acstd.h"
#include "accmplpub.h"

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
       542,    22,    24,    41,    71,   129,   168,   175,   192,   241,   277,   342,   347,   357,   373,   383,   397,   430,   454,   474,   489,   513,    23,     3,    40,
        37,     2,    13,    19,     7,    19,     2,    38,    42,    30,     7,    19,     4,    42,    20,     3,    70,    32,    20,    27,    42,    37,     2,    58,    27,
         1,    58,     2,    58,    42,    30,    27,     5,    42,    36,    44,    20,    42,     5,     1,    68,    10,     4,    42,     1,     3,   128,     9,    11,    19,
         6,    17,    13,     0,    14,    26,    15,     4,    16,    22,    17,    21,    18,     2,    97,    37,     2,    31,    28,    42,    20,     4,     2,   105,    32,
        18,    27,    42,    20,     4,     2,   114,    32,     8,     8,     5,    42,    20,     4,     2,   126,    42,     5,     1,   125,    46,     1,     7,     4,     5,
         4,     7,    12,     3,   167,     2,   139,    47,     2,    32,    28,     7,     5,     4,    48,     6,    43,    29,    33,    40,    35,     0,     7,    10,     2,
       157,    37,     2,    14,    24,     7,     6,     2,   167,    42,     8,     7,     5,    42,    22,     7,     5,     3,   174,     1,   174,    27,    45,    21,     3,
       191,    27,     2,   191,    42,    37,    44,    36,     1,   189,     7,     5,    45,    30,    42,    36,     3,   240,     2,   229,    42,    37,     1,   209,    46,
        36,     1,   209,    27,    46,    36,    42,    30,     5,    42,    36,     2,   220,    42,     5,     7,     4,    42,     1,     4,    42,    13,    42,    37,     7,
         5,    42,    36,     4,     7,     6,     1,   240,     2,   240,    42,    25,     7,     7,     5,     3,   276,    24,     2,   250,    42,     7,     7,     8,     4,
         2,   263,    37,     2,     3,     6,    42,    37,     7,     6,    42,    36,     4,     2,   272,    42,    37,     7,     5,    42,    36,     4,    43,    41,     7,
         7,     3,   341,     7,     9,     1,   293,     2,   293,    47,     4,    26,    21,    22,    25,     7,     7,     5,    48,     2,    32,    28,     2,   303,    32,
        32,     7,     6,     2,   318,    47,     9,    15,    31,    27,    34,    23,    38,     3,    17,     9,     7,     5,     2,   341,    47,    17,     2,    39,     4,
        40,    14,    11,    16,    10,    18,    12,    19,    42,    33,    29,    35,    24,     6,     7,     5,     3,   346,    27,    42,    22,     3,   356,    44,    20,
         9,     2,     7,     5,    42,    20,     3,   372,    42,    37,     7,     5,    42,    36,     7,     4,     2,   372,    32,    25,     7,     4,     3,   382,    42,
        37,     7,     5,    42,    36,     7,     4,     3,   396,     7,     4,    32,     0,    42,    37,     7,     5,    42,    36,    42,    20,     3,   429,    42,    37,
         7,     5,    42,    36,    42,     5,     1,   429,    46,     1,     2,   421,    32,    27,    42,    22,     7,     4,    42,     1,     6,    32,    30,    22,    42,
        22,     7,     4,     5,     3,   453,    42,    37,     7,    12,     1,   443,    46,    20,     7,     5,    42,    20,     1,   451,    46,    36,     7,     5,    42,
        36,     7,     4,     3,   473,    42,    37,     7,     5,    32,    16,     7,     5,     2,   469,    42,    22,     7,     5,    42,    36,     7,     4,     3,   488,
        27,     2,   482,    32,    32,     7,     6,     2,   488,    42,    15,     7,     5,     3,   512,    38,     9,     9,    11,    10,    15,    23,     7,     5,     1,
        29,     2,   510,    32,     2,     7,    19,    42,    20,     4,     7,     3,     3,   541,    32,    12,    27,     2,   527,    42,    22,     1,   527,     7,     6,
        45,    30,    42,     5,     1,   541,     7,    20,     2,   540,    42,     1,    42,    20,     6,     5,     3,   549,     9,     2,     9,    21,     9,     3,     3 };

    
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

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        if (__ac_check_step(EXECPROC(block_statement))) { 
            continue;
        }

        break;
    }
    __ac_exec_step(END_PROCSEQ);
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


    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {
        __ac_exec_step(IDENT)
        if (__ac_check_step(TOKEN("::"))) { 
            continue;
        }

        break;
    }
    __ac_exec_step(END_PROCSEQ);

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

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {

        if (__ac_check_step(SYMBOL('.'))) { 
            __ac_exec_step(EXECPROC(variable_name))
            continue;
        }
        break;
    }
    __ac_exec_step(END_PROCSEQ);

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

    __ac_exec_step(BEG_PROCSEQ);
    while (__ac_pocess_next(cmplhndp)) {

        if (__ac_check_step(TOKEN("->", "::", ":", "."))) { 
            __ac_exec_step(EXECPROC(variable_name))
            continue;
        }
        break;
    }
    __ac_exec_step(END_PROCSEQ);
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
                   AND SYMBOL(')')
                   AND EXECPROC(block_statement))
        break;
    }
    __ac_exec_step(END_PROCSEQ);

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
        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(')')
                   AND EXECPROC(block_statement))
    }

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
