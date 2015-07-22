
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
#include "md5.h"

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
		if (i < NR_TASKS) {	/* 任务 */
			p_task		= task_table + i;
			privilege	= PRIVILEGE_TASK;
			rpl		= RPL_TASK;
			eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			/* 用户进程 */
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

	//修改这里的优先级和ticks
	proc_table[0].priority = 15;
	proc_table[1].priority =  5;
	proc_table[2].priority =  5;
	proc_table[3].priority =  5;
	proc_table[4].priority =  7;
	proc_table[5].priority =  10;
	proc_table[6].priority =  10;
	proc_table[7].priority =  10;

	//对优先队列初始化
	firstLen=firstHead=secondLen=0;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		addToQueue(proc_table+i);
	}
	//指定控制台
	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 1;
	proc_table[4].nr_tty = 1;
	proc_table[5].nr_tty = 1;
	proc_table[6].nr_tty = 4;
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

/*  以上代码在vc6下编译通过，运行正常，能够得到正确的md5值  */
//---------------------------------------------

void ASand()
{
	printf("Caesar's code ! Please input 3 strings :\nyour original message, your gap, and your purpose(encode/decode).\n");
	while(1){
		printf("\nStart !\n");
		char message[255];
		char gap[20];
		char purpose[20];

		char strrrrr[255];

		TTY *p_tty=tty_table+5;
		p_tty->startScanf=0;
		strrrrr[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(message,p_tty->str,255);
		//printf(message);

		p_tty=tty_table+5;
		p_tty->startScanf=0;
		strrrrr[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(gap,p_tty->str,20);
		//printf(gap);

		p_tty=tty_table+5;
		p_tty->startScanf=0;
		strrrrr[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(purpose,p_tty->str,20);
		//printf(purpose);

		int i=0;
		int gapInt=0;
		for(i=0;i<5;i++){
			if(gap[i]>='0'&&gap[i]<='9')
				gapInt=gapInt*10+gap[i]-'0';
			else
				break;
		}
		gapInt=gapInt%26;
		strlwr(message);

		int purp=0;
		if(purpose[0]=='d'||purpose[0]=='D')
			purp=1;
		else
			purp=-1;
		
		for(i=0;i<255;i++){
			if(message[i]=='\0')
				break;
			int tmp=message[i]+purp*gapInt;
			if(tmp>'z'||tmp<'a')
				tmp=tmp-purp*26;
			message[i]=tmp;
		}

		printf("Your Result : ");
		printf(message);
		printf("\n");
	}
}


//md5

/*
* Note: this code is harmless on little-endian machines.
*/
void byteReverse(buf, longs)
    unsigned char *buf; unsigned longs;
{
    uint32 t;
    do {
    t = (uint32) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
        ((unsigned) buf[1] << 8 | buf[0]);
    *(uint32 *) buf = t;
    buf += 4;
    } while (--longs);
}

void MD5Init(ctx)
    struct MD5Context *ctx;
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}

/*
* Update context to reflect the concatenation of another buffer full
* of bytes.
*/
void MD5Update(ctx, buf, len)
    struct MD5Context *ctx; unsigned char *buf; unsigned len;
{
    uint32 t;

    /* Update bitcount */

    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((uint32) len << 3)) < t)
    ctx->bits[1]++;     /* Carry from low to high */
    ctx->bits[1] += len >> 29;

    t = (t >> 3) & 0x3f;    /* Bytes already in shsInfo->data */

    /* Handle any leading odd-sized chunks */

    if (t) {
    unsigned char *p = (unsigned char *) ctx->in + t;

    t = 64 - t;
    if (len < t) {
        memcpy(p, buf, len);
        return;
    }
    memcpy(p, buf, t);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32 *) ctx->in);
    buf += t;
    len -= t;
    }
    /* Process data in 64-byte chunks */

    while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32 *) ctx->in);
    buf += 64;
    len -= 64;
    }

    /* Handle any remaining bytes of data. */

    memcpy(ctx->in, buf, len);
}

/*
* Final wrapup - pad to 64-byte boundary with the bit pattern
* 1 0* (64-bit count of bits processed, MSB-first)
*/
void MD5Final(digest, ctx)
    unsigned char digest[16]; struct MD5Context *ctx;
{
    unsigned count;
    unsigned char *p;

    /* Compute number of bytes mod 64 */
    count = (ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80. This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = 0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8) {
    /* Two lots of padding: Pad the first block to 64 bytes */
    memset(p, 0, count);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32 *) ctx->in);

    /* Now fill the next block with 56 bytes */
    memset(ctx->in, 0, 56);
    } else {
    /* Pad block to 56 bytes */
    memset(p, 0, count - 8);
    }
    byteReverse(ctx->in, 14);

    /* Append length in bits and transform */
    ((uint32 *) ctx->in)[14] = ctx->bits[0];
    ((uint32 *) ctx->in)[15] = ctx->bits[1];

    MD5Transform(ctx->buf, (uint32 *) ctx->in);
    byteReverse((unsigned char *) ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));        /* In case it's sensitive */
}


/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
    ( w += f(x, y, z) + data, w = w<<s | w>>(32-s), w += x )

/*
* The core of the MD5 algorithm, this alters an existing MD5 hash to
* reflect the addition of 16 longwords of new data. MD5Update blocks
* the data and converts bytes into longwords for this routine.
*/
void MD5Transform(buf, in)
    uint32 buf[4]; uint32 in[16];
{
    register uint32 a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

void printHex(const int a){
	switch (a){
		case 0 :
			printf("0");break;
		case 1 :
			printf("1");break;
		case 2 :
			printf("2");break;
		case 3 :
			printf("3");break;
		case 4 :
			printf("4");break;
		case 5 :
			printf("5");break;
		case 6 :
			printf("6");break;
		case 7 :
			printf("7");break;
		case 8 :
			printf("8");break;
		case 9 :
			printf("9");break;
		case 10 :
			printf("A");break;
		case 11 :
			printf("B");break;
		case 12 :
			printf("C");break;
		case 13 :
			printf("D");break;
		case 14 :
			printf("E");break;
		case 15 :
			printf("F");break;
	}
}

void Yume()
{	
	printf("\nThis Program Can Calculate The MD5 Of A String For You !\n");
	while(1){
		printf("Please Input Your String:\n");
		char s[255];
		TTY *p_tty=tty_table+4;
		p_tty->startScanf=0;
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(s,p_tty->str,255);

		struct MD5Context md5c;
		unsigned char ss[16];
		char buf[33]={'\0'};
		char tmp[3]={'\0'};
		int i;
		MD5Init( &md5c );
		MD5Update( &md5c, s, strlen(s) );
		MD5Final( ss, &md5c );

		printf("Done ! The MD5 Of The String Is :\n" );
		for( i=0; i<16; i++ ){
			int first = ss[i]/16;
			int second = ss[i]%16;
			printHex(first);
			printHex(second);
		}
		printf("\n\n");
	}
}