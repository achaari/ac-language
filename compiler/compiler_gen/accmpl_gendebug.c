
#include "accmpluti.h"
#include "accmpl.h"

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

static void ac_print_step(pac_cmplgen_ cmplgenp, e_step_def_ stepdef, int *pxtab, int *indx, int level, char *prefix, FILE *outputp)
{
    const char *__tabs = "                                                                            ";
    int tablidx = strlen(__tabs) - level * 4;
    e_step_ext_ extl;
    e_step_def_ extdef;
    char *token;
    
    int endl, idx;

    switch (stepdef) {
        case STEP_DEF_ENDPROC:
            fprintf(outputp, "\n    return(__end_proc(cmplhndp, &procp));\n}\n");
            break;

        case STEP_DEF_PROCSEQ_RECALL:
            fprintf(outputp, "%s%RECALL_PROCSEQ \n", &__tabs[tablidx], prefix);
            break;

        case STEP_DEF_PROCSEQ_BREAK:
            fprintf(outputp, "%s%RECALL_BREAK \n", &__tabs[tablidx], prefix);
            break;

        case STEP_DEF_EXECPROC:
            fprintf(outputp, "%s%sEXEC_PROC(%s) \n", &__tabs[tablidx], prefix, ac_get_proc(cmplgenp->proc_listp, pxtab[(*indx)++]));
            break;      

        case STEP_DEF_LITERAL:
            fprintf(outputp, "%s%sCHECK_LITERAL \n", &__tabs[tablidx], prefix);
            break;

        case STEP_DEF_GETIDENT:
            fprintf(outputp, "%s%sGET_IDENT \n", &__tabs[tablidx], prefix);
            break;

        case STEP_DEF_ACCEPTPROC:
            fprintf(outputp, "%s%sACCEPT_PROC \n", &__tabs[tablidx], prefix);
            break;

        case STEP_DEF_KEYWORD:
            fprintf(outputp, "%s%sCHECK_KEYWORD(\"%s\") \n", &__tabs[tablidx], prefix, ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]));
            break;

        case STEP_DEF_MULTI_KEYWORD:
            fprintf(outputp, "%s%sCHECK_MULTI_KEYWORD(", &__tabs[tablidx], prefix);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : ")\n");
                idx++;
            }
            break;

        case STEP_DEF_TOKEN:
            token = ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]);
            if (strlen(token) > 1) {
                fprintf(outputp, "%s%sCHECK_TOKEN(\"%s\") \n", &__tabs[tablidx], prefix, token);
            }
            else {
                fprintf(outputp, "%s%sCHECK_SYMBOL('%c') \n", &__tabs[tablidx], prefix, token[0]);
            }
            break;

        case STEP_DEF_MULTI_TOKEN:
            fprintf(outputp, "%s%sCHECK_MULTI_TOKEN(", &__tabs[tablidx], prefix);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : ")\n");
                idx++;
            }
            break;

        case STEP_DEF_PROCSEQ:
            fprintf(outputp, "%sBEG_PROCSEQ \n", &__tabs[tablidx]);
            endl = pxtab[(*indx)++];
            while (*indx < endl) {
                ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, "", outputp);
            }
            fprintf(outputp, "%sEND_PROCSEQ\n", &__tabs[tablidx]);
            break;

        case STEP_DEF_OPTSEQ:
            endl = pxtab[(*indx)++];
            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, "IF_", outputp);
            while (*indx < endl) {
                ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level+1, "", outputp);
            }
            fprintf(outputp, "%sEND_IF\n", &__tabs[tablidx]);
            break;

        default:
            extl = (stepdef - STEP_DEF_EXECPROC) % 5;
            if (extl > 0) {
                extdef = stepdef - extl;
                switch (extl) {
                    case STEP_EXT_OPTSTEP:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, "OPT_", outputp);
                        break;
                    case STEP_EXT_ACCEPT_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, "ACCEPT_PROC_IF_", outputp);
                        break;
                    case STEP_EXT_BREAK_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, "BREAK_PROCSEQ_IF_", outputp);
                        break;
                    case STEP_EXT_RECALL_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, "RECALL_PROCSEQ_IF_", outputp);
                        break;
                }
            }
            else {
                fprintf(outputp, "%s%sUNKNOWN_STEP : %d \n", &__tabs[tablidx], prefix, stepdef);
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

        fprintf(outputp, "\nstatic int __exec_%s(p_accmpl_ cmplhndp)\n{\n    p_accmpl_proc_ procp = __init_proc(cmplhndp, __exec_%s, \"%s\");\n\n", proc->names, proc->names, proc->names);
        idx = proc->initpos+1;
        while (idx <= pxtab[proc->initpos]) {
            ac_print_step(cmplgenp, pxtab[idx++], pxtab, &idx, 1, "", outputp);
        }
        proc = proc->nextp;
    }
}
