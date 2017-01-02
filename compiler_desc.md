AC Compiler Descriptor language is a simple and flexible language to define compilation rules as :

-	Steps : a step is one compilation statement
-	Procedures : a procedure is a list of consecutive steps, defined as : “. proc_name : step_1 step_n ;”, procedure will fail if one of its step fail (compiler rise an error if not an optional call).

Steps could be simple statement : keywords, tokens, literal, identifier …
Or complex :
-	Steps sequence : a nested list of consecutive steps, defined as : “{ step_1 … stepn_n }”
-	Optional sequence : a list of consecutive steps executed only if the conditional step is matched, defined as  “[ conditional_step step_1 … step_n ]”
-	Procedure call defined as : “< proc_name >”

Note that :
-	Conditional step could not be Steps sequence or Optional sequence.
-	When conditional step is a procedure call,  the compiler will try to match the whole procedure, but no error will be raised if the procedure fail.

Some specific steps could be used in a steps sequence : 
-	break step defined as “,” : to interrupt the steps sequence and accept is
-	recall step defined as “*” : to recall the steps sequence from the beginning 

Also, in a steps sequence or optional sequence, we could force the current procedure acceptation by using the accept step defined as “;”
