
#include "acstd.h"
#include "aclexer.h"

#include "accmpluti.h"
#include "accmplstep.h"

const char *rep = "D:\\ach projects\\accmplgen\\acdir\\";

FILE *tracefp = NULLP;
#ifdef WIN32
#define CMPL_INFO(msg, ...)  {if (tracefp) fprintf(tracefp, msg, __VA_ARGS__); printf(msg, __VA_ARGS__);}
#else
#define CMPL_INFO(...)  {if (tracefp) fprintf(tracefp, __VA_ARGS__); printf(msg, __VA_ARGS__);}
#endif

static ac_cmplgen_ cmplgen;
static int proc_index = 0;

static pac_proc_ ac_add_proc(pac_proc_ inproc, int isdecb)
{
    pac_proc_ curprocp = cmplgen.proc_listp;

    if (! curprocp) {
        cmplgen.proc_listp = curprocp = mem_get(sizeof(ac_proc_));
    }
    else {
        /* vérifier si on a déja la même proc */
        while (curprocp) {
            if (iseqstr(curprocp->names, inproc->names)) {
                if (curprocp->isdefb || (curprocp->type != inproc->type)) {
                    ac_error(ERROR_PROC_REDEFINE);
                    return(NULLP);
                }
                else {
                    if (isdecb) curprocp->isdefb = TRUE;
                    return(curprocp);
                }
            }
            else if (curprocp->nextp == NULLP) {
                break;
            }

            curprocp = curprocp->nextp;
        }

        curprocp->nextp = mem_get(sizeof(ac_proc_));
        curprocp = curprocp->nextp;
    }

    if (!curprocp) {
        ac_error(ERROR_MEMORY_ALLOC, "proc");
        return(NULLP);
    }

    mem_copy(inproc, curprocp, sizeof(ac_proc_));
    curprocp->isdefb = isdecb;
    if (curprocp->type != PROC_TYPE_MAIN) {
        curprocp->index = ++proc_index;
    }
    return(curprocp);
}

pac_proc_ ac_get_procptr(pac_proc_ proc, int index)
{
    while (proc) {
        if (proc->index == index) {
            return(proc);
        }
        else if (proc->index > index) {
            return(NULLP);
        }

        proc = proc->nextp;
    }

    return(NULLP);
}

pac_proc_ ac_get_proc(pac_proc_ proclistp, char *names)
{
    pac_proc_ curprocp = proclistp;

    while (curprocp) {
        if (iseqstr(curprocp->names, names)) {
            return(curprocp);
        }

        curprocp = curprocp->nextp;
    }

#ifndef AC_PROC_DATA
    ac_proc_ curproc;
    ac_warning(ERROR_UNDEFINED_PROC, names);
    mem_reset(&curproc, sizeof(ac_proc_));
    strcpy(curproc.names, names);
    return(ac_add_proc(&curproc, FALSE));
#else
    return(NULLP);
#endif    
}

int ac_list_add_strptr(char *str, pac_data_list_ *firstp)
{
    pac_data_list_ strlistp = *firstp;
    
    if (!*firstp) {
        *firstp = mem_get(sizeof(ac_data_list_));
        if (*firstp == NULLP) return(FALSE);
        (*firstp)->data.codes = str;
        return(TRUE);
    }
    else {
        while (strlistp) {
            if (strlistp->data.codes == str) {
                return(TRUE);
            }
            else if (strlistp->nextp == NULLP) {
                strlistp->nextp = mem_get(sizeof(ac_data_list_));
                if (strlistp->nextp == NULLP) return(FALSE);
                strlistp->nextp->data.codes = str;
                return(TRUE);
            }

            strlistp = strlistp->nextp;
        }
    }

    return(FALSE);
}

int ac_list_add_int(int inl, pac_data_list_ *firstp)
{
    pac_data_list_ strlistp = *firstp;
    
    if (!*firstp) {
        *firstp = mem_get(sizeof(ac_data_list_));
        if (*firstp == NULLP) return(FALSE);
        (*firstp)->data.inl = inl;
    }
    else {
        while (strlistp) {
            if (strlistp->data.inl == inl) {
                return(TRUE);
            }
            else if (strlistp->nextp == NULLP) {
                strlistp->nextp = mem_get(sizeof(ac_data_list_));
                if (strlistp->nextp == NULLP) return(FALSE);
                strlistp->nextp->data.inl = inl;
                return(TRUE);
            }

            strlistp = strlistp->nextp;
        }
    }

    return(TRUE);
}

char *ac_list_add_ordered_str(char *str, pac_data_list_ *firstp)
{
    pac_data_list_ curkeyp, keyp = *firstp;

    /* Add to token list */
    curkeyp = mem_get(sizeof(ac_data_list_));

    if (! curkeyp) {
        mem_free(str);
        return(NULLP);
    }

    curkeyp->data.codes = str;

    if (!*firstp || strcmp((*firstp)->data.codes, str) < 0) {
        curkeyp->nextp = *firstp;
        *firstp = curkeyp;
        return(str);
    }

    while (keyp) {
        if (!strcmp(keyp->data.codes, str)) {
            mem_free(str);
            mem_free(curkeyp);
            return(keyp->data.codes);
        }
        else if (keyp->nextp && strcmp(keyp->nextp->data.codes, str) < 0) {
            curkeyp->nextp = keyp->nextp;
            keyp->nextp = curkeyp;
            return(str);
        }
        else if (keyp->nextp == NULLP) {
            break;
        }

        keyp = keyp->nextp;
    }

    if (keyp) {
        keyp->nextp = curkeyp;
    }

    return(str);
}

char *ac_get_keyword(PTR inputp, char *keyword)
{
    char *identp;
    
    if (!(identp = get_ident(inputp))) {
        ac_error(ERROR_EXPECTED, "identifier");
        return(NULLP);
    }

    if (keyword) {
        strcpy(keyword, identp);
    }

    /* Add to keyword list */
    identp = ac_list_add_ordered_str(identp, &cmplgen.keyword_listp);
    if (! identp) {
        ac_error(ERROR_MEMORY_ALLOC, "keyword");
        return(NULLP);
    }

    return(identp);
}

static char *ac_add_token(char *token)
{
    /* Add to token list */
    token = ac_list_add_ordered_str(token, &cmplgen.token_listp);
    if (!token) {
        ac_error(ERROR_MEMORY_ALLOC, "token");
        return(NULLP);
    }

    return(token);
}

char *ac_get_token(PTR inputp)
{
    char *identp;

    if (! get_string(inputp, &identp)) {
        ac_error(ERROR_EXPECTED, "string");
        return(NULLP);
    }

    return(ac_add_token(identp));
}

int ac_add_symbol(char symbolc)
{
    char *identp = mem_get(2);
    identp[0] = symbolc;
    return(ac_add_token(identp) != NULLP);
}

int ac_add_symbol_list(char *symbols)
{
    for (unsigned int i = 0; i < strlen(symbols); i++) {
        if (! ac_add_symbol(symbols[i]))
            return(FALSE);
    }

    return(TRUE);
}

static int ac_proc_step(PTR inputp, pac_proc_ proc, pac_cmplstep_ *curstep)
{
    pac_cmplstep_ step = ac_cmplstep_get_step(&cmplgen, inputp, NULLP, NULLP);

    if (step == NULLP) return(FALSE);

    if (*curstep == NULLP) {
        proc->steplist = *curstep = step;
    }
    else {
        (*curstep)->nextp = step;
        *curstep = (*curstep)->nextp;
    }

    return(TRUE);
}

static int ac_proc(PTR inputp)
{
    ac_proc_ proc, *procp;
    pac_cmplstep_ step = NULLP;

    mem_reset(&proc, sizeof(ac_proc_));

    if (! get_identstr(inputp, proc.names)) {
        ac_error(ERROR_PROC_NAME);
        return(FALSE);
    }

    if (iseqstr(proc.names, "main")) {
        proc.type = PROC_TYPE_MAIN;
    }
    else if (iseqstr(proc.names, "keyword")) {
        proc.type = PROC_TYPE_KEYWORD;
    }

    if (proc.type > PROC_TYPE_MAIN && !get_identstr(inputp, proc.names)) {
        ac_error(ERROR_PROC_NAME);
        return(FALSE);
    }

    if (check_char(inputp, ';')) {
        return(ac_add_proc(&proc, FALSE) != NULLP);
    }
    else if (! check_char(inputp, ':')) {
        ac_error(ERROR_EXPECTED, "char", ':');
        return(FALSE);
    }

    procp = ac_add_proc(&proc, TRUE);
    if (!procp) return(FALSE);

    CMPL_INFO("Processing '%s' Proc ...\n", procp->names);

    while (next(inputp)) {
        if (check_char(inputp, ';')) {
            break;
        }
        else if (!ac_proc_step(inputp, procp, &step)) {
            return(FALSE);
        }
    }

    return(TRUE);
}

static int ac_gen(PTR inputp)
{
    int tmpl = FALSE; 
    char *tmps, idents[MAX_LEN];
    
    while (next(inputp)) {

        switch(peek(inputp)) {
            case '.' :
                consume(inputp);
                if (! ac_proc(inputp)) {
                    return(FALSE);
                }
                break;

            case '%' :
                consume(inputp);
                if (! get_identstr(inputp, idents)) {
                    ac_error(ERROR_EXPECTED, "identifier");
                    return(FALSE);
                }

                if (iseqstr(idents, "module_name")) {
                    cmplgen.module_name = get_ident(inputp);
                    if (!cmplgen.module_name) {
                        ac_error(ERROR_EXPECTED, "module_name");
                        return(FALSE);
                    }
                }
                else if (iseqstr(idents, "include")) {
                    while (TRUE) {
                        if (!get_string(inputp, &tmps)) {
                            ac_error(ERROR_EXPECTED, "header_file");
                            return(FALSE);
                        }
                        else if (!ac_list_add_strptr(tmps, &cmplgen.headers)) {
                            ac_error(ERROR_MEMORY_ALLOC, "header_file");
                            return(FALSE);
                        }

                        if (!check_char(inputp, ',')) {
                            break;
                        }
                    }
                }
                else {
                    ac_warning(ERROR_UNEXPECTED, "ident", ident);
                }
                break;

            default :
                ac_error(ERROR_UNEXPECTED, "char" peek(inputp));
                return(FALSE);
        }
    }

    return(TRUE);
}

int main(int argc, char **argv)
{
    char *module = "testac";
    PTR inputp;
    char filenames[256]; 
    FILE *outputp;
    int cmplok, indexl;
    pac_proc_ proc;

    /* init cmplgen */
    mem_reset(&cmplgen, sizeof(ac_cmplgen_));

    for (indexl = 1; indexl < argc; indexl++) {
        if (argv[indexl][0] != '-') {
            continue;
        }

        switch (argv[indexl][1]) {
            case 'm': case 'M':
                module = argv[indexl] + 2;
                break;
        }
    }
    
    sprintf(filenames, "%s%s.log", rep, module);
    tracefp = fopen(filenames, "w");

    sprintf(filenames, "%s%s.ac", rep, module);
    inputp = opensource(filenames);

    sprintf(filenames, "%sacgen.c", rep);
    outputp = fopen(filenames, "w");
    
    cmplok = ac_gen(inputp);

    closesource(&inputp);

    if (cmplok) {
        proc = cmplgen.proc_listp;
        while (proc) {
            if (proc->inuseb && !proc->isdefb) {
                ac_error(ERROR_UNDEF_PROC_USED, proc->names);
                cmplok = FALSE;
            }
            proc = proc->nextp;
        }

        if (cmplok) {
            ac_print_compiler(&cmplgen, outputp, proc_index);
        }
    }
    
    fclose(outputp);
    fclose(tracefp);
    return(cmplok);
}
