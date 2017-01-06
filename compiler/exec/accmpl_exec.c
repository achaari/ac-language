
#include "acstd.h"
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

typedef struct token_ {
    e_ac_token_ type;
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
    char *code;
    char *curpos;
    char *name;
    int  line;
    int  newline;
    char *firstpos;
    char *deeppos;
} ac_file_, *pac_file_;

typedef struct cmpl_ {
    pac_file_  flp;
    ac_token_  curtoken;
    const char **keyword;
    int        nbkeyword;
    const char **token;
    int        nbtoken;
} ac_cmpl_, *pac_cmpl_;

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

int __ac_compl_exec(p_accmpl_ cmplhndp, int *cmpldatap, int count)
{
    return(FALSE);
}

int __ac_exit_compiler(p_accmpl_ *cmplhndp)
{
    return(FALSE);
}

p_accmpl_proc_ __ac_init_proc(p_accmpl_ cmplhndp, const char *procname, e_proc_type type)
{
    return(FALSE);
}

int __ac_compl_exec_mainproc(p_accmpl_ cmplhndp, __exec prcfctp)
{
    return(FALSE);
}

static void __ac_init_stat(pac_cmpl_ cmplp, int checkstepb)
{
}

static void __ac_save_stat(pac_cmpl_ cmplp)
{

}

static int __ac_get_stat(pac_cmpl_ cmplp)
{
    return(FALSE);
}

static void __ac_set_stat(pac_cmpl_ cmplp, int stat)
{

}

int __ac_process_step(p_accmpl_ cmplhndp, int checkstepb, int step, ...)
{
    pac_cmpl_ cmplp = cmplhndp;
    va_list args;
    int retstepb = TRUE;
    PTR datap;

    va_start(args, step);

    __ac_init_stat(cmplhndp, checkstepb);

    while (TRUE) {
        
        __ac_save_stat(cmplhndp);

        /* Get Step DATA */
        datap = va_arg(args, PTR);

        switch (step) {
            case AC_STEP_END_EXEC:
                va_end(args);
                return(__ac_get_stat(cmplhndp));

            case AC_STEP_APPEND_AND:
                if (!checkstepb) {
                    ac_error(AC_UNEXPECTED_STEP, AC_STEP_APPEND_AND);
                    va_end(args);
                    return(FALSE);
                }
                else if (! __ac_get_stat(cmplhndp)) {
                    /* if previous step is not matched, we stop sequence */
                    va_end(args);
                    return(FALSE);
                }
                break;

            case AC_STEP_APPEND_OR:
                if (checkstepb) {
                    ac_error(AC_UNEXPECTED_STEP, AC_STEP_APPEND_OR);
                    va_end(args);
                    return(FALSE);
                }
                else if (__ac_get_stat(cmplhndp)) {
                    /* if previous step is matched, we stop sequence */
                    va_end(args);
                    return(TRUE);
                }
                break;

            case AC_STEP_IDENT:
                retstepb = (cmplp->curtoken.type == AC_TOKEN_IDENT);
                break;
        }

        __ac_set_stat(cmplhndp, retstepb);
        
        /* Get NextStep */
        step = va_arg(args, int);
    }

    va_end(args);
    return(FALSE);
}

int __ac_end_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp)
{
    return(FALSE);
}

int __ac_stop_proc(p_accmpl_ cmplhndp, p_accmpl_proc_ *procpp)
{
    return(FALSE);
}

int __ac_pocess_next(p_accmpl_ cmplhndp)
{
    return(FALSE);
}

static void __ac_newline(pac_cmpl_ cmplhndp, int resetline)
{
    if (cmplhndp->flp->curpos > cmplhndp->flp->deeppos) {
        cmplhndp->flp->deeppos = cmplhndp->flp->curpos;
        cmplhndp->flp->line++;
    }

    if (! resetline) {
        cmplhndp->flp->newline  = TRUE;
        cmplhndp->flp->firstpos = NULLP;
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

    cmplhndp->flp->code   = flc;
    cmplhndp->flp->curpos = cmplhndp->flp->code;
    cmplhndp->flp->name   = st_dup(filename);
    
    __ac_newline(cmplhndp, TRUE);

    return(TRUE);
}

static int __ac_is_keyword(pac_cmpl_ cmplhndp, char *idents, int *indexl)
{
    int i = 0, cmp, low = 0, high = cmplhndp->nbkeyword - 1;

    if (cmplhndp->keyword == NULLP) {
        return(FALSE);
    }

    while (TRUE) {
        i = (low + high) / 2;
        cmp = strcmp(idents, cmplhndp->keyword[i]);

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

static int __ac_check_token(pac_cmpl_ cmplhndp, int *indexl)
{
    int i = 0, foundi = -1, low = 0, high = cmplhndp->nbtoken - 1;

    char checkchr = *cmplhndp->flp->curpos;

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
    while (i >= 0) {
        if (! strncmp(cmplhndp->token[i], cmplhndp->flp->curpos, strlen(cmplhndp->token[i]))) {
            *indexl = i;
            cmplhndp->flp->curpos += strlen(cmplhndp->token[i]);
            return(TRUE);
        }
        else if (cmplhndp->token[i][0] != checkchr) {
            break;
        }

        i--;
    }

    i = foundi + 1;
    while (i < cmplhndp->nbtoken) {
        if (! strncmp(cmplhndp->token[i], cmplhndp->flp->curpos, strlen(cmplhndp->token[i]))) {
            *indexl = i;
            cmplhndp->flp->curpos += strlen(cmplhndp->token[i]);
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

    if (_is_end(*cmplhndp->flp->curpos)) {
        return(FALSE);
    }
    
    while (TRUE) {
        /* Skip emply lines */
        if (*cmplhndp->flp->curpos == '\\') {
            cmplhndp->flp->curpos++;
            escape = TRUE;
        }

        if (*cmplhndp->flp->curpos == '\r') {
            cmplhndp->flp->curpos++;
        }

        if (*cmplhndp->flp->curpos == '\n') {
            __ac_newline(cmplhndp, !escape);
            cmplhndp->flp->curpos++;
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
    while (_is_blank(*cmplhndp->flp->curpos)) {
        cmplhndp->flp->curpos++;
        reuseb = TRUE;
    }

    /* Escape comments */
    if (*cmplhndp->flp->curpos == '/') {
        if (*(cmplhndp->flp->curpos + 1) == '/') {
            cmplhndp->flp->curpos += 2;

            while (!_is_end(*cmplhndp->flp->curpos)) {
                if (*cmplhndp->flp->curpos == '\n') {
                    if (*(cmplhndp->flp->curpos - 1) != '\\') {
                        break;
                    }
                    else {
                        __ac_newline(cmplhndp, TRUE);
                    }
                }
                cmplhndp->flp->curpos++;
            }

            /* Get next code */
            reuseb = TRUE;
        }
        else if (*(cmplhndp->flp->curpos + 1) == '*') {
            cmplhndp->flp->curpos += 2;

            while (!_is_end(*cmplhndp->flp->curpos)) {
                if (*cmplhndp->flp->curpos == '\n') {
                    __ac_newline(cmplhndp, TRUE);
                }
                else if (*cmplhndp->flp->curpos == '*' && *(cmplhndp->flp->curpos + 1) == '/') {
                    cmplhndp->flp->curpos += 2;
                    break;
                }

                cmplhndp->flp->curpos++;
            }

            /* Get next code */
            reuseb = TRUE;
        }
    }

    if (reuseb) {
        return(__ac_get_next_token(cmplhndp));
    }

    if (_is_end(*cmplhndp->flp->curpos)) {
        return(FALSE);
    }

    /* Initialize new token */
    mem_reset(&cmplhndp->curtoken, sizeof(ac_token_));

    if (cmplhndp->flp->newline) {
        cmplhndp->curtoken.innewline = TRUE;
        cmplhndp->flp->firstpos = cmplhndp->flp->curpos;
        cmplhndp->flp->newline  = FALSE;
    }
       
    cmplhndp->curtoken.isfirst = (cmplhndp->flp->firstpos == cmplhndp->flp->curpos);

    if (*cmplhndp->flp->curpos == '\'') {
        cmplhndp->flp->curpos++;

        if (_is_eol(*cmplhndp->flp->curpos)) {
            ac_error(NEWLINE_IN_CONSTANT);
            return(FALSE);
        }

        if (*cmplhndp->flp->curpos == '\\') {
            cmplhndp->flp->curpos++;
            if (!_is_esc_seq(*cmplhndp->flp->curpos)) {
                ac_warning(UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE);
                chr = '\\';
            }
            else {
                chr = __get_seq_str(*cmplhndp->flp->curpos);
            }
        }
        else {
            chr = *cmplhndp->flp->curpos;
        }

        cmplhndp->flp->curpos++;

        if (*cmplhndp->flp->curpos != '\'') {
            ac_warning(CHAR_TOO_LONG);
            while (*cmplhndp->flp->curpos != '\'') {
                if (_is_eol(*cmplhndp->flp->curpos)) {
                    ac_error(NEWLINE_IN_CONSTANT);
                    return(FALSE);
                }
                cmplhndp->flp->curpos++;
            }
        }

        cmplhndp->curtoken.type = AC_TOKEN_CHAR;
        cmplhndp->curtoken.data.chr = chr;

        /* End of string */
        cmplhndp->flp->curpos++;
    }
    else if (*cmplhndp->flp->curpos == '"') {
        tmppos = ++cmplhndp->flp->curpos;

        while (*cmplhndp->flp->curpos != '\"') {
            if (_is_eol(*cmplhndp->flp->curpos)) {
                ac_error(NEWLINE_IN_CONSTANT);
                return(FALSE); 
            }
            else if (*cmplhndp->flp->curpos == '\\') {
                cmplhndp->flp->curpos++;
                if (*cmplhndp->flp->curpos == '\n') {
                    __ac_newline(cmplhndp, TRUE);
                }
                else {
                    if (!_is_esc_seq(*cmplhndp->flp->curpos)) {
                        ac_warning(UNRECOGNIZED_CHARACTER_ESCAPE_SEQUENCE);
                    }   
                }
            }
            cmplhndp->flp->curpos++;        
        }
                
        push = *cmplhndp->flp->curpos;
        *cmplhndp->flp->curpos = '\0';
        idents = st_dup(tmppos);
        *cmplhndp->flp->curpos = push;

        cmplhndp->curtoken.type = AC_TOKEN_STRING;
        cmplhndp->curtoken.data.strs = idents;

        /* End of string */
        cmplhndp->flp->curpos++;
    }
    else if (_is_debident(*cmplhndp->flp->curpos)) {
        tmppos = cmplhndp->flp->curpos;
        while (_is_ident(*cmplhndp->flp->curpos)) {
            cmplhndp->flp->curpos++;
        }

        push = *cmplhndp->flp->curpos;
        *cmplhndp->flp->curpos = '\0';
        idents = st_dup(tmppos);
        *cmplhndp->flp->curpos = push;

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
    else if (_is_digit(*cmplhndp->flp->curpos)) {
        tmppos = cmplhndp->flp->curpos;

        while (_is_digit(*cmplhndp->flp->curpos)) {
            cmplhndp->flp->curpos++;
        }

        if (*cmplhndp->flp->curpos == '.') {
            isfloatb = TRUE;
            cmplhndp->flp->curpos++;

            while (_is_digit(*cmplhndp->flp->curpos)) {
                cmplhndp->flp->curpos++;
            }
        }

        push = *cmplhndp->flp->curpos;
        *cmplhndp->flp->curpos = '\0';

        if (isfloatb) {
            cmplhndp->curtoken.type = AC_TOKEN_FLOAT;
            cmplhndp->curtoken.data.dbl = atof(tmppos);
        }
        else {
            cmplhndp->curtoken.type      = AC_TOKEN_INTEGER;
            cmplhndp->curtoken.data.intl = atoi(tmppos);
        }

        *cmplhndp->flp->curpos = push;
    }
    else if (__ac_check_token(cmplhndp, &cmplhndp->curtoken.data.intl)) {
        cmplhndp->curtoken.type = AC_TOKEN_TOKEN;
    }
    else {
        cmplhndp->curtoken.type = AC_TOKEN_SYMBOL; 
        cmplhndp->curtoken.data.chr = *cmplhndp->flp->curpos;
        cmplhndp->flp->curpos++;
    }

    return(TRUE);
}

static void __ac_reset_token(pac_token_ tokenp)
{
    if (tokenp->type == AC_TOKEN_IDENT) {
        mem_free(tokenp->data.strs);
    }

    mem_reset(tokenp, sizeof(ac_token_));
}

static int __ac_next_token(pac_cmpl_ cmplhndp)
{
    if (cmplhndp->curtoken.type != AC_TOKEN_NA && !cmplhndp->curtoken.consumed) {
        return(TRUE);
    }
    else {
        __ac_reset_token(&cmplhndp->curtoken);

        return(__ac_get_next_token(cmplhndp));
    }
}

#ifdef __AC_TEST__
extern int __accmpl_exec_module();
int main() 
{
    const char *files = "D:\\ach projects\\accmplgen\\acdir\\test.c";
    const char *logs = "D:\\ach projects\\accmplgen\\acdir\\logtest.log";
    FILE *logp;

    pac_cmpl_ cmpl = __ac_new_compiler();

    /* Test Module */
    __accmpl_exec_module(cmpl);

    __ac_openfile(cmpl, files);

    logp = fopen(logs, "w");

    while (__ac_next_token(cmpl)) {
        switch (cmpl->curtoken.type) {
            case AC_TOKEN_IDENT :
                fprintf(logp, "Ident : %s (%d) \n", cmpl->curtoken.data.strs, cmpl->flp->line);   
                break;
            case AC_TOKEN_KEYWORD:
                fprintf(logp, "Keyword : %s (%d) \n", cmpl->keyword[cmpl->curtoken.data.intl], cmpl->flp->line);
                break; 
            case AC_TOKEN_TOKEN:
                fprintf(logp, "Token : '%s' (%d) \n", cmpl->token[cmpl->curtoken.data.intl], cmpl->flp->line);
                break;
            case AC_TOKEN_SYMBOL:
                fprintf(logp, "Symbol : '%c' (%d) \n", cmpl->curtoken.data.chr, cmpl->flp->line);
                break;
            case AC_TOKEN_STRING:
                fprintf(logp, "String : \"%s\" (%d) \n", cmpl->curtoken.data.strs, cmpl->flp->line);
                break;
            case AC_TOKEN_CHAR:
                fprintf(logp, "Char : '%s' (%d) \n", __ac_print_char(cmpl->curtoken.data.chr), cmpl->flp->line);
                break;
            case AC_TOKEN_INTEGER:
                fprintf(logp, "Integer : '%d' (%d) \n", cmpl->curtoken.data.intl, cmpl->flp->line);
                break;
            case AC_TOKEN_FLOAT:
                fprintf(logp, "Float : '%f' (%d) \n", cmpl->curtoken.data.dbl, cmpl->flp->line);
                break;
        }

        cmpl->curtoken.consumed = TRUE;
    }

}
#endif
