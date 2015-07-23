
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


char myGetChar(){
	TTY *mytty=tty_table+2;
	char mytty_cmd[255];
	mytty->startScanf=0;
	openStartScanf(mytty);
	while (mytty->startScanf) ;
	//mystrncpy(mytty_cmd,lalala->str,255);
	return mytty->str[0];
}

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
	init_clock();
	//disp_str("-----\"tinix_main\" begins-----\n");
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                 AAAAAAA                                              *\n",0xe);
	disp_color_str("*              AAAAAAAAAAAAA                                           *\n",0xe);
	disp_color_str("*            AAAAAAAAAAAAAAAAA                                         *\n",0xe);
	disp_color_str("*           AAAAAAAAAAA  AAAAAA                                        *\n",0xe);
	disp_color_str("*          AAAAAAAAAAAAAAAAAAAAA                                       *\n",0xe);
	disp_color_str("*          AAAAAAAAAAAAAAAAAAAAA                                       *\n",0xe);
	disp_color_str("*         AAAAAAAAAAAAAAAAAAAAAAA             OO                       *\n",0xe);
	disp_color_str("*         AAAAAAAAAAAAAAAAAAAAAAA            OOOO                      *\n",0xe);
	disp_color_str("*         AAAAAAAAAAAAAAAAAAAAAAA             OO                       *\n",0xe);
	disp_color_str("*          AAAAAAAAAAAAAAAAAAAAA                                       *\n",0xe);
	disp_color_str("*          AAAAAAAAAAAAAAAAAAAAA                                       *\n",0xe);
	disp_color_str("*           AAAAAAAAAAAAAAAAAAA                                        *\n",0xe);
	disp_color_str("*            AAAAAAAAAAAAAAAAA                                         *\n",0xe);
	disp_color_str("*              AAAAAAAAAAAA                                            *\n",0xe);
	disp_color_str("*                 AAAAAAA                                              *\n",0xe);
	disp_color_str("*                                                                      *\n",0xc);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                                                                      *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);
	milli_delay(2000);
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                      AAAAAAA                                         *\n",0xe);
	disp_color_str("*                   AAAAAAAAAAAAA                                      *\n",0xe);
	disp_color_str("*                 AAAAAAAAAAAAAAAAA                                    *\n",0xe);
	disp_color_str("*                AAAAAAAAAAA  AAAA.A                                   *\n",0xe);
	disp_color_str("*               AAAAAAAAAAAAAAAA.`                                     *\n",0xe);
	disp_color_str("*               AAAAAAAAAAAAAA.`                                       *\n",0xe);
	disp_color_str("*              AAAAAAAAAAAAA.`                OO                       *\n",0xe);
	disp_color_str("*              AAAAAAAAAAA-,                 OOOO                      *\n",0xe);
	disp_color_str("*              AAAAAAAAAAAAA`.                OO                       *\n",0xe);
	disp_color_str("*               AAAAAAAAAAAAAA`.                                       *\n",0xe);
	disp_color_str("*               AAAAAAAAAAAAAAAA`.                                     *\n",0xe);
	disp_color_str("*                AAAAAAAAAAAAAAAAA'A                                   *\n",0xe);
	disp_color_str("*                 AAAAAAAAAAAAAAAAA                                    *\n",0xe);
	disp_color_str("*                   AAAAAAAAAAAAA                                      *\n",0xe);
	disp_color_str("*                      AAAAAAA                                         *\n",0xe);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                            July ,2015                                *\n",0xc);
	disp_color_str("*                                                                      *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);

	milli_delay(2000);
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                           AAAAAAA                                    *\n",0xe);
	disp_color_str("*                        AAAAAAAAAAAAA                                 *\n",0xe);
	disp_color_str("*                      AAAAAAAAAAAAAAAAA                               *\n",0xe);
	disp_color_str("*                     AAAAAAAAAAA  AAAAAA                              *\n",0xe);
	disp_color_str("*                    AAAAAAAAAAAAAAAAAAAAA                             *\n",0xe);
	disp_color_str("*                    AAAAAAAAAAAAAAAAAAAAA                             *\n",0xe);
	disp_color_str("*                   AAAAAAAAAAAAAAAAAAAAAAA   OO                       *\n",0xe);
	disp_color_str("*                   AAAAAAAAAAAAAAAAAAAAAAA  OOOO                      *\n",0xe);
	disp_color_str("*                   AAAAAAAAAAAAAAAAAAAAAAA   OO                       *\n",0xe);
	disp_color_str("*                    AAAAAAAAAAAAAAAAAAAAA                             *\n",0xe);
	disp_color_str("*                    AAAAAAAAAAAAAAAAAAAAA                             *\n",0xe);
	disp_color_str("*                     AAAAAAAAAAAAAAAAAAA                              *\n",0xe);
	disp_color_str("*                      AAAAAAAAAAAAAAAAA                               *\n",0xe);
	disp_color_str("*                        AAAAAAAAAAAAA                                 *\n",0xe);
	disp_color_str("*                           AAAAAAA                                    *\n",0xe);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                            July ,2015                                *\n",0xc);
	disp_color_str("*                By Wang Zeyu | Guo Yuqing | Zhu Mengyao               *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);
	milli_delay(2000);
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                                AAAAAAA                               *\n",0xe);
	disp_color_str("*                             AAAAAAAAAAAAA                            *\n",0xe);
	disp_color_str("*                           AAAAAAAAAAAAAAAAA                          *\n",0xe);
	disp_color_str("*                          AAAAAAAAAAA  AAAA.A                         *\n",0xe);
	disp_color_str("*                         AAAAAAAAAAAAAAAA.`                           *\n",0xe);
	disp_color_str("*                         AAAAAAAAAAAAAA.`                             *\n",0xe);
	disp_color_str("*                        AAAAAAAAAAAAA.`      OO                       *\n",0xe);
	disp_color_str("*                        AAAAAAAAAAAA-,      OOOO                      *\n",0xe);
	disp_color_str("*                        AAAAAAAAAAAAA`.      OO                       *\n",0xe);
	disp_color_str("*                         AAAAAAAAAAAAAA`.                             *\n",0xe);
	disp_color_str("*                         AAAAAAAAAAAAAAAA`.                           *\n",0xe);
	disp_color_str("*                          AAAAAAAAAAAAAAAAA'A                         *\n",0xe);
	disp_color_str("*                           AAAAAAAAAAAAAAAAA                          *\n",0xe);
	disp_color_str("*                             AAAAAAAAAAAAA                            *\n",0xe);
	disp_color_str("*                                AAAAAAA                               *\n",0xe);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                            July ,2015                                *\n",0xc);
	disp_color_str("*                By Wang Zeyu | Guo Yuqing | Zhu Mengyao               *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);
	milli_delay(2000);
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                                     AAAAAAA                          *\n",0xe);
	disp_color_str("*                                  AAAAAAAAAAAAA                       *\n",0xe);
	disp_color_str("*                                AAAAAAAAAAAAAAAAA                     *\n",0xe);
	disp_color_str("*                               AAAAAAAAAAA  AAAAAA                    *\n",0xe);
	disp_color_str("*                              AAAAAAAAAAAAAAAAAAAAA                   *\n",0xe);
	disp_color_str("*                              AAAAAAAAAAAAAAAAAAAAA                   *\n",0xe);
	disp_color_str("*                             AAAAAAAAAAAAAAAAAAAAAAA                  *\n",0xe);
	disp_color_str("*                             AAAAAAAAAAAAAAAAAAAAAAA                  *\n",0xe);
	disp_color_str("*                             AAAAAAAAAAAAAAAAAAAAAAA                  *\n",0xe);
	disp_color_str("*                              AAAAAAAAAAAAAAAAAAAAA                   *\n",0xe);
	disp_color_str("*                              AAAAAAAAAAAAAAAAAAAAA                   *\n",0xe);
	disp_color_str("*                               AAAAAAAAAAAAAAAAAAA                    *\n",0xe);
	disp_color_str("*                                AAAAAAAAAAAAAAAAA                     *\n",0xe);
	disp_color_str("*                                  AAAAAAAAAAAAA                       *\n",0xe);
	disp_color_str("*                                     AAAAAAA                          *\n",0xe);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                            July ,2015                                *\n",0xc);
	disp_color_str("*                By Wang Zeyu | Guo Yuqing | Zhu Mengyao               *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);
	milli_delay(2000);
	clearScreen();
	disp_color_str("************************************************************************\n",0xc);
	disp_color_str("*                                          AAAAAAA                     *\n",0xe);
	disp_color_str("*                                       AAAAAAAAAAAAA                  *\n",0xe);
	disp_color_str("*                                     AAAAAAAAAAAAAAAAA                *\n",0xe);
	disp_color_str("*                                    AAAAAAAAAAA  AAAA.A               *\n",0xe);
	disp_color_str("*                                   AAAAAAAAAAAAAAAA.`                 *\n",0xe);
	disp_color_str("*                                   AAAAAAAAAAAAAA.`                   *\n",0xe);
	disp_color_str("*                                  AAAAAAAAAAAAA.`                     *\n",0xe);
	disp_color_str("*                                  AAAAAAAAAAA-, Hello World!          *\n",0xe);
	disp_color_str("*                                  AAAAAAAAAAAAA`.                     *\n",0xe);
	disp_color_str("*                                   AAAAAAAAAAAAAA`.                   *\n",0xe);
	disp_color_str("*                                   AAAAAAAAAAAAAAAA`.                 *\n",0xe);
	disp_color_str("*                                    AAAAAAAAAAAAAAAAA'A               *\n",0xe);
	disp_color_str("*                                     AAAAAAAAAAAAAAAAA                *\n",0xe);
	disp_color_str("*                                       AAAAAAAAAAAAA                  *\n",0xe);
	disp_color_str("*                                          AAAAAAA                     *\n",0xe);
	disp_color_str("*                          The OS Project                              *\n",0xc);
	disp_color_str("*                            July ,2015                                *\n",0xc);
	disp_color_str("*                By Wang Zeyu | Guo Yuqing | Zhu Mengyao               *\n",0xc);
	disp_color_str("************************************************************************\n",0xc);

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

	//对优先队列初始化
	firstLen=firstHead=secondLen=0;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		addToQueue(proc_table+i);
	}
	//指定控制台
	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 2;
	proc_table[4].nr_tty = 3;
	proc_table[5].nr_tty = 4;
	//proc_table[6].nr_tty = 4;
	//proc_table[7].nr_tty = 5;

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
	printf("           */////////////////////////////////////////////*/\n");
	printf("               Designed By Gretchen,Sylvia and Shepard    \n");
	printf("           */////////////////////////////////////////////*/\n");
	printf("\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("      *////  help         --------  shwo the help menu     ////*\n");
	printf("      *////  clear        --------  clear screen           ////*\n");
	printf("      *////  F1           --------  go back to terminal    ////*\n");
	printf("      *////  F2           --------  days matter            ////*\n");
	printf("      *////  F3           --------  2048 on linux          ////*\n");
	printf("      *////  F4           --------  Caesar code            ////*\n");
	printf("      *////  F5           --------  MD5 calculate          ////*\n");
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
                               2048
 *======================================================================*/
extern x;
int x = 1;

void TestC()
{
	while(1){
        char ch;
        int p[5][5] = { 0 };
        int m[5][5] = { 0 };
        int a[5][5] = { 0 };
        int b, c, d, e;


        b = getNextNumber() % 5;
        for (; b == 0;)
            b = getNextNumber() % 5;

        c = getNextNumber() % 5;
        for (; c == 0;)
            c = getNextNumber() % 5;

        d = getNextNumber() % 5;
        for (; d == 0;)
            d = getNextNumber() % 5;

        e = getNextNumber() % 5;
        for (; e == 0;)
            e = getNextNumber() % 5;
        a[b][c] = 2;
        if ((b == d) && (c == e))
        {
            d = 5 - d;
            c = 5 - c;
        }
        a[d][e] = 2;
        display(a);
        f3(a, m, p);
        ch = myGetChar();
	}
}

/*char myGetChar(){
	TTY *p_tty=tty_table+2;
	p_tty->startScanf=0;
	char inputs[255];
	openStartScanf(p_tty);
	while (p_tty->startScanf) ;
	mystrncpy(inputs,p_tty->str,255);
	char tmp=inputs[0];
	return (tmp==0? ' ':tmp);
}

char myGetChar(){
	TTY *mytty=tty_table+3;
	char mytty_cmd[255];
	openStartScanf(mytty);
	while (mytty->startScanf) ;
	//mystrncpy(mytty_cmd,lalala->str,255);
	return mytty->str[0];
}
*/

int PC(int ze[5][5])
{
	ze[4][4] = 2048;
	return 0;
}
int f2048(int ze[5][5])
{
	int k; int i; int j;
	k = 0;
	for (i = 1; i < 5; i++)
		for (j = 1; j < 5; j++)
		{
			if (ze[i][j] == 2048)
			{
				k = 1;
				return k;
			}
		}
	return k;

}
int f(int n)
{
	int a, i, c;
	a = 0;
	c = n;
	for (i = 0; i < 5; i++)
	{
		if (n > 0)
		{
			a = a + 1;
		}
		n = n / 10;
	}
	for (i = 0; i < 4 - a; i++)
	{
		printf(" ");
	}
	if (c > 0)
	{
		printf("%d", c);
	}
	return 0;
}


int display(int z[5][5])
{
	sys_clear(tty_table+2);
	printf("Welcome To 2048\nInput a to move left,s to down,d to right,w to up.\nTwice input r to restart\n");
	printf(" ---- ---- ---- ----\n");
	printf("|    |    |    |    |\n");
	printf("|"); f(z[1][1]);
	printf("|"); f(z[1][2]);
	printf("|"); f(z[1][3]);
	printf("|"); f(z[1][4]); printf("|\n"); 
	printf(" ---- ---- ---- ----\n");
	printf("|    |    |    |    |\n");
	printf("|"); f(z[2][1]); printf("|"); f(z[2][2]); printf("|"); f(z[2][3]); printf("|"); f(z[2][4]); printf("|\n"); 
	printf(" ---- ---- ---- ----\n");
	printf("|    |    |    |    |\n");
	printf("|"); f(z[3][1]); printf("|"); f(z[3][2]); printf("|"); f(z[3][3]); printf("|"); f(z[3][4]); printf("|\n"); 
	printf(" ---- ---- ---- ----\n");
	printf("|    |    |    |    |\n");
	printf("|"); f(z[4][1]); printf("|"); f(z[4][2]); printf("|"); f(z[4][3]); printf("|"); f(z[4][4]); printf("|\n"); 
	printf(" ---- ---- ---- ----\n");
	return 0;
}

int check1(int td[5][5])
{
	int n;
	for (n = 1; n < 5; n++)
	{
		if ((td[1][n] + td[2][n] + td[3][n] + td[4][n]) == 0);
		else{
			for (; td[1][n] == 0;)
			{
				td[1][n] = td[2][n];
				td[2][n] = td[3][n];
				td[3][n] = td[4][n];
				td[4][n] = 0;
			}
			if ((td[2][n] + td[3][n] + td[4][n]) == 0);
			else{
				for (; td[2][n] == 0;)
				{
					td[2][n] = td[3][n];
					td[3][n] = td[4][n];
					td[4][n] = 0;
				}
			}
			if ((td[3][n] + td[4][n]) == 0);
			else{
				for (; td[3][n] == 0;)
				{
					td[3][n] = td[4][n];
					td[4][n] = 0;
				}
			}
		}
	}
	return 0;
}

int check2(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if ((td[1][n] + td[2][n] + td[3][n] + td[4][n]) == 0);
		else{
			for (; td[4][n] == 0;)
			{
				td[4][n] = td[3][n];
				td[3][n] = td[2][n];
				td[2][n] = td[1][n];
				td[1][n] = 0;
			}
			for (; td[3][n] == 0;)
			{
				td[3][n] = td[2][n];
				td[2][n] = td[1][n];
				td[1][n] = 0;
				if ((td[2][n] + td[3][n] + td[1][n]) == 0)
					break;
			}
			for (; td[2][n] == 0;)
			{
				td[2][n] = td[1][n];
				td[1][n] = 0;
				if ((td[2][n] + td[1][n]) == 0)
					break;
			}
		}
	}
	return 0;
}
int check3(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if ((td[n][1] + td[n][2] + td[n][3] + td[n][4]) == 0);
		else{
			for (; td[n][1] == 0;)
			{
				td[n][1] = td[n][2];
				td[n][2] = td[n][3];
				td[n][3] = td[n][4];
				td[n][4] = 0;
			}
			for (; td[n][2] == 0;)
			{
				td[n][2] = td[n][3];
				td[n][3] = td[n][4];
				td[n][4] = 0;
				if ((td[n][2] + td[n][3] + td[n][4]) == 0)
					break;
			}
			for (; td[n][3] == 0;)
			{
				td[n][3] = td[n][4];
				td[n][4] = 0;
				if ((td[n][3] + td[n][4]) == 0)
					break;
			}
		}
	}
	return 0;
}

int check4(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if ((td[n][1] + td[n][2] + td[n][3] + td[n][4]) == 0);
		else
		{
			for (; td[n][4] == 0;)
			{
				td[n][4] = td[n][3];
				td[n][3] = td[n][2];
				td[n][2] = td[n][1];
				td[n][1] = 0;
			}
			for (; td[n][3] == 0;)
			{
				td[n][3] = td[n][2];
				td[n][2] = td[n][1];
				td[n][1] = 0;
				if ((td[n][3] + td[n][2] + td[n][1]) == 0)
					break;
			}
			for (; td[n][2] == 0;)
			{
				td[n][2] = td[n][1];
				td[n][1] = 0;
				if ((td[n][2] + td[n][1]) == 0)
					break;
			}
		}
	}
	return 0;
}

int up(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if (td[1][n] == td[2][n])
		{
			td[1][n] = 2 * td[1][n];
			td[2][n] = 0;
		}
		if (td[2][n] == td[3][n])
		{
			td[2][n] = 2 * td[2][n];
			td[3][n] = 0;
		}
		if (td[3][n] == td[4][n])
		{
			td[3][n] = 2 * td[3][n];
			td[4][n] = 0;
		}
		check1(td);
	}
	return 0;
}

int down(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if (td[3][n] == td[4][n])
		{
			td[4][n] = 2 * td[4][n];
			td[3][n] = 0;
		}
		if (td[2][n] == td[3][n])
		{
			td[3][n] = 2 * td[3][n];
			td[2][n] = 0;
		}
		if (td[1][n] == td[2][n])
		{
			td[2][n] = 2 * td[2][n];
			td[1][n] = 0;
		}
		check2(td);
	}
	return 0;
}

int left(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if (td[n][1] == td[n][2])
		{
			td[n][1] = 2 * td[n][1];
			td[n][2] = 0;
		}
		if (td[n][2] == td[n][3])
		{
			td[n][2] = 2 * td[n][2];
			td[n][3] = 0;
		}
		if (td[n][3] == td[n][4])
		{
			td[n][3] = 2 * td[n][3];
			td[n][4] = 0;
		}
		check3(td);
	}
	return 0;
}

int right(int td[5][5]){
	int n;
	for (n = 1; n < 5; n++)
	{
		if (td[n][4] == td[n][3])
		{
			td[n][4] = 2 * td[n][4];
			td[n][3] = 0;
		}
		if (td[n][3] == td[n][2])
		{
			td[n][3] = 2 * td[n][3];
			td[n][2] = 0;
		}
		if (td[n][2] == td[n][1])
		{
			td[n][2] = 2 * td[n][2];
			td[n][1] = 0;
		}
		check4(td);
	}
	return 0;
}

int f378(int o[5][5])
{
	int a, b, i;
	i = 0; x++;

	a = getNextNumber() % 4 + 1;

	b = getNextNumber() % 4 + 1;
	for (; o[a][b] != 0;)
	{
		x++;

		a = getNextNumber() % 4 + 1;

		b = getNextNumber() % 4 + 1;
	}
	o[a][b] = 2;
	return 0;
}

int P1(int o[5][5]){
	check1(o);
	up(o);
	return 0;
}

int P2(int o[5][5])
{
	check2(o);
	down(o);
	return 0;
}

int P3(int o[5][5]){
	check3(o);
	left(o);
	return 0;
}

int P4(int o[5][5]){
	check4(o);
	right(o);
	return 0;
}

int fake(int f[5][5], int q[5][5]){
	int i; int j;
	for (i = 1; i < 5; i++)
		for (j = 1; j < 5; j++)
		{
			f[i][j] = q[i][j];
		}

	return 0;
}

int getNextNumber(){
    static int index = 10;
    index=index*1.354+2;
    if(index<0)
        index=0;
    return get_ticks()%128;
}

int f3(int q[5][5], int f[5][5], int s[5][5])
{
	char press; 
	int k; int t; int i; int j; int u;
	t = 1;
	for (; k = 1;)
	{
		press = myGetChar();
		fake(f, q);
		switch (press)
		{
		case 'w': P1(q); break;
		case 's': P2(q); break;
		case 'a': P3(q); break;
		case 'd': P4(q); break;
		case 'W': P1(q); break;
		case 'S': P2(q); break;
		case 'A': P3(q); break;
		case 'D': P4(q); break;
		case 'C': PC(q); break;
		case 'r': return 0;
		case 'R': return 0;
		default : break;
		}
		u = 0;
		for (i = 1; i < 5; i++)
			for (j = 1; j < 5; j++)
			{
				if (q[i][j] != f[i][j])
				{
					u = 1; break;
				}
			}
		if (!u)
		{
		}
		else
		{
			f378(q);
		}
		display(f);
		display(q);
		if (f2048(q))
		{
			printf("You Win!");
			break;
		}
	}
	return 0;
}

/*======================================================================*
                               DE
 *======================================================================*/

void TestD()
{
	int i=0;

	while (1)
	{
		//printf("%d",get_ticks()%4);
		milli_delay(1000);
	}
}

void TestE()
{
	int i=0;

	while (1)
	{
		//printf("E");
		milli_delay(1000);
	}
}

/*  以上代码在vc6下编译通过，运行正常，能够得到正确的md5值  */
//---------------------------------------------

void ASand()
{
	sys_clear(tty_table+3);
	printf("Caesar's code ! Please input 3 strings :\nyour original message, your gap, and your purpose(encode/decode).\n");
	while(1){
		printf("\nStart !\n");
		char message[255];
		char gap[20];
		char purpose[20];

		char strrrrr[255];

		TTY *p_tty=tty_table+3;
		p_tty->startScanf=0;
		strrrrr[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(message,p_tty->str,255);
		//printf(message);

		p_tty=tty_table+3;
		p_tty->startScanf=0;
		strrrrr[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(gap,p_tty->str,20);
		//printf(gap);

		p_tty=tty_table+3;
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
	sys_clear(tty_table+4);
	printf("This Program Can Calculate The MD5 Of A String For You !\n");
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

int year,month,day,year1,month1,day1,result,n,i,x,y;
int table[]={29,31,28,31,30,31,30,31,31,30,31,30,31};     //定义每个月的天数
int table1[] = {28,31,29,31,30,31,30,31,31,30,31,30,31};
int mem =0;
	
void initDates(){
	year=0;
	month=0;
	day=0;
	year1=0;
	month1=0;
	day1=0;
	result=0;
	n=0;
	i=0;
	x=0;
	y=0;
}

void TestB()
{
	sys_clear(tty_table+1);
	printf("Please input the beginning and the ending date:\n  like: yyyy mm dd yyyy mm dd\n");
	while(1){
		initDates();
		TTY *p_tty=tty_table+1;
		p_tty->startScanf=0;
		char dates[255];
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		mystrncpy(dates,p_tty->str,255);
	// printf(dates);
	//	printf("at least in length:%d",strlen(dates));

		int index=0;
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				year=year*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				month=month*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				day=day*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				year1=year1*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				month1=month1*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}
		for(;index<strlen(dates);index++){
		//printf("this is %d",index);
			if(dates[index]<='9'&&dates[index]>='0'){
				day1=day1*10+dates[index]-'0';
			}
			else{
				index++;
			//printf("kneel at %d\n",index);
				break;
			}
		}

		//printf("%d%d%d%d%d%d\n", year,month,day,year1,month1,day1);

		if(year*100+month*10+day>year1*100+month1*10+day1||year<1||year1<1||
			month<1||month>12||month1<1||month1>12||
			day<1||day1<1||
			day>table1[month]||day1>table1[month1]){
			printf("Date Invalid !\n"); 
			continue;
		}
		else if(year%4!=0 && month==2 && day>28)   {
			printf("February Error !\n"); 
			continue;
		}

		else if(year1%4!=0 && month1==2 && day1>28)   {
			printf("February Error !\n"); 
			continue;
		}
		else
		{
			x = month;
			y = year;
			if(year==year1)   //while they are in the same year!
			{
				if(month==month1)   // and the same month!
					result = day1-day;  //   so....
				else    //the same year but different  monthes.
				{
					if(year%4==0 && month==2)  
						result = table[0]-day;  
					else
						result = table[month]-day;    //英语不够用啦，先将起始年的起始月剩余天数算出。
					for(month++;month<month1;month++)
					{
						if(year%4==0 && month==2)
							result = result + table[0];  
						else
							result = result+table[month];     //将整月的天数加起来！
					}
					result = result+day1;     //最后一个月包含的天数加起来@！
				}
			}
			else          //when they are in different years.   similar to上面的。
			{
				if(year%4==0 && month==2)
					result = table[0]-day;
				else
					result = table[month]-day;
				for(month++;month<=12;month++)
					if(year%4==0 && month==2)
						result = result + table[0];
					else
						result = result + table[month];
					mem =year;
					for(year++;year<year1;year++)
					{	
						if(year%4==0)
							result = result + 366;
						else
							result = result + 365;
					}
					for(i=1;i<month1;i++)
						result = result + table[i];
					result = result + day1;
				}
			printf("%d.%d.%d to %d.%d.%d \nstill have %ddays \n\n",mem,x,day,year1,month1,day1,result);     //输出结果
		}
	}
}