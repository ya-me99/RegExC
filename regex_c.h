
#ifndef REGEX_C_H
#define REGEX_C_H

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// Regex Expression Format =  Object1Logic&Control Object2Logic&Control Object3Logic&Control ...

#define MAX_ARGUMENT_LENGTH        512
#define MAX_ARGUMENTS              32
#define MAX_OP_CODES               256
#define MAX_WORD_LENGTH            256

#define DEFAULT_TABLE_SIZE         2048
#define MAX_GROUP_LENGTH           1024
#define MAX_GROUP_NAME_LENGTH      256

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
 EQUAL=1,
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



//--------------------------- Regex Core Prototypes Start

int8_t Regex_AddOpCode(Regex_Operation *operation,Regex_OpCode op_code, char* argument);

char* Regex_ExecuteOperations(char* text,
                              Regex_Operation* operations,
                              uint16_t op_count);

// --------------------------- Regex Core Prototypes End


// --------------------------- StrictWord Prototypes Start

int16_t Regex_IsStrictWord(char** head);

int16_t Regex_GetObjectStrictWord(char** head,char word[MAX_WORD_LENGTH]);

//---------------------------- StrictWord Prototypes End


//---------------------------- Char Prototypes Start


int8_t Regex_GetObjectStrictChar(char** head,char* strict_char);

//---------------------------- Word Prototypes End


//-------------------------------- Utils Start

int64_t Regex_JumpToLetter(char** head);

int8_t Regex_JumpToSpace(char** head);

//--------------------------------- Utils End


//------------------------ GroupTable Prototypes Start

uint64_t Regex_HashGroup(char* name);

int8_t Regex_RegisterGroup(char* name,char* group);

int8_t Regex_InitGroupTable();

void Regex_PrintGroupTable();

//-------------------------- GroupTable Prototypes End


//-------------------------- Tests Prototypes Sart

int8_t Regex_TestStrictWord(Regex_Operation operation,char* word);

int8_t Regex_StrictWordEqual(char* a,char* b);

//-------------------------- Tests Prototypes Sart



char lorem[2048]={ "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Ste "};
#endif
