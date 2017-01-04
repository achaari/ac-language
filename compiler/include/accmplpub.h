
#ifndef AC_CMPL_PUB_H
#define AC_CMPL_PUB_H

#ifdef AC_CMPL_INT
#define AC_EXTERN
#else
#define AC_EXTERN extern
#endif

typedef void *p_accmpl_;

AC_EXTERN p_accmpl_ __ac_new_compiler();

AC_EXTERN int __ac_compl_set_keywords(p_accmpl_ cmplhndp, const char **keywords, int count);
AC_EXTERN int __ac_compl_set_tokens(p_accmpl_ cmplhndp, const char **tokens, int count);
AC_EXTERN int __ac_compl_exec(p_accmpl_ cmplhndp, int *cmpldatap, int count);
AC_EXTERN int __ac_exit_compiler(p_accmpl_ *cmplhndp);

#endif
