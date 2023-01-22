# RegExC
This Is a atempt to build a Regex Libray in C

the goal is to provide fucnticons in the sytle of

Regex_SourceType( char* Pattern , FnctPtr Action)

Defining Pattern
Pattern is a string Containing The Expression To Test for
each Letter is A Test The Integer of that Letter maps
to a Test in A Big Switch Statement.
Some Of The Letters may take arguments like = test for equality
If The Source Consits of Words We have to write =WordImLookingFor
as a command in the Pattern for example to to test if there is
a certian Word Wich Is Led by Another and Written in Uppercasee
we might write Pattern="=Word^>Word2^ maby this syntax will result
in Precedence Problems witch need to be resolved. 
