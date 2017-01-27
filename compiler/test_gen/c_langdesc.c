

#include "acstd.h"
#include "accmplpub.h"

static const char * __keywords[] = {
                "while",             "void",            "union",          "typedef",           "switch",           "struct",           "static",           "sizeof",           "return",
                  "int",          "include",               "if",              "for",            "float",             "else",           "double",           "define",          "default",
             "continue",             "char",             "case",            "break" };


static const char * __tokens[] = {
         "}",    "||",    "|=",     "|",     "{",    "^=",     "^",     "]",     "[",     "?",   ">>=",    ">>",    ">=",     ">",    "==",     "=",    "<=",   "<<=",    "<<",     "<",
         ";",     ":",    "/=",     "/",     ".",    "->",    "--",    "-+",     "-",     ",",    "+=",    "++",     "+",    "*=",     "*",     ")",     "(",    "&=",    "&&",     "&",
        "%=",     "%",     "#",    "!=",     "!" };


#ifndef AC_DEBUG
static const int __prcex[] = {
       582,    20,    30,    38,    53,    67,    81,    95,   132,   242,   250,   262,   275,   283,   317,   332,   347,   354,   365,   558,    29,   111,     5,     1,     9,
        15,    13,    19,    89,     5,    37,     4,    36,   119,    34,     1,    89,     5,    52,     9,     4,     9,     2,     4,    50,   119,     8,    49,   119,     7,
       119,    20,     5,    66,     3,    64,   109,     2,     5,     2,    11,     5,    89,     6,     9,     1,     5,    80,    90,   119,     4,     9,     3,     2,    80,
       123,     0,     9,     3,     7,     5,    94,     9,     4,     9,     2,     4,    92,   119,    29,     9,     2,   119,    20,     5,   131,    89,     4,   105,   119,
         8,    54,     0,   119,     7,     3,   123,   119,    36,     2,   123,   123,    35,     9,     8,     4,   121,   119,    29,     9,     8,   119,    35,     3,   131,
       129,     2,    24,    25,     9,     7,     5,   241,    11,    14,     9,    13,   130,     2,    31,    26,     4,   150,   129,     3,    34,    23,    41,     9,     8,
         4,   158,   129,     2,    32,    28,     9,     8,     4,   166,   129,     2,    18,    11,     9,     8,     4,   176,   129,     4,    13,    19,    16,    12,     9,
         8,     4,   184,   129,     2,    14,    43,     9,     8,     4,   190,   119,    39,     9,     8,     4,   196,   119,     6,     9,     8,     4,   202,   119,     3,
         9,     8,     4,   208,   119,    38,     9,     8,     4,   214,   119,     1,     9,     8,     3,   224,   119,     9,     9,     8,   119,    21,     9,     8,     4,
       241,   129,    11,    15,    30,    27,    33,    22,    37,     2,     5,    40,    17,    10,     9,     8,     5,   249,   130,     2,    34,    39,     9,     7,     5,
       261,     3,   259,   109,     2,     5,     2,    89,     6,     9,     1,     5,   274,   119,    36,     9,    10,     4,   272,   119,    34,     1,   119,    35,     5,
       282,   119,    36,     9,     7,   119,    35,     5,   316,    41,     3,   296,    99,     7,     2,   295,    13,    12,     9,    11,     6,    11,     9,     3,   310,
       129,     5,    44,    32,    28,    31,    26,     9,     8,     6,   119,    36,     9,     8,   119,    35,     5,   331,   119,    36,     9,    10,     4,   327,   119,
        34,     1,   119,    35,     9,     8,     5,   346,     3,   339,    99,    10,    79,     6,     3,   346,    99,    16,    89,    49,     6,     5,   353,     9,    10,
        14,     2,     2,     5,   364,   119,     4,     2,   364,   123,     0,     9,    18,     7,     5,   557,   121,    20,     3,   377,   109,     2,    21,    18,   119,
        20,     6,     3,   388,    99,     8,   121,    20,     9,     8,   119,    20,     6,    11,    17,     3,   403,    99,     0,   119,    36,     9,     8,   119,    35,
         9,    18,     6,     3,   422,    99,    11,   119,    36,     9,     8,   119,    35,     9,    18,     3,   421,    99,    14,     9,    18,     6,     3,   485,    99,
         4,   119,    36,     9,     8,   119,    35,   119,     4,     4,   457,    99,    20,     2,   443,    53,    63,    89,   119,    21,     2,   457,     3,   454,    99,
        21,   119,    20,     8,     9,    18,     7,     3,   482,    99,    17,   119,    21,     9,    18,     2,   481,   123,     0,     3,   478,    99,    21,   119,    20,
       119,     0,     8,     9,    18,     7,     6,   119,     0,     6,     3,   518,    99,    12,   119,    36,     2,   499,   123,    20,     9,     8,   119,    20,     2,
       507,   123,    20,     9,     8,   119,    20,     2,   515,   123,    35,     9,     8,   119,    35,     9,    18,     6,     3,   553,     9,    16,     4,   529,   119,
         8,    49,   119,     7,     2,   550,     3,   537,   119,    15,     9,     8,     3,   550,   119,    29,    89,     4,   549,   119,     8,    49,   119,     7,     7,
       119,    20,     6,     9,     8,   119,    20,     5,   581,   100,     6,     9,    16,   119,    36,     3,   575,     9,    16,     4,   575,   119,    29,     9,    16,
       119,    35,   121,    20,     9,    17,     5,   596,     3,   590,   119,    42,     9,    15,     6,    26,     1,     3,     6,     9,    19,     5 };

#else

#include "accmpldefs.h"


/************* AC-PROCs PROTOTYPE *************/
static int __exec_base_type_dec(p_accmpl_ cmplhndp);
static int __exec_data_name(p_accmpl_ cmplhndp);
static int __exec_struct_one_fld(p_accmpl_ cmplhndp);
static int __exec_data_type_defs(p_accmpl_ cmplhndp);
static int __exec_struct_fields(p_accmpl_ cmplhndp);
static int __exec_keyword_typedef(p_accmpl_ cmplhndp);
static int __exec_variable(p_accmpl_ cmplhndp);
static int __exec_expression(p_accmpl_ cmplhndp);
static int __exec_variable_ref(p_accmpl_ cmplhndp);
static int __exec_variable_type(p_accmpl_ cmplhndp);
static int __exec_sizeof_type(p_accmpl_ cmplhndp);
static int __exec_sizeof_variable(p_accmpl_ cmplhndp);
static int __exec_base_expression(p_accmpl_ cmplhndp);
static int __exec_cast_expression(p_accmpl_ cmplhndp);
static int __exec_preproc(p_accmpl_ cmplhndp);
static int __exec_name_declaration(p_accmpl_ cmplhndp);
static int __exec_statement_block(p_accmpl_ cmplhndp);
static int __exec_one_statement(p_accmpl_ cmplhndp);
static int __exec_function(p_accmpl_ cmplhndp);
static int __exec_main(p_accmpl_ cmplhndp);

/************* AC-PROCs DEFINITION ************/
static int __exec_base_type_dec(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "base_type_dec")) return(FALSE);

    if (__ac_check_step(KEYWORD("void", "int", "double", "float", "char"))) {
        return(__ac_end_proc(cmplhndp));
    }

    __ac_exec_step(IDENT)

    return(__ac_end_proc(cmplhndp));
}

static int __exec_data_name(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "data_name")) return(FALSE);

    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }
    __ac_exec_step(IDENT)

    return(__ac_end_proc(cmplhndp));
}

static int __exec_struct_one_fld(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "struct_one_fld")) return(FALSE);

    __ac_exec_step(EXECPROC(data_type_defs)
               AND EXECPROC(data_name))
    while (__ac_check_step(SYMBOL('['))) { 
        __ac_exec_step(INTEGER
                   AND SYMBOL(']'))
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_data_type_defs(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "data_type_defs")) return(FALSE);

    if (__ac_check_step(KEYWORD("struct", "union"))) { 
        if (__ac_check_step(EXECPROC(struct_fields))) {
            return(__ac_end_proc(cmplhndp));
        }

        __ac_exec_step(IDENT)

        return(__ac_end_proc(cmplhndp));
    }
    __ac_exec_step(EXECPROC(base_type_dec))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_struct_fields(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "struct_fields")) return(FALSE);

    __ac_try_step(IDENT) 
    __ac_exec_step(SYMBOL('{')
               AND EXECPROC(struct_one_fld))

    __procseq_0_beg:
    {
        if (__ac_check_step(SYMBOL('}'))) { 
            goto __procseq_0_end;
        }

        __ac_exec_step(EXECPROC(struct_one_fld))
        goto __procseq_0_beg;
    }
    __procseq_0_end:

    return(__ac_end_proc(cmplhndp));
}

static int __exec_keyword_typedef(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "typedef")) return(FALSE);

    __ac_exec_step(EXECPROC(data_type_defs)
               AND EXECPROC(data_name))
    while (__ac_check_step(SYMBOL(','))) { 
        __ac_exec_step(EXECPROC(data_name))
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_variable(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "variable")) return(FALSE);

    __ac_exec_step(IDENT)
    while (__ac_check_step(SYMBOL('['))) { 
        __ac_exec_step(INTEGER_DATA(&(b))
                   AND SYMBOL(']'))
    }
    if (__ac_check_step(SYMBOL('('))) { 
            if (__ac_check_step(SYMBOL(')'))) { 
                goto __procseq_1_end;
            }

            __ac_exec_step(EXECPROC(expression))
            while (__ac_check_step(SYMBOL(','))) { 
                __ac_exec_step(EXECPROC(expression))
            }
            __ac_exec_step(SYMBOL(')'))
        }
        __procseq_1_end:
    }
    if (__ac_check_step(TOKEN(".", "->"))) { 
        __ac_exec_step(EXECPROC(variable))
    }

    return(__ac_end_proc(cmplhndp));
}

static int __exec_expression(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "expression")) return(FALSE);

    if (__ac_check_step(EXECPROC(cast_expression))) {
        return(__ac_end_proc(cmplhndp));
    }

    __ac_exec_step(EXECPROC(base_expression))
    __ac_try_step(TOKEN("++", "--")) 
    while (__ac_check_step(TOKEN("*", "/", "%"))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("+", "-"))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("<<", ">>"))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN(">", "<", "<=", ">="))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("==", "!="))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(SYMBOL('&'))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(SYMBOL('^'))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(SYMBOL('|'))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("&&"))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("||"))) { 
        __ac_exec_step(EXECPROC(expression))
    }
    if (__ac_check_step(SYMBOL('?'))) { 
        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(':')
                   AND EXECPROC(expression))
    }
    while (__ac_check_step(TOKEN("=", "+=", "-+", "*=", "/=", "&=", "|=", "^=", "%=", "<<=", ">>="))) { 
        __ac_exec_step(EXECPROC(expression))
    }

    return(__ac_end_proc(cmplhndp));
}

static int __exec_variable_ref(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "variable_ref")) return(FALSE);

    __ac_try_step(TOKEN("*", "&")) 
    __ac_exec_step(EXECPROC(variable))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_variable_type(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "variable_type")) return(FALSE);

    if (__ac_check_step(KEYWORD("struct", "union"))) { 
        __ac_exec_step(IDENT)

        return(__ac_end_proc(cmplhndp));
    }
    __ac_exec_step(EXECPROC(base_type_dec))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_sizeof_type(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "sizeof_type")) return(FALSE);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(variable_type))
    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }
    __ac_exec_step(SYMBOL(')'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_sizeof_variable(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "sizeof_variable")) return(FALSE);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(variable)
               AND SYMBOL(')'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_base_expression(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "base_expression")) return(FALSE);

    if (__ac_check_step(LITERAL)) {
        return(__ac_end_proc(cmplhndp));
    }

    if (__ac_check_step(KEYWORD("sizeof"))) { 

        __procseq_2_beg:
        {
            if (__ac_check_step(EXECPROC(sizeof_variable))) { 
                goto __procseq_2_end;
            }

            __ac_exec_step(EXECPROC(sizeof_type))
        }
        __procseq_2_end:

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(EXECPROC(variable_ref))) {
        return(__ac_end_proc(cmplhndp));
    }

    if (__ac_check_step(TOKEN("!", "+", "-", "++", "--"))) { 
        __ac_exec_step(EXECPROC(expression))

        return(__ac_end_proc(cmplhndp));
    }
    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(expression)
               AND SYMBOL(')'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_cast_expression(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "cast_expression")) return(FALSE);

    __ac_exec_step(SYMBOL('(')
               AND EXECPROC(variable_type))
    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }
    __ac_exec_step(SYMBOL(')')
               AND EXECPROC(expression))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_preproc(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "preproc")) return(FALSE);

    if (__ac_check_step(KEYWORD("include"))) { 
        __ac_exec_step(STRING)

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(KEYWORD("define"))) { 
        __ac_exec_step(IDENT
                   AND INTEGER)

        return(__ac_end_proc(cmplhndp));
    }

    return(__ac_end_proc(cmplhndp));
}

static int __exec_name_declaration(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "name_declaration")) return(FALSE);

    __ac_exec_step(EXECPROC(variable_type)
               AND EXECPROC_DATA(&($1->test), data_name))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_statement_block(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "statement_block")) return(FALSE);

    __ac_exec_step(SYMBOL('{'))

    __procseq_3_beg:
    {
        if (__ac_check_step(SYMBOL('}'))) { 
            goto __procseq_3_end;
        }

        __ac_exec_step(EXECPROC(one_statement))
        goto __procseq_3_beg;
    }
    __procseq_3_end:

    return(__ac_end_proc(cmplhndp));
}

static int __exec_one_statement(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "one_statement")) return(FALSE);

    if (__ac_check_step(SYMBOL(';'))) {
        return(__ac_end_proc(cmplhndp));
    }

    if (__ac_check_step(KEYWORD("break", "continue"))) { 
        __ac_exec_step(SYMBOL(';'))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(KEYWORD("return"))) { 
        if (__ac_check_step(SYMBOL(';'))) {
            return(__ac_end_proc(cmplhndp));
        }

        __ac_exec_step(EXECPROC(expression)
                   AND SYMBOL(';'))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(EXECPROC(statement_block))) {
        return(__ac_end_proc(cmplhndp));
    }

    if (__ac_check_step(KEYWORD("while"))) { 
        __ac_exec_step(SYMBOL('(')
                   AND EXECPROC(expression)
                   AND SYMBOL(')')
                   AND EXECPROC(one_statement))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(KEYWORD("if"))) { 
        __ac_exec_step(SYMBOL('(')
                   AND EXECPROC(expression)
                   AND SYMBOL(')')
                   AND EXECPROC(one_statement))
        if (__ac_check_step(KEYWORD("else"))) { 
            __ac_exec_step(EXECPROC(one_statement))
        }

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(KEYWORD("switch"))) { 
        __ac_exec_step(SYMBOL('(')
                   AND EXECPROC(expression)
                   AND SYMBOL(')')
                   AND SYMBOL('{'))
        while (__ac_check_step(KEYWORD("case"))) { 
                if (__ac_check_step(INTEGER)) { 
                    goto __procseq_4_end;
                }

                if (__ac_check_step(CHAR)) { 
                    goto __procseq_4_end;
                }

                __ac_exec_step(IDENT)
            }
            __procseq_4_end:
            __ac_exec_step(SYMBOL(':'))

            __procseq_5_beg:
            {
                if (__ac_check_step(KEYWORD("break"))) { 
                    __ac_exec_step(SYMBOL(';'))
                    goto __procseq_5_end;
                }
                __ac_exec_step(EXECPROC(one_statement))
                goto __procseq_5_beg;
            }
            __procseq_5_end:
        }
        if (__ac_check_step(KEYWORD("default"))) { 
            __ac_exec_step(SYMBOL(':')
                       AND EXECPROC(one_statement))

            __procseq_6_beg:
            {
                if (__ac_check_step(SYMBOL('}'))) { 
                    goto __procseq_6_end;
                }

                if (__ac_check_step(KEYWORD("break"))) { 
                    __ac_exec_step(SYMBOL(';')
                               AND SYMBOL('}'))
                    goto __procseq_6_end;
                }
                __ac_exec_step(EXECPROC(one_statement))
                goto __procseq_6_beg;
            }
            __procseq_6_end:

            return(__ac_end_proc(cmplhndp));
        }
        __ac_exec_step(SYMBOL('}'))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(KEYWORD("for"))) { 
        __ac_exec_step(SYMBOL('('))
            if (__ac_check_step(SYMBOL(';'))) { 
                goto __procseq_7_end;
            }

            __ac_exec_step(EXECPROC(expression)
                       AND SYMBOL(';'))
        }
        __procseq_7_end:
            if (__ac_check_step(SYMBOL(';'))) { 
                goto __procseq_8_end;
            }

            __ac_exec_step(EXECPROC(expression)
                       AND SYMBOL(';'))
        }
        __procseq_8_end:
            if (__ac_check_step(SYMBOL(')'))) { 
                goto __procseq_9_end;
            }

            __ac_exec_step(EXECPROC(expression)
                       AND SYMBOL(')'))
        }
        __procseq_9_end:
        __ac_exec_step(EXECPROC(one_statement))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(EXECPROC(name_declaration))) { 
        while (__ac_check_step(SYMBOL('['))) { 
            __ac_exec_step(INTEGER
                       AND SYMBOL(']'))
        }

        __procseq_10_beg:
        {
            if (__ac_check_step(SYMBOL('='))) { 
                __ac_exec_step(EXECPROC(expression))
            }
            if (__ac_check_step(SYMBOL(','))) { 
                __ac_exec_step(IDENT)
                while (__ac_check_step(SYMBOL('['))) { 
                    __ac_exec_step(INTEGER
                               AND SYMBOL(']'))
                }
                goto __procseq_10_beg;
            }
        }
        __procseq_10_end:
        __ac_exec_step(SYMBOL(';'))

        return(__ac_end_proc(cmplhndp));
    }
    __ac_exec_step(EXECPROC(expression)
               AND SYMBOL(';'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_function(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "function")) return(FALSE);

    __ac_try_step(KEYWORD("static")) 
    __ac_exec_step(EXECPROC(name_declaration)
               AND SYMBOL('('))
    if (__ac_check_step(EXECPROC(name_declaration))) { 
        while (__ac_check_step(SYMBOL(','))) { 
            __ac_exec_step(EXECPROC(name_declaration))
        }
    }
    __ac_exec_step(SYMBOL(')'))
    if (__ac_check_step(SYMBOL(';'))) {
        return(__ac_end_proc(cmplhndp));
    }

    __ac_exec_step(EXECPROC(statement_block))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_main(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "main")) return(FALSE);

    if (__ac_check_step(SYMBOL('#'))) { 
        __ac_exec_step(EXECPROC(preproc))

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(EXECKEYWORD_DATA(&(a), KEY(typedef)))) {
        return(__ac_end_proc(cmplhndp));
    }

    __ac_exec_step(EXECPROC(function))

    return(__ac_end_proc(cmplhndp));
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
