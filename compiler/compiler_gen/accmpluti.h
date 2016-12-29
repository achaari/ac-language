
#ifndef AC_CMPL_UTI_H
#define AC_CMPL_UTI_H

#include "acstd.h"
#include "aclexer.h"

#define MAX_LEN 100

#ifndef AC_MANAGE_ERROR
#define ac_error(a, ...)
#define ac_warning(a, ...)
#endif

typedef enum {
    PROC_TYPE_PROC = 0,
    PROC_TYPE_MAIN
} e_proc_type_;

#define DEF_STEP(a) STEP_TYPE_##a, STEP_TYPE_ACCEPT_IF_##a,  STEP_TYPE_RECALL_IF_##a, STEP_TYPE_BREAK_IF_##a

typedef enum {
    STEP_TYPE_NAN = 0,
    STEP_TYPE_PROCSEQ,
    STEP_TYPE_PROC_ACCEPT,
    STEP_TYPE_PROCSEQ_RECALL,
    STEP_TYPE_PROCSEQ_BREAK,
    STEP_TYPE_OPTSEQ,
    DEF_STEP(EXEC_PROC),
    DEF_STEP(LITERAL),
    DEF_STEP(KEYWORD),
    DEF_STEP(MULTI_KEYWORD),
    DEF_STEP(GETIDENT),
    DEF_STEP(STRCODE),   
    DEF_STEP(MULTI_STRCODE),
    DEF_STEP(SYMBOL),
    DEF_STEP(MULTI_SYMBOL)
} e_step_type_;

typedef struct str_list_ {
    char *codes;
    int indexl;
    struct str_list_ *nextp;
} ac_str_list_, *pac_str_list_;

typedef struct step_ {
    e_step_type_ type;
    union {
    struct proc_  *procp;
        pac_str_list_ strlistp;
        char idents[MAX_LEN];
        char *codes;
        char chr;
    } datap;
    struct step_ *rootp;
    struct step_ *headp;
    struct step_ *childp;
    struct step_ *nextp;
} ac_step_, *pac_step_;

typedef struct proc_{
    char         names[MAX_LEN];
    e_proc_type_ type;
    int      index;
    int      initpos;
    int      isdefb;
    pac_step_    steplist;
    struct proc_ *nextp;
} ac_proc_, *pac_proc_;

typedef struct cmplgen_ {
    char      *module_name;
    pac_proc_     proc_listp;
    pac_str_list_ keyword_listp;
    pac_str_list_ token_listp;
} ac_cmplgen_, *pac_cmplgen_;

void ac_print_proc(pac_cmplgen_ proc_listp, FILE *outputp);

void ac_print_compiler(pac_cmplgen_ proc_listp, FILE *outputp, int procount);

#endif
