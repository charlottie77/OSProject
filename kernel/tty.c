
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               tty.c
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
#include "keyboard.h"
#include "proto.h"


#define TTY_FIRST	(tty_table)
#define TTY_END		(tty_table + NR_CONSOLES)


/* 本文件内函数声明 */
PRIVATE void	init_tty(TTY* p_tty);
PRIVATE void	tty_do_read(TTY* p_tty);
PRIVATE void	tty_do_write(TTY* p_tty);
PRIVATE void	put_key(TTY* p_tty, t_32 key);

PRIVATE void 	clearTempStr(TTY* p_tty);
PRIVATE void	putToTemp(TTY* p_tty,t_32 key);
PRIVATE void 	putToStr(TTY* p_tty);


/*======================================================================*
                           task_tty
 *======================================================================*/
PUBLIC void task_tty()
{
	TTY*	p_tty;

	init_keyboard();

	for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
		init_tty(p_tty);
	}

	select_console(0);

	while (1) {
		for (p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++) {
			tty_do_read(p_tty);
			tty_do_write(p_tty);
		}
	}
}


/*======================================================================*
                           init_tty
 *======================================================================*/
PRIVATE void init_tty(TTY* p_tty)
{
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}


/*======================================================================*
                           in_process
 *======================================================================*/
PUBLIC void in_process(TTY* p_tty, t_32 key)
{
	if (!(key & FLAG_EXT)) {
		if (p_tty->startScanf==0) return ;
		put_key(p_tty, key);
		putToTemp(p_tty,key);
	}
	else {
		int raw_code = key & MASK_RAW;
		switch(raw_code) {
		case ENTER:
			put_key(p_tty, '\n');
			putToStr(p_tty);
			break;
		case BACKSPACE:
			if (p_tty->tmpLen)
			{
				put_key(p_tty, '\b');
				p_tty->tmpLen--;
			}
			break;
		case UP:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {	/* Shift + Up */
				scroll_screen(p_tty->p_console, SCROLL_SCREEN_UP);
			}
			break;
		case DOWN:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {	/* Shift + Down */
				scroll_screen(p_tty->p_console, SCROLL_SCREEN_DOWN);
			}
			break;
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:
			if (key) {	/* Alt + F1~F12 */
				select_console(raw_code - F1);
			}
			break;
		default:
			break;
		}
	}
}

PRIVATE void clearTemp(TTY* p_tty)
{
	p_tty->tmpLen=0;
}


PRIVATE void putToTemp(TTY* p_tty,t_32 key)
{
	p_tty->tmpStr[p_tty->tmpLen]=key;
	p_tty->tmpLen++;
}

PRIVATE void putToStr(TTY *p_tty)
{
	int i;
	for (i=0; i<p_tty->tmpLen; i++)
	{
		p_tty->str[i]=p_tty->tmpStr[i];
	}
	p_tty->str[i]='\0';
	p_tty->len=p_tty->tmpLen;
	closeStartScanf(p_tty);
}
/*======================================================================*
                              put_key
*======================================================================*/
PRIVATE void put_key(TTY* p_tty, t_32 key)
{
	if (p_tty->inbuf_count < TTY_IN_BYTES) {
		*(p_tty->p_inbuf_head) = key;
		p_tty->p_inbuf_head++;
		if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_head = p_tty->in_buf;
		}
		p_tty->inbuf_count++;
	}
}


/*======================================================================*
                              tty_do_read
*======================================================================*/
PRIVATE void tty_do_read(TTY* p_tty)
{
	if (is_current_console(p_tty->p_console)) {
		keyboard_read(p_tty);
	}
}




/*======================================================================*
                              tty_do_write
*======================================================================*/
PRIVATE void tty_do_write(TTY* p_tty)
{
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;

		//disp_int(ch);
		out_char(p_tty->p_console, ch);
	}
}


/*======================================================================*
                              tty_write
*======================================================================*/
PUBLIC void tty_write(TTY* p_tty, char* buf, int len)
{
	char* p = buf;
	int i = len;

	while (i) {
		out_char(p_tty->p_console, *p++);
		i--;
	}
}


/*======================================================================*
                              sys_write
*======================================================================*/
PUBLIC int sys_write(char* buf, int len, PROCESS* p_proc)
{
	tty_write(&tty_table[p_proc->nr_tty], buf, len);
	return 0;
}


PUBLIC void openStartScanf(TTY* p_tty)
{
	p_tty->startScanf=1;
	clearTemp(p_tty);
}

PUBLIC void closeStartScanf(TTY* p_tty)
{
	p_tty->startScanf=0;
	clearTemp(p_tty);
}
