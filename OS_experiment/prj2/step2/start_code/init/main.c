/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "irq.h"
#include "test.h"
#include "stdio.h"
#include "sched.h"
#include "screen.h"
#include "common.h"
#include "syscall.h"

#include "lock.h"	// part2

extern pid_t process_id;
extern pcb_t pcb[NUM_MAX_TASK];//task array
//extern unsigned int pcb_array_length;

extern queue_t ready_queue;
//extern queue_t block_queue;
extern mutex_lock_t block_queue_array[NUM_MUTEX_LOCK];

extern int num_lock_tasks;
extern mutex_lock_t mutex_lock;
static void init_pcb()
{
	int i, j;
	for(i = 0; i < NUM_MAX_TASK; i++)
	{
		pcb[i].status = TASK_EXITED;
		pcb[i].first_running = 1;
		for( j = 0; j < 32; j++)
		{
			pcb[i].kernel_context.regs[j] = 0;
			pcb[i].user_context.regs[j] = 0;
		}
		pcb[i].kernel_context.cp0_status = 0;
		pcb[i].user_context.cp0_status = 0;
		pcb[i].kernel_context.hi = 0;
		pcb[i].user_context.hi = 0;
		pcb[i].kernel_context.lo = 0;
		pcb[i].user_context.lo = 0;
		pcb[i].kernel_context.cp0_badvaddr = 0;
		pcb[i].user_context.cp0_badvaddr = 0;
		pcb[i].kernel_context.cp0_cause = 0;
		pcb[i].user_context.cp0_cause = 0;
		pcb[i].kernel_context.cp0_epc = 0;
		pcb[i].user_context.cp0_epc = 0;
		pcb[i].kernel_context.pc = 0;
		pcb[i].user_context.pc = 0;

		pcb[i].kernel_stack_top = 0;
		pcb[i].user_stack_top = 0;

		pcb[i].prev = NULL;
		pcb[i].next = NULL;

		pcb[i].pid = 0;

		pcb[i].type = KERNEL_PROCESS;

		pcb[i].cursor_x = 0;
		pcb[i].cursor_y = 0;
		
		//for (int k = 0; k < NUM_MUTEX_LOCK; k++)
		//	pcb[i].lock_address[k] = NULL;
		//pcb[i].lock_address = NULL;

	}

	for(i = 1; i < num_lock_tasks + 1 ; i++)
	{//this is used to load 2 tasks on pcb
		pcb[i].kernel_context.regs[29] = STACK_TOP - i * STACK_SIZE;	//sp
		pcb[i].kernel_context.regs[31] = lock_tasks[i - 1]->entry_point;			//ra

		pcb[i].kernel_stack_top = STACK_TOP - i * STACK_SIZE;

		pcb[i].type = lock_tasks[i - 1]->type;
		pcb[i].status = TASK_READY;

		//assume that kernel shares the ccontext with user since there is no user in this task
		pcb[i].user_context.regs[29] = pcb[i].kernel_context.regs[29];
		pcb[i].user_context.regs[31] = pcb[i].kernel_context.regs[31];
		
		pcb[i].user_stack_top = pcb[i].kernel_stack_top;


		queue_push(&ready_queue, &pcb[i]);
	}

	for(;i <num_lock_tasks + num_sched1_tasks + 1 ; i++)
	{//this is used to load 3 tasks on pcb
		pcb[i].kernel_context.regs[29] = STACK_TOP - i * STACK_SIZE;	//sp
		pcb[i].kernel_context.regs[31] = sched1_tasks[i - num_lock_tasks - 1]->entry_point;			//ra

		pcb[i].kernel_stack_top = STACK_TOP - i * STACK_SIZE;

		pcb[i].type = sched1_tasks[i - num_lock_tasks - 1]->type;
		pcb[i].status = TASK_READY;

		//assume that kernel shares the ccontext with user since there is no user in this task
		pcb[i].user_context.regs[29] = pcb[i].kernel_context.regs[29];
		pcb[i].user_context.regs[31] = pcb[i].kernel_context.regs[31];
		
		pcb[i].user_stack_top = pcb[i].kernel_stack_top;


		queue_push(&ready_queue, &pcb[i]);
	}

	// let pcb[0] become the first process
	
	// init_pcb() works accorrding to the definition style of pcb structure 
	/* register context */
	pcb[0].kernel_context.regs[29] = STACK_TOP;	// sp <= stack top
	pcb[0].kernel_stack_top = STACK_TOP;

	// accorrding to lecture2.pdf, user shares the same address with kernel
	pcb[0].user_context.regs[29] = pcb[0].kernel_context.regs[29];
	pcb[0].user_stack_top = pcb[0].kernel_stack_top;

    /* previous, next pointer */
	pcb[0].prev = NULL;
	pcb[0].next = NULL;
    /* process id */
	pcb[0].pid = 0;
    /* kernel/user thread/process */
    pcb[0].type = KERNEL_PROCESS;
	/* BLOCK | READY | RUNNING */
	pcb[0].status = TASK_RUNNING;
    /* cursor position */ 





	pcb[0].first_running = 0;

	current_running = &pcb[0];
	
	return;
}


static void init_queue()
{
	queue_init(&ready_queue); // part1
	// queue_init(&block_queue);
	//int i;
	//for (i = 0; i < NUM_MUTEX_LOCK; i++ )	// part2
	//{
	//	do_mutex_lock_init(block_queue_array[i]);
	//}
	do_mutex_lock_init(&mutex_lock);
	return;
}


static void init_exception_handler()
{
}

static void init_exception()
{
	// 1. Get CP0_STATUS
	// 2. Disable all interrupt
	// 3. Copy the level 2 exception handling code to 0x80000180
	// 4. reset CP0_COMPARE & CP0_COUNT register
}

static void init_syscall(void)
{
	// init system call table.
}

// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
void __attribute__((section(".entry_function"))) _start(void)
{
	// Close the cache, no longer refresh the cache 
	// when making the exception vector entry copy
	asm_start();

	// init interrupt (^_^)
	init_exception();
	printk("> [INIT] Interrupt processing initialization succeeded.\n");

	// init system call table (0_0)
	init_syscall();
	printk("> [INIT] System call initialized successfully.\n");

	// init Process Control Block (-_-!)
	init_queue();
	init_pcb();
	printk("> [INIT] PCB initialization succeeded.\n");

	// init screen (QAQ)
	init_screen();
	printk("> [INIT] SCREEN initialization succeeded.\n");

	// TODO Enable interrupt
	
	while (1)
	{
		// (QAQQQQQQQQQQQ)
		// If you do non-preemptive scheduling, you need to use it to surrender control
		do_scheduler();
	};
	return;
}
