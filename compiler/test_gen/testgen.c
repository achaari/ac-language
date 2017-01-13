

#include "acstd.h"
#include "accmplpub.h"

static const char * __keywords[] = {
                 "void",            "union",          "typedef",           "struct",              "int",            "float",           "double",           "define",             "char" };

    

static const char * __tokens[] = {
         "}",     "{",     "]",     "[",     ";",     ",",     "*",     "#" };


#ifndef AC_DEBUG
static const int __prcex[] = {
        82,     7,    17,    25,    40,    54,    68,    16,    41,     5,     0,     4,     6,     5,     8,    29,     5,    24,     4,    23,    44,     6,     1,    29,     5,
        39,     9,     4,     9,     2,     4,    37,    44,     3,    24,    44,     2,    44,     4,     5,    53,     3,    51,    39,     2,     3,     1,    11,     5,    29,
         6,     9,     1,     5,    67,    30,    44,     1,     9,     3,     4,    65,     9,     3,     1,    44,     0,     5,    81,     9,     4,     9,     2,     4,    79,
        44,     5,     9,     2,    44,     4,     5,    95,     3,    92,    44,     7,    34,     7,    29,    24,     6,    16,     2,     6,     5 };

#else

#include "accmpldefs.h"


/************* AC-PROCs PROTOTYPE *************/
static int __exec_base_type_dec(p_accmpl_ cmplhndp);
static int __exec_data_name(p_accmpl_ cmplhndp);
static int __exec_struct_one_fld(p_accmpl_ cmplhndp);
static int __exec_data_type_defs(p_accmpl_ cmplhndp);
static int __exec_struct_fields(p_accmpl_ cmplhndp);
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
        __ac_exec_step(LITERAL
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
    while (__ac_check_step(EXECPROC(struct_one_fld))) { 
        ;
    }
    __ac_exec_step(SYMBOL('}'))

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
