#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// Regex Expression Format =  Object1 Logic&Control Object2 Logic&Control Object3 ...
                                  
#define MAX_ARGUMENT_LENGTH 512
#define MAX_ARGUMENTS       26
#define MAX_OP_CODES        255
#define MAX_WORD_LENGTH     256

//Todo : Try Out text++ to search throught text;
//Todo : Build Execution Engine

typedef enum
{
 STRICT_WORD,
 STRICT_CHAR,
 CHAR,
}Regex_Object;

typedef enum
{
 EQUAL,
 IN_GROUP,
 UPPERCASE,
 LOWERCASE,
 NOT,
}Regex_OpCode;

typedef enum
{
 JUMP,
 FOR,
}Regex_Control;

typedef enum
{
 OR,
 XOR,
}Regex_Logic;

typedef struct 
{
 Regex_Object    object;
 Regex_OpCode    op_code[MAX_OP_CODES];
 Regex_Logic     logic;
 Regex_Control   control;
 uint8_t         operation_count;
 char            argument[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
}Regex_Operation;


typedef uint8_t (*Regex_Action)(uint64_t trigger, char* text);

uint8_t TestAction(uint64_t trigger , char* text);

uint8_t Regex_AddOpCode(Regex_Operation *operation, Regex_OpCode op_code, char* argument);

uint8_t Regex_ExecuteOperations(char* text, Regex_Operation* operations, uint16_t op_count , Regex_Action action);

int64_t Regex_JumpToLetter(char* text, uint64_t current_pos);

int16_t Regex_IsStrictWord(char* text, uint64_t pos);

uint64_t Regex_GetObject(Regex_Object object, uint64_t current_pos, char* text);

int8_t Regex_GetObjectStrictWord(uint64_t pos,char* text,char word[MAX_WORD_LENGTH]);

int8_t Regex_GetObjectStrictChar(uint64_t pos,char* text,char* strict_char);

int main()
{

 Regex_Operation operation={};
 operation.logic=NOT;
 operation.object=STRICT_WORD;
 Regex_AddOpCode(&operation,EQUAL,"Hallo");
  
 char* test="Bu!lu % Blib Blabber";

 Regex_GetObject(STRICT_WORD,0,test);
 Regex_GetObject(STRICT_CHAR,0,test);
 Regex_GetObject(CHAR,0,test);

 printf(" %s \n ", operation.argument[0]);

 return 0;
}

uint8_t Regex_AddOpCode(Regex_Operation *operation, Regex_OpCode op_code, char* argument)
{
 if(operation->operation_count>MAX_OP_CODES){ printf(" To Many OpCodes Add Failed\n "); return 0; }

 operation->op_code[operation->operation_count]=op_code;
 strcpy(operation->argument[operation->operation_count],argument);
 
 operation->operation_count++;

 return 1;
}

uint8_t Regex_ExecuteOperations(char* text, Regex_Operation* operations, uint16_t op_count , Regex_Action action)
{
 return 1;
}


//ASCII Letters Are form 65 to 90 For Capital Letters and 97 to 122 for lowercase letters

uint64_t Regex_GetObject(Regex_Object object, uint64_t current_pos, char* text)
{
 char  word[MAX_WORD_LENGTH];
 char  strict_char;
 char  character;

 uint64_t pos=current_pos;

 switch(object)
 { 
  case STRICT_WORD: Regex_GetObjectStrictWord(pos,text,word);
                    break;

  case STRICT_CHAR: Regex_GetObjectStrictChar(pos,text,&strict_char); 

  case CHAR:if(text[current_pos]=='\0'){ printf(" No Futher Characters \n "); return 0; }
            character=text[current_pos];
            printf(" Not Strict Character %c Found \n ", character);
            break;

  default  : break;
 }


}

int8_t Regex_GetObjectStrictChar(uint64_t pos,char* text,char* strict_char)
{

 while(text[pos]!='\0')
 {

  if( (text[pos]>64 && text[pos]<91) || (text[pos]>96 && text[pos]<128) ){ 
   printf(" Strict Character %c", text[pos]);
   strict_char=text[pos]; 
   return pos; 
  }

  pos++;
 }
 
 printf(" No Object Found \n ");
 return -1;
}

int8_t Regex_GetObjectStrictWord(uint64_t pos,char* text,char word[MAX_WORD_LENGTH])
{
 int16_t is_word=0;  
 int64_t jump=0;

 while(1){
  jump=Regex_JumpToLetter(text,pos);
 
  if(jump==-1){ printf(" Couldnt Find Object"); return 0; }
 
  pos+=jump;
  jump=0;
  
  is_word=Regex_IsStrictWord(text,pos);
             
  if(is_word!=-1){ 
   strncpy(word,&text[pos],is_word);
   word[is_word+1]='\0';
   printf(" Found Word %s \n ", word);
   break;
  }
             
  while(text[pos]!=' '){ 
   pos++; 

   if(text[pos]=='\0'){
    printf(" Couldnt Find Object"); return 0; 
   }
  }

 }
}

int16_t Regex_IsStrictWord(char* text, uint64_t pos)
{
 uint16_t length=0;
 
 while( (text[pos]>64 && text[pos]<91) || (text[pos]>96 && text[pos]<128)  ){ 
   printf(" Found Letter %c at pos %d \n ", text[pos],pos);
   length++;
   pos++;
 }
 
 if(text[pos]=='\0' || text[pos]==' '){  return length; } 

 printf(" Found SpecialChar %c at pos %d \n ", text[pos],pos);

 return -1;
}


int64_t Regex_JumpToLetter(char* text, uint64_t pos)
{

 uint64_t length=0;

 while(! ( (text[pos]>64 && text[pos]<91) || (text[pos]>96 && text[pos]<128) ) ){ 
   if(text[pos]=='\0'){ return -1; }
   pos++;
   length++;
 }

 return length;
}

