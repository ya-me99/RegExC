#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// Pattern Syntax = ControlFlowChar ObjectChar Operation Chars  

#define MAX_ARGUMENT_LENGTH 512
#define MAX_ARGUMENTS       26
#define MAX_OP_CODES        255

//Todo : Get Rid Of Disgusting Typedefs 
//Todo : Clean Up Substitute Functions Maby 
//Todo : Handle Control Flow And Logical Evaluation
//Todo : Build Execution Engine


typedef char*  Regex_CharStream;

typedef char*  Regex_Pattern;

typedef uint8_t (*Regex_Action)(uint64_t trigger, Regex_CharStream text);

uint8_t Regex_CheckCharStream(Regex_CharStream text , 
                              Regex_Pattern pattern , 
                              Regex_Action action);

uint8_t Action(uint64_t trigger , Regex_CharStream text);

int64_t Regex_JumpToLetter(Regex_Pattern pattern ,uint16_t current_position, char letter);


typedef struct 
{
 uint8_t    object;
 uint8_t    op_code[MAX_OP_CODES];
 uint8_t    logic;
 char       argument[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH+1];
}Regex_Operation;


int main()
{
 Regex_Pattern pattern="$!W=(Blubber)^> $C=(D)^>       ";
                       
 
 Regex_Action action=&Action;

 Regex_CharStream stream=NULL;

 Regex_CheckCharStream(stream,pattern,Action);
 
 return 0;
}


uint8_t Regex_CheckCharStream(Regex_CharStream text , 
                              Regex_Pattern pattern , 
                              Regex_Action action)
{
 uint8_t action_fire=0;
 Regex_Operation operations[256]={};

 // Step 1 Parse Pattern

 uint8_t operation_counter=0;
 uint64_t i=0;
 uint64_t jump;

 while(1)
 {  
  jump=Regex_JumpToLetter(pattern,i,'$'); 
  if(jump==-1){ break; }
  i+=jump+1;

  if(pattern[i]=='!' || pattern[i]=='&' || pattern[i]=='|' ||
     pattern[i]=='~' || pattern[i]=='*')
     { operations[operation_counter].logic=pattern[i]; i++; };
   
  operations[operation_counter].object=pattern[i];
  i++;
  
  printf(" Logical Operator = %c  , Object %c  \n ", operations[operation_counter].logic,operations[operation_counter].object);
  
  // Parse All OpCodes For The Object 
  uint8_t  argument_counter=0;
  uint8_t  op_code_counter=0;

  while(pattern[i]!=' ')
  {
   
   printf(" Op Code  %d \n ", op_code_counter);

   operations[operation_counter].op_code[op_code_counter]=pattern[i]; 
   i++;
 
    if(pattern[i]=='(')
    { 
     printf(" Op Code  %d  Found Argument \n ", op_code_counter);
     i++;
     
     uint8_t length=Regex_JumpToLetter(pattern,i,')');     
     i+=length;     

     if(length==0){ printf(" Fail Length 0 \n "); exit(1); }
     
     strncpy(operations[operation_counter].argument[argument_counter],
                        &pattern[i-length],length);
    
     operations[operation_counter].argument[argument_counter][length+1]='\0';

     printf(" Op Code  %d  Found Argument %s  \n ", op_code_counter , operations[operation_counter].argument[argument_counter]);
     argument_counter++;
     i++;
     if(argument_counter==MAX_ARGUMENTS){ printf(" Hard Fault To Many Arguments %d \n", argument_counter); exit(1); };
    }


   if(op_code_counter>20){ exit(1); }
   op_code_counter++;
  }  

  operation_counter++;
 }
 
 printf(" Recived %d Operations " , operation_counter);
}

uint8_t Action(uint64_t trigger , Regex_CharStream text)
{
 printf(" Bluber Die Blub \n  ");
}

int64_t Regex_JumpToLetter(Regex_Pattern pattern ,uint16_t current_position, char letter)
{ 
 uint8_t length=0;

 while(pattern[current_position]!=letter)
 {
  if(pattern[current_position]=='\0'){ return -1; }; 
   current_position++;
   length++;
 }

 return length;
}
