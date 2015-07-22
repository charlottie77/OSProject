
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

/*TTY *mytty=tty_table+3;
char mytty_cmd[255];
char myGetChar(){
	openStartScanf(mytty);
	while (mytty->startScanf) ;
	//mystrncpy(mytty_cmd,lalala->str,255);
	return mytty->str[0];
}*/

int strcmp(char *str1,char *str2)
{
	int i;
	for (i=0; i<strlen(str1); i++)
	{
		if (i==strlen(str2)) return 1;
		if (str1[i]>str2[i]) return 1;
		else if (str1[i]<str2[i]) return -1;
	}
	return 0;
}

void display(int z[5][5])
{
	//clearScreen();
	printf(" ---- ---- ---- ----\n");
}

void strlwr(char *str)
{
	int i;
	for (i=0; i<strlen(str); i++)
	{
		if ('A'<=str[i] && str[i]<='Z') str[i]=str[i]+'a'-'A';
	}
}

char *mystrncpy(char *dest,const char *src,int n)  
{  
    char *strDest=dest;  
    //assert((dest!=NULL)&&(src!=NULL));  
    while( n &&(*dest++=*src++)!='\0')  
    {  
        n--;  
    }  
    if (n)  
    {  
        while(--n)  
        {  
            *dest++='\0';  
        }  
    }  
    return strDest;  
} 

void addToQueue(PROCESS* p)
{
	p->state=kRUNNABLE;
	if (p->priority>=10)
	{
		firstQueue[firstLen]=p;
		firstLen++;
		p->ticks=2;
		p->whichQueue=1;
	}
	else
	{
		secondQueue[secondLen]=p;
		secondLen++;
		p->ticks=p->priority;
		p->whichQueue=2;
	}
}

/*======================================================================*
                            tinix_main
 *======================================================================*/
PUBLIC int tinix_main()
{
	//disp_str("-----\"tinix_main\" begins-----\n");
	clearScreen();
	disp_str("         *************************************************************\n");
	disp_str("        *                                                             \n");
	disp_str("       * **********    ******    ***      **    ******    **    **    \n");
	disp_str("      *      **          **      ****     **      **        *  *      \n");
	disp_str("     *       **          **      ** **    **      **         **       \n");
	disp_str("    *        **          **      **  **   **      **         **       \n");
	disp_str("   *         **          **      **   **  **      **         **       \n");
	disp_str("  *          **          **      **    ** **      **         **       \n");
	disp_str(" *           **          **      **     ****      **       *    *     \n");
	disp_str("*            **        ******    **      ***    ******   **      **   \n");	
	disp_str("*                                                                     \n");
	disp_str("*                                                                     \n");
	disp_str("*                                                                     \n");
	disp_str("*   TUDIDI                                                            \n");
	disp_str("*                                                                     \n");
	disp_str("*                                                                     \n");
	disp_str("*                                                                     \n");
	disp_str("**********************************************************************\n");
	TASK*		p_task;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	t_16		selector_ldt	= SELECTOR_LDT_FIRST;
	int		i;
	t_8		privilege;
	t_8		rpl;
	int		eflags;
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		if (i < NR_TASKS) {	/* ���� */
			p_task		= task_table + i;
			privilege	= PRIVILEGE_TASK;
			rpl		= RPL_TASK;
			eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			/* �û����� */
			p_task		= user_proc_table + (i - NR_TASKS);
			privilege	= PRIVILEGE_USER;
			rpl		= RPL_USER;
			eflags		= 0x202;	/* IF=1, bit 2 is always 1 */
		}

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid	= i;			/* pid */

		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip	= (t_32)p_task->initial_eip;
		p_proc->regs.esp	= (t_32)p_task_stack;
		p_proc->regs.eflags	= eflags;

		p_proc->nr_tty		= 0;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	//�޸���������ȼ���ticks
	proc_table[0].priority = 15;
	proc_table[1].priority =  5;
	proc_table[2].priority =  5;
	proc_table[3].priority =  5;
	proc_table[4].priority =  7;
	proc_table[5].priority =  10;
	proc_table[6].priority =  10;
	proc_table[7].priority =  10;

	//�����ȶ��г�ʼ��
	firstLen=firstHead=secondLen=0;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		addToQueue(proc_table+i);
	}
	//ָ������̨
	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 1;
	proc_table[4].nr_tty = 1;
	proc_table[5].nr_tty = 1;
	proc_table[6].nr_tty = 2;
	proc_table[7].nr_tty = 5;

	k_reenter	= 0;
	ticks		= 0;

	p_proc_ready	= proc_table;

	init_clock();

	restart();

	while(1){}
}

void clearScreen()
{
	int i;
	disp_pos=0;
	for(i=0;i<80*25;i++)
	{
		disp_str(" ");
	}
	disp_pos=0;
}


void help()
{
	printf("           *////////////////////////////////////////////*/\n");
	printf("                   design by Doubi && LiangPuHe         \n");
	printf("           *////////////////////////////////////////////*/\n");
	printf("\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("      *////  help         --------  shwo the help menu     ////*\n");
	printf("      *////  clear        --------  clear screen           ////*\n");
	printf("      *////  alt+F2       --------  show the process run   ////*\n");
	printf("      *////  alt+F3       --------  goBang game            ////*\n");
	printf("      *////  alt+F4       --------  guess number game      ////*\n");
	printf("      *////  kill 2~5     --------  kill the process 2~5   ////*\n");
	printf("      *////  start 2~5    --------  start the process 2~5  ////*\n");
	printf("      *////  show         --------  show the process state ////*\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("\n");
}

void show()
{
	PROCESS* p;
	int i;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		p=&proc_table[i];
		printf("process%d:",p->pid);
		switch (p->state)
		{
		case kRUNNABLE:
			printf("    Runnable\n");
			break;
		case kRUNNING:
			printf("    Running\n");
			break;
		case kREADY:
			printf("    Ready\n");
			break;
		}
	}
}

void readOneStringAndOneNumber(char* command,char* str,int* number)
{
	int i;
	int j=0;
	for (i=0; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]==' ') break;
		str[j]=command[i];
		j++;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}

	*number=0;
	for (; i<strlen(command) && '0'<=command[i] && command[i]<='9'; i++)
	{
		*number=*number*10+(int) command[i]-'0';
	}
}

void dealWithCommand(char* command)
{
	strlwr(command);
	if (strcmp(command,"clear")==0)
	{
		clearScreen();
		sys_clear(tty_table);
		return ;
	}
	if (strcmp(command,"help")==0)
	{
		help();
		return ;
	}
	if (strcmp(command,"show")==0)
	{
		show();
		return ;
	}

	char str[100];
	int number;
	readOneStringAndOneNumber(command,str,& number);
	if (strcmp(str,"kill")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kREADY;
			printf("kill process %d successful\n",number);
		}
		return ;
	}
	if (strcmp(str,"start")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kRUNNABLE;
			printf("start process %d successful\n",number);
		}
		return ;
	}
	printf("can not find this command\n");
}

/*======================================================================*
                               Terminal
 *======================================================================*/
void Terminal()
{
	TTY *p_tty=tty_table;
	p_tty->startScanf=0;
	while(1)
	{
		printf("DB=>");
		//printf("<Ticks:%x>", get_ticks());
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		dealWithCommand(p_tty->str);
	}
}


/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	int i = 0;
	while(1){
		printf("B");
		milli_delay(1000);
	}
}



/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()
{
	int i = 0;
	while(1){
		printf("C");
		milli_delay(1000);
	}
}

void TestD()
{
	int i=0;
	while (1)
	{
		printf("D");
		milli_delay(1000);
	}
}

void TestE()
{
	int i=0;
	while (1)
	{
		printf("E");
		milli_delay(1000);
	}
}
//----------------------------------------------
/*                 md5.h           */
#ifndef _MD5_H_
#define _MD5_H_

#define R_memset(x, y, z) memset(x, y, z)
#define R_memcpy(x, y, z) memcpy(x, y, z)
#define R_memcmp(x, y, z) memcmp(x, y, z)

typedef unsigned long UINT4;
typedef unsigned char *POINTER;

/* MD5 context. */
typedef struct {
  /* state (ABCD) */   
  /*�ĸ�32bits�������ڴ�����ռ���õ�����ϢժҪ������Ϣ���ȡ�512bitsʱ��Ҳ���ڴ��ÿ��512bits���м���*/ 
  UINT4 state[4];   

  /* number of bits, modulo 2^64 (lsb first) */    
  /*�洢ԭʼ��Ϣ��bits������,����������bits���Ϊ 2^64 bits����Ϊ2^64��һ��64λ�������ֵ*/
  UINT4 count[2];
  
  /* input buffer */ 
  /*����������Ϣ�Ļ�������512bits*/
  unsigned char buffer[64];  
} MD5_CTX;

void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
void MD5Final(unsigned char [16], MD5_CTX *);

#endif /* _MD5_H_ */


///////////////////////////////////////////////////////////////////////////

/*    md5.cpp   */


/* Constants for MD5Transform routine. */
/*md5ת���õ��ĳ������㷨����涨��*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void MD5Transform(UINT4 [4], unsigned char [64]);
static void Encode(unsigned char *, UINT4 *, unsigned int);
static void Decode(UINT4 *, unsigned char *, unsigned int);

/*
����bits���Ļ�������ΪʲôҪ64���ֽ��أ���Ϊ�������ܵ���Ϣ��bits����512��������Ϊ448ʱ��
��Ҫ����bits�����ֵΪ512=64*8 ��
*/
static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
���������⼸���궨����md5�㷨�涨�ģ����Ƕ���Ϣ����md5���ܶ�Ҫ�������㡣
��˵�о���ĸ��ָ��ٳ���ʱ�����⼸������Ĳ����Ϳ��Զ϶��ǲ����õ�md5
*/
/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
  Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
 }

/* MD5 initialization. Begins an MD5 operation, writing a new context. */
/*��ʼ��md5�Ľṹ*/
void MD5Init (MD5_CTX *context)
{
  /*����ǰ����Ч��Ϣ�ĳ������0,����ܼ�,��û����Ч��Ϣ,���ȵ�Ȼ��0��*/
  context->count[0] = context->count[1] = 0;

  /* Load magic initialization constants.*/
  /*��ʼ�����ӱ������㷨Ҫ�����������û��������*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
  operation, processing another message block, and updating the
  context. */
/*������ܵ���Ϣ���ݸ�md5�ṹ�����Զ�ε���
context����ʼ�����˵�md5�ṹ
input�������ܵ���Ϣ���������ⳤ
inputLen��ָ��input�ĳ���
*/
void MD5Update(MD5_CTX *context,unsigned char * input,unsigned int  inputLen)
{
 unsigned int i, index, partLen;
 

 /* Compute number of bytes mod 64 */
 /*����������Ϣ��bits���ȵ��ֽ�����ģ64, 64bytes=512bits��
 �����ж�������Ϣ���ϵ�ǰ����������Ϣ���ܳ����ܲ��ܴﵽ512bits��
 ����ܹ��ﵽ��Դչ���512bits����һ�δ���*/
 index = (unsigned int)((context->count[0] >> 3) & 0x3F);

 /* Update number of bits *//*����������Ϣ��bits����*/
 if((context->count[0] += ((UINT4)inputLen << 3)) < ((UINT4)inputLen << 3))
  context->count[1]++;
 context->count[1] += ((UINT4)inputLen >> 29);

 /*�������е��ֽ������Ȼ�������ֽڿ��� �ճ�64��������*/
 partLen = 64 - index;

 /* Transform as many times as possible.
  */
 /*�����ǰ������ֽ��� ���� �����ֽ������Ȳ���64�ֽ�������������ֽ���*/
 if(inputLen >= partLen) 
    {
  /*�õ�ǰ��������ݰ�context->buffer�����ݲ���512bits*/
  R_memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
  /*�û����������������512bits���Ѿ����浽context->buffer�У� ��һ��ת����ת��������浽context->state��*/
  MD5Transform(context->state, context->buffer);

 /*
 �Ե�ǰ�����ʣ���ֽ���ת�������ʣ����ֽ�<�������input��������>����512bits�Ļ� ����
 ת��������浽context->state��
 */
  for(i = partLen; i + 63 < inputLen; i += 64)/*��i+63<inputlen��Ϊi+64<=inputlen���������*/
   MD5Transform(context->state, &input[i]);

        index = 0;
    }
    else
  i = 0;

 /* Buffer remaining input */
 /*�����뻺�����еĲ��������512bits��ʣ��������䵽context->buffer�У������Ժ���������*/
 R_memcpy((POINTER)&context->buffer[index], (POINTER)&input[i], inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context. */
/*��ȡ���� �����ս��
digest���������յļ��ܴ�
context����ǰ���ʼ������������Ϣ��md5�ṹ
*/
void MD5Final (unsigned char digest[16],MD5_CTX *context)
{
 unsigned char bits[8];
 unsigned int index, padLen;

 /* Save number of bits */
 /*��Ҫ��ת������Ϣ(���е�)��bits���ȿ�����bits��*/
 Encode(bits, context->count, 8);

 /* Pad out to 56 mod 64. */
 /* �������е�bits���ȵ��ֽ�����ģ64, 64bytes=512bits*/
 index = (unsigned int)((context->count[0] >> 3) & 0x3f);
 /*������Ҫ�����ֽ�����padLen��ȡֵ��Χ��1-64֮��*/
 padLen = (index < 56) ? (56 - index) : (120 - index);
 /*��һ�κ������þ��Բ����ٵ���MD5Transform�ı����ã���Ϊ��һ�β�������512bits*/
 MD5Update(context, PADDING, padLen);

 /* Append length (before padding) */
 /*����ԭʼ��Ϣ��bits���ȣ�bits���ȹ̶�����64bits��ʾ������һ���ܹ�ǡ�ɴչ�512bits�������Ҳ������*/
 MD5Update(context, bits, 8);

 /* Store state in digest */
 /*�����յĽ�����浽digest�С�ok�����ڴ󹦸����*/
 Encode(digest, context->state, 16);

 /* Zeroize sensitive information. */

 R_memset((POINTER)context, 0, sizeof(*context));
}

/* MD5 basic transformation. Transforms state based on block. */
/*
��512bits��Ϣ(��block������)����һ�δ���ÿ�δ����������
state[4]��md5�ṹ�е�state[4]�����ڱ����512bits��Ϣ���ܵ��м����������ս��
block[64]�������ܵ�512bits��Ϣ
*/
static void MD5Transform (UINT4 state[4], unsigned char block[64])
{
 UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

 Decode(x, block, 64);

 /* Round 1 */
 FF(a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
 FF(d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
 FF(c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
 FF(b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
 FF(a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
 FF(d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
 FF(c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
 FF(b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
 FF(a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
 FF(d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
 FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
 FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
 FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
 FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
 FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
 FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
 GG(a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
 GG(d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
 GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
 GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
 GG(a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
 GG(d, a, b, c, x[10], S22,  0x2441453); /* 22 */
 GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
 GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
 GG(a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
 GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
 GG(c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
 GG(b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
 GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
 GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
 GG(c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
 GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

 /* Round 3 */
 HH(a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
 HH(d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
 HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
 HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
 HH(a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
 HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
 HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
 HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
 HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
 HH(d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
 HH(c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
 HH(b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
 HH(a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
 HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
 HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
 HH(b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

 /* Round 4 */
 II(a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
 II(d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
 II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
 II(b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
 II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
 II(d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
 II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
 II(b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
 II(a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
 II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
 II(c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
 II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
 II(a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
 II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
 II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
 II(b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

 state[0] += a;
 state[1] += b;
 state[2] += c;
 state[3] += d;

 /* Zeroize sensitive information. */
 R_memset((POINTER)x, 0, sizeof(x));
}

/* Encodes input (UINT4) into output (unsigned char). Assumes len is
  a multiple of 4. */
/*��4�ֽڵ�����copy���ַ���ʽ�Ļ�������
output������������ַ�������
input����ת�������ֽڵ�������ʽ������
len��output�������ĳ��ȣ�Ҫ����4��������
*/
static void Encode(unsigned char *output, UINT4 *input,unsigned int  len)
{
 unsigned int i, j;

 for(i = 0, j = 0; j < len; i++, j += 4) {
  output[j] = (unsigned char)(input[i] & 0xff);
  output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
  output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
  output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
 }
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
  a multiple of 4. */
/*������ĺ��������෴����һ�����ַ���ʽ�Ļ������е�����copy��4�ֽڵ������У�����������ʽ���棩
output������ת����������
input����ת�����ַ�������
len��������ַ��������ĳ��ȣ�Ҫ����4��������
*/
static void Decode(UINT4 *output, unsigned char *input,unsigned int  len)
{
 unsigned int i, j;

 for(i = 0, j = 0; j < len; i++, j += 4)
  output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
   (((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
}


////////////////////////////////////////////////////////////////////////////////

// md5test.cpp : Defines the entry point for the console application.
//


void Yume()
{
 MD5_CTX md5;
 MD5Init(&md5);                         //��ʼ������md5���ܵĽṹ
 
 unsigned char encrypt[200];     //����ڼ��ܵ���Ϣ
 unsigned char decrypt[200];       //��ż��ܺ�Ľ��
 TTY *p_tty=tty_table+2;
 p_tty->startScanf=0;  
 openStartScanf(p_tty);
 while (p_tty->startScanf) ;
 printf("p_tty ");
 printf(p_tty->str);
 printf("\n");
 mystrncpy(encrypt,p_tty->str,200);             //������ܵ��ַ�
 printf("en ");
 printf(encrypt);
 printf("\n");
 MD5Update(&md5,encrypt,strlen((char *)encrypt));   //�������ܵ��ַ����м���

 MD5Final(decrypt,&md5);                                            //������ս��
 
 printf("before:%s\n after:",encrypt);
int a=0;
 /*for(a=0;a<16;a++)
{
  printf("  ");
  printf(decrypt);
}*/
printf(decrypt);
 
 printf("\n\n\nFinished!\n");
 
}

/*  ���ϴ�����vc6�±���ͨ���������������ܹ��õ���ȷ��md5ֵ  */
//---------------------------------------------

void ASand()
{

	TTY *p_tty=tty_table+5;
	p_tty->startScanf=0;
	char strrrrr[30];
	printf("aaaaaaa");
	openStartScanf(p_tty);
	while (p_tty->startScanf) ;
	mystrncpy(strrrrr,p_tty->str,30);
	printf(strrrrr);
	while(1);

}
