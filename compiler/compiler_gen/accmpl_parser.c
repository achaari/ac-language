
#include "acstd.h"
#include "aclexer.h"

#include "accmpluti.h"

const char *rep = "D:\\ach projects\\accmplgen\\acdir\\";

FILE *tracefp = NULLP;
#ifdef WIN32
#define CMPL_INFO(msg, ...)  {if (tracefp) fprintf(tracefp, msg, __VA_ARGS__); printf(msg, __VA_ARGS__);}
#else
#define CMPL_INFO(...)  {if (tracefp) fprintf(tracefp, __VA_ARGS__); printf(msg, __VA_ARGS__);}
#endif

static ac_cmplgen_ cmplgen;

static pac_step_ ac_get_step(PTR inputp, pac_step_ rootp, pac_step_ *curstepp);

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

static pac_proc_ ac_get_procptr(pac_proc_ proc, int index)
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

static pac_proc_ ac_get_proc(pac_proc_ proclistp, char *names)
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

static pac_step_ ac_new_step(e_step_type_ type, pac_step_ rootp, pac_step_ *curstepp)
{
    pac_step_ step = mem_get(sizeof(ac_step_));

    if (step == NULLP) return(NULL);

    step->type = type;

    if (curstepp) {
        if (*curstepp) (*curstepp)->nextp = step;
        *curstepp = step;
    }

    step->rootp = rootp;

    return(step);
}

static void ac_free_step(pac_step_ *step)
{
    /* ToDo free step data */
    mem_free(*step);
    *step = NULLP;
}

static pac_step_ ac_find_current_procseq(pac_step_ rootp)
{
    while (rootp) {
        if (rootp->type == STEP_TYPE_PROCSEQ) {
            return(rootp);
        }

        rootp = rootp->rootp;
    }

    return(NULLP);
}

static pac_step_ ac_get_control_step(PTR inputp, e_step_type_ type, pac_step_ rootp, pac_step_ *curstepp, char stopchar)
{
    pac_step_ step;

    if (stopchar != NULLC && (peek(inputp) != stopchar))  {
        ac_error(ERROR_EXPECTED, "char", stopchar);
        return(NULLP);
    }

    if (curstepp && !*curstepp && (rootp->type == STEP_TYPE_OPTSEQ)) {
        /* Pure control optional step */
        rootp->type = rootp->headp->type + type - STEP_TYPE_PROCSEQ;
        rootp->headp->type = STEP_TYPE_NAN; /* To avoid freeing datap */
        rootp->stp_datap = rootp->headp->stp_datap;
        mem_copy(&rootp->headp->datap, &rootp->datap, sizeof(rootp->datap));
        ac_free_step(&rootp->headp);

        step = rootp;
    }
    else {
        step = ac_new_step(type, rootp, curstepp);
        if (!step) {
            ac_error(ERROR_MEMORY_ALLOC, "cntrolstep");
            return(NULLP);
        }       
    }

    if (type != STEP_TYPE_PROC_ACCEPT) {
        step->headp = ac_find_current_procseq(rootp);
        if (!step->headp) {
            ac_free_step(&step);
            ac_error(ERROR_NOT_IN_POCSEQ);
            return(NULLP);
        }

        if (type == STEP_TYPE_PROCSEQ_RECALL) {
            step->headp->stepflagb |= (1 << STEP_FLAG_RECALL);
        }
        else if (type == STEP_TYPE_PROCSEQ_BREAK) {
            step->headp->stepflagb |= (1 << STEP_FLAG_BREAK);
        }
    }

    return(step);
}

static pac_step_ ac_get_step_child(PTR inputp, pac_step_ rootp, char stopchar)
{
    pac_step_ nextp = NULLP, firstp = NULLP;

    while (next(inputp)) {
        if (check_char(inputp, stopchar)) {
            break;
        }
        else if (check_char(inputp, '+')) {
            if (rootp->type != STEP_TYPE_OPTSEQ) {
                ac_error(ERROR_UNEXPECTED, "char", '+');
                ac_free_step(&firstp);
                return(NULL);
            }
            else if (!check_char(inputp, ']')) {
                ac_error(ERROR_EXPECTED, "char", ']');
                ac_free_step(&firstp);
                return(NULL);
            }
            rootp->type = STEP_TYPE_OPTLOOP;
            break;
        }
        else if (check_char(inputp, '*')) {
            /* Recall ProcSequence */
            if (! ac_get_control_step(inputp, STEP_TYPE_PROCSEQ_RECALL, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_free_step(&firstp);
                return(NULL);
            }       
        }
        else if (check_char(inputp, ',')) {
            /* Break ProcSequence */
            if (!ac_get_control_step(inputp, STEP_TYPE_PROCSEQ_BREAK, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_free_step(&firstp);
                return(NULL);
            }
        }
        else if (check_char(inputp, ';')) {
            /* Accept current Proc */
            if (!ac_get_control_step(inputp, STEP_TYPE_PROC_ACCEPT, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_free_step(&firstp);
                return(NULL);
            }
        }
        else if (! ac_get_step(inputp, rootp, &nextp)) {
            ac_error(ERROR_INVALID_STEP);
            ac_free_step(&firstp); 
            return(NULL);
        }
        
        if (!firstp) {
            firstp = nextp;
        }
    }

    return(firstp);
}

static int ac_list_add_strptr(char *str, pac_data_list_ *firstp)
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

static int ac_list_add_int(int inl, pac_data_list_ *firstp)
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

static char *ac_list_add_ordered_str(char *str, pac_data_list_ *firstp)
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

static char *ac_get_keyword(PTR inputp, char *keyword)
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

static char *ac_get_token(PTR inputp)
{
    char *identp;

    if (! get_string(inputp, &identp)) {
        ac_error(ERROR_EXPECTED, "string");
        return(NULLP);
    }

    return(ac_add_token(identp));
}

static int ac_add_symbol(char symbolc)
{
    char *identp = mem_get(2);
    identp[0] = symbolc;
    return(ac_add_token(identp) != NULLP);
}

static int ac_add_symbol_list(char *symbols)
{
    for (unsigned int i = 0; i < strlen(symbols); i++) {
        if (! ac_add_symbol(symbols[i]))
            return(FALSE);
    }

    return(TRUE);
}

static pac_step_ ac_step_datap(PTR inputp, pac_step_ stepp)
{
    char *stepdata;
    if (check_char(inputp, '(')) {
        stepdata = get_next_code(inputp, ')', '(');
        if (!stepdata) {
            ac_free_step(&stepp);
            return(NULLP);
        }
        stepp->stp_datap = ac_list_add_ordered_str(stepdata, &cmplgen.stepdata_listp);
        stepp->type += DEF_STEP_DATA_EXT;
    }
    return(stepp);
}

static pac_step_ ac_get_step(PTR inputp, pac_step_ rootp, pac_step_ *curstepp)
{
    pac_proc_ tmptrpc;
    pac_step_ step;
    pac_data_list_ datalist;
    char idents[MAX_LEN], *identp;
    char chr = get(inputp);

    switch (chr) {
        case '{':
            if (rootp && !rootp->headp && rootp->type == STEP_TYPE_OPTSEQ) {
                ac_error(ERROR_INVALID_STEP, "procseq");
                return(NULLP);
            }

            step = ac_new_step(STEP_TYPE_PROCSEQ, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "procseq");
                return(NULLP);
            }
            step->childp = ac_get_step_child(inputp, step, '}');
            if (! step->childp) {
                ac_error(ERROR_MEMORY_ALLOC, "seqchild");
                ac_free_step(&step); 
                return(NULLP);
            }
            return(step);

        case '[':
            if (rootp && !rootp->headp && rootp->type == STEP_TYPE_OPTSEQ) {
                ac_error(ERROR_INVALID_STEP, "optseq");
                return(NULLP);
            }

            step = ac_new_step(STEP_TYPE_OPTSEQ, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "optseq");
                return(NULLP);
            }

            step->headp = ac_get_step(inputp, NULLP, NULLP);
            if (! step->headp) {
                ac_error(ERROR_MEMORY_ALLOC, "optexpr");
                ac_free_step(&step);
                return(NULLP);
            }

            if (check_char(inputp, ']')) {
                /* Optional Step */
                return(step);
            }

            step->childp = ac_get_step_child(inputp, step, ']');
            if (!step->childp && step->type == STEP_TYPE_OPTSEQ) {
                ac_error(ERROR_MEMORY_ALLOC, "seqchild");
                ac_free_step(&step);
                return(NULLP);
            }
            return(step);

        case '\'':
            chr = get(inputp);
            if (chr == EOF || !check_char(inputp, '\'')) {
                ac_error(ERROR_INVALID_SYMBOL);
                return(NULLP);
            }
            else if (! ac_add_symbol(chr)) {
                return(NULLP);
            }
            
            step = ac_new_step(STEP_TYPE_SYMBOL, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "symbol");
                return(NULLP);
            }
            step->datap.chr = chr;
            return(ac_step_datap(inputp, step));

        case '"':
            push_back(inputp);
            step = ac_new_step(STEP_TYPE_TOKEN, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "strcode");
                return(NULLP);
            }
            if (!(step->datap.codes = ac_get_token(inputp))) {
                ac_error(ERROR_EXPECTED, "string");
                ac_free_step(&step);
                return(NULLP);
            }
            return(ac_step_datap(inputp, step));

        case '<' :
            if (! get_identstr(inputp, idents)) {
                ac_error(ERROR_EXPECTED, "identifier");
                return(NULLP);
            }
            else if (!check_char(inputp, '>')) {
                ac_error(ERROR_EXPECTED, "char", '>');
                return(NULLP);
            }

            tmptrpc = ac_get_proc(cmplgen.proc_listp, idents);
            if (!tmptrpc) {
                ac_error(ERROR_UNDEFINED_PROC, idents);
                return(NULLP);
            }
            else if (tmptrpc->type != PROC_TYPE_PROC){
                ac_error(ERROR_UNEXPECTED_PROC, idents);
                return(NULLP);
            }

            step = ac_new_step(STEP_TYPE_EXEC_PROC, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "execproc");
                return(NULLP);
            }

            tmptrpc->inuseb = TRUE;
            step->datap.procp = tmptrpc;
            return(ac_step_datap(inputp, step));

        case '^':
            if (check_char(inputp, '(')) {
                step = ac_new_step(STEP_TYPE_EXEC_ONEKEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "execonekeyword");
                    return(NULLP);
                }

                while (next(inputp)) {
                    if (!(identp = ac_get_keyword(inputp, NULLP))) {
                        ac_error(ERROR_EXPECTED, "identifier");
                        ac_free_step(&step);
                        return(NULLP);
                    }

                    tmptrpc = ac_get_proc(cmplgen.proc_listp, identp);
                    if (!tmptrpc) {
                        ac_error(ERROR_UNDEFINED_PROC, identp);
                        ac_free_step(&step);
                        return(NULLP);
                    }
                    else if (tmptrpc->type != PROC_TYPE_KEYWORD){
                        ac_error(ERROR_UNEXPECTED_PROC, identp);
                        ac_free_step(&step);
                        return(NULLP);
                    }

                    if (! ac_list_add_int(tmptrpc->index, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                        ac_free_step(&step);
                        return(NULLP);
                    }

                    tmptrpc->inuseb = TRUE;

                    if (check_char(inputp, ',')) {
                        continue;
                    }
                    else if (check_char(inputp, ')')) {
                        break;
                    }
                    else {
                        ac_error(ERROR_EXPECTED, "char", ')');
                        ac_free_step(&step);
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = STEP_TYPE_EXEC_KEYWORD;
                    step->datap.procp = ac_get_procptr(cmplgen.proc_listp, datalist->data.inl);
                    mem_free(datalist);
                }
                return(ac_step_datap(inputp, step));
            }
            else {
                if (!ac_get_keyword(inputp, idents)) {
                    ac_error(ERROR_EXPECTED, "identifier");
                    return(NULLP);
                }

                tmptrpc = ac_get_proc(cmplgen.proc_listp, idents);
                if (!tmptrpc) {
                    ac_error(ERROR_UNDEFINED_PROC, idents);
                    return(NULLP);
                }
                else if (tmptrpc->type != PROC_TYPE_KEYWORD){
                    ac_error(ERROR_UNEXPECTED_PROC, idents);
                    return(NULLP);
                }

                step = ac_new_step(STEP_TYPE_EXEC_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "execkeyword");
                    return(NULLP);
                }
                tmptrpc->inuseb = TRUE;
                step->datap.procp = tmptrpc;
                return(ac_step_datap(inputp, step));
            }

        case '~' :
            if (check_char(inputp, '(')) {
                step = ac_new_step(STEP_TYPE_MULTI_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                    return(NULLP);
                }

                while (next(inputp)) {
                    if (!(identp = ac_get_keyword(inputp, NULLP))) {
                        ac_error(ERROR_EXPECTED, "identifier");
                        ac_free_step(&step);
                        return(NULLP);
                    }
                    else if (! ac_list_add_strptr(identp, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                        ac_free_step(&step);
                        return(NULLP);
                    }
                    
                    if (check_char(inputp, ',')) {
                        continue;
                    }
                    else if (check_char(inputp, ')')) {
                        break;
                    }
                    else {
                        ac_error(ERROR_EXPECTED, "char", ')');
                        ac_free_step(&step);
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = STEP_TYPE_KEYWORD;
                    step->datap.codes = datalist->data.codes;
                    mem_free(datalist);
                }
            }
            else {
                step = ac_new_step(STEP_TYPE_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "keyword");
                    return(NULLP);
                }

                if (!(step->datap.codes = ac_get_keyword(inputp, NULLP))) {
                    ac_error(ERROR_EXPECTED, "identifier");
                    ac_free_step(&step); 
                    return(NULLP);
                }           
            }
            return(ac_step_datap(inputp, step));

        case '%' :
            if (peek(inputp) == '"') {
                if (! get_string(inputp, &identp)) {
                    ac_error(ERROR_EXPECTED, "string");
                    return(NULLP);
                }
                else if (!ac_add_symbol_list(identp)) {
                    return(NULLP);
                }

                step = ac_new_step(STEP_TYPE_MULTI_SYMBOL, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "multicode");
                    return(NULLP);
                }

                step->datap.codes = identp;
                return(ac_step_datap(inputp, step));
            }
            else if (check_char(inputp, '(')) {
                step = ac_new_step(STEP_TYPE_MULTI_TOKEN, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "multicode");
                    return(NULLP);
                }
                
                while (next(inputp)) {
                    if (!(identp = ac_get_token(inputp))) {
                        ac_error(ERROR_EXPECTED, "string");
                        ac_free_step(&step); 
                        return(NULLP);
                    }
                    else if (! ac_list_add_strptr(identp, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "multicode");
                        ac_free_step(&step); 
                        return(NULLP);
                    }

                    if (check_char(inputp, ',')) {
                        continue;
                    }
                    else if (check_char(inputp, ')')) {
                        break;
                    }
                    else {
                        ac_error(ERROR_EXPECTED, "char", ')');
                        ac_free_step(&step); 
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = STEP_TYPE_TOKEN;
                    step->datap.codes = datalist->data.codes;
                    mem_free(datalist);
                }
            }
            else {
                ac_error(ERROR_EXPECTED, "char", '(');
                return(NULLP);
            }
            return(ac_step_datap(inputp, step));

        default:
            push_back(inputp);
            if (! get_identstr(inputp, idents)) {
                ac_error(ERROR_EXPECTED, "identifier");
                return(NULLP);
            }

            if (iseqstr(idents, "ident")) {
                step = ac_new_step(STEP_TYPE_GETIDENT, rootp, curstepp);
            }
            else if (iseqstr(idents, "literal")) {
                step = ac_new_step(STEP_TYPE_LITERAL, rootp, curstepp);
            }
            else if (iseqstr(idents, "int")) {
                step = ac_new_step(STEP_TYPE_INTEGER, rootp, curstepp);
            }
            else if (iseqstr(idents, "char")) {
                step = ac_new_step(STEP_TYPE_CHAR, rootp, curstepp);
            }
            else if (iseqstr(idents, "flaot")) {
                step = ac_new_step(STEP_TYPE_FLOAT, rootp, curstepp);
            }
            else if (iseqstr(idents, "string")) {
                step = ac_new_step(STEP_TYPE_STRING, rootp, curstepp);
            }
            else {
                ac_error(ERROR_UNEXPECTED_IDENT, idents);
                return(NULLP);
            }

            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "procseq");
                return(NULLP);
            }
            return(ac_step_datap(inputp, step));
    }

    return(NULLP);
}

static int ac_proc_step(PTR inputp, pac_proc_ proc, pac_step_ *curstep)
{
    pac_step_ step = ac_get_step(inputp, NULLP, NULLP);

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
    pac_step_ step = NULLP;

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
