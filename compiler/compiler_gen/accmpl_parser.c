
#include "acstd.h"
#include "aclexer.h"

#define MAX_LEN 100

#define ac_error(a, ...)

const char *rep = "D:\\ach projects\\accmplgen\\acdir\\";

typedef enum {
    PROC_TYPE_PROC = 0,
    PROC_TYPE_MAIN
} e_proc_type_;

#define DEF_STEP(a) STEP_TYPE_##a, STEP_TYPE_ACCEPT_IF_##a,  STEP_TYPE_RECALL_IF_##a, STEP_TYPE_BREAK_IF_##a

typedef enum {
    STEP_TYPE_NAN = 0,
    STEP_TYPE_PROCSEQ,
    STEP_TYPE_PROCSEQ_ACCEPT,
    STEP_TYPE_PROCSEQ_RECALL,
    STEP_TYPE_PROCSEQ_BREAK,
    STEP_TYPE_OPTSEQ,
    DEF_STEP(GETIDENT),
    DEF_STEP(STRCODE),   
} e_step_type_;

typedef struct step_ {
    e_step_type_ type;
    union {
        char idents[MAX_LEN];
        char *codes;
    } datap;
    struct step_ *rootp;
    struct step_ *headp;
    struct step_ *childp;
    struct step_ *nextp;
} ac_step_, *pac_step_;

typedef struct proc_{
    char         names[MAX_LEN];
    e_proc_type_ type;
    INT32        isdefb;
    pac_step_    steplist;
    struct proc_ *nextp;
} ac_proc_, *pac_proc_;

static pac_proc_ proc_listp = NULLP;

static pac_step_ ac_get_step(PTR inputp, pac_step_ rootp, pac_step_ *curstepp);


static pac_proc_ ac_add_proc(pac_proc_ inproc, int isdecb)
{
    pac_proc_ curprocp = proc_listp;

    if (! curprocp) {
        proc_listp = curprocp = mem_get(sizeof(ac_proc_));
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

    return(curprocp);
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
        rootp->headp->type = STEP_TYPE_NAN; /* pour ne pas libérer le datap */
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

    step->headp = ac_find_current_procseq(rootp);
    if (!step->headp) {
        ac_free_step(&step);
        ac_error(ERROR_NOT_IN_POCSEQ);
        return(NULLP);
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
        else if (check_char(inputp, '*')) {
            /* Recall ProcSequence */
            if (! ac_get_control_step(inputp, STEP_TYPE_PROCSEQ_RECALL, rootp, &nextp, stopchar)) {
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

static pac_step_ ac_get_step(PTR inputp, pac_step_ rootp, pac_step_ *curstepp)
{
    pac_step_ step;
    char idents[MAX_LEN];
    char chr = get(inputp);

    switch (chr) {
        case '{':
            if (rootp && rootp->type == STEP_TYPE_OPTSEQ) {
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
            if (rootp && rootp->type == STEP_TYPE_OPTSEQ) {
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
            step->childp = ac_get_step_child(inputp, step, ']');
            if (!step->childp && step->type == STEP_TYPE_OPTSEQ) {
                ac_error(ERROR_MEMORY_ALLOC, "seqchild");
                ac_free_step(&step);
                return(NULLP);
            }
            return(step);

        case '"':
            push_back(inputp);
            step = ac_new_step(STEP_TYPE_STRCODE, rootp, curstepp);
            if (!step) {
                ac_error(ERROR_MEMORY_ALLOC, "strcode");
                return(NULLP);
            }
            if (!get_string(inputp, &step->datap.codes)) {
                ac_free_step(&step);
                return(NULLP);
            }
            return(step);
            break;

        default:
            push_back(inputp);
            if (! get_identstr(inputp, idents)) {
                ac_error(ERROR_EXPECTED, "identifier");
                return(NULLP);
            }

            if (iseqstr(idents, "ident")) {
                step = ac_new_step(STEP_TYPE_GETIDENT, rootp, curstepp);
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

    if (proc.type && !get_identstr(inputp, proc.names)) {
        ac_error(ERROR_PROC_NAME);
        return(FALSE);
    }

    if (check_char(inputp, ';')) {
        return(ac_add_proc(&proc, FALSE) != NULLP);
    }
    else if (! check_char(inputp, ':')) {
        ac_error(ERROR_EXPECTED, ":");
        return(FALSE);
    }

    procp = ac_add_proc(&proc, TRUE);
    if (!procp) return(FALSE);

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
    INT32 tmpl = FALSE;     
    
    while (next(inputp)) {

        switch(peek(inputp)) {
            case '.' :
                consume(inputp);
                if (! ac_proc(inputp)) {
                    return(FALSE);
                }
                break;
        }
    }

    return(TRUE);
}


int main(int argc, char **argv)
{
    PTR inputp;
    FILE *outputp;
    char filenames[256];
    
    sprintf(filenames, "%s%s.ac", rep, "testac");
    inputp = opensource(filenames);

    sprintf(filenames, "%s%s.c", rep, "testac");
    outputp = fopen(filenames, "w");

    ac_gen(inputp);    

    closesource(&inputp);
    fclose(outputp);
    return(TRUE);
}
