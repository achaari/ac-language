
#include "acstd.h"
#include "accmpl.h"

#include "accmplpub.h"
#include "accmpldefs.h"

#include <stdarg.h>

#define _is_upper(a)      (((a) >= 'A') && ((a) <= 'Z'))
#define _is_lower(a)      (((a) >= 'a') && ((a) <= 'z'))
#define _is_chr(a)        (_is_upper(a) || _is_lower(a))
#define _is_digit(a)      (((a) >= '0') && ((a) <= '9'))
#define _is_debident(a)   (_is_chr(a) || ((a) == '_'))
#define _is_ident(a)      (_is_digit(a) || _is_debident(a))
#define _is_blank(a)      (((a) == ' ') || ((a) == '\t') || ((a) == '\v') || ((a) == '\a'))
#define _is_end(a)        (((a) == '\0') || ((a) == EOF))
#define _is_eol(a)        (((a) == '\0') || ((a) == EOF) || ((a) == '\n') || ((a) == '\r'))
#define _is_esc_seq(a)    (strchr("0anrtv'\"", (a)) != NULLP)

#ifndef AC_MANAGE_ERROR
#define ac_error(a, ...)
#define ac_warning(a, ...)
#define ac_log(a, ...)       {if(cmplp->logp) fprintf(cmplp->logp,a, __VA_ARGS__);}
#endif

typedef enum {
    AC_TOKEN_NA = 0,
    AC_TOKEN_KEYWORD,
    AC_TOKEN_TOKEN,
    AC_TOKEN_SYMBOL,
    AC_TOKEN_IDENT,   
    AC_TOKEN_STRING,
    AC_TOKEN_CHAR,
    AC_TOKEN_INTEGER,
    AC_TOKEN_FLOAT,
} e_ac_token_;

typedef struct filepos_ {
    char *curpos;
    int  line;
    int  newline;
    char *firstpos;
} ac_filepos_, *pac_filepos_;

typedef struct token_ {
    e_ac_token_ type;
    ac_filepos_ startpos;
    int         isfirst;
    int         innewline;
    int         consumed;
    union {
        char   *strs;
        int    intl;
        char   chr;
        double dbl;
    } data;
} ac_token_, *pac_token_;

typedef struct file_ {
    char        *code;
    char        *name;
    ac_filepos_ filepos;
} ac_file_, *pac_file_;

typedef struct stat_ {
    int stat;
    int checkstep;
    struct stat_ *nextp;
    struct stat_ *prevp;
} ac_stat_, *pac_stat_;

typedef struct procseq_ {
    int             endseq;
    int             recall;
    struct procseq_ *nextp;
    struct procseq_ *prevp;
} ac_procseq_, *pac_procseq_;

typedef struct proc_ {
    int          stat;
    int          one_matche;
    int          endproc;
    const char   *names;
    ac_filepos_  start_pos;
    int          start_idx;
    pac_procseq_ curseqp;
    struct proc_ *nextp;
    struct proc_ *prevp;
} ac_proc_, *pac_proc_;

typedef struct cmpl_ {
    int        modedebug;
    pac_file_  flp;
    ac_token_  curtoken;
    pac_proc_  procp;
    pac_stat_  statp;
    int        forceline;
    int        curidx;
    int        endread;
    int        fatalerrb;
    const char **keyword;
    int        nbkeyword;
    const char **token;
    int        nbtoken;
    const int  *prcdtx;
    int        nbprcdtx;
    FILE       *logp;
} ac_cmpl_, *pac_cmpl_;

static int __ac_exec_stat(pac_cmpl_ cmplhndp, e_step_def_ stepdef, int checkstepb, PTR retdata);
static int __ac_exec_one_step(pac_cmpl_ cmplhndp, e_ac_step_ step, PTR stepdata, PTR retdata);
static int __ac_exec_proc_step(pac_cmpl_ cmplhndp, e_step_def_ stepdef);
static int __ac_exec_proc(pac_cmpl_ cmplhndp, int procindx, PTR retdata);
static void __ac_print_token(pac_cmpl_ cmpl, const char *prefix);
static int __ac_init_stat(pac_cmpl_ cmplp, int checkstepb);
static int __ac_next_token(pac_cmpl_ cmplhndp);

/*************************************************************************/
p_accmpl_ __ac_new_compiler()
{
    pac_cmpl_ pcmpl = mem_get(sizeof(ac_cmpl_));
    return(pcmpl);
}

int __ac_compl_set_keywords(p_accmpl_ cmplhndp, const char **keywords, int count)
{
    pac_cmpl_ pcmpl = cmplhndp;

    pcmpl->keyword   = keywords;
    pcmpl->nbkeyword = count;

    return(TRUE);
}

int __ac_compl_set_tokens(p_accmpl_ cmplhndp, const char **tokens, int count)
{
    pac_cmpl_ pcmpl = cmplhndp;

    pcmpl->token   = tokens;
    pcmpl->nbtoken = count;

    return(TRUE);
}

int __ac_compl_exec(p_accmpl_ cmplhndp, const int *cmpldatap, int count)
{
    int retfct = TRUE;
    pac_cmpl_ cmplp = cmplhndp;

    cmplp->prcdtx  = cmpldatap;
    cmplp->nbprcdtx = count;

    __ac_init_stat(cmplhndp, TRUE);
    
    while (!cmplp->fatalerrb && retfct && __ac_next_token(cmplhndp)) {
        retfct = __ac_exec_proc(cmplhndp, 0, NULLP);
    }

    if (cmplp->fatalerrb) {
        ac_error(AC_FATAL_ERROR);
    }
    else if (!retfct) {
        ac_error(AC_COMPILATION_ERROR);
    }

    mem_free(cmplp->statp);
    cmplp->statp = NULLP;

    __ac_next_token(cmplhndp);
    if (cmplp->curtoken.type != AC_TOKEN_NA) {
        __ac_print_token(cmplhndp, "Unexpected ");
    }

    return(retfct);
}

int __ac_exit_compiler(p_accmpl_ *cmplhndp)
{
    return(FALSE);
}

static void __ac_save_pos(pac_cmpl_ cmplhndp, pac_filepos_ startpos)
{
    if (cmplhndp->curtoken.type != AC_TOKEN_NA) {
        mem_copy(&cmplhndp->curtoken.startpos, startpos, sizeof(ac_filepos_));
    }
    else {
        mem_copy(&cmplhndp->flp->filepos, startpos, sizeof(ac_filepos_));
    }
}

int __ac_init_proc(p_accmpl_ cmplhndp, const char *procname)
{
    pac_cmpl_ cmplp = cmplhndp;
    pac_proc_ procp = mem_get(sizeof(ac_proc_));

    if (procp == NULLP) return(FALSE);
    
    if (cmplp->procp) {
        cmplp->procp->nextp = procp;
        procp->prevp = cmplp->procp;
    }

    cmplp->procp = procp;
    
    procp->start_idx = cmplp->curidx;
    procp->names     = procname;
    procp->stat      = TRUE;

    /* Save current position */
    __ac_save_pos(cmplp, &procp->start_pos);

    if (cmplp->modedebug) {
        ac_log("Start PROC : '%s'\n", procname);
    }

    return(TRUE);
}

static void __ac_pop_proc(pac_cmpl_ cmplp)
{
    cmplp->procp = cmplp->procp->prevp;

    if (cmplp->procp) {
        mem_free(cmplp->procp->nextp);
        cmplp->procp->nextp = NULLP;
    }
}

static void __ac_reset_pos(pac_cmpl_ cmplp, pac_filepos_ curpos)
{
    if (curpos == NULLP || cmplp->flp->filepos.curpos != curpos->curpos) {
        if (cmplp->curtoken.type == AC_TOKEN_IDENT || cmplp->curtoken.type == AC_TOKEN_STRING) {
            mem_free(cmplp->curtoken.data.strs);
            cmplp->curtoken.data.strs = NULLP;
        }
        cmplp->curtoken.type = AC_TOKEN_NA;

        if (curpos) {
            mem_copy(curpos, &cmplp->flp->filepos, sizeof(ac_filepos_));
        }
    }
}

int __ac_end_proc(p_accmpl_ cmplhndp)
{
    pac_cmpl_ cmplp = cmplhndp; 
    
    if (cmplp->procp->one_matche && cmplp->procp->stat) {
        if (cmplp->modedebug) {
            ac_log("End PROC '%s' with status 'MATCHED'\n", cmplp->procp->names);
        }
        __ac_pop_proc(cmplp);
        return(TRUE);
    }
    else if (! cmplp->procp->stat) {
        /* reset previous position */
        __ac_reset_pos(cmplp, &cmplp->procp->start_pos);
    }

    if (cmplp->modedebug) {
        ac_log("End PROC '%s' with status 'UNMATCHED'\n", cmplp->procp->names);
    }
    __ac_pop_proc(cmplp);
    return(FALSE);
}

int __ac_stop_proc(p_accmpl_ cmplhndp)
{
    pac_cmpl_ cmplp = cmplhndp; 
        
    /* reset token and position */
    __ac_reset_pos(cmplp, &cmplp->procp->start_pos);

    if (cmplp->procp->one_matche) {
        if (cmplp->modedebug) {
            ac_log("End PROC '%s' with status 'ERROR'\n", cmplp->procp->names);
        }
    }
    else {
        if (cmplp->modedebug) {
            ac_log("End PROC '%s' with status 'UNMATCHED'\n", cmplp->procp->names);
        }
    }
    __ac_pop_proc(cmplp);

    return(FALSE);
}

static int __ac_init_stat(pac_cmpl_ cmplp, int checkstepb)
{
    pac_stat_ statp = mem_get(sizeof(ac_stat_));

    if (statp == NULLP) return(FALSE);

    statp->checkstep = checkstepb;

    if (cmplp->statp) {
        cmplp->statp->nextp = statp;
        statp->prevp = cmplp->statp;
    }

    cmplp->statp = statp;
    return(TRUE);
}

static int __ac_end_stat(pac_cmpl_ cmplp)
{
    int stat = cmplp->statp->stat;

    cmplp->statp = cmplp->statp->prevp;

    mem_free(cmplp->statp->nextp);
    cmplp->statp->nextp = NULLP;

    return(stat);
}

static void __ac_set_stat(pac_cmpl_ cmplp, int stat)
{
    cmplp->statp->stat = stat;

    if (cmplp->statp->checkstep) {
        cmplp->procp->stat &= stat;
    }
    else if (stat) {
        cmplp->procp->stat = TRUE;
    }

    cmplp->procp->one_matche |= stat;
}

int __ac_pocess_next(p_accmpl_ cmplhndp)
{
    return(TRUE);
}

static void __ac_newline(pac_cmpl_ cmplhndp, int resetline)
{
    cmplhndp->flp->filepos.line++;
    
    if (! resetline) {
        cmplhndp->flp->filepos.newline = TRUE;
        cmplhndp->flp->filepos.firstpos = NULLP;
    }
}

static int __ac_openfile(pac_cmpl_ cmplhndp, const char *filename)
{
    char *flc;
    size_t len;
    FILE *fp = fopen(filename, "rb");

    if (!fp) return(FALSE);

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    if (len <= 0) { fclose(fp); return(FALSE); }

    flc = mem_get(len + 1);
    if (!flc) { fclose(fp); return(FALSE); }

    fseek(fp, 0, SEEK_SET);
    if (fread(flc, sizeof(char), len, fp) < len) {
        mem_free(flc); fclose(fp); return(FALSE);
    }

    fclose(fp);

    cmplhndp->flp = mem_get(sizeof(ac_file_));
    if (! cmplhndp->flp) { mem_free(flc);  return(FALSE); }

    cmplhndp->flp->code = flc;
    cmplhndp->flp->name = st_dup(filename);

    /* Set start position */
    cmplhndp->flp->filepos.curpos = cmplhndp->flp->code;
    
    __ac_newline(cmplhndp, TRUE);

    return(TRUE);
}

static int __ac_is_str(char *idents, int *indexl, const char **tab, int len)
{
    int i = 0, cmp, low = 0, high = len - 1;

    if (tab == NULLP) {
        return(FALSE);
    }

    while (TRUE) {
        i = (low + high) / 2;
        cmp = strcmp(idents, tab[i]);

        if (! cmp) {
            *indexl = i;
            return(TRUE);
        }
        else if (cmp < 0) {
            low = i + 1;
            if (low > high) {
                break;
            }
        }
        else if (cmp > 0) {
            high = i - 1;
            if (high < low) {
                break;
            }
        }
    }

    return(FALSE);
}

static int __ac_is_keyword(pac_cmpl_ cmplhndp, char *idents, int *indexl)
{
    return(__ac_is_str(idents, indexl, cmplhndp->keyword, cmplhndp->nbkeyword));
}

static int __ac_is_token(pac_cmpl_ cmplhndp, char *idents, int *indexl)
{
    return(__ac_is_str(idents, indexl, cmplhndp->token, cmplhndp->nbtoken));
}

static int __ac_check_token(pac_cmpl_ cmplhndp, int *indexl)
{
    int i = 0, foundi = -1, low = 0, high = cmplhndp->nbtoken - 1;

    char checkchr = *cmplhndp->flp->filepos.curpos;

    if (cmplhndp->token == NULLP) {
        return(FALSE);
    }

    while (TRUE) {
        i = (low + high) / 2;

        if (cmplhndp->token[i][0] == checkchr) {
            foundi = i;
            break;
        }
        else if (cmplhndp->token[i][0] > checkchr) {
            low = i + 1;
            if (low > high) {
                break;
            }
        }
        else if (cmplhndp->token[i][0] < checkchr) {
            high = i - 1;
            if (high < low) {
                break;
            }
        }
    }

    if (foundi < 0) {
        return(FALSE);
    }

    /* Get First Token */
    i = foundi;
    while (i > 0) {
        if (cmplhndp->token[i - 1][0] != checkchr) {
            break;
        }
        i--;
    }
    
    /* Get Token */
    while (i < cmplhndp->nbtoken) {
        if (! strncmp(cmplhndp->token[i], cmplhndp->flp->filepos.curpos, strlen(cmplhndp->token[i]))) {
            *indexl = i;
            cmplhndp->flp->filepos.curpos += strlen(cmplhndp->token[i]);
            return(TRUE);
        }
        else if (cmplhndp->token[i][0] != checkchr) {
            break;
        }

        i++;
    }

    return(FALSE);
}

static char __get_seq_str(char chr)
{
    switch (chr) {
        case '\'': return '\'';
        case '0': return '\0';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'v': return '\v';
        case 'a': return '\a';
        case '"': return '"';
        default : return -1;
    }
}

static char *__ac_print_char(char chr)
{
    static char chrs[2];

    mem_reset(chrs, sizeof(chrs));

    switch (chr) {
        case '\'': return "\\'";
        case '\0': return "\\0";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\a': return "\\a";
        case '"': return  "\\\"";
        default: 
            chrs[0] =  chr;
            return chrs;
    }
}

static int __ac_get_next_token(pac_cmpl_ cmplhndp)
{
    int escape = FALSE, reuseb = FALSE, isfloatb = FALSE;
    char push, chr, *tmppos, *idents;

    if (_is_end(*cmplhndp->flp->filepos.curpos)) {
        return(FALSE);
    }
    
    while (TRUE) {
        /* Skip emply lines */
        if (*cmplhndp->flp->filepos.curpos == '\\') {
            cmplhndp->flp->filepos.curpos++;
            escape = TRUE;
        }

        if (*cmplhndp->flp->filepos.curpos == '\r') {
            cmplhndp->flp->filepos.curpos++;
        }

        if (*cmplhndp->flp->filepos.curpos == '\n') {
            __ac_newline(cmplhndp, !escape);
            cmplhndp->flp->filepos.curpos++;
            escape = FALSE;
            continue;
        }

        if (escape) {
            ac_error(AC_ILLEGAL_ESC_SEQUENCE);
            return(FALSE);
        }

        break;
    }

    /* Get First Non whitespace */
    while (_is_blank(*cmplhndp->flp->filepos.curpos)) {
        cmplhndp->flp->filepos.curpos++;
        reuseb = TRUE;
    }

    /* Escape comments */
    if (*cmplhndp->flp->filepos.curpos == '/') {
        if (*(cmplhndp->flp->filepos.curpos + 1) == '/') {
            cmplhndp->flp->filepos.curpos += 2;

            while (!_is_end(*cmplhndp->flp->filepos.curpos)) {
                if (*cmplhndp->flp->filepos.curpos == '\n') {
                    if (*(cmplhndp->flp->filepos.curpos - 1) != '\\') {
                        break;
                    }
                    else {
                        __ac_newline(cmplhndp, TRUE);
                    }
                }
                cmplhndp->flp->filepos.curpos++;
            }

            /* Get next code */
            reuseb = TRUE;
        }
        else if (*(cmplhndp->flp->filepos.curpos + 1) == '*') {
            cmplhndp->flp->filepos.curpos += 2;

            while (!_is_end(*cmplhndp->flp->filepos.curpos)) {
                if (*cmplhndp->flp->filepos.curpos == '\n') {
                    __ac_newline(cmplhndp, TRUE);
                }
                else if (*cmplhndp->flp->filepos.curpos == '*' && *(cmplhndp->flp->filepos.curpos + 1) == '/') {
                    cmplhndp->flp->filepos.curpos += 2;
                    break;
                }

                cmplhndp->flp->filepos.curpos++;
            }

            /* Get next code */
            reuseb = TRUE;
        }
    }

    if (reuseb) {
        return(__ac_get_next_token(cmplhndp));
    }

    if (_is_end(*cmplhndp->flp->filepos.curpos)) {
        return(FALSE);
    }

    /* Initialize new token */
    mem_reset(&cmplhndp->curtoken, sizeof(ac_token_));

    /* Save start position */
    mem_copy(&cmplhndp->flp->filepos, &cmplhndp->curtoken.startpos, sizeof(ac_filepos_));

    if (cmplhndp->flp->filepos.newline) {
        cmplhndp->curtoken.innewline = TRUE;
        cmplhndp->flp->filepos.firstpos = cmplhndp->flp->filepos.curpos;
        cmplhndp->flp->filepos.newline = FALSE;
    }    
    
    cmplhndp->curtoken.isfirst  = (cmplhndp->flp->filepos.firstpos == cmplhndp->flp->filepos.curpos);

    if (*cmplhndp->flp->filepos.curpos == '\'') {
        cmplhndp->flp->filepos.curpos++;

        if (_is_eol(*cmplhndp->flp->filepos.curpos)) {
            ac_error(NEWLINE_IN_CONSTANT);
            return(FALSE);
        }

        if (*cmplhndp->flp->filepos.curpos == '\\') {
            cmplhndp->flp->filepos.curpos++;
            if (!_is_esc_seq(*cmplhndp->flp->filepos.curpos)) {
                ac_warning(UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE);
                chr = '\\';
            }
            else {
                chr = __get_seq_str(*cmplhndp->flp->filepos.curpos);
            }
        }
        else {
            chr = *cmplhndp->flp->filepos.curpos;
        }

        cmplhndp->flp->filepos.curpos++;

        if (*cmplhndp->flp->filepos.curpos != '\'') {
            ac_warning(CHAR_TOO_LONG);
            while (*cmplhndp->flp->filepos.curpos != '\'') {
                if (_is_eol(*cmplhndp->flp->filepos.curpos)) {
                    ac_error(NEWLINE_IN_CONSTANT);
                    return(FALSE);
                }
                cmplhndp->flp->filepos.curpos++;
            }
        }

        cmplhndp->curtoken.type = AC_TOKEN_CHAR;
        cmplhndp->curtoken.data.chr = chr;

        /* End of string */
        cmplhndp->flp->filepos.curpos++;
    }
    else if (*cmplhndp->flp->filepos.curpos == '"') {
        tmppos = ++cmplhndp->flp->filepos.curpos;

        while (*cmplhndp->flp->filepos.curpos != '\"') {
            if (_is_eol(*cmplhndp->flp->filepos.curpos)) {
                ac_error(NEWLINE_IN_CONSTANT);
                return(FALSE); 
            }
            else if (*cmplhndp->flp->filepos.curpos == '\\') {
                cmplhndp->flp->filepos.curpos++;
                if (*cmplhndp->flp->filepos.curpos == '\n') {
                    __ac_newline(cmplhndp, TRUE);
                }
                else {
                    if (!_is_esc_seq(*cmplhndp->flp->filepos.curpos)) {
                        ac_warning(UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE);
                    }   
                }
            }
            cmplhndp->flp->filepos.curpos++;
        }
                
        push = *cmplhndp->flp->filepos.curpos;
        *cmplhndp->flp->filepos.curpos = '\0';
        idents = st_dup(tmppos);
        *cmplhndp->flp->filepos.curpos = push;

        cmplhndp->curtoken.type = AC_TOKEN_STRING;
        cmplhndp->curtoken.data.strs = idents;

        /* End of string */
        cmplhndp->flp->filepos.curpos++;
    }
    else if (_is_debident(*cmplhndp->flp->filepos.curpos)) {
        tmppos = cmplhndp->flp->filepos.curpos;
        while (_is_ident(*cmplhndp->flp->filepos.curpos)) {
            cmplhndp->flp->filepos.curpos++;
        }

        push = *cmplhndp->flp->filepos.curpos;
        *cmplhndp->flp->filepos.curpos = '\0';
        idents = st_dup(tmppos);
        *cmplhndp->flp->filepos.curpos = push;

        if (!idents) {
            ac_error(ERROR_MEMORY_ALLOC, "idents");
            return(FALSE);
        }

        if (__ac_is_keyword(cmplhndp, idents, &cmplhndp->curtoken.data.intl)) {
            cmplhndp->curtoken.type = AC_TOKEN_KEYWORD;
            mem_free(idents);
        }
        else {
            cmplhndp->curtoken.type = AC_TOKEN_IDENT;
            cmplhndp->curtoken.data.strs = idents;
        }
    }
    else if (_is_digit(*cmplhndp->flp->filepos.curpos)) {
        tmppos = cmplhndp->flp->filepos.curpos;

        while (_is_digit(*cmplhndp->flp->filepos.curpos)) {
            cmplhndp->flp->filepos.curpos++;
        }

        if (*cmplhndp->flp->filepos.curpos == '.') {
            isfloatb = TRUE;
            cmplhndp->flp->filepos.curpos++;

            while (_is_digit(*cmplhndp->flp->filepos.curpos)) {
                cmplhndp->flp->filepos.curpos++;
            }
        }

        push = *cmplhndp->flp->filepos.curpos;
        *cmplhndp->flp->filepos.curpos = '\0';

        if (isfloatb) {
            cmplhndp->curtoken.type = AC_TOKEN_FLOAT;
            cmplhndp->curtoken.data.dbl = atof(tmppos);
        }
        else {
            cmplhndp->curtoken.type      = AC_TOKEN_INTEGER;
            cmplhndp->curtoken.data.intl = atoi(tmppos);
        }

        *cmplhndp->flp->filepos.curpos = push;
    }
    else if (__ac_check_token(cmplhndp, &cmplhndp->curtoken.data.intl)) {
        cmplhndp->curtoken.type = AC_TOKEN_TOKEN;
    }
    else {
        cmplhndp->curtoken.type = AC_TOKEN_SYMBOL; 
        cmplhndp->curtoken.data.chr = *cmplhndp->flp->filepos.curpos;
        cmplhndp->flp->filepos.curpos++;
    }

    return(TRUE);
}

static int __ac_next_token(pac_cmpl_ cmplhndp)
{
    if (cmplhndp->curtoken.type != AC_TOKEN_NA && !cmplhndp->curtoken.consumed) {
        return(TRUE);
    }
    else {
        __ac_reset_pos(cmplhndp, NULLP);
        return(__ac_get_next_token(cmplhndp));
    }
}

static int __ac_validate_matched(pac_cmpl_ cmplhndp, PTR retdata)
{
    __ac_print_token(cmplhndp, "Matched ");

    cmplhndp->curtoken.consumed = TRUE;

    return(TRUE);
}

static int __ac_push_procseq(pac_cmpl_ cmplp)
{
    pac_procseq_ procseq = mem_get(sizeof(ac_procseq_));

    if (procseq == NULLP) return(FALSE);

    procseq->prevp = cmplp->procp->curseqp;

    if (cmplp->procp->curseqp) {
        cmplp->procp->curseqp->nextp = procseq;
    }

    cmplp->procp->curseqp = procseq;
    return(TRUE);
}

static void __ac_pop_procseq(pac_cmpl_ cmplp)
{
    cmplp->procp->curseqp = cmplp->procp->curseqp->prevp;

    if (cmplp->procp->curseqp) {
        mem_free(cmplp->procp->curseqp->nextp);
        cmplp->procp->curseqp->nextp = NULLP;
    }
}

static int __ac_exec_stat(pac_cmpl_ cmplhndp, e_step_def_ stepdef, int checkstepb, PTR retdata)
{
    int retstepb = TRUE, nextidx, curidx, count, endpos, startpos, prevmatch;

    /* Need to keep the same execution as debug mode */
    __ac_init_stat(cmplhndp, checkstepb);

    retstepb = TRUE;
    switch (stepdef) {
        case STEP_DEF_ACCEPTPROC:
            cmplhndp->procp->endproc = TRUE;
            break;

        case STEP_DEF_EXECPROC:
            nextidx  = cmplhndp->prcdtx[cmplhndp->curidx++];
            retstepb = __ac_exec_proc(cmplhndp, nextidx, retdata);
            break;

        case STEP_DEF_GETIDENT:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_IDENT, NULLP, NULLP);
            break;

        case STEP_DEF_LITERAL:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_LITERAL, NULLP, NULLP);
            break;

        case STEP_DEF_INTEGER:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_INTEGER, NULLP, NULLP);
            break;

        case STEP_DEF_CHAR:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_CHAR, NULLP, NULLP);
            break;

        case STEP_DEF_FLOAT:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_FLOAT, NULLP, NULLP);
            break;

        case STEP_DEF_STRING:
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_STRING, NULLP, NULLP);
            break;

        case STEP_DEF_TOKEN:
            nextidx = cmplhndp->prcdtx[cmplhndp->curidx++];
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_TOKEN, &nextidx, NULLP);
            break;
            
        case STEP_DEF_KEYWORD:
            nextidx = cmplhndp->prcdtx[cmplhndp->curidx++];
            retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_KEYWORD, &nextidx, NULLP);
            break;

        case STEP_DEF_MULTI_KEYWORD:
            retstepb = FALSE;
            count = cmplhndp->prcdtx[cmplhndp->curidx++];
            for (curidx = 0; !retstepb && curidx < count; curidx++) {
                nextidx = cmplhndp->prcdtx[cmplhndp->curidx + curidx];
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_KEYWORD, &nextidx, NULLP);
            }

            cmplhndp->curidx += count;
            break;

        case STEP_DEF_MULTI_TOKEN:
            retstepb = FALSE;
            count = cmplhndp->prcdtx[cmplhndp->curidx++];
            for (curidx = 0; !retstepb && curidx < count; curidx++) {
                nextidx = cmplhndp->prcdtx[cmplhndp->curidx + curidx];
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_TOKEN, &nextidx, NULLP);
            }

            cmplhndp->curidx += count;
            break;

        case STEP_DEF_EXECKEYWORD:
            retstepb  = FALSE;
            curidx    = cmplhndp->prcdtx[cmplhndp->curidx++];
            nextidx   = cmplhndp->prcdtx[cmplhndp->curidx++];
            prevmatch = cmplhndp->procp->one_matche;
            if (__ac_exec_one_step(cmplhndp, AC_STEP_KEYWORD, &curidx, NULLP)) {
                retstepb = __ac_exec_proc(cmplhndp, nextidx, NULLP);
                /* Step is matched only if keyword + spoc are matched */
                if (!prevmatch && cmplhndp->procp->one_matche) {
                    cmplhndp->procp->one_matche = retstepb;
                }
            }
            break;

        case STEP_DEF_OPTSEQ:
            retstepb = FALSE;
            endpos = cmplhndp->prcdtx[cmplhndp->curidx++];
            stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
            if (__ac_exec_stat(cmplhndp, stepdef, FALSE, NULLP)) {
                retstepb = TRUE;
                while (cmplhndp->curidx < endpos) {
                    /* Get first step */
                    stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
                    if (!__ac_exec_proc_step(cmplhndp, stepdef)) {
                        retstepb = FALSE;
                        break;
                    }
                    else if (cmplhndp->procp->endproc || (cmplhndp->procp->curseqp && cmplhndp->procp->curseqp->endseq)) {
                        break;
                    }
                }
            }
            cmplhndp->curidx = endpos;
            break;

        case STEP_DEF_OPTLOOP:
            retstepb = FALSE;
            endpos = cmplhndp->prcdtx[cmplhndp->curidx++];
            startpos = cmplhndp->curidx;
            while (TRUE) {
                cmplhndp->curidx = startpos;                
                stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
                if (! __ac_exec_stat(cmplhndp, stepdef, FALSE, NULLP)) {
                    break;
                }
                retstepb = TRUE;
                if (cmplhndp->prcdtx[cmplhndp->curidx] == STEP_DEF_NOOP) {
                    cmplhndp->curidx++;
                }
                else {
                    while (cmplhndp->curidx < endpos) {
                        /* Get first step */
                        stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
                        if (!__ac_exec_proc_step(cmplhndp, stepdef)) {
                            retstepb = FALSE;
                            break;
                        }
                        else if (cmplhndp->procp->endproc || (cmplhndp->procp->curseqp && cmplhndp->procp->curseqp->endseq)) {
                            break;
                        }
                    }

                    if (cmplhndp->procp->endproc || (cmplhndp->procp->curseqp && cmplhndp->procp->curseqp->endseq) || !retstepb) {
                        break;
                    }
                }
            }
            cmplhndp->curidx = endpos;
            break;

        case STEP_DEF_PROCSEQ:
            retstepb = FALSE;
            if (!__ac_push_procseq(cmplhndp)) {
                ac_error(ERROR_MEMORY_ALLOC, "procseq");
                retstepb = FALSE;
                break;
            }
            endpos = cmplhndp->prcdtx[cmplhndp->curidx++];
            startpos = cmplhndp->curidx;
            while (TRUE) {
                retstepb  = TRUE;
                cmplhndp->curidx = startpos;
                cmplhndp->procp->curseqp->recall = FALSE;
                cmplhndp->procp->curseqp->endseq = FALSE;
                while (cmplhndp->curidx < endpos) {
                    /* Get sequence step */
                    stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
                    if (!__ac_exec_proc_step(cmplhndp, stepdef)) {
                        retstepb = FALSE;
                        break;
                    }
                    else if (cmplhndp->procp->endproc || cmplhndp->procp->curseqp->endseq) {
                        break;
                    }
                }

                if (!cmplhndp->procp->curseqp->recall) {
                    break;
                }
            }
            cmplhndp->curidx = endpos;
            __ac_pop_procseq(cmplhndp);
            break;

        case STEP_DEF_PROCSEQ_RECALL:
            cmplhndp->procp->curseqp->endseq = TRUE;
            cmplhndp->procp->curseqp->recall = TRUE;
            break;

        case STEP_DEF_PROCSEQ_BREAK:
            cmplhndp->procp->curseqp->endseq = TRUE;
            cmplhndp->procp->curseqp->recall = FALSE;
            break;

        default:
            ac_error(AC_INVALID_STEP, stepdef);
            __ac_end_stat(cmplhndp);
            return(FALSE);

    }

    __ac_set_stat(cmplhndp, retstepb);
    return(__ac_end_stat(cmplhndp));
}

static int __ac_exec_proc_step(pac_cmpl_ cmplhndp, e_step_def_ stepdef)
{
    e_step_def_ extdef;
    e_step_ext_ stepext;

    if (stepdef == STEP_DEF_ENDPROC) {
        cmplhndp->procp->endproc = TRUE;
        return(TRUE);
    }
    else if (stepdef == STEP_DEF_OPTLOOP || stepdef == STEP_DEF_OPTSEQ) {
        __ac_exec_stat(cmplhndp, stepdef, FALSE, NULLP);
        return(TRUE);
    }
    else if (stepdef < STEP_DEF_EXECPROC) {
        return(__ac_exec_stat(cmplhndp, stepdef, TRUE, NULLP));
    }

    stepext = (stepdef - STEP_DEF_EXECPROC) % 5;
    extdef = stepdef - stepext;
    switch (stepext) {
        case STEP_EXT_NA:
            return(__ac_exec_stat(cmplhndp, extdef, TRUE, NULLP));

        case STEP_EXT_OPTSTEP:
            __ac_exec_stat(cmplhndp, extdef, FALSE, NULLP);
            return(TRUE);

        case STEP_EXT_ACCEPT_IF:
            if (__ac_exec_stat(cmplhndp, extdef, FALSE, NULLP)) {
                cmplhndp->procp->endproc = TRUE;
            }
            return(TRUE);

        case STEP_EXT_BREAK_IF:
            if (cmplhndp->procp->curseqp == NULLP) {
                ac_error(AC_UNEXPECTED_STEP, stepdef);
                cmplhndp->fatalerrb = TRUE;
                return(FALSE);
            }
            else if (__ac_exec_stat(cmplhndp, extdef, FALSE, NULLP)) {
                cmplhndp->procp->curseqp->endseq = TRUE;
                cmplhndp->procp->curseqp->recall = FALSE;
            }
            return(TRUE);

        case STEP_EXT_RECALL_IF:
            if (cmplhndp->procp->curseqp == NULLP) {
                ac_error(AC_UNEXPECTED_STEP, stepdef);
                cmplhndp->fatalerrb = TRUE;
                return(FALSE);
            }
            else if (__ac_exec_stat(cmplhndp, extdef, FALSE, NULLP)) {
                cmplhndp->procp->curseqp->endseq = TRUE;
                cmplhndp->procp->curseqp->recall = TRUE;
            }
            return(TRUE);           
    }

    ac_error(AC_UNEXPECTED_STEP, stepdef);
    cmplhndp->fatalerrb = TRUE;
    return(FALSE);
}

static int __ac_exec_proc(pac_cmpl_ cmplhndp, int procindx, PTR retdata)
{
    int offset   = cmplhndp->prcdtx[procindx];
    int exdpoc   = cmplhndp->prcdtx[offset];
    e_step_def_  stepdef;

    if (! __ac_init_proc(cmplhndp, NULLP)) {
        cmplhndp->fatalerrb = TRUE;
        ac_error(ERROR_MEMORY_ALLOC, "proc");
        return(FALSE);
    }
        
    cmplhndp->curidx = offset + 1;

    while (!cmplhndp->fatalerrb && !cmplhndp->endread) {
        stepdef = cmplhndp->prcdtx[cmplhndp->curidx++];
        
        if (! __ac_exec_proc_step(cmplhndp, stepdef)) {
            cmplhndp->curidx = cmplhndp->procp->start_idx;
            return(__ac_stop_proc(cmplhndp));
        }
        else if (cmplhndp->procp->endproc) {
            cmplhndp->curidx = cmplhndp->procp->start_idx;
            return(__ac_end_proc(cmplhndp));
        }
    }

    if (cmplhndp->fatalerrb) {
        cmplhndp->curidx = cmplhndp->procp->start_idx;
        return(__ac_stop_proc(cmplhndp));
    }
    else if (cmplhndp->endread) {
        ac_error(AC_UNEXPECTED_ENDOFFILE);
        cmplhndp->curidx = cmplhndp->procp->start_idx;
        return(__ac_stop_proc(cmplhndp));
    }
    else {
        ac_error(AC_CMP_STACK_CORRUPTED);
        cmplhndp->curidx = cmplhndp->procp->start_idx;
        return(__ac_stop_proc(cmplhndp));
    }
}

static int __ac_check_one_step(pac_cmpl_ cmplhndp, e_ac_step_ step, PTR stepdata, PTR retdata)
{
    switch (step) {
            
        case AC_STEP_IDENT :
            if (cmplhndp->curtoken.type == AC_TOKEN_IDENT) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_LITERAL:
            if ((cmplhndp->curtoken.type == AC_TOKEN_INTEGER) ||
                (cmplhndp->curtoken.type == AC_TOKEN_FLOAT)   ||
                (cmplhndp->curtoken.type == AC_TOKEN_STRING)  ||
                (cmplhndp->curtoken.type == AC_TOKEN_CHAR)) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_INTEGER:
            if (cmplhndp->curtoken.type == AC_TOKEN_INTEGER) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_FLOAT:
            if (cmplhndp->curtoken.type == AC_TOKEN_FLOAT) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_CHAR:
            if (cmplhndp->curtoken.type == AC_TOKEN_CHAR) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_STRING:
            if (cmplhndp->curtoken.type == AC_TOKEN_STRING) {
                return(__ac_validate_matched(cmplhndp, retdata));
            }
            break;

        case AC_STEP_TOKEN:
            if (cmplhndp->curtoken.type == AC_TOKEN_TOKEN) {
                if (cmplhndp->modedebug) {
                    if (iseqstr(cmplhndp->token[cmplhndp->curtoken.data.intl], (char *)stepdata)) {
                        return(__ac_validate_matched(cmplhndp, retdata));
                    }
                }
                else if (cmplhndp->curtoken.data.intl == *(int *)stepdata) {
                    return(__ac_validate_matched(cmplhndp, retdata));
                }
            }
            break;

        case AC_STEP_SYMBOL:
            if (!cmplhndp->modedebug) {
                ac_error(AC_UNEXPECTED_STEP, AC_STEP_SYMBOL);
                cmplhndp->fatalerrb = TRUE;
                return(FALSE);
            }
            else if (cmplhndp->curtoken.type == AC_TOKEN_SYMBOL) {
                if (cmplhndp->curtoken.data.chr == *(char *)stepdata) {
                    return(__ac_validate_matched(cmplhndp, retdata));
                }
            }
            else if (cmplhndp->curtoken.type == AC_TOKEN_TOKEN) {
                if (iseqstr(cmplhndp->token[cmplhndp->curtoken.data.intl], (char *)stepdata)) {
                    return(__ac_validate_matched(cmplhndp, retdata));
                }
            }
            break;

        case AC_STEP_KEYWORD:
            if (cmplhndp->curtoken.type == AC_TOKEN_KEYWORD) {
                if (cmplhndp->modedebug) {
                    if (iseqstr(cmplhndp->keyword[cmplhndp->curtoken.data.intl], (char *)stepdata)) {
                        return(__ac_validate_matched(cmplhndp, retdata));
                    }
                }
                else if (cmplhndp->curtoken.data.intl == *(int *)stepdata) {
                    return(__ac_validate_matched(cmplhndp, retdata));
                }
            }
            break;
    }

    return(FALSE);
}

static int __ac_exec_one_step(pac_cmpl_ cmplhndp, e_ac_step_ step, PTR stepdata, PTR retdata)
{
    ac_filepos_ start_pos;

    mem_reset(&start_pos, sizeof(ac_filepos_));
    
    if (cmplhndp->curtoken.type == AC_TOKEN_NA || cmplhndp->curtoken.consumed) {
        if (_is_end(*cmplhndp->flp->filepos.curpos) || !__ac_next_token(cmplhndp)) {
            cmplhndp->endread = TRUE;
            return(FALSE);
        }
    }

    if (cmplhndp->forceline && cmplhndp->curtoken.innewline) {
        /* Force the current line */
        return(FALSE);
    }

    /* Save current position */
    __ac_save_pos(cmplhndp, &start_pos);

    if (! __ac_check_one_step(cmplhndp, step, stepdata, retdata)) {
        /* reset position */
        __ac_reset_pos(cmplhndp, &start_pos);

        return(FALSE);
    }

    return(TRUE);
}


int __ac_process_step(p_accmpl_ cmplhndp, int checkstepb, e_ac_step_ step, ...)
{
    pac_cmpl_ cmplp = cmplhndp;
    __exec prcfctp;
    va_list args;
    int retstepb = TRUE, prevmatch;
    PTR datap = NULLP;
    char *strs, chr[2];

    mem_reset(chr, sizeof(chr));

    va_start(args, step);

    __ac_init_stat(cmplhndp, checkstepb);

    while (TRUE) {
        
        retstepb = TRUE;

        if (step >= AC_STEP_EXECPROC) {
            /* Get Step DATA */
            datap = va_arg(args, PTR);
        }

        switch (step) {
            case AC_STEP_END_EXEC:
                va_end(args);
                return(__ac_end_stat(cmplhndp));

            case AC_STEP_APPEND_AND:
                if (!checkstepb) {
                    ac_error(AC_UNEXPECTED_STEP, AC_STEP_APPEND_AND);
                    cmplp->fatalerrb = TRUE;
                    __ac_end_stat(cmplhndp);
                    va_end(args);
                    return(FALSE);
                }
                else if (! cmplp->statp->stat) {
                    /* if previous step is not matched, we stop sequence */
                    va_end(args);
                    return(__ac_end_stat(cmplhndp));
                }
                break;

            case AC_STEP_APPEND_OR:
                if (checkstepb) {
                    ac_error(AC_UNEXPECTED_STEP, AC_STEP_APPEND_OR);
                    cmplp->fatalerrb = TRUE; 
                    __ac_end_stat(cmplhndp);
                    va_end(args);
                    return(FALSE);
                }
                else if (cmplp->statp->stat) {
                    /* if previous step is matched, we stop sequence */
                    va_end(args);
                    return(__ac_end_stat(cmplhndp));
                }
                break;

            case AC_STEP_EXECPROC:
                prcfctp = va_arg(args, __exec);
                if (prcfctp == NULLP) {
                    __ac_end_stat(cmplhndp);
                    va_end(args);
                    return(FALSE);
                }

                retstepb = (*prcfctp)(cmplhndp);
                break;

            case AC_STEP_EXECKEYWORD:
                retstepb = FALSE;
                strs = va_arg(args, char *);
                while (strs != NULLP) {
                    prcfctp = va_arg(args, __exec);
                    if (!retstepb) {
                        prevmatch = cmplp->procp->one_matche;
                        if (__ac_exec_one_step(cmplhndp, AC_STEP_KEYWORD, strs, datap)) {
                            retstepb = (*prcfctp)(cmplhndp);
                            /* Step is matched only if keyword + spoc are matched */
                            if (!prevmatch && cmplp->procp->one_matche) {
                                cmplp->procp->one_matche = retstepb;
                            }
                        }
                    }
                    
                    /* Next Keyword */
                    strs = va_arg(args, char *);
                }
                break;

            case AC_STEP_KEYWORD:
                retstepb = FALSE;
                strs = va_arg(args, char *);
                while (strs != NULLP) {
                    if (! retstepb) {
                        retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_KEYWORD, strs, datap);
                    }

                    /* Next Keyword */
                    strs = va_arg(args, char *);
                }
                break;

            case AC_STEP_IDENT:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_IDENT, NULLP, datap);
                break;

            case AC_STEP_SYMBOL:
                chr[0] = va_arg(args, char);

                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_SYMBOL, chr, datap);
                break;

            case AC_STEP_TOKEN:
                retstepb = FALSE;
                strs = va_arg(args, char *);
                while (strs != NULLP) {
                    if (! retstepb) {
                        retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_TOKEN, strs, datap);
                    }
                    
                    /* Next Keyword */
                    strs = va_arg(args, char *);
                }
                break;

            case AC_STEP_LITERAL:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_LITERAL, NULLP, datap);
                break;

            case AC_STEP_INTEGER:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_INTEGER, NULLP, datap);
                break;

            case AC_STEP_CHAR:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_CHAR, NULLP, datap);
                break;

            case AC_STEP_FLOAT:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_FLOAT, NULLP, datap);
                break;

            case AC_STEP_STRING:
                retstepb = __ac_exec_one_step(cmplhndp, AC_STEP_STRING, NULLP, datap);
                break;

            default :
                va_end(args);
                __ac_end_stat(cmplhndp);
                ac_error(AC_UNKNOWN_STEP, step);
                return(FALSE);
        }

        if (step > AC_STEP_APPEND_OR) {
            __ac_set_stat(cmplhndp, retstepb);
        }
        
        /* Get NextStep */
        step = va_arg(args, int);
    }

    va_end(args);
    return(__ac_end_stat(cmplhndp));
}

int __ac_compl_exec_mainproc(p_accmpl_ cmplhndp, __exec prcfctp)
{
    int retfct = TRUE;
    pac_cmpl_ cmplp = cmplhndp;

    __ac_init_stat(cmplhndp, TRUE);

    cmplp->modedebug = TRUE;

    while (retfct && __ac_next_token(cmplhndp)) {
        retfct = (*prcfctp)(cmplhndp);
    }

    mem_free(cmplp->statp);
    cmplp->statp = NULLP;

    __ac_next_token(cmplhndp);
    if (cmplp->curtoken.type != AC_TOKEN_NA) {
        __ac_print_token(cmplhndp, "Unexpected ");
    }

    return(retfct);
}

static void __ac_print_token(pac_cmpl_ cmpl, const char *prefix)
{
    if (cmpl->logp == NULLP) {
        return;
    }

    switch (cmpl->curtoken.type) {
        case AC_TOKEN_IDENT:
            fprintf(cmpl->logp, "%sIdent : %s (%d) \n", prefix, cmpl->curtoken.data.strs, cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_KEYWORD:
            fprintf(cmpl->logp, "%sKeyword : %s (%d) \n", prefix, cmpl->keyword[cmpl->curtoken.data.intl], cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_TOKEN:
            fprintf(cmpl->logp, "%sToken : '%s' (%d) \n", prefix, cmpl->token[cmpl->curtoken.data.intl], cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_SYMBOL:
            fprintf(cmpl->logp, "%sSymbol : '%c' (%d) \n", prefix, cmpl->curtoken.data.chr, cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_STRING:
            fprintf(cmpl->logp, "%sString : \"%s\" (%d) \n", prefix, cmpl->curtoken.data.strs, cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_CHAR:
            fprintf(cmpl->logp, "%sChar : '%s' (%d) \n", prefix, __ac_print_char(cmpl->curtoken.data.chr), cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_INTEGER:
            fprintf(cmpl->logp, "%sInteger : '%d' (%d) \n", prefix, cmpl->curtoken.data.intl, cmpl->curtoken.startpos.line);
            break;
        case AC_TOKEN_FLOAT:
            fprintf(cmpl->logp, "%sFloat : '%f' (%d) \n", prefix, cmpl->curtoken.data.dbl, cmpl->curtoken.startpos.line);
            break;
    }
}

#ifdef __AC_TEST__
int __accmpl_exec_module(p_accmpl_ cmpl);
int main() 
{
    const char *files = "D:\\ach projects\\accmplgen\\acdir\\test.c";
    const char *logs = "D:\\ach projects\\accmplgen\\acdir\\logtest.log";
    
    pac_cmpl_ cmpl = __ac_new_compiler();

    __ac_openfile(cmpl, files);

    cmpl->logp = fopen(logs, "w");

    /* Test Module */
    __accmpl_exec_module(cmpl);

    fclose(cmpl->logp);
}

#endif
