#ifndef __DB_LEXER__
#define __DB_LEXER__

#include "acstd.h"

typedef PTR p_lexer_;

p_lexer_ opensource(const char *filename);
void closesource(p_lexer_ *srcp);

void set_userfp(p_lexer_ srcp, PFI fp, PTR datap);

const char *get_str(p_lexer_ source);
int next(p_lexer_ srcp);
char peek(p_lexer_ srcp);
char get(p_lexer_ srcp);
char peek_char(p_lexer_ srcp);
void consume(p_lexer_ srcp);
void push_back(p_lexer_ srcp);
void skip(p_lexer_ srcp, int nbchar);
void save(p_lexer_ source);
int accept(p_lexer_ srcp);
int rollback(p_lexer_ srcp);
void skipline(p_lexer_ srcp);
int find_code(p_lexer_ srcp, const char *code);
int find_ident(p_lexer_ srcp, const char *code);
int check_code(p_lexer_ srcp, char *valp);
int check_ident(p_lexer_ srcp, char *valp);
int check_char(p_lexer_ srcp, char valc);
char *get_ident(p_lexer_ srcp);
char *get_identstr(p_lexer_ srcp, char *ident);
int get_string(p_lexer_ srcp, char **codes);

int get_int(p_lexer_ srcp, int *valp);
char *get_next_code(p_lexer_ srcp, char split, char reset);
char *split_code(p_lexer_ srcp, char *tostrs, char *codep);
char *split_code_with_alloc(p_lexer_ srcp, char *tostrs);

int get_laststate(p_lexer_ srcp);



#define __eval(srcp, expr)   (save(srcp), tmpl = (expr), (tmpl || get_laststate(srcp)) ? (tmpl && accept(srcp)) : rollback(srcp))
#define __check(srcp, expr)  (save(srcp), tmpl = (expr), (rollback(srcp) || tmpl))

#endif
