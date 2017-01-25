
#include "acstd.h"

#define _is_upper(a)        (((a) >= 'A') && ((a) <= 'Z'))
#define _is_lower(a)        (((a) >= 'a') && ((a) <= 'z'))
#define _is_chr(a)          (_is_upper(a) || _is_lower(a))
#define _is_digit(a)        (((a) >= '0') && ((a) <= '9'))
#define _is_debident(a)     (_is_chr(a) || ((a) == '_'))
#define _is_ident(a)        (_is_digit(a) || _is_debident(a))
#define _is_blank(a)        (((a) == ' ') || ((a) == '\t') || ((a) == '\v') || ((a) == '\a'))

typedef struct file_ {
    char *code;
    char *name;
} ac_file_, *pac_file_;

typedef struct lexer_ {
    pac_file_ file;
    //int       line;
    char      *current;
    //char      *debline;
    char      *savepos;
    int       lasstat; 
    /* For user process read */
    PFI       userreadfp;
    PTR       userreaddatap;
} ac_lexer_, *pac_lexer_;

static pac_file_ openfile(const char *filename)
{
    char *flc;
    size_t len;
    pac_file_ flp;
    FILE *fp = fopen(filename, "rb");    
    if (! fp) return(NULLP);

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    if (len <= 0) {fclose(fp); return(NULLP);}

    flc = mem_get(len+1);
    if (! flc) {fclose(fp); return(NULLP);}

    fseek(fp, 0, SEEK_SET);
    if (fread(flc, sizeof(char), len, fp) < len) {
        mem_free(flc); fclose(fp); return(NULLP);
    }

    flp = mem_get(sizeof(ac_file_));
    if (! flp) {mem_free(flc); fclose(fp); return(NULLP);}

    flp->code = flc;
    flp->name = st_dup(filename);
    fclose(fp); 
    return(flp);
}

static void closefile(pac_file_ *filep)
{
    mem_free((*filep)->code);
    mem_free((*filep)->name);
    mem_free(*filep);
    *filep = NULLP;
}

static int __next(pac_lexer_ srcp, int forcenewline) 
{
    int newline   = FALSE;
    int incomment = FALSE;
    int done      = FALSE;

    while (! done) {
        switch(*srcp->current) {
            case '\0' :
                if (incomment == 2) {
                    /* end of file on comment */
                }
                return(FALSE);

            case '\r' :
                srcp->current++;  
                continue;

            case '\\' :
                if (*(srcp->current+1) != '\n') {
                    if (incomment || forcenewline) {
                        srcp->current++;
                        continue;
                    }
                    done = TRUE;
                    break;
                }
                /* No break, continue to case '\n' */

            case '\n' :
                srcp->current++;
                newline = TRUE;
                if (incomment == 1) {
                    incomment = 0;
                }
                if (forcenewline && !incomment) {
                    forcenewline = FALSE;
                }
                continue;

            case '/':
                if (incomment) {
                    srcp->current++;
                    continue;
                }
                else if (*(srcp->current+1) == '*') {
                    incomment = 2;
                    srcp->current +=2;
                    continue;
                }
                else if (*(srcp->current+1) == '/') {
                    incomment = 1;
                    srcp->current +=2;
                    continue;
                }
                else if (forcenewline) {
                    srcp->current++;
                    continue;
                }
                done = TRUE;
                break; 

            case '*':
                if (incomment) {
                    if (*(srcp->current+1) == '/') {
                        incomment = 0;
                        srcp->current +=2;
                        if (forcenewline && newline) {
                            done = TRUE;
                            break;
                        }
                    }
                    else {
                        srcp->current++;
                    }
                    continue;
                }
                else if (forcenewline) {
                    srcp->current++;
                    continue;
                }
                done = TRUE;
                break;

            case ' ':
            case '\t':
            case '\v':
            case '\a':
                while (_is_blank(*srcp->current)) {
                    srcp->current++;
                }
                continue;

            default :
                if (srcp->userreadfp) {
                    if ((srcp->userreadfp)(srcp, incomment, srcp->userreaddatap)) {
                        if (incomment || forcenewline) {
                            continue;
                        }
                    }
                }
                
                if (incomment || forcenewline) {
                    srcp->current++;
                    continue;
                }

                done = TRUE;
                break;
        }
    }

    return(TRUE);
}

/**************************************/
pac_lexer_ opensource(const char *filename)
{
    pac_lexer_ srcp = mem_get(sizeof(ac_lexer_));

    if (! srcp) return(NULLP);

    srcp->file = openfile(filename);
    if (! srcp->file) {
        mem_free(srcp);
        srcp = NULLP;
        return(NULLP);
    }

    srcp->current = srcp->file->code;

    return(srcp);
}

void set_userfp(pac_lexer_ srcp, PFI fp, PTR datap)
{
    srcp->userreadfp    = fp;
    srcp->userreaddatap = datap;
}

void closesource(pac_lexer_ *source) 
{
    closefile(&(*source)->file);
    mem_free(*source);
    *source = NULLP; 
}

const char *get_str(pac_lexer_ source) 
{
    return(source->current);
}

int next(pac_lexer_ srcp) 
{
    return(__next(srcp, FALSE));
}

char peek(pac_lexer_ srcp) 
{
    if (! next(srcp)) return EOF;
    return(*(srcp->current));
}

char get(pac_lexer_ srcp) 
{
    char chr;
    if (! next(srcp)) return EOF;
    chr = *(srcp->current);
    srcp->current++;
    return(chr);
}

char peek_char(pac_lexer_ srcp)
{
    return(*(srcp->current));
}

void consume(pac_lexer_ srcp) 
{
    srcp->current++;
}

void push_back(pac_lexer_ srcp)
{
    srcp->current--;
}

void skip(pac_lexer_ srcp, int nbchar)
{
    srcp->current += nbchar;
}

void save(pac_lexer_ srcp)
{
    if (next(srcp)) {
        srcp->savepos = srcp->current;
    }

    srcp->lasstat = FALSE;
}

int accept(pac_lexer_ srcp)
{
    srcp->savepos = srcp->current;
    return(TRUE);
}

int rollback(pac_lexer_ srcp)
{
    srcp->current = srcp->savepos;
    srcp->lasstat = FALSE;
    return(FALSE);
}

int get_laststate(pac_lexer_ srcp)
{
    return(srcp->lasstat);
}

int skipline(pac_lexer_ srcp) 
{
    return(__next(srcp, TRUE));
}

char *get_ident(pac_lexer_ srcp)
{
    char *ident, *first, push;

    if (! next(srcp) || ! _is_debident(*srcp->current)) {
        return(NULLP);
    }

    first = srcp->current;

    while(_is_ident(*srcp->current)) srcp->current++;

    push = *srcp->current;
    *srcp->current = '\0';
    ident = st_dup(first);
    *srcp->current = push;
    srcp->lasstat = TRUE;
    return(ident);
}

char *get_identstr(pac_lexer_ srcp, char *ident)
{
    char *first, push;

    *ident = '\0';

    if (! next(srcp) || ! _is_debident(*srcp->current)) {
        return(NULLP);
    }

    first = srcp->current;

    while(_is_ident(*srcp->current)) srcp->current++;

    push = *srcp->current;
    *srcp->current = '\0';
    strcpy(ident, first);
    *srcp->current = push;
    srcp->lasstat = TRUE;
    return(ident);
}

int find_code(pac_lexer_ srcp, const char *code)
{
    char *tmp   = strstr(srcp->current, code);
    char *nline = strchr(srcp->current, '\n');

    if (! tmp || (nline && (nline < tmp))) return(FALSE);

    srcp->current = tmp + strlen(code);
    srcp->lasstat = TRUE;
    return(TRUE);
}

int find_ident(pac_lexer_ srcp, const char *code)
{
    char *tmp   = strstr(srcp->current, code);
    char *nline = strchr(srcp->current, '\n');
    char *oldp;

    if (! tmp || (nline && (nline < tmp))) return(FALSE);

    if (_is_ident(*(tmp-1)) || _is_ident(*(tmp + strlen(code)))) {
        oldp = srcp->current;
        srcp->current = tmp + strlen(code);
        if (find_ident(srcp, code)) {
            return(TRUE);
        }
        srcp->current = oldp;
        return(FALSE);
    }

    srcp->current = tmp + strlen(code);
    srcp->lasstat = TRUE;
    return(TRUE);
}

int get_int(pac_lexer_ srcp, int *valp)
{
    char *first, push;
    int isneg = FALSE;

    if (! next(srcp)) {
        return(FALSE);
    }

    if (*srcp->current == '-') {
        isneg = TRUE;
        srcp->current++;
    }

    if (! _is_digit(*srcp->current)) {
        return(FALSE);
    }

    first = srcp->current;
    
    while(_is_digit(*srcp->current)) 
        srcp->current++;

    if (*srcp->current == '.') {
        return(FALSE);
    }

    push = *srcp->current;
    *srcp->current = '\0';
    *valp = atoi(first);
    *srcp->current = push;
    srcp->lasstat = TRUE;

    if (isneg) *valp *= -1;

    return(TRUE);
}

int check_ident(pac_lexer_ srcp, char *valp)
{
    if (! next(srcp)) {
        return(FALSE);
    }

    if (strncmp(srcp->current, valp, strlen(valp))) {
        return(FALSE);
    }

    else if (_is_ident(*(srcp->current + strlen(valp)))) {
        return(FALSE);
    }

    srcp->current += strlen(valp);
    srcp->lasstat = TRUE;

    return(TRUE);
}

int check_code(pac_lexer_ srcp, char *valp)
{
    if (! next(srcp)) {
        return(FALSE);
    }

    if (strncmp(srcp->current, valp, strlen(valp))) {
        return(FALSE);
    }

    srcp->current += strlen(valp);
    srcp->lasstat = TRUE;

    return(TRUE);
}

char *get_next_code(pac_lexer_ srcp, char split, char reset)
{
    char peekc, *cur, *next;
    int idx = 1;

    cur = srcp->current;
    while (idx > 0) {
        if (cur == NULLP || *cur == '\0' || *cur == EOF) {
            break;
        }
        else if (*cur == reset) {
            idx++; cur++;
        }
        else if (*cur == split) {
            idx--; cur++;
        }
        else if (*cur == '/' && *(cur+1) == '*') {
            cur = strstr(cur, "/*");
        }
        else if (*cur == '/' && *(cur + 1) == '/') {
            cur = strchr(cur, '\n');
        }
        else {
            cur++;
        }
    }

    if (cur == NULLP) {
        return(NULLP);
    }

    peekc = *(cur-1); *(cur-1) = '\0';
    next = st_dup(srcp->current);
    *(cur - 1) = peekc;
    srcp->current = cur;
    return(next);
}

int check_char(pac_lexer_ srcp, char valc)
{
    if (! next(srcp)) {
        return(FALSE);
    }

    if (*srcp->current != valc) {
        return(FALSE);
    }

    srcp->current++;
    srcp->lasstat = TRUE;

    return(TRUE);
}

int get_string(pac_lexer_ srcp, char **codes)
{
    char *tmps, *firsts;

    if (! next(srcp)) {
        return(FALSE);
    }

    if (*srcp->current != '\"') return(FALSE);

    firsts = tmps = srcp->current + 1;

    while(*tmps != '\"') {
        if (*tmps == '\0') {
            return(FALSE);
        }
        else if (*tmps == '\n' && *(tmps-1) != '\\') {
            return(FALSE);
        }
        else {
            tmps++;
        }
    }

    *tmps = '\0';
    *codes = st_dup(firsts);
    *tmps = '\"';
    srcp->current = tmps + 1;

    srcp->lasstat = TRUE;

    return(TRUE);
}
