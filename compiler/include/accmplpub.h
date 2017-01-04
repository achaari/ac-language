
#ifndef AC_CMPL_PUB_H
#define AC_CMPL_PUB_H

typedef void *p_accmpl_;

extern int __ac_compl_set_keywords(p_accmpl_ cmplhndp, char **keywords, int count);
extern int __ac_compl_set_tokens(p_accmpl_ cmplhndp, char **tokens, int count);

extern int __ac_compl_exec(p_accmpl_ cmplhndp, int *cmpldatap, int count);

extern int __ac_exit_compiler(p_accmpl_ *cmplhndp);

#endif
