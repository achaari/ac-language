
#include "acstd.h"
#include "aclexer.h"
#include "accmplstep.h"

static pac_cmplstep_ ac_cmplstep_new_step(e_cmplstep_type_ type, pac_cmplstep_ rootp, pac_cmplstep_ *curstepp)
{
    pac_cmplstep_ step = mem_get(sizeof(ac_cmplstep_));

    if (step == NULLP) return(NULL);

    step->type = type;

    if (curstepp) {
        if (*curstepp) (*curstepp)->nextp = step;
        *curstepp = step;
    }

    step->rootp = rootp;

    return(step);
}

static void ac_cmplstep_free_step(pac_cmplstep_ *step)
{
    /* ToDo free step data */
    mem_free(*step);
    *step = NULLP;
}

static pac_cmplstep_ ac_find_current_procseq(pac_cmplstep_ rootp)
{
    while (rootp) {
        if (rootp->type == CMPLSTEP_TYPE_PROCSEQ) {
            return(rootp);
        }

        rootp = rootp->rootp;
    }

    return(NULLP);
}

static pac_cmplstep_ ac_cmplstep_get_control_step(pac_cmplgen_ cmplgenp, PTR inputp, e_cmplstep_type_ type, e_cmplstep_ext_ extl, 
                                                  pac_cmplstep_ rootp, pac_cmplstep_ *curstepp, char stopchar)
{
    pac_cmplstep_ step;

    if (stopchar != NULLC && (peek(inputp) != stopchar))  {
        ac_error(ERROR_EXPECTED, "char", stopchar);
        return(NULLP);
    }

    if (curstepp && !*curstepp && (rootp->type == CMPLSTEP_TYPE_OPTSEQ)) {
        /* Pure control optional step */
        rootp->type = rootp->headp->type;
        rootp->ext  = extl;
        rootp->headp->type = CMPLSTEP_TYPE_NAN; /* To avoid freeing datap */
        rootp->stp_datap = rootp->headp->stp_datap;
        mem_copy(&rootp->headp->datap, &rootp->datap, sizeof(rootp->datap));
        ac_cmplstep_free_step(&rootp->headp);

        step = rootp;
    }
    else {
        step = ac_cmplstep_new_step(type, rootp, curstepp);
        if (!step) {
            ac_error(ERROR_MEMORY_ALLOC, "cntrolstep");
            return(NULLP);
        }       
    }

    if (type == CMPLSTEP_TYPE_PROCSEQ_RECALL || type == CMPLSTEP_TYPE_PROCSEQ_BREAK) {
        step->headp = ac_find_current_procseq(rootp);
        if (! step->headp) {
            ac_cmplstep_free_step(&step);
            ac_error(ERROR_NOT_IN_POCSEQ);
            return(NULLP);
        }

        if (type == CMPLSTEP_TYPE_PROCSEQ_RECALL) {
            step->headp->stepflagb |= (1 << CMPLSTEP_FLAG_RECALL);
        }
        else if (type == CMPLSTEP_TYPE_PROCSEQ_BREAK) {
            step->headp->stepflagb |= (1 << CMPLSTEP_FLAG_BREAK);
        }
    }

    return(step);
}

static pac_cmplstep_ ac_cmplstep_get_step_child(pac_cmplgen_ cmplgenp, PTR inputp, pac_cmplstep_ rootp, char stopchar)
{
    pac_cmplstep_ nextp = NULLP, firstp = NULLP;

    while (next(inputp)) {
        if (check_char(inputp, stopchar)) {
            break;
        }
        else if (check_char(inputp, '+')) {
            if (rootp->type != CMPLSTEP_TYPE_OPTSEQ) {
                ac_error(ERROR_UNEXPECTED, "char", '+');
                ac_cmplstep_free_step(&firstp);
                return(NULL);
            }
            else if (!check_char(inputp, ']')) {
                ac_error(ERROR_EXPECTED, "char", ']');
                ac_cmplstep_free_step(&firstp);
                return(NULL);
            }
            rootp->type = CMPLSTEP_TYPE_OPTLOOP;
            break;
        }
        else if (check_char(inputp, '*')) {
            /* Recall ProcSequence */
            if (! ac_cmplstep_get_control_step(cmplgenp, inputp, CMPLSTEP_TYPE_PROCSEQ_RECALL, CMPLTSTEP_EXT_RECALL, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_cmplstep_free_step(&firstp);
                return(NULL);
            }       
        }
        else if (check_char(inputp, ',')) {
            /* Break ProcSequence */
            if (! ac_cmplstep_get_control_step(cmplgenp, inputp, CMPLSTEP_TYPE_PROCSEQ_BREAK, CMPLTSTEP_EXT_BREAK, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_cmplstep_free_step(&firstp);
                return(NULL);
            }
        }
        else if (check_char(inputp, ';')) {
            /* Accept current Proc */
            if (! ac_cmplstep_get_control_step(cmplgenp, inputp, CMPLSTEP_TYPE_PROC_ACCEPT, CMPLTSTEP_EXT_ACCEPT, rootp, &nextp, stopchar)) {
                ac_error(ERROR_INVALID_STEP);
                ac_cmplstep_free_step(&firstp);
                return(NULL);
            }
        }
        else if (! ac_cmplstep_get_step(cmplgenp, inputp, rootp, &nextp)) {
            ac_error(ERROR_INVALID_STEP);
            ac_cmplstep_free_step(&firstp); 
            return(NULL);
        }
        
        if (!firstp) {
            firstp = nextp;
        }
    }

    return(firstp);
}

static pac_cmplstep_ ac_cmplstep_get_execstep_ext(pac_cmplgen_ cmplgenp, PTR inputp, pac_cmplstep_ rootp, pac_cmplstep_ *curstepp)
{
    pac_proc_ tmptrpc;
    pac_cmplstep_ step;
    pac_data_list_ datalist;
    char idents[MAX_LEN], *identp;
    char chr = get(inputp);

    switch (chr) {
        case '\'':
            chr = get(inputp);
            if (chr == EOF || !check_char(inputp, '\'')) {
                ac_error(ERROR_INVALID_SYMBOL);
                return(NULLP);
            }
            else if (!ac_add_symbol(chr)) {
                return(NULLP);
            }

            step = ac_cmplstep_new_step(CMPLSTEP_TYPE_SYMBOL, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "symbol");
                return(NULLP);
            }
            step->datap.chr = chr;
            return(step);

        case '"':
            push_back(inputp);
            step = ac_cmplstep_new_step(CMPLSTEP_TYPE_TOKEN, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "strcode");
                return(NULLP);
            }
            if (!(step->datap.codes = ac_get_token(inputp))) {
                ac_error(ERROR_EXPECTED, "string");
                ac_cmplstep_free_step(&step);
                return(NULLP);
            }
            return(step);

        case '<':
            if (!get_identstr(inputp, idents)) {
                ac_error(ERROR_EXPECTED, "identifier");
                return(NULLP);
            }
            else if (!check_char(inputp, '>')) {
                ac_error(ERROR_EXPECTED, "char", '>');
                return(NULLP);
            }

            tmptrpc = ac_get_proc(cmplgenp->proc_listp, idents);
            if (!tmptrpc) {
                ac_error(ERROR_UNDEFINED_PROC, idents);
                return(NULLP);
            }
            else if (tmptrpc->type != PROC_TYPE_PROC){
                ac_error(ERROR_UNEXPECTED_PROC, idents);
                return(NULLP);
            }

            step = ac_cmplstep_new_step(CMPLSTEP_TYPE_EXEC_PROC, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "execproc");
                return(NULLP);
            }

            tmptrpc->inuseb = TRUE;
            step->datap.procp = tmptrpc;
            return(step);

        case '^':
            if (check_char(inputp, '(')) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_EXEC_ONEKEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "execonekeyword");
                    return(NULLP);
                }

                while (next(inputp)) {
                    if (!(identp = ac_get_keyword(inputp, NULLP))) {
                        ac_error(ERROR_EXPECTED, "identifier");
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }

                    tmptrpc = ac_get_proc(cmplgenp->proc_listp, identp);
                    if (!tmptrpc) {
                        ac_error(ERROR_UNDEFINED_PROC, identp);
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                    else if (tmptrpc->type != PROC_TYPE_KEYWORD){
                        ac_error(ERROR_UNEXPECTED_PROC, identp);
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }

                    if (!ac_list_add_int(tmptrpc->index, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                        ac_cmplstep_free_step(&step);
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
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = CMPLSTEP_TYPE_EXEC_KEYWORD;
                    step->datap.procp = ac_get_procptr(cmplgenp->proc_listp, datalist->data.inl);
                    mem_free(datalist);
                }
                return(step);
            }
            else {
                if (!ac_get_keyword(inputp, idents)) {
                    ac_error(ERROR_EXPECTED, "identifier");
                    return(NULLP);
                }

                tmptrpc = ac_get_proc(cmplgenp->proc_listp, idents);
                if (!tmptrpc) {
                    ac_error(ERROR_UNDEFINED_PROC, idents);
                    return(NULLP);
                }
                else if (tmptrpc->type != PROC_TYPE_KEYWORD){
                    ac_error(ERROR_UNEXPECTED_PROC, idents);
                    return(NULLP);
                }

                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_EXEC_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "execkeyword");
                    return(NULLP);
                }
                tmptrpc->inuseb = TRUE;
                step->datap.procp = tmptrpc;
                return(step);
            }

        case '~':
            if (check_char(inputp, '(')) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_MULTI_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                    return(NULLP);
                }

                while (next(inputp)) {
                    if (!(identp = ac_get_keyword(inputp, NULLP))) {
                        ac_error(ERROR_EXPECTED, "identifier");
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                    else if (!ac_list_add_strptr(identp, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "keyliste");
                        ac_cmplstep_free_step(&step);
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
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = CMPLSTEP_TYPE_KEYWORD;
                    step->datap.codes = datalist->data.codes;
                    mem_free(datalist);
                }
            }
            else {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_KEYWORD, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "keyword");
                    return(NULLP);
                }

                if (!(step->datap.codes = ac_get_keyword(inputp, NULLP))) {
                    ac_error(ERROR_EXPECTED, "identifier");
                    ac_cmplstep_free_step(&step);
                    return(NULLP);
                }
            }
            return(step);

        case '%':
            if (peek(inputp) == '"') {
                if (!get_string(inputp, &identp)) {
                    ac_error(ERROR_EXPECTED, "string");
                    return(NULLP);
                }
                else if (! ac_add_symbol_list(identp)) {
                    return(NULLP);
                }

                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_MULTI_SYMBOL, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "multicode");
                    return(NULLP);
                }

                step->datap.codes = identp;
                return(step);
            }
            else if (check_char(inputp, '(')) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_MULTI_TOKEN, rootp, curstepp);
                if (!step) {
                    ac_error(ERROR_MEMORY_ALLOC, "multicode");
                    return(NULLP);
                }

                while (next(inputp)) {
                    if (!(identp = ac_get_token(inputp))) {
                        ac_error(ERROR_EXPECTED, "string");
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                    else if (!ac_list_add_strptr(identp, &step->datap.strlistp)) {
                        ac_error(ERROR_MEMORY_ALLOC, "multicode");
                        ac_cmplstep_free_step(&step);
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
                        ac_cmplstep_free_step(&step);
                        return(NULLP);
                    }
                }

                /* Check list count */
                if (step->datap.strlistp->nextp == NULLP) {
                    datalist = step->datap.strlistp;
                    step->type = CMPLSTEP_TYPE_TOKEN;
                    step->datap.codes = datalist->data.codes;
                    mem_free(datalist);
                }
            }
            else {
                ac_error(ERROR_EXPECTED, "char", '(');
                return(NULLP);
            }
            return(step);

        default:
            push_back(inputp);
            if (!get_identstr(inputp, idents)) {
                ac_error(ERROR_EXPECTED, "identifier");
                return(NULLP);
            }

            if (iseqstr(idents, "ident")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_GETIDENT, rootp, curstepp);
            }
            else if (iseqstr(idents, "literal")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_LITERAL, rootp, curstepp);
            }
            else if (iseqstr(idents, "int")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_INTEGER, rootp, curstepp);
            }
            else if (iseqstr(idents, "char")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_CHAR, rootp, curstepp);
            }
            else if (iseqstr(idents, "flaot")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_FLOAT, rootp, curstepp);
            }
            else if (iseqstr(idents, "string")) {
                step = ac_cmplstep_new_step(CMPLSTEP_TYPE_STRING, rootp, curstepp);
            }
            else {
                ac_error(ERROR_UNEXPECTED_IDENT, idents);
                return(NULLP);
            }

            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "procseq");
                return(NULLP);
            }
            return(step);
    }

    return(NULLP);
}

static pac_cmplstep_ ac_cmplstep_get_execstep(pac_cmplgen_ cmplgenp, PTR inputp, pac_cmplstep_ rootp, pac_cmplstep_ *curstepp)
{
    char *stepdata; 
    pac_cmplstep_ stepp = ac_cmplstep_get_execstep_ext(cmplgenp, inputp, rootp, curstepp);
    
    if (!stepp) {
        return(NULLP);
    }

    if (check_char(inputp, '(')) {
        stepdata = get_next_code(inputp, ')', '(');
        if (!stepdata) {
            ac_cmplstep_free_step(&stepp);
            return(NULLP);
        }
        stepp->stp_datap = ac_list_add_ordered_str(stepdata, &cmplgenp->stepdata_listp);
        stepp->type += 1 /* DEF_STEP_DATA_EXT */;
    }

    return(stepp);
}

pac_cmplstep_ ac_cmplstep_get_step(pac_cmplgen_ cmplgenp, PTR inputp, pac_cmplstep_ rootp, pac_cmplstep_ *curstepp)
{
    pac_cmplstep_ step;
    char chr = get(inputp);

    switch (chr) {
        case '{':
            step = ac_cmplstep_new_step(CMPLSTEP_TYPE_PROCSEQ, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "procseq");
                return(NULLP);
            }
            step->childp = ac_cmplstep_get_step_child(cmplgenp, inputp, step, '}');
            if (!step->childp) {
                ac_error(ERROR_MEMORY_ALLOC, "seqchild");
                ac_cmplstep_free_step(&step);
                return(NULLP);
            }
            return(step);

        case '[':
            step = ac_cmplstep_new_step(CMPLSTEP_TYPE_OPTSEQ, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "optseq");
                return(NULLP);
            }

            step->headp = ac_cmplstep_get_execstep(cmplgenp, inputp, NULLP, NULLP);
            if (!step->headp) {
                ac_error(ERROR_MEMORY_ALLOC, "optexpr");
                ac_cmplstep_free_step(&step);
                return(NULLP);
            }

            if (check_char(inputp, ']')) {
                /* Optional Step */
                return(step);
            }

            step->childp = ac_cmplstep_get_step_child(cmplgenp, inputp, step, ']');
            if (!step->childp && step->type == CMPLSTEP_TYPE_OPTSEQ) {
                ac_error(ERROR_MEMORY_ALLOC, "seqchild");
                ac_cmplstep_free_step(&step);
                return(NULLP);
            }
            return(step);

        default:
            push_back(inputp);
            return(ac_cmplstep_get_execstep(cmplgenp, inputp, rootp, curstepp));
    }
}
