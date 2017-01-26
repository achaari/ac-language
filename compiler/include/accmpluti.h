
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
    PROC_TYPE_MAIN,
    PROC_TYPE_KEYWORD
} e_proc_type_;

typedef struct data_list_ {
    union {
        char *codes;
        int  inl;
    } data;
    int indexl;
    struct data_list_ *nextp;
} ac_data_list_, *pac_data_list_;

typedef struct proc_{
    char         names[MAX_LEN];
    e_proc_type_ type;
    int          index;
    int          initpos;
    int          isdefb;
    int          inuseb;
    struct cmplstep_ *steplist;
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

/* CMPL UTI */
char *ac_list_add_ordered_str(char *str, pac_data_list_ *firstp);
int ac_add_symbol_list(char *symbols);
int ac_add_symbol(char symbolc);
char *ac_get_token(PTR inputp);
pac_proc_ ac_get_procptr(pac_proc_ proc, int index);
pac_proc_ ac_get_proc(pac_proc_ proclistp, char *names);
char *ac_get_keyword(PTR inputp, char *keyword);
int ac_list_add_int(int inl, pac_data_list_ *firstp);
int ac_list_add_strptr(char *str, pac_data_list_ *firstp);

#endif
