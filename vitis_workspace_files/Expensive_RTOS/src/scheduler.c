#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "scheduler.h"
#include <stdio.h>

#define TIMERBase XPAR_RTSTIMER_0_S00_AXI_BASEADDR
static XScuGic IntcInstance;
int *timer;

void timerISR();



int initScheduler(scheduler *scheduler,void *callback(void *),void *callBackParameter){
	int Status;
	static XScuGic_Config *GicConfig;
	GicConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	Status = XScuGic_CfgInitialize(&IntcInstance, GicConfig, GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		print("Interrupt controller init failed\n\r");
		return XST_FAILURE;
	}
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,&IntcInstance);
	Xil_ExceptionEnable();
	Status = XScuGic_Connect(&IntcInstance, XPAR_FABRIC_RTSTIMER_0_O_INTR_INTR,(Xil_ExceptionHandler)timerISR,(void *)scheduler);
	if (Status != XST_SUCCESS) {
		print("GIC connect failed\n\r");
		return XST_FAILURE;
	}
	XScuGic_Enable(&IntcInstance, XPAR_FABRIC_RTSTIMER_0_O_INTR_INTR);
	scheduler->callback = (void *)callback;
	scheduler->callbackParameter = (void *)callBackParameter;
	return 0;
}

void timerISR(void * schduler){
	//print("Interrupt received\n\r");
	timer = (int *)TIMERBase;
	*timer = 0; //Clear interrupt
	scheduler *s = (scheduler *)schduler;
    if (s->callback != NULL) {
        s->callback(s->callbackParameter);  // call the callback
    }
}

void loadTimer(u32 timerVal){
	timer = (int *)TIMERBase;
	*((int *)timer+1) = timerVal*100;
	*timer = 1;
}
