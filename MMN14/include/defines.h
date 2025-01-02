/* Define boolean values */
#pragma once

typedef int bool;
#define FALSE  0
#define TRUE 1

#define MAX_LINE_LEN 256

#define BLOCK_LEN 16

#define CATEGORY_NONE 0
#define CATEGORY_CODE 1
#define CATEGORY_DATA 2

#define OPR_NUM_0 0
#define OPR_NUM_1 1
#define OPR_NUM_2 2

/* operand types */
#define IMMEDIATE_OPR 1 
#define DIRECT_OPR 2
#define INDIRECT_OPR 4
#define REGISTER_OPR 8

#define ABSOLUTE 0x4
#define RELATIVE 0x2
#define EXTERNAL 0x1


