#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// Regex Expression Format =  Object1Logic&Control Object2Logic&Control Object3Logic&Control ...

#define MAX_ARGUMENT_LENGTH 512
#define MAX_ARGUMENTS       26
#define MAX_OP_CODES        255
#define MAX_WORD_LENGTH     256

#define DEFAULT_TABLE_SIZE         2048
#define MAX_GROUP_LENGTH           1024
#define MAX_GROUP_NAME_LENGTH      256

//Todo : Try Out text++ to search throught text
//Todo : Add Default Groups And Group Tests
//Todo : Build Execution Engine

//ASCII Letters Are form 65 to 90 For Capital Letters and 97 to 122 for lowercase letters

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

typedef struct
{
 char* name;
 char* group;
 void* next;
}Regex_GroupTableBucket;

Regex_GroupTableBucket* group_table;

typedef int8_t (*Regex_Action)(uint64_t trigger, char* text);

int8_t TestAction(uint64_t trigger , char* text);

int8_t Regex_AddOpCode(Regex_Operation *operation, Regex_OpCode op_code, char* argument);

int8_t Regex_ExecuteOperations(char* text, Regex_Operation* operations, uint16_t op_count , Regex_Action action);



int64_t Regex_GetObject(Regex_Object object, char** head);


int16_t Regex_IsStrictWord(char** head);

int64_t Regex_GetObjectStrictWord(char** head,char word[MAX_WORD_LENGTH]);


int64_t Regex_GetObjectStrictChar(char** head,char* strict_char);




int64_t Regex_JumpToLetter(char** head);

int8_t Regex_JumpToSpace(char** head);




uint64_t Regex_HashGroup(char* name);

int8_t Regex_RegisterGroup(char* name,char* group);

int8_t Regex_InitGroupTable();

void Regex_PrintGroupTable();


int main()
{
 Regex_Action action=&TestAction;
 Regex_Operation operation={};
 operation.logic=NOT;
 operation.object=STRICT_WORD;
 Regex_AddOpCode(&operation,EQUAL,"Hallo");


 Regex_InitGroupTable();

 char* group="Hallo,Bye,Tschüss,Was,Geht";

 char* test="Brau";
 char* test1="Adau";
 char* test2="Dradwau";
 char* test3="Rraau";

 Regex_RegisterGroup(test,group);
 Regex_RegisterGroup(test,group);
 Regex_RegisterGroup(test1,group);
 Regex_RegisterGroup(test2,group);
 Regex_RegisterGroup(test3,group);

 //Regex_ExecuteOperations(test,NULL,0,action);

 Regex_PrintGroupTable();

}


int8_t TestAction(uint64_t trigger, char* text)
{
 return 1;
}

int8_t Regex_ExecuteOperations(char* text, Regex_Operation* operations, uint16_t op_count , Regex_Action action)
{
 char* head=text;
 int8_t eof=0;

 eof=Regex_GetObject(STRICT_WORD,&head);
 if(eof==-1){ printf(" End of File \n "); return 1; }

 eof=Regex_GetObject(STRICT_CHAR,&head);
 if(eof==-1){ printf(" End of File \n "); return 1; }

 eof=Regex_GetObject(CHAR,&head);
 if(eof==-1){ printf(" End of File \n "); return 1; }

 eof=Regex_GetObject(STRICT_WORD,&head);
 if(eof==-1){ printf(" End of File \n "); return 1; }

}


// ----------------------------------------------------------------- Word Functions Start

int64_t Regex_GetObjectStrictWord(char** head, char word[MAX_WORD_LENGTH])
{
  int16_t is_word=0;

  while(1){

    if(Regex_JumpToLetter(head)==-1){ printf(" Couldnt Find Object"); return -1; }

    printf(" Head Jumped To Character %c \n" , **head );

    is_word=Regex_IsStrictWord(head);

    printf("Word is %d Long \n ", is_word);

    if(is_word!=-1){
      if(is_word>MAX_WORD_LENGTH){ printf(" word to long \n ");  return -1; }
      char* word_start=(*head)-is_word;
      strncpy(word,word_start,is_word);
      word[is_word]='\0';
      printf(" Found Word %s \n ", word);
      break;
    }

    if(Regex_JumpToSpace(head)==-1){ printf(" End Of File Reached"); return -1; }

  }
}

int16_t Regex_IsStrictWord(char** head)
{
  uint16_t length=0;

  while( (**head>64 && **head<91) || (**head>96 && **head<128)  ){
    length++;
    (*head)++;
  }

  if(**head=='\0' || **head==' '){  return length; }

  printf(" Found SpecialChar %c  \n ", **head);

  return -1;
}


// ----------------------------------------------------------------- Word Functions End


// ----------------------------------------------------------------- Char Functions Start

int64_t Regex_GetObjectStrictChar(char** head,char* strict_char)
{

  while(**head!='\0')
    {

      if( (**head>64 && **head<91) || (**head>96 && **head<128) ){
        printf(" Strict Character %c \n ", **head);
        *strict_char=**head;
        (*head)++;
        return 1;
      }
      (*head)++;
    }

  printf(" No Object Found \n ");
  return -1;
}

// ----------------------------------------------------------------- Char Functions End


// ----------------------------------------------------------------- Op Code Functions Start

int8_t Regex_AddOpCode(Regex_Operation *operation, Regex_OpCode op_code, char* argument)
{
 if(operation->operation_count>MAX_OP_CODES){ printf(" To Many OpCodes Add Failed\n "); return 0; }

 operation->op_code[operation->operation_count]=op_code;
 strcpy(operation->argument[operation->operation_count],argument);

 operation->operation_count++;

 return 1;
}

// ----------------------------------------------------------------- Op Code Functions End


int64_t Regex_GetObject(Regex_Object object, char** head)
{

 char  word[MAX_WORD_LENGTH];
 char  strict_char;
 char  character;

 switch(object)
 {
  case STRICT_WORD: return Regex_GetObjectStrictWord(head,word); break;

  case STRICT_CHAR: return Regex_GetObjectStrictChar(head,&strict_char); break;

  case CHAR:if(**head=='\0'){ printf(" No Futher Characters \n "); return -1; }
            character=**head;
            printf(" Not Strict Character %c Found \n ", character);
            break;

  default  : break;
 }

}


// ----------------------------------------------------------------- Utils


int64_t Regex_JumpToLetter(char** head)
{

 uint64_t length=0;

 while(! ( ( **head>64 && **head<91 ) || ( **head >96 && **head<128 ) ) ){
   if(**head=='\0'){ return -1; }
   (*head)++;
   length++;
 }

 return length;
}

int8_t Regex_JumpToSpace(char** head)
{

 while(**head!=' ')
  {
   if(**head=='\0'){ printf(" End of Text Reached "); return -1; }
   (*head)++;
  }

  return 1;
}


// ----------------------------------------------------------------- GroupTable


uint64_t Regex_InitGroupTable()
{
 group_table=calloc(sizeof(Regex_GroupTableBucket),DEFAULT_TABLE_SIZE);

 for(uint16_t i=0; i<DEFAULT_TABLE_SIZE;i++)
 {
   group_table[i].name=calloc(1,MAX_GROUP_NAME_LENGTH);
   group_table[i].name[0]='\0';
   group_table[i].group=calloc(1,MAX_GROUP_LENGTH);
   group_table[i].next=NULL;
 }
}

int8_t Regex_RegisterGroup(char* name,char* group)
{

 uint64_t slot=Regex_HashGroup(name)%DEFAULT_TABLE_SIZE;

 if(group_table[slot].name[0]=='\0')
 {
  strncpy(group_table[slot].name,name,MAX_GROUP_NAME_LENGTH);
  strncpy(group_table[slot].group,name,MAX_GROUP_LENGTH);

  return 1;
 }

 Regex_GroupTableBucket* next_bucket = group_table[slot].next = calloc(sizeof(Regex_GroupTableBucket),1);

 next_bucket->name=calloc(1,MAX_GROUP_NAME_LENGTH);
 next_bucket->group=calloc(1,MAX_GROUP_LENGTH);

 strncpy(next_bucket->name,name,MAX_GROUP_NAME_LENGTH);
 strncpy(next_bucket->group,group,MAX_GROUP_LENGTH);

}

uint64_t Regex_HashGroup(char* name)
{
  unsigned long hash = 5381;
  int c;

  while (c = *name++)
  {
   hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

void Regex_PrintGroupTable()
{
  for(uint16_t i=0;i<DEFAULT_TABLE_SIZE;i++)
    {
      Regex_GroupTableBucket* bucket=&(group_table[i]);

      if(bucket->name[0]!='\0')
        {

          printf(" Registerd Group %s at %d \n " , bucket->name,i);

          while( bucket->next !=NULL)
            {
              printf(" Registerd Group %s at %d \n " , bucket->name,i);
              bucket=(Regex_GroupTableBucket*)bucket->next;
            }
        }
    }
}
