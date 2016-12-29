
#ifndef AC_CMPL_H
#define AC_CMPL_H

#define STEP_DEF(a) STEP_DEF_##a, STEP_DEF_OPTSTEP_##a, STEP_DEF_ACCEPT_IF_##a,  STEP_DEF_RECALL_IF_##a, STEP_DEF_BREAK_IF_##a

typedef enum {
    STEP_EXT_NA = 0,
    STEP_EXT_OPTSTEP,
    STEP_EXT_ACCEPT_IF,
    STEP_EXT_RECALL_IF,
    STEP_EXT_BREAK_IF
} e_step_ext_;

typedef enum {
    STEP_DEF_NA = 0,
    STEP_DEF_PROCSEQ,
    STEP_DEF_OPTSEQ,
    STEP_DEF_ENDPROC,
    STEP_DEF_ACCEPTPROC,
    STEP_DEF_PROCSEQ_RECALL,
    STEP_DEF_PROCSEQ_BREAK,
    STEP_DEF(EXECPROC),
    STEP_DEF(LITERAL),
    STEP_DEF(GETIDENT),
    STEP_DEF(KEYWORD),
    STEP_DEF(MULTI_KEYWORD),
    STEP_DEF(TOKEN),
    STEP_DEF(MULTI_TOKEN)
} e_step_def_;

#endif
