

#include "acstd.h"
#include "accmplpub.h"

static const char * __keywords[] = {
                 "void",            "union",          "typedef",           "struct",              "int",            "float",           "double",           "define",             "char" };

    

static const char * __tokens[] = {
         "}",     "{",     "]",     "[",     ";",     ",",     "*",     "#" };


#ifndef AC_DEBUG
static const int __prcex[] = {
        88,     5,    15,    34,    66,    14,    41,     5,     0,     4,     6,     5,     8,    29,     5,    33,     9,     3,     4,    23,    44,     6,     1,    29,     4,
        31,    44,     3,    24,    44,     2,    44,     4,     5,    65,    11,     1,    39,     2,     3,     1,     3,    53,    44,     1,     4,    50,     9,     2,     1,
        44,     0,     6,    29,     3,    65,    44,     1,     4,    63,     9,     2,     1,    44,     0,     5,    87,     9,     3,     4,    74,    44,     6,     1,    29,
         4,    85,    44,     5,     4,    84,    44,     6,     1,    29,    44,     4,     5,   101,     3,    98,    44,     7,    34,     7,    29,    24,     6,    16,     2,
         4,     5 };

#else

#include "accmpldefs.h"


/************* AC-PROCs PROTOTYPE *************/
static int __exec_base_type_dec(p_accmpl_ cmplhndp);
static int __exec_struct_member(p_accmpl_ cmplhndp);
static int __exec_data_type_defs(p_accmpl_ cmplhndp);
static int __exec_keyword_typedef(p_accmpl_ cmplhndp);
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

static int __exec_struct_member(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "struct_member")) return(FALSE);

    __ac_exec_step(EXECPROC(data_type_defs))

    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }
    __ac_exec_step(IDENT)

    while (__ac_check_step(SYMBOL('['))) { 
        __ac_exec_step(LITERAL
                   AND SYMBOL(']'))
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_data_type_defs(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "data_type_defs")) return(FALSE);

    if (__ac_check_step(EXECPROC(base_type_dec))) {
        return(__ac_end_proc(cmplhndp));
    }

    __ac_exec_step(KEYWORD("struct", "union"))

    if (__ac_check_step(SYMBOL('{'))) { 

        while (__ac_check_step(EXECPROC(struct_member))) { 
            ;
        }
        __ac_exec_step(SYMBOL('}'))

        return(__ac_end_proc(cmplhndp));
    }
    __ac_exec_step(IDENT)

    if (__ac_check_step(SYMBOL('{'))) { 

        while (__ac_check_step(EXECPROC(struct_member))) { 
            ;
        }
        __ac_exec_step(SYMBOL('}'))
    }

    return(__ac_end_proc(cmplhndp));
}

static int __exec_keyword_typedef(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "typedef")) return(FALSE);

    __ac_exec_step(EXECPROC(data_type_defs))

    while (__ac_check_step(SYMBOL('*'))) { 
        ;
    }
    __ac_exec_step(IDENT)

    while (__ac_check_step(SYMBOL(','))) { 

        while (__ac_check_step(SYMBOL('*'))) { 
            ;
        }
        __ac_exec_step(IDENT)
    }
    __ac_exec_step(SYMBOL(';'))

    return(__ac_end_proc(cmplhndp));
}

static int __exec_main(p_accmpl_ cmplhndp)
{
    if (!__ac_init_proc(cmplhndp, "main")) return(FALSE);


    if (__ac_check_step(SYMBOL('#'))) { 
        __ac_exec_step(KEYWORD("define")
                   AND IDENT
                   AND LITERAL)

        return(__ac_end_proc(cmplhndp));
    }
    if (__ac_check_step(EXECKEYWORD(KEY(typedef)))) {
        return(__ac_end_proc(cmplhndp));
    }


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
