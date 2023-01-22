# RegExC
This Is a atempt to build a Regex Libray in C

the goal is to provide fucnticons in the sytle of

Regex_SourceType(char* Pattern , enum PatternMode , FnctPtr Action)

Sources May be a Character Stream or Multidimensional Array of Lines

There is some Ambiguity when it comes to Word Patterns for example
what is defined to be a word is Peter a word because its seperated
with 2 empty spaces ? that woud mean that Peter. is also a word allthoug
it might make no sence as such , assuming that we Alphanumeric / Special
chars are not a Word is not good enought either since you might want to
search for a Email Address.
Threfore i chose the PatternMode enum to Provide additional information for
such cases to differ between Strict Words and Alphanumeric Words etc. 


Defining Pattern
Pattern is a string Containing The Expression To Test for
each Letter is a Test The Integer of that Letter maps
to a Test in A Big Switch Statement Letters may be preceded by a Modifire like ! to Change The Output Or Provide Primitive Control Flow .
Also Each Command is Led By A Object Identifire
C har
W ord
S entence
L ine  
Some Of The Letters may take arguments like = test for equality
If we look for a Word we have to write "=WordImLookingFor"
as a command in the Pattern for example to to test if there is
a certian Word Wich Is Led by Another and Written in Uppercasee
we might write Pattern="=Word^>Word2^.

There is a Precendence Problem in this Souliton witch may waste Performance 
by default we evaluate the statement form left to right how ever the precedence
needs to be choosen best by the cost of the test if i test 
Pattern="=Word^>Word2^" it may be hard to evaluate witch test is the cheapest
also some tests 
 
Object :
W = Specifys that the Object Will be A Word
C = Specifys that the Object Will be A Character 
S = Specifys that the Object Will be A Sentence
L = Specifys that the Object Will be A Line


Modifires :
! = the Action will Fire if The Test Fails
| = Or


Pattern Cmds:
"Mod = Object" Tests For Equality                                       Cost Character = + Word = ++ = Line = +++ Expensive Sentence = +++ 
"Mod } Object" Test If Someting Contains Something                      Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  
"Mod . Object" Test If Someting Ends With                               Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  
"Mod > Object" Proceeded by Something                                   Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  
"Mod < Object" Preceeded by Something                                   Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  
"Mod $ Object" Checks if something is or contains Numbers               Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  
"Mod # Object" Checks for Special Characters                            Cost Character = - Word = ++ = Line = +++ Expensive Sentence = +++  


void foo (type a , type b );

Pattern = Start Word Of Type proceeded with StrictWord Identifier proceded by Char ( proceded by Type Proceeded by Strict Word Identifire proceeded by Char , proceeded with )  Or jmp back 2 preceeded
by ; 


Pattern =W[Types>W=IdentifireC=(>W[Type>|C=,#2>C=)>C=;





     