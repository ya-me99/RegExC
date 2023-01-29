#include "regex_c.h"


//Todo : Handle Control Flow

//Todo : Finish All Tests

//Todo : And A Few Default Groups

//Todo : ??? More Dynamic Memory Alloation Than Static OverSized Arrays ???

//Todo : Find a way to Put Trigger At Start Not at end of Match

//Todo : Build a Trigger Struct


Regex_GroupTableBucket* group_table;

int8_t TestAction(char* trigger);



int main()
{

 Regex_InitGroupTable();

 Regex_Action action=&TestAction;
 Regex_Operation operation={ };
 operation.logic=0;
 operation.object=STRICT_WORD;
 Regex_AddOpCode(&operation,EQUAL,"Lorem");
 Regex_AddOpCode(&operation,EQUAL,"ipsum");
 Regex_AddOpCode(&operation,EQUAL,"dolor");


 Regex_ExecuteOperations(lorem,&operation,1,action);

 printf("%s \n " ,lorem);
}

int8_t TestAction(char* trigger)
{
 *trigger='!';
 return 1;
}

int8_t Regex_ExecuteOperations(char* text,
                               Regex_Operation* operations,
                               uint16_t op_count ,
                               Regex_Action action)
{
 char* head=text;
 char* trigger;
 int8_t eof=0;

 int8_t action_fire=0;


 char  word[MAX_WORD_LENGTH];
 char  strict_char;
 char  character;

 while(1)
 {
  for(uint16_t i=0;i<op_count;i++)
  {
    switch(operations[i].object)
    {
     case STRICT_WORD: eof=Regex_GetObjectStrictWord(&head,word); trigger=head-eof; break;

     case STRICT_CHAR: eof=Regex_GetObjectStrictChar(&head,&strict_char); break;

     case CHAR:if(*head=='\0'){ printf(" No Futher Characters \n "); eof=-1; }
               character=*head;
               printf(" Not Strict Character %c Found \n ", character);
               break;

     default : break;
    }

    if(eof==-1){ printf(" End Of File Reached "); return 1; }

   switch(operations[i].object)
   {
    case STRICT_WORD:action_fire=Regex_TestStrictWord(operations[i],word); break;

    default  : break;
   }

   if(action_fire==1){ action(trigger); }
  }

 }
}


// ----------------------------------------------------------------- Word Functions Start

int64_t Regex_GetObjectStrictWord(char** head, char word[MAX_WORD_LENGTH])
{
  int16_t word_length=0;

  while(1){

    if(Regex_JumpToLetter(head)==-1){ printf(" Couldnt Find Object"); return -1; }

    printf(" Head Jumped To Character %c \n" , **head );

    word_length=Regex_IsStrictWord(head);

    printf("Word is %d Long \n ", word_length);

    if(word_length!=-1){
      if(word_length>MAX_WORD_LENGTH){ printf(" word to long \n ");  return -1; }
      char* word_start=(*head)-word_length;
      strncpy(word,word_start,word_length);
      word[word_length]='\0';
      printf(" Found Word %s \n ", word);
      return word_length;
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

  return length;
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
 if(operation->operation_count>MAX_OP_CODES){
    printf(" To Many OpCodes Add Failed\n "); return -1; }

 operation->op_code[operation->operation_count]=op_code;
 strcpy(operation->argument[operation->operation_count],argument);

 operation->operation_count++;

 return 1;
}

// ----------------------------------------------------------------- Op Code Functions End


// ----------------------------------------------------------------- Tests Start

int8_t Regex_TestStrictWord(Regex_Operation operation,char* word)
{
 int8_t result=0;
 int op_code_counter=0;

 while(operation.op_code[op_code_counter]!=0)
 {
   switch(operation.op_code[op_code_counter])
   {
    case EQUAL : result=Regex_StrictWordEqual(word,operation.argument[op_code_counter]);break;
    default: result=-1 ;break;
   }

   op_code_counter++;
 }

 return result;
}


int8_t Regex_StrictWordEqual(char* a,char* b)
{
  if(strcmp(a,b)==0){ return 1; }

  return 0;
}

// ----------------------------------------------------------------- Tests End

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


int8_t Regex_InitGroupTable()
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

 Regex_GroupTableBucket* next_bucket = group_table[slot].next =
 calloc(sizeof(Regex_GroupTableBucket),1);

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
