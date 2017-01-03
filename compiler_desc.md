AC Compiler Descriptor language is a simple and flexible language to define compilation rules as :

-	Steps : a step is one compilation statement
-	Procedures : a procedure is a list of consecutive steps, defined as : “. proc_name : step_1 step_n ;”, procedure will fail if one of its step fail (compiler rise an error if not an optional call).

Steps could be simple statement : keywords (start with ~), tokens (as a string), literal, identifier …
Or complex :
-	Steps sequence : a nested list of consecutive steps, defined as : “{ step_1 … stepn_n }”
-	Optional sequence : a list of consecutive steps executed only if the conditional step is matched, defined as  “[ conditional_step step_1 … step_n ]”
-	Procedure call defined as : “< proc_name >”

Note that :
-	Conditional step could not be Steps sequence or Optional sequence.
-	When conditional step is a procedure call,  the compiler will try to match the whole procedure, but no error will be raised if the procedure fail.

Some specific step could be used in a steps sequence : 
-	break step defined as “,” : to interrupt the steps sequence and accept is
-	recall step defined as “*” : to recall the steps sequence from the beginning 

Also, in a steps sequence or optional sequence, we could force the current procedure acceptation by using the accept step defined as “;”

For example “if” statement could be defined as : “.if_statement : ~if < expression > < code_block > [~else < code_statement >]”

As an alternative (recommended in some cases) we can define the “if” statement as keyword procedure as following 

“.keyword if : < expression > < code_block > [~else < code_statement >]” 

and to call this procedure just use “^if”.

Not that when a conditional step is a keyword procedure call, we will get an error if one of its steps fail. 
