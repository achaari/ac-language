
#include "accmpluti.h"
#include "accmpl.h"

typedef enum {
    STAT_DESC_EXEC = 0,
    STAT_DESC_TRY,
    STAT_DESC_CHECK,
    STAT_DESC_BEG_CHECK,
    STAT_DESC_EXTEND_CHECK_OR,
    STAT_DESC_EXTEND_CHECK_AND
} s_stat_desc_;

static const char *stats[]    = { "__ac_exec_step(", "__ac_try_step(", "__ac_check_step(", "__ac_check_step(", "", "" };
static const char *prefixs[]  = { "", "", "if (", "if (", "                 OR ", "           AND " };
static const char *postfixs[] = { "", ") \n", ")) { \n", "", "", "" };


static int ac_is_simple_step(e_step_def_ stepdef)
{
    if (stepdef >= STEP_DEF_EXECPROC && (stepdef - STEP_DEF_EXECPROC) % 5 == 0) {
        return(TRUE);
    }
    
    return(FALSE);
}

static char *ac_get_str(pac_data_list_ strlistp, int index)
{
    while (strlistp) {
        if (strlistp->indexl == index) {
            return(strlistp->data.codes);
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
    static e_step_def_ prevstp;
    int tablidx = strlen(__tabs) - level * 4;
    e_step_ext_ extl;
    e_step_def_ extdef;
    char *token;
    
    int endl, idx;

    switch (stepdef) {
        case STEP_DEF_ENDPROC:
            fprintf(outputp, "\n    return(__ac_end_proc(cmplhndp, &procp));\n}\n");
            break;

        case STEP_DEF_PROCSEQ_RECALL:
            fprintf(outputp, "%scontinue;\n", &__tabs[tablidx]); 
            break;

        case STEP_DEF_PROCSEQ_BREAK:
            fprintf(outputp, "%sbreak;\n", &__tabs[tablidx]);  
            break;

        case STEP_DEF_EXECPROC:
            fprintf(outputp, "%s%s%sEXECPROC(%s)%s", &__tabs[tablidx], prefixs[stat], stats[stat], ac_get_procname(cmplgenp->proc_listp, pxtab[(*indx)++]), postfixs[stat]);
            break;   

        case STEP_DEF_EXECKEYWORD:
            fprintf(outputp, "%s%s%sEXECKEYWORD(KEY(%s))%s", 
                    &__tabs[tablidx], prefixs[stat], stats[stat], 
                    ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)]), 
                    postfixs[stat]);
            (*indx) += 2;
            break;

        case STEP_DEF_EXECONEKEYWORD:
            fprintf(outputp, "%s%s%sEXECKEYWORD(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "KEY(%s)%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)]), (idx < endl - 1) ? ", " : "");
                (*indx) += 2;
                idx++;
            }
            fprintf(outputp, ")%s", postfixs[stat]);
            break;

        case STEP_DEF_LITERAL:
            fprintf(outputp, "%s%s%sLITERAL%s", &__tabs[tablidx], prefixs[stat], stats[stat], postfixs[stat]);
            break;

        case STEP_DEF_GETIDENT:
            fprintf(outputp, "%s%s%sIDENT%s", &__tabs[tablidx], prefixs[stat], stats[stat], postfixs[stat]);
            break;

        case STEP_DEF_ACCEPTPROC:
            fprintf(outputp, "\n%sreturn(__ac_end_proc(cmplhndp, &procp));\n", &__tabs[tablidx]);
            break;

        case STEP_DEF_KEYWORD:
            fprintf(outputp, "%s%s%sKEYWORD(\"%s\")%s", &__tabs[tablidx], prefixs[stat], stats[stat], ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]), postfixs[stat]);
            break;

        case STEP_DEF_MULTI_KEYWORD:
            fprintf(outputp, "%s%s%sKEYWORD(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : "");
                idx++;
            }
            fprintf(outputp, ")%s", postfixs[stat]);
            break;

        case STEP_DEF_TOKEN:
            token = ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]);
            if (strlen(token) > 1) {
                fprintf(outputp, "%s%s%sTOKEN(\"%s\")%s", &__tabs[tablidx], prefixs[stat], stats[stat], token, postfixs[stat]);
            }
            else {
                fprintf(outputp, "%s%s%sSYMBOL('%c')%s", &__tabs[tablidx], prefixs[stat], stats[stat], token[0], postfixs[stat]);
            }
            break;

        case STEP_DEF_MULTI_TOKEN:
            fprintf(outputp, "%s%s%sTOKEN(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "\"%s\"%s", ac_get_str(cmplgenp->token_listp, pxtab[(*indx)++]), (idx < endl - 1) ? ", " : "");
                idx++;
            }
            fprintf(outputp, ")%s", postfixs[stat]);
            break;

        case STEP_DEF_PROCSEQ:
            fprintf(outputp, "\n%s__ac_exec_step(BEG_PROCSEQ);\n", &__tabs[tablidx]);
            fprintf(outputp, "%swhile (__ac_pocess_next(cmplhndp)) {\n", &__tabs[tablidx]); 
            endl = pxtab[(*indx)++];
            while (*indx < endl) {
                if (*indx == endl - 1 && pxtab[(*indx)] == STEP_DEF_PROCSEQ_RECALL) {
                    prevstp = STEP_DEF_PROCSEQ_RECALL;
                    (*indx)++;
                }
                else {
                    ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, outputp);
                }
            }

            if (prevstp != STEP_DEF_PROCSEQ_RECALL && prevstp != STEP_DEF_PROCSEQ_BREAK) {
                fprintf(outputp, "%sbreak;\n%s}\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
            }
            else {
                fprintf(outputp, "%s}\n", &__tabs[tablidx]);
            }
            fprintf(outputp, "%s__ac_exec_step(END_PROCSEQ);\n", &__tabs[tablidx]);
            break;

        case STEP_DEF_OPTSEQ:
            endl = pxtab[(*indx)++];
            fprintf(outputp, "\n");
            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, STAT_DESC_CHECK, outputp);
            while (*indx < endl) {
                ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, outputp);
            }
            fprintf(outputp, "%s}\n", &__tabs[tablidx]);
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
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_BEG_CHECK, outputp);
                        while ((pxtab[(*indx)] - STEP_DEF_EXECPROC) % 5 == STEP_EXT_ACCEPT_IF) {
                            fprintf(outputp, "\n");
                            ac_print_step(cmplgenp, pxtab[(*indx)++] - STEP_EXT_ACCEPT_IF, pxtab, indx, level, STAT_DESC_EXTEND_CHECK_OR, outputp);
                        }
                        fprintf(outputp, ")) {\n%sreturn(__ac_end_proc(cmplhndp, &procp));\n%s}\n\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
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

            /* Do not execute AND operator */
            prevstp = stepdef;
            return;
    }

    /* Set previous step */
    prevstp = stepdef;

    if (!stat && ac_is_simple_step(stepdef)) {
        /* Process AND operator for consecutiv simple steps */
        while (ac_is_simple_step(pxtab[(*indx)])) {
            fprintf(outputp, "\n");
            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, STAT_DESC_EXTEND_CHECK_AND, outputp);
        }

        fprintf(outputp, ")\n");
    }    
}

void ac_print_proc(pac_cmplgen_ cmplgenp, int *pxtab, FILE *outputp)
{
    pac_proc_ proc;

    int idx;

    fprintf(outputp, "\n#include \"accmpldefs.h\"\n\n");
    
    fprintf(outputp, "\n/************* AC-PROCs PROTOTYPE *************/\n");
    proc = cmplgenp->proc_listp;
    while (proc) {
        fprintf(outputp, "static int __exec_%s%s(p_accmpl_ cmplhndp);\n", (proc->type == PROC_TYPE_KEYWORD) ? "keyword_" : "", proc->names, proc->names, proc->names);
        proc = proc->nextp;
    }

    fprintf(outputp, "\n/************* AC-PROCs DEFINITION ************/");
    proc = cmplgenp->proc_listp;
    while (proc) {

        fprintf(outputp, "\nstatic int __exec_%s%s(p_accmpl_ cmplhndp)\n{\n    p_accmpl_proc_ procp = __ac_init_proc(cmplhndp, \"%s\", AC_TYPE_%s);\n\n", 
                (proc->type == PROC_TYPE_KEYWORD) ? "keyword_" : "", proc->names, proc->names,
                (proc->type == PROC_TYPE_KEYWORD) ? "KEYWORD" : (proc->type == PROC_TYPE_MAIN) ? "MAIN" : "PROC" );

        idx = proc->initpos+1;
        while (idx <= pxtab[proc->initpos]) {
            ac_print_step(cmplgenp, pxtab[idx++], pxtab, &idx, 1, 0, outputp);
        }
        proc = proc->nextp;
    }
}
