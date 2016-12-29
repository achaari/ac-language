
#include "accmpluti.h"
#include "accmpl.h"

static int ac_get_str_index(pac_str_list_ strlistp, char *keyword)
{
    int cmpl;
    
    while (strlistp) {
        cmpl = strcmp(strlistp->codes, keyword);
        if (!cmpl) {
            return(strlistp->indexl);
        }
        else if (cmpl < 0) {
            return(-1);
        }

        strlistp = strlistp->nextp;
    }

    return(-1);
}

static void ac_add_step(pac_cmplgen_ cmplgenp, pac_step_ step, e_step_type_ steptype, e_step_ext_ stepext, int *tab, int* index)
{
    pac_step_ substep;
    pac_str_list_ strlistp; 
    e_step_type_ exttype;
    e_step_ext_ extl;
    char str[2];

    int endpos, count;

    mem_reset(str, sizeof(str));

    if (steptype < 0) {
        steptype = step->type;
    }

    switch (steptype) {
        case STEP_TYPE_OPTSEQ:  
            if (step->childp) {
                stepext = STEP_EXT_NA;
                tab[(*index)++] = STEP_DEF_OPTSEQ;
                endpos = (*index)++;
            }
            else {
                stepext = STEP_EXT_OPTSTEP;
            }

            ac_add_step(cmplgenp, step->headp, -1, stepext, tab, index);

            if (step->childp) {
                substep = step->childp;
                while (substep) {
                    ac_add_step(cmplgenp, substep, -1, STEP_EXT_NA, tab, index);
                    substep = substep->nextp;
                }

                tab[endpos] = *index;
            }
            break;

        case STEP_TYPE_PROCSEQ:
            tab[(*index)++] = STEP_DEF_PROCSEQ;
            endpos = (*index)++;
            substep = step->childp;
            while (substep) {
                ac_add_step(cmplgenp, substep, -1, STEP_EXT_NA, tab, index);
                substep = substep->nextp;
            }
            tab[endpos] = *index;
            break;

        case STEP_TYPE_PROC_ACCEPT:
            tab[(*index)++] = STEP_DEF_ACCEPTPROC;
            break;

        case STEP_TYPE_PROCSEQ_RECALL:
            tab[(*index)++] = STEP_DEF_PROCSEQ_RECALL;
            break;

        case STEP_TYPE_PROCSEQ_BREAK:
            tab[(*index)++] = STEP_DEF_PROCSEQ_BREAK;
            break;

        case STEP_TYPE_LITERAL:
            tab[(*index)++] = STEP_DEF_LITERAL + stepext;
            break;

        case STEP_TYPE_GETIDENT:
            tab[(*index)++] = STEP_DEF_GETIDENT + stepext;
            break;

        case STEP_TYPE_EXEC_PROC:
            tab[(*index)++] = STEP_DEF_EXECPROC + stepext;
            tab[(*index)++] = step->datap.procp->index;
            break;      

        case STEP_TYPE_KEYWORD:
            tab[(*index)++] = STEP_DEF_KEYWORD + stepext;
            tab[(*index)++] = ac_get_str_index(cmplgenp->keyword_listp, step->datap.idents);
            break;

        case STEP_TYPE_MULTI_KEYWORD:
            tab[(*index)++] = STEP_DEF_MULTI_KEYWORD + stepext;
            endpos = (*index)++;
            count  = 0;
            strlistp = step->datap.strlistp;
            while (strlistp) {
                count++;
                tab[(*index)++] = ac_get_str_index(cmplgenp->keyword_listp, strlistp->codes);
                strlistp = strlistp->nextp;
            }
            tab[endpos] = count;
            break;

        case STEP_TYPE_STRCODE:
            tab[(*index)++] = STEP_DEF_TOKEN + stepext;
            tab[(*index)++] = ac_get_str_index(cmplgenp->token_listp, step->datap.codes);
            break;

        case STEP_TYPE_MULTI_STRCODE:
            tab[(*index)++] = STEP_DEF_MULTI_TOKEN + stepext;
            endpos = (*index)++;
            count = 0;
            strlistp = step->datap.strlistp;
            while (strlistp) {
                count++;
                tab[(*index)++] = ac_get_str_index(cmplgenp->token_listp, strlistp->codes);
                strlistp = strlistp->nextp;
            }
            tab[endpos] = count;
            break;

        case STEP_TYPE_SYMBOL:
            str[0] = step->datap.chr;
            tab[(*index)++] = STEP_DEF_TOKEN + stepext;
            tab[(*index)++] = ac_get_str_index(cmplgenp->token_listp, str);
            break;

        case STEP_TYPE_MULTI_SYMBOL:
            tab[(*index)++] = STEP_DEF_MULTI_TOKEN + stepext;
            tab[(*index)++] = (int)strlen(step->datap.codes);
            for (count = 0; count < (int) strlen(step->datap.codes); count++) {
                str[0] = step->datap.codes[count];
                tab[(*index)++] = ac_get_str_index(cmplgenp->token_listp, str);
            }
            break;

        default :
            extl = (step->type - STEP_TYPE_EXEC_PROC) % 4 + 1;
            if (extl > 0) {
                exttype = step->type - extl + 1;

                ac_add_step(cmplgenp, step, exttype, extl, tab, index);
            }
            else {
                tab[(*index)++] = -10 * step->type;
            }
    }
}

static char *ac_fmtstr(char *ori, char *dest)
{
    sprintf(dest, "\"%s\"", ori);
    return(dest);
}

void ac_print_compiler(pac_cmplgen_ cmplgenp, FILE *outputp, int procount)
{
    pac_proc_ proc;
    pac_step_ step;
    pac_str_list_ strlistp;
    char keystr[100];

    int _tab[10000], index = procount+1, idx = 0;
    
    mem_reset(_tab, sizeof(_tab));

    fprintf(outputp, "\n\n#include \"accmpl.h\"\n\n");

    if (cmplgenp->keyword_listp) {
        idx = 0;
        fprintf(outputp, "static const char * __keywords[] = {\n    ");
        strlistp = cmplgenp->keyword_listp;
        while (strlistp) {
            strlistp->indexl = idx;
            fprintf(outputp, " %18s%s", ac_fmtstr(strlistp->codes, keystr), (strlistp->nextp) ? "," : " };\n");
            if (!((idx++ + 1) % 9)) {
                fprintf(outputp, "\n    ");
            }

            strlistp = strlistp->nextp;
        }
    }

    if (cmplgenp->token_listp) {
        idx = 0;
        fprintf(outputp, "\n\nstatic const char * __tokens[] = {\n    ");
        strlistp = cmplgenp->token_listp;
        while (strlistp) {
            strlistp->indexl = idx;
            fprintf(outputp, " %7s%s", ac_fmtstr(strlistp->codes, keystr), (strlistp->nextp) ? "," : " };\n");
            if (!((idx++ + 1) % 20)) {
                fprintf(outputp, "\n    ");
            }

            strlistp = strlistp->nextp;
        }
    }

    proc = cmplgenp->proc_listp;
    while (proc) {
        proc->initpos = index++;

        /* Proc position */
        _tab[proc->index] = proc->initpos;
        
        step = proc->steplist;
        while (step) {
            ac_add_step(cmplgenp, step, -1, STEP_EXT_NA, _tab, &index);
            step = step->nextp;
        }

        _tab[index++]       = STEP_DEF_ENDPROC;
        _tab[proc->initpos] = index - 1;

        proc = proc->nextp;
    }

#ifdef AC_GEN_DEBUG
    fprintf(outputp, "\n\n#ifndef AC_DEBUG\n");
#endif

    fprintf(outputp, "static const int __prcex[] = {\n    ");
    for (idx = 0; idx < index; idx++) {
        fprintf(outputp, " %5d%s", _tab[idx], (idx == index - 1) ? " };\n" : ",");
        if ( !((idx+1) % 25)) {
            fprintf(outputp, "\n    ");
        }
    }

#ifdef AC_GEN_DEBUG
    fprintf(outputp, "\n#else\n");
    ac_print_proc(cmplgenp, _tab, outputp);
    fprintf(outputp, "#endif\n");
#endif

    fprintf(outputp, "\n\n/*********** MAIN COMPILER FUNCTION ***********/\n");
    fprintf(outputp, "int __accmpl_exec_%s()\n{\n", (cmplgenp->module_name) ? cmplgenp->module_name : "module");
    fprintf(outputp, "    p_accmpl_ cmplhndp = __ac_new_compiler();\n");
    if (cmplgenp->keyword_listp) {
        fprintf(outputp, "\n    __ac_compl_set_keyword(cmplhndp, __keywords, sizeof(__keywords)/sizeof(char*));\n");
    }

#ifdef AC_GEN_DEBUG
    fprintf(outputp, "\n\n#ifndef AC_DEBUG");
#endif

    fprintf(outputp, "\n    __ac_compl_exec(cmplhndp, __prcex, sizeof(__prcex)/sizeof(int));\n");

#ifdef AC_GEN_DEBUG
    fprintf(outputp, "\n#else");
    fprintf(outputp, "\n    EXEC_PROC(main);\n");
    fprintf(outputp, "#endif\n");
#endif

    fprintf(outputp, "\n    return(__ac_exit_compiler(&cmplhndp));\n");
    fprintf(outputp, "}\n");

    fclose(outputp);
}
