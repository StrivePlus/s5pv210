#define rVIC0IRQSTATUS 	0xF2000000
#define rVIC0ADDRESS	0xF2000F00

.text
.extern main
.extern uart_init
.global _start
_start:
    stmfd sp!, {lr}
	
	@使能IRQ中断
	mrs r1,cpsr		//r1 = cpsr
	bic r1,#0x80	//r1&=~0x80
	msr cpsr,r1		//cpsr = r1
	
	
	@为0xD0037418地址处添加IRQ中断处理函数
	ldr r0,=0xD0037418
	ldr r1,=HandlerIRQ
	str r1,[r0]
	
	
	@uart_init();
	bl uart_init
	
	@main;
	bl main
	
    ldmfd sp!, {pc}

HandlerIRQ:

	@lr = lr -4,获取当前执行的地址
	sub lr,lr,#4
	
	@保护寄存器，压栈
	stmfd sp!,{r0-r12,lr}
	
	@设置中断服务函数返回后的地址
	ldr lr,=return_irq;
	
	
	@判断中断源
	ldr r0,=rVIC0IRQSTATUS
	ldr r1,[r0]
	cmp r1,#0
	bne vic0_handler
		


		
return_irq:
	ldmfd sp!,{r0-r12,pc}^
	
	
vic0_handler:
	ldr r0,=rVIC0ADDRESS
	ldr pc,[r0]



.end
