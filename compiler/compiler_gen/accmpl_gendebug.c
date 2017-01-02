
#include "accmpluti.h"
#include "accmpl.h"

typedef enum {
    STAT_DESC_EXEC = 0,
    STAT_DESC_TRY,
    STAT_DESC_CHECK
} s_stat_desc_;

static const char *stats[]    = { "__ac_exec_step", "__ac_try_step", "__ac_check_step" };
static const char *prefixs[]  = { "", "", "if (" };
static const char *postfixs[] = { "", "", ") {" };

static char *ac_get_proc(pac_proc_ proc, int index)
{
    while (proc) {
        if (proc->index == index) {
            return(proc->names);
        }
        else if (proc->index > index) {
            return("");
        }

        proc = proc->nextp;
    }

    return("");
}

static char *ac_get_str(pac_str_list_ strlistp, int index)
{
    while (strlistp) {
        if (strlistp->indexl == index) {
            return(strlistp->codes);
        }
        else if (strlistp->indexl > index) {
            return("");
        }

        strlistp = strlistp->nextp;
    }

    return("");
}

static void ac_print_step(pac_cmplgen_ cmplgenp, e_step_def_ stepdef, int *pxtab, int *indx, int level, s_stat_desc_ stat, FILE *outputp)
{
    const char *__tabs = "                                                                            ";
    int tablidx = strlen(__tabs) - level * 4;
    e_step_ext_ extl;
    e_step_def_ extdef;
    char *token;
    
    int endl, idx;

    switch (stepdef) {
        case STEP_DEF_ENDPROC:
            fprintf(outputp, "    return(__ac_end_proc(cmplhndp, &procp));\n}\n");
            break;

        case STEP_DEF_PROCSEQ_RECALL:
            fprintf(outputp, "%scontinue;\n", &__tabs[tablidx]); 
            break;

        case STEP_DEF_PROCSEQ_BREAK:
            fprintf(outputp, "%sbreak;\n", &__tabs[tablidx]);  
            break;

        case STEP_DEF_EXECPROC:
            fprintf(outputp, "%s%s%s(EXECPROC(__exec_%s))%s \n", &__tabs[tablidx], prefixs[stat], stats[stat], ac_get_proc(cmplgenp->proc_listp, pxtab[(*indx)++]), postfixs[stat]);
            break;      

        case STEP_DEF_LITERAL:
            fprintf(outputp, "%s%s%s(LITERAL)%s \n", &__tabs[tablidx], prefixs[stat], stats[stat], postfixs[stat]);
            break;

        case STEP_DEF_GETIDENT:
            fprintf(outputp, "%s%s%s(IDENT)%s \n", &__tabs[tablidx], prefixs[stat], stats[stat], postfixs[stat]);
            break;

        case STEP_DEF_ACCEPTPROC:
            fprintf(outputp, "%s%s%s(ACCEPT)%s\n", &__tabs[tablidx], prefixs[stat], stats[stat], postfixs[stat]);
            break;

        case STEP_DEF_KEYWORD:
            fprintf(outputp, "%s%s%s(KEYWORD(\"%s\"))%s\n", &__tabs[tablidx], prefixs[stat], stats[stat], ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]), postfixs[stat]);
            break;

        case STEP_DEF_MULTI_KEYWORD:
            fprintf(outputp, "%s%s%s(ONE_KEYWORD(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : "");
                idx++;
            }
            fprintf(outputp, ")%s\n", postfixs[stat]);
            break;

        case STEP_DEF_TOKEN:
            token = ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]);
            if (strlen(token) > 1) {
                fprintf(outputp, "%s%s%s(TOKEN(\"%s\"))%s \n", &__tabs[tablidx], prefixs[stat], stats[stat], token, postfixs[stat]);
            }
            else {
                fprintf(outputp, "%s%s%s(SYMBOL('%c'))%s \n", &__tabs[tablidx], prefixs[stat], stats[stat], token[0], postfixs[stat]);
            }
            break;

        case STEP_DEF_MULTI_TOKEN:
            fprintf(outputp, "%s%s%s(ONE_TOKEN(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : "");
                idx++;
            }
            fprintf(outputp, ")%s\n", postfixs[stat]);
            break;

        case STEP_DEF_PROCSEQ:
            fprintf(outputp, "%s__ac_exec_stat(BEG_PROCSEQ);\n", &__tabs[tablidx]);
            fprintf(outputp, "%swhile (__ac_pocess_next(cmplhndp)) {\n", &__tabs[tablidx]); 
            endl = pxtab[(*indx)++];
            while (*indx < endl) {
                if (*indx == endl - 1 && pxtab[(*indx)] == STEP_DEF_PROCSEQ_RECALL) {
                    (*indx)++;
                }
                else {
                    ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, outputp);
                }
            }

            if (pxtab[(*indx) - 1] != STEP_DEF_PROCSEQ_RECALL && pxtab[(*indx) - 1] != STEP_DEF_PROCSEQ_BREAK) {
                fprintf(outputp, "%sbreak;\n%s}\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
            }
            else {
                fprintf(outputp, "%s}\n", &__tabs[tablidx]);
            }
            fprintf(outputp, "%s__ac_exec_stat(END_PROCSEQ);\n\n", &__tabs[tablidx]);
            break;

        case STEP_DEF_OPTSEQ:
            endl = pxtab[(*indx)++];
            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, STAT_DESC_CHECK, outputp);
            while (*indx < endl) {
                ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, outputp);
            }
            fprintf(outputp, "%s}\n\n", &__tabs[tablidx]);
            break;

        default:
            extl = (stepdef - STEP_DEF_EXECPROC) % 5;
            if (extl > 0) {
                extdef = stepdef - extl;
                switch (extl) {
                    case STEP_EXT_OPTSTEP:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_TRY, outputp);
                        break;
                    case STEP_EXT_ACCEPT_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_CHECK, outputp);
                        fprintf(outputp, "%sreturn(__ac_end_proc(cmplhndp, &procp));\n%s}\n\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
                        break;
                    case STEP_EXT_BREAK_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_CHECK, outputp);
                        fprintf(outputp, "%sbreak;\n%s}\n\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
                        break;
                    case STEP_EXT_RECALL_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_CHECK, outputp);
                        fprintf(outputp, "%scontinue;\n%s}\n\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
                        break;
                }
            }
            else {
                fprintf(outputp, "%s%sUNKNOWN_STEP : %d \n", &__tabs[tablidx], prefixs[stat], stepdef);
            }
    }
}

void ac_print_proc(pac_cmplgen_ cmplgenp, int *pxtab, FILE *outputp)
{
    pac_proc_ proc;
    pac_step_ step;

    int idx;
    
    fprintf(outputp, "\n/************* AC-PROCs PROTOTYPE *************/\n");
    proc = cmplgenp->proc_listp;
    while (proc) {
        fprintf(outputp, "static int __exec_%s(p_accmpl_ cmplhndp);\n", proc->names, proc->names, proc->names);
        proc = proc->nextp;
    }

    fprintf(outputp, "\n/************* AC-PROCs DEFINITION ************/");
    proc = cmplgenp->proc_listp;
    while (proc) {

        fprintf(outputp, "\nstatic int __exec_%s(p_accmpl_ cmplhndp)\n{\n    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, __exec_%s, \"%s\");\n\n", proc->names, proc->names, proc->names);
        idx = proc->initpos+1;
        while (idx <= pxtab[proc->initpos]) {
            ac_print_step(cmplgenp, pxtab[idx++], pxtab, &idx, 1, 0, outputp);
        }
        proc = proc->nextp;
    }
}
