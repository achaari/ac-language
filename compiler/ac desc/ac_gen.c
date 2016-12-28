

#include "acdluti.h"

DEF_PROC(statement_block) 
        CHECK_SYMBOL('{') 
        INIT_PROCSEQ
                BREAK_PROCSEQ_IF_CHECK_SYMBOL('}') 
                EXEC_PROC(block_statement) 
        END_PROCSEQ
END_PROC

DEF_PROC(variable_declaration) 
        CHECK_ONE_KEYWORD("local" ,"global")
        EXEC_PROC(variable_definition) 
        IF_CHECK_SYMBOL(',') 
                EXEC_PROC(variable_definition) 
                ACCEPT_PROC 
        END_IF
        CHECK_SYMBOL(';') 
END_PROC

DEF_PROC(function_definition) 
        CHECK_KEYWORD("function") 
        GET_IDENT 
        CHECK_SYMBOL('(') 
        INIT_PROCSEQ
                BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                INIT_PROCSEQ
                        GET_IDENT 
                        BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                        CHECK_SYMBOL(',') 
                END_PROCSEQ
        END_PROCSEQ
        ACCEPT_PROC_IF_CHECK_SYMBOL(';') 
        EXEC_PROC(statement_block) 
END_PROC

DEF_PROC(expression) 
        IF_CHECK_ONE_TOKEN("++" ,"--")
                EXEC_PROC(expression) 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_ONE_SYMBOL("!-+&*~") 
                EXEC_PROC(unit_expression) 
        END_IF
        IF_CHECK_ONE_KEYWORD("is" ,"extend")
                EXEC_PROC(type_name) 
        END_IF
        IF_CHECK_SYMBOL('?') 
                EXEC_PROC(expression) 
                CHECK_SYMBOL(':') 
                EXEC_PROC(expression) 
        END_IF
END_PROC

DEF_PROC(type_name) 
        INIT_PROCSEQ
                GET_IDENT 
                RECALL_PROCSEQ_IF_CHECK_TOKEN("::") 
        END_PROCSEQ
END_PROC

DEF_PROC(variable_name) 
        GET_IDENT 
        IF_CHECK_SYMBOL('(') 
                ACCEPT_PROC_IF_CHECK_SYMBOL(')') 
                INIT_PROCSEQ
                        EXEC_PROC(expression) 
                        RECALL_PROCSEQ_IF_CHECK_SYMBOL(',') 
                END_PROCSEQ
                CHECK_SYMBOL(')') 
        END_IF
END_PROC

DEF_PROC(lamda_definition) 
        IF_CHECK_SYMBOL('(') 
                INIT_PROCSEQ
                        BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                        INIT_PROCSEQ
                                GET_IDENT 
                                BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                                CHECK_SYMBOL(',') 
                        END_PROCSEQ
                END_PROCSEQ
                CHECK_SYMBOL(')') 
                IF_CHECK_SYMBOL('{') 
                        EXEC_PROC(block_statement) 
                        CHECK_SYMBOL('}') 
                        ACCEPT_PROC 
                END_IF
                CHECK_TOKEN("=>") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expressions) 
                CHECK_SYMBOL(')') 
                ACCEPT_PROC 
        END_IF
        EXEC_PROC(type_name) 
        INIT_PROCSEQ
                IF_CHECK_SYMBOL('.') 
                        EXEC_PROC(variable_name) 
                END_IF
        END_PROCSEQ
END_PROC

DEF_PROC(variable_expression) 
        ACCEPT_PROC_IF_CHECK_LITERAL 
        IF_CHECK_SYMBOL('@') 
                EXEC_PROC(lamda_definition) 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_ONE_KEYWORD("typeof" ,"sizeof")
                CHECK_SYMBOL('(') 
                EXEC_PROC(type_name) 
                CHECK_SYMBOL(')') 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_SYMBOL('(') 
                EXEC_PROC(expressions) 
                CHECK_SYMBOL(')') 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_SYMBOL('$') 
        END_IF
        EXEC_PROC(variable_name) 
END_PROC

DEF_PROC(unit_expression) 
        EXEC_PROC(variable_expression) 
        INIT_PROCSEQ
                IF_CHECK_ONE_TOKEN("->" ,"::" ,":" ,".")
                        EXEC_PROC(variable_name) 
                END_IF
        END_PROCSEQ
        IF_CHECK_ONE_TOKEN("++" ,"--")
        END_IF
        IF_CHECK_KEYWORD("as") 
                EXEC_PROC(type_name) 
        END_IF
        IF_CHECK_ONE_TOKEN("=" ,"+=" ,"-=" ,"*=" ,"/=" ,"&=" ,"|=")
                EXEC_PROC(expression) 
        END_IF
        IF_CHECK_ONE_TOKEN("||" ,"&&" ,"|" ,"&" ,"==" ,">=" ,"<=" ,">>" ,"<<" ,">" ,"<" ,"!=" ,"+" ,"-" ,"*" ,"/" ,"^")
                EXEC_PROC(expression) 
        END_IF
END_PROC

DEF_PROC(expression_statement) 
        ACCEPT_PROC_IF_CHECK_SYMBOL(';') 
        IF_CHECK_SYMBOL(':') 
                GET_IDENT 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_ONE_KEYWORD("break" ,"continue")
                CHECK_SYMBOL(';') 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("goto") 
                GET_IDENT 
                CHECK_SYMBOL(';') 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("return") 
                IF_EXEC_PROC(expression) 
                END_IF
                CHECK_SYMBOL(';') 
                ACCEPT_PROC 
        END_IF
        ACCEPT_PROC_IF_EXEC_PROC(variable_declaration) 
        EXEC_PROC(expression) 
        CHECK_SYMBOL(';') 
END_PROC

DEF_PROC(block_statement) 
        ACCEPT_PROC_IF_EXEC_PROC(statement_block) 
        IF_CHECK_KEYWORD("if") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression) 
                CHECK_SYMBOL(')') 
                EXEC_PROC(block_statement) 
                IF_CHECK_KEYWORD("else") 
                        EXEC_PROC(block_statement) 
                END_IF
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("while") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression) 
                CHECK_SYMBOL(')') 
                EXEC_PROC(block_statement) 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("do") 
                EXEC_PROC(block_statement) 
                CHECK_KEYWORD("while") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression) 
                CHECK_SYMBOL(')') 
                CHECK_SYMBOL(';') 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("switch") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression) 
                CHECK_SYMBOL(')') 
                CHECK_SYMBOL('{') 
                INIT_PROCSEQ
                        BREAK_PROCSEQ_IF_CHECK_SYMBOL('}') 
                        IF_CHECK_KEYWORD("default") 
                                CHECK_SYMBOL(':') 
                                EXEC_PROC(block_statement) 
                                CHECK_SYMBOL('}') 
                        END_IF
                        CHECK_KEYWORD("case") 
                        CHECK_LITERAL 
                        CHECK_SYMBOL(':') 
                        EXEC_PROC(block_statement) 
                END_PROCSEQ
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("for") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression_statement) 
                INIT_PROCSEQ
                        BREAK_PROCSEQ_IF_CHECK_SYMBOL(';') 
                        EXEC_PROC(expression) 
                        CHECK_SYMBOL(';') 
                END_PROCSEQ
                INIT_PROCSEQ
                        BREAK_PROCSEQ_IF_CHECK_SYMBOL(')') 
                        EXEC_PROC(expression) 
                        CHECK_SYMBOL(')') 
                END_PROCSEQ
                EXEC_PROC(block_statement) 
                ACCEPT_PROC 
        END_IF
        IF_CHECK_KEYWORD("foreach") 
                CHECK_SYMBOL('(') 
                EXEC_PROC(expression) 
                CHECK_KEYWORD("in") 
                EXEC_PROC(expression) 
                IF_CHECK_SYMBOL(':') 
                        EXEC_PROC(expression) 
                END_IF
                CHECK_SYMBOL(')') 
                EXEC_PROC(block_statement) 
                ACCEPT_PROC 
        END_IF
        EXEC_PROC(expression_statement) 
END_PROC

DEF_PROC(variable_definition) 
        GET_IDENT 
        IF_CHECK_KEYWORD("as") 
                EXEC_PROC(type_name) 
        END_IF
        IF_CHECK_SYMBOL('=') 
                EXEC_PROC(expression) 
        END_IF
END_PROC

DEF_PROC(object_member) 
        IF_CHECK_ONE_KEYWORD("public" ,"private" ,"protected" ,"internal" ,"extern" ,"sealed" ,"static" ,"virtual" ,"const")
        END_IF
        IF_CHECK_KEYWORD("var") 
                EXEC_PROC(variable_definition) 
                CHECK_SYMBOL(';') 
                ACCEPT_PROC 
        END_IF
        EXEC_PROC(function_definition) 
END_PROC

DEF_PROC(object_definition) 
        CHECK_KEYWORD("object") 
        GET_IDENT 
        IF_CHECK_SYMBOL(':') 
                INIT_PROCSEQ
                        EXEC_PROC(type_name) 
                        RECALL_PROCSEQ_IF_CHECK_SYMBOL(',') 
                END_PROCSEQ
        END_IF
        CHECK_SYMBOL('{') 
        INIT_PROCSEQ
                EXEC_PROC(object_member) 
                IF_CHECK_SYMBOL('}') 
                        CHECK_SYMBOL(';') 
                END_IF
        END_PROCSEQ
END_PROC

DEF_PROC(main) 
        ACCEPT_PROC_IF_EXEC_PROC(variable_declaration) 
        ACCEPT_PROC_IF_EXEC_PROC(object_definition) 
        ACCEPT_PROC_IF_EXEC_PROC(function_definition) 
END_PROC

