
#ifndef AC_CMPL_STEP_H
#define AC_CMPL_STEP_H

#include "accmpluti.h"

#define DEF_CMPLSTEP(a)    CMPLSTEP_TYPE_##a, CMPLSTEP_TYPE_##a##_EXT
#define CMPLSTEP_ISEXT(a)  (((a) > CMPLSTEP_TYPE_EXEC_PROC) && ((a) % 2))

typedef enum {
    CMPLSTEP_TYPE_NAN = 0,
    CMPLSTEP_TYPE_PROCSEQ,
    CMPLSTEP_TYPE_PROC_ACCEPT,
    CMPLSTEP_TYPE_PROCSEQ_RECALL,
    CMPLSTEP_TYPE_PROCSEQ_BREAK,
    CMPLSTEP_TYPE_OPTSEQ,
    CMPLSTEP_TYPE_OPTLOOP,
    DEF_CMPLSTEP(EXEC_PROC),
    DEF_CMPLSTEP(EXEC_KEYWORD),
    DEF_CMPLSTEP(EXEC_ONEKEYWORD),
    DEF_CMPLSTEP(LITERAL),
    DEF_CMPLSTEP(INTEGER),
    DEF_CMPLSTEP(CHAR),
    DEF_CMPLSTEP(FLOAT),
    DEF_CMPLSTEP(STRING),
    DEF_CMPLSTEP(KEYWORD),
    DEF_CMPLSTEP(MULTI_KEYWORD),
    DEF_CMPLSTEP(GETIDENT),
    DEF_CMPLSTEP(TOKEN),
    DEF_CMPLSTEP(MULTI_TOKEN),
    DEF_CMPLSTEP(SYMBOL),
    DEF_CMPLSTEP(MULTI_SYMBOL)
} e_cmplstep_type_;

typedef enum {
    CMPLTSTEP_EXT_NA = 0,
    CMPLTSTEP_EXT_OPTSTEP,
    CMPLTSTEP_EXT_ACCEPT,
    CMPLTSTEP_EXT_RECALL,
    CMPLTSTEP_EXT_BREAK,
    CMPLTSTEP_EXT_MAX
} e_cmplstep_ext_;

typedef enum {
    CMPLSTEP_FLAG_NON = 0,
    CMPLSTEP_FLAG_BREAK,
    CMPLSTEP_FLAG_RECALL
} e_cmplstep_flag_;

typedef struct cmplstep_ {
    e_cmplstep_type_ type;
    e_cmplstep_ext_  ext;
    union {
    struct proc_  *procp;
    pac_data_list_ strlistp;
    char *codes;
    char chr;
    } datap;
    int stepflagb;
    char *stp_datap;
    struct cmplstep_ *rootp;
    struct cmplstep_ *headp;
    struct cmplstep_ *childp;
    struct cmplstep_ *nextp;
} ac_cmplstep_, *pac_cmplstep_;

pac_cmplstep_ ac_cmplstep_get_step(pac_cmplgen_ cmplgenp, PTR inputp, pac_cmplstep_ rootp, pac_cmplstep_ *curstepp);

#endif
