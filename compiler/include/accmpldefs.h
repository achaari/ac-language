
#ifndef AC_CMPL_DEFS_H
#define AC_CMPL_DEFS_H

#include "acstd.h"
#include "accmplpub.h"

typedef enum {
    AC_STEP_END_EXEC = 0,
    AC_STEP_APPEND_AND,
    AC_STEP_APPEND_OR,
    AC_STEP_BEG_PROCSEQ,
    AC_STEP_END_PROCSEQ,
    AC_STEP_EXECPROC,
    AC_STEP_EXECKEYWORD,
    AC_STEP_KEYWORD,
    AC_STEP_TOKEN,
    AC_STEP_SYMBOL,
    AC_STEP_IDENT,
    AC_STEP_LITERAL,
} e_ac_step_;

typedef enum {
    AC_TYPE_PROC,
    AC_TYPE_MAIN,
    AC_TYPE_KEYWORD,
} e_proc_type;

typedef void *p_accmpl_proc_;
typedef int(*__exec)(p_accmpl_ cmplhndp);

extern p_accmpl_proc_ __ac_init_proc(p_accmpl_ cmplhndp, const char *procname, e_proc_type type);

AC_EXTERN int __ac_compl_exec_mainproc(p_accmpl_ cmplhndp, __exec prcfctp);
AC_EXTERN int __ac_process_step(p_accmpl_ cmplhndp, int checkstepb, int step, ...);
AC_EXTERN int __ac_end_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);
AC_EXTERN int __ac_stop_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);
AC_EXTERN int __ac_pocess_next(p_accmpl_ cmplhndp);

#define __ac_proc_step(mand, ...)  __ac_process_step(cmplhndp, mand, __VA_ARGS__, AC_STEP_END_EXEC)
#define __ac_check_step(stat, ...) __ac_proc_step(FALSE, stat, __VA_ARGS__)
#define __ac_exec_step(stat, ...)  if(! __ac_proc_step(TRUE, stat, __VA_ARGS__)) { return(__ac_stop_proc(cmplhndp, &procp)); }
#define __ac_try_step(stat, ...)   __ac_proc_step(FALSE, stat, __VA_ARGS__);

#define AND                    ,AC_STEP_APPEND_AND, 
#define OR                     ,AC_STEP_APPEND_OR,
#define KEY(a)                 #a, __exec_keyword_##a
#define ENDSTEP                NULLP

/* AC Steps Definition */
#define BEG_PROCSEQ            AC_STEP_BEG_PROCSEQ
#define END_PROCSEQ            AC_STEP_END_PROCSEQ
#define EXECPROC(a)            AC_STEP_EXECPROC, __exec_##a
#define EXECKEYWORD(a, ...)    AC_STEP_EXECKEYWORD, a, __VA_ARGS__, ENDSTEP
#define KEYWORD(a, ...)        AC_STEP_KEYWORD, a, __VA_ARGS__, ENDSTEP
#define TOKEN(a, ...)          AC_STEP_TOKEN, a, __VA_ARGS__, ENDSTEP
#define SYMBOL(a)              AC_STEP_SYMBOL, a         
#define IDENT                  AC_STEP_IDENT
#define LITERAL                AC_STEP_LITERAL

#endif
