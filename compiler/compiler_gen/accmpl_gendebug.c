
#include "accmpluti.h"
#include "accmpl.h"

#define CASE_STEP_DEF(stp, exc, mcr)                                                                                        \
    case STEP_DEF_##stp:                                                                                                    \
    case STEP_DEF_##stp##_DATA:                                                                                             \
        fprintf(outputp, "%s%s%s%s", &__tabs[tablidx], prefixs[stat], stats[stat], #exc);                                   \
        if (stepdef == STEP_DEF_##stp##_DATA) {                                                                             \
            fprintf(outputp, "_DATA(&(%s)%s", ac_get_str(cmplgenp->stepdata_listp, pxtab[(*indx)++]), (mcr) ? ", " : "");   \
            if (! mcr) { fprintf(outputp, ")%s", postfixs[stat]); break;}}                                                  \
        else if (mcr) fprintf(outputp, "(");  else { fprintf(outputp, "%s", postfixs[stat]); break; }


typedef enum {
    STAT_DESC_EXEC = 0,
    STAT_DESC_TRY,
    STAT_DESC_CHECK,
    STAT_DESC_LOOP,
    STAT_DESC_BEG_CHECK,
    STAT_DESC_EXTEND_CHECK_OR,
    STAT_DESC_EXTEND_CHECK_AND
} s_stat_desc_;

static const char *stats[]    = { "__ac_exec_step(", "__ac_try_step(", "__ac_check_step(", "__ac_check_step(", "__ac_check_step(", "", "" };
static const char *prefixs[]  = { "", "", "if (", "while (", "if (", "                 OR ", "           AND " };
static const char *postfixs[] = { "", ") \n", ")) { \n", ")) { \n", "", "", "" };


static int ac_is_simple_step(e_step_def_ stepdef)
{
    if (stepdef >= STEP_DEF_EXEC_PROC && (stepdef - STEP_DEF_EXEC_PROC) % 5 == 0) {
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

static void ac_print_step(pac_cmplgen_ cmplgenp, e_step_def_ stepdef, int *pxtab, int *indx, int level, s_stat_desc_ stat, int maxendl, FILE *outputp)
{
    const char *__tabs = "                                                                            ";
    int tablidx = strlen(__tabs) - level * 4, tmpidx;
    e_step_ext_ extl;
    e_step_def_ extdef;
    char *token;
    
    int endl, idx, prevseqidx;

    switch (stepdef) {
        CASE_STEP_DEF(LITERAL,  LITERAL, FALSE)
        CASE_STEP_DEF(INTEGER,  INTEGER, FALSE)
        CASE_STEP_DEF(CHAR,     CHAR,    FALSE)
        CASE_STEP_DEF(FLOAT,    FLOAT,   FALSE)
        CASE_STEP_DEF(STRING,   STRING,  FALSE)
        CASE_STEP_DEF(GETIDENT, IDENT,   FALSE)

        case STEP_DEF_ENDPROC:
            fprintf(outputp, "\n    return(__ac_end_proc(cmplhndp));\n}\n");
            break;

        case STEP_DEF_PROCSEQ_RECALL:
            fprintf(outputp, "%sgoto __procseq_%d_beg;\n", &__tabs[tablidx], cmplgenp->seqidx);
            break;

        case STEP_DEF_PROCSEQ_BREAK:
            fprintf(outputp, "%sgoto __procseq_%d_end;\n", &__tabs[tablidx], cmplgenp->seqidx);
            break;

        CASE_STEP_DEF(EXEC_PROC, EXECPROC, TRUE)
            fprintf(outputp, "%s)%s", ac_get_procname(cmplgenp->proc_listp, pxtab[(*indx)++]), postfixs[stat]);
            break; 
        
        CASE_STEP_DEF(EXEC_KEYWORD, EXECKEYWORD, TRUE)
            fprintf(outputp, "KEY(%s))%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)]), postfixs[stat]);
            (*indx) += 2;
            break;
            
        case STEP_DEF_EXEC_ONEKEYWORD:
            fprintf(outputp, "%s%s%sEXECKEYWORD(", &__tabs[tablidx], prefixs[stat], stats[stat]);
            endl = pxtab[(*indx)++]; idx = 0;
            while (idx < endl) {
                fprintf(outputp, "KEY(%s)%s", ac_get_str(cmplgenp->keyword_listp, pxtab[(*indx)]), (idx < endl - 1) ? ", " : "");
                (*indx) += 2;
                idx++;
            }
            fprintf(outputp, ")%s", postfixs[stat]);
            break;        
      
        case STEP_DEF_ACCEPTPROC:
            fprintf(outputp, "\n%sreturn(__ac_end_proc(cmplhndp));\n", &__tabs[tablidx]);
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
            prevseqidx = cmplgenp->seqidx;
            cmplgenp->seqidx = cmplgenp->procseq++;
            endl = pxtab[(*indx)++];

            /* Check if we need BEG Label */
            for (tmpidx = *indx; tmpidx < endl; tmpidx++) {
                if ((pxtab[(tmpidx)] == STEP_DEF_PROCSEQ_RECALL) ||
                    (pxtab[(tmpidx)] >= STEP_DEF_EXEC_PROC && (pxtab[(tmpidx)] - STEP_DEF_EXEC_PROC) % 5 == STEP_EXT_RECALL_IF)) {
                    fprintf(outputp, "\n__procseq_%d_beg:\n", cmplgenp->seqidx);
                    break;
                }
            }
            fprintf(outputp, "%s{\n", &__tabs[tablidx]);
            tmpidx = *indx;
            while (*indx < endl) {
                ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, endl, outputp);
            }

            /* Check if we need END Label */
            for (; tmpidx < endl; tmpidx++) {
                if ((pxtab[(tmpidx)] == STEP_DEF_PROCSEQ_BREAK) ||
                    (pxtab[(tmpidx)] >= STEP_DEF_EXEC_PROC && (pxtab[(tmpidx)] - STEP_DEF_EXEC_PROC) % 5 == STEP_EXT_BREAK_IF)) {
                    fprintf(outputp, "%s}\n__procseq_%d_end:;\n", &__tabs[tablidx], cmplgenp->seqidx);
                    break;
                }
            }

            cmplgenp->seqidx = prevseqidx;
            break;

        case STEP_DEF_OPTSEQ:
        case STEP_DEF_OPTLOOP:
            endl = pxtab[(*indx)++];

            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, (stepdef == STEP_DEF_OPTSEQ) ? STAT_DESC_CHECK : STAT_DESC_LOOP, 0, outputp);
            if (stepdef == STEP_DEF_OPTLOOP && pxtab[(*indx)] == STEP_DEF_NOOP) {
                fprintf(outputp, "%s;\n", &__tabs[tablidx - 4]);
                (*indx)++;
            }
            else {
                while (*indx < endl) {
                    ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level + 1, 0, endl, outputp);
                }
            }
            fprintf(outputp, "%s}\n", &__tabs[tablidx]);
            break;

        default:
            extl = (stepdef - STEP_DEF_EXEC_PROC) % 10;

            if (extl >= 5) {
                extl -= 5;
            }

            if (extl > 0) {
                extdef = stepdef - extl;
                switch (extl) {
                    case STEP_EXT_OPTSTEP:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_TRY, 0, outputp);
                        break;
                    case STEP_EXT_ACCEPT_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_BEG_CHECK, 0, outputp);
                        while ((pxtab[(*indx)] - STEP_DEF_EXEC_PROC) % 5 == STEP_EXT_ACCEPT_IF) {
                            fprintf(outputp, "\n");
                            ac_print_step(cmplgenp, pxtab[(*indx)++] - STEP_EXT_ACCEPT_IF, pxtab, indx, level, STAT_DESC_EXTEND_CHECK_OR, 0, outputp);
                        }
                        fprintf(outputp, ")) {\n%sreturn(__ac_end_proc(cmplhndp));\n%s}\n\n", &__tabs[tablidx - 4], &__tabs[tablidx]);
                        break;
                    case STEP_EXT_BREAK_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_CHECK, 0, outputp);
                        fprintf(outputp, "%sgoto __procseq_%d_end;\n%s}\n\n", &__tabs[tablidx - 4], cmplgenp->seqidx, &__tabs[tablidx]);
                        break;
                    case STEP_EXT_RECALL_IF:
                        ac_print_step(cmplgenp, extdef, pxtab, indx, level, STAT_DESC_CHECK, 0, outputp);
                        fprintf(outputp, "%sgoto __procseq_%d_beg;\n%s}\n\n", &__tabs[tablidx - 4], cmplgenp->seqidx, &__tabs[tablidx]);
                        break;
                }
            }
            else {
                fprintf(outputp, "%s%sUNKNOWN_STEP : %d \n", &__tabs[tablidx], prefixs[stat], stepdef);
            }

            /* Do not execute AND operator */
            return;
    }

    if (!stat && ac_is_simple_step(stepdef)) {
        /* Process AND operator for consecutiv simple steps */
        while (ac_is_simple_step(pxtab[(*indx)]) && (!maxendl || *indx < maxendl)) {
            fprintf(outputp, "\n");
            ac_print_step(cmplgenp, pxtab[(*indx)++], pxtab, indx, level, STAT_DESC_EXTEND_CHECK_AND, 0, outputp);
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

        if (!proc->isdefb) {
            proc = proc->nextp;
        }

        fprintf(outputp, "\nstatic int __exec_%s%s(p_accmpl_ cmplhndp)\n{\n    if (!__ac_init_proc(cmplhndp, \"%s\")) return(FALSE);\n\n", 
                (proc->type == PROC_TYPE_KEYWORD) ? "keyword_" : "", proc->names, proc->names,
                (proc->type == PROC_TYPE_KEYWORD) ? "KEYWORD" : (proc->type == PROC_TYPE_MAIN) ? "MAIN" : "PROC" );

        idx = proc->initpos+1;
        while (idx <= pxtab[proc->initpos]) {
            ac_print_step(cmplgenp, pxtab[idx++], pxtab, &idx, 1, 0, 0, outputp);
        }
        proc = proc->nextp;
    }
}
