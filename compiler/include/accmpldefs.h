
#ifndef AC_CMPL_DEFS_H
#define AC_CMPL_DEFS_H

#include "acstd.h"
#include "accmplpub.h"

typedef enum {
    AC_STEP_END_EXEC = 0,
    AC_STEP_APPEND_AND,
    AC_STEP_APPEND_OR,
    AC_STEP_EXECKEYWORD,
    AC_STEP_KEYWORD,
    AC_STEP_SYMBOL,
} e_ac_step_;

typedef void *p_accmpl_proc_;
typedef int(*__exec)(p_accmpl_ cmplhndp);

extern p_accmpl_proc_ __ac_init_proc(p_accmpl_ cmplhndp, __exec prcfctp, const char *procname);

extern int __ac_compl_exec_mainproc(p_accmpl_ cmplhndp, __exec prcfctp);
extern int __ac_process_step(p_accmpl_ cmplhndp, int checkstepb, int step, ...);
extern int __ac_end_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);
extern int __ac_stop_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp);

#define __ac_proc_step(mand, ...)  __ac_process_step(cmplhndp, mand, __VA_ARGS__, AC_STEP_END_EXEC)
#define __ac_check_step(stat, ...) __ac_proc_step(stat, FALSE)
#define __ac_exec_step(stat, ...)  if(! __ac_proc_step(TRUE, stat, __VA_ARGS__)) { return(__ac_stop_proc(cmplhndp, &procp)); }
#define __ac_try_step(stat, ...)   __ac_proc_step(FALSE, stat, __VA_ARGS__);

#define AND                    ,AC_STEP_APPEND_AND, 
#define OR                     ,AC_STEP_APPEND_OR,
#define KEY(a)                 #a, __exec_keyword_##a
#define ENDKEY                 "#"

/* AC Steps Definition */
#define EXECKEYWORD(a, ...)    AC_STEP_EXECKEYWORD, a, __VA_ARGS__, ENDKEY
#define KEYWORD(a, ...)        AC_STEP_KEYWORD, a, __VA_ARGS__, ENDKEY
#define EXECPROC(a)            AC_STEP_KEYWORD, __exec_##a
#define SYMBOL(a)              AC_STEP_SYMBOL, a           

#endif
