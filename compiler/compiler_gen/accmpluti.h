
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
    STEP_FLAG_NON    = 0,
    STEP_FLAG_BREAK,
    STEP_FLAG_RECALL 
} e_step_flag_;

typedef enum {
    PROC_TYPE_PROC = 0,
    PROC_TYPE_MAIN,
    PROC_TYPE_KEYWORD
} e_proc_type_;

#define DEF_STEP_EXT(a)       STEP_TYPE_##a, STEP_TYPE_ACCEPT_IF_##a,  STEP_TYPE_RECALL_IF_##a, STEP_TYPE_BREAK_IF_##a
#define DEF_STEP(a)           DEF_STEP_EXT(a), DEF_STEP_EXT(a##_DATA)
#define DEF_STEP_DATA_EXT     (STEP_TYPE_EXEC_PROC_DATA - STEP_TYPE_EXEC_PROC)

typedef enum {
    STEP_TYPE_NAN = 0,
    STEP_TYPE_PROCSEQ,
    STEP_TYPE_PROC_ACCEPT,
    STEP_TYPE_PROCSEQ_RECALL,
    STEP_TYPE_PROCSEQ_BREAK,
    STEP_TYPE_OPTSEQ,
    STEP_TYPE_OPTLOOP,
    DEF_STEP(EXEC_PROC),
    DEF_STEP(EXEC_KEYWORD),
    DEF_STEP(EXEC_ONEKEYWORD),
    DEF_STEP(LITERAL),
    DEF_STEP(INTEGER),
    DEF_STEP(CHAR),
    DEF_STEP(FLOAT),
    DEF_STEP(STRING),
    DEF_STEP(KEYWORD),
    DEF_STEP(MULTI_KEYWORD),
    DEF_STEP(GETIDENT),
    DEF_STEP(TOKEN),   
    DEF_STEP(MULTI_TOKEN),
    DEF_STEP(SYMBOL),
    DEF_STEP(MULTI_SYMBOL)
} e_step_type_;

typedef struct data_list_ {
    union {
        char *codes;
        int  inl;
    } data;
    int indexl;
    struct data_list_ *nextp;
} ac_data_list_, *pac_data_list_;

typedef struct step_ {
    e_step_type_ type;
    union {
    struct proc_  *procp;
        pac_data_list_ strlistp;
        char *codes;
        char chr;
    } datap;
    int stepflagb;
    char *stp_datap;
    struct step_ *rootp;
    struct step_ *headp;
    struct step_ *childp;
    struct step_ *nextp;
} ac_step_, *pac_step_;

typedef struct proc_{
    char         names[MAX_LEN];
    e_proc_type_ type;
    int          index;
    int          initpos;
    int          isdefb;
    int          inuseb;
    pac_step_    steplist;
    struct proc_ *nextp;
} ac_proc_, *pac_proc_;

typedef struct cmplgen_ {
    char           *module_name;
    int            procseq;
    int            seqidx;
    pac_proc_      proc_listp;
    pac_data_list_ keyword_listp;
    pac_data_list_ token_listp;
    pac_data_list_ stepdata_listp;
    pac_data_list_ headers;
} ac_cmplgen_, *pac_cmplgen_;

char *ac_get_procname(pac_proc_ proc, int index);
void ac_print_proc(pac_cmplgen_ cmplgenp, int *pxtab, FILE *outputp);
void ac_print_compiler(pac_cmplgen_ proc_listp, FILE *outputp, int procount);

#endif
