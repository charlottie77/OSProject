
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* EXTERN is defined as extern except in global.c */
#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

EXTERN	int		ticks;

EXTERN	int		disp_pos;
EXTERN	t_8		gdt_ptr[5];	// 0~15:Limit  16~47:Base
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	t_8		idt_ptr[5];	// 0~15:Limit  16~47:Base
EXTERN	GATE		idt[IDT_SIZE];

EXTERN	t_32		k_reenter;

EXTERN	TSS		tss;
EXTERN	PROCESS*	p_proc_ready;
EXTERN 	PROCESS*	firstQueue[20];
EXTERN 	PROCESS*	secondQueue[20];
EXTERN 	int		firstLen;
EXTERN 	int 		firstHead;
EXTERN 	int 		secondLen;
EXTERN	int		nr_current_console;

extern	PROCESS		proc_table[];
extern	char		task_stack[];
extern	TASK		task_table[];
extern	TASK		user_proc_table[];
extern	TTY		tty_table[];
extern	CONSOLE		console_table[];


extern	t_pf_irq_handler	irq_table[];

extern	t_sys_call		sys_call_table[];


