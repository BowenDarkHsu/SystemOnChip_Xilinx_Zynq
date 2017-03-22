/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "xil_io.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "sleep.h"
#include "led_interrupt_ip.h"

XScuGic InterruptController; /* Instance of the Interrupt Controller */
static XScuGic_Config *GicConfig;/* The configuration parameters of the controller */


// Interrupt Handler

void INTR_Handler_91(void *baseaddr_p){
	print("INTR_Handler_91 !!!!!!!!!!!! \r\n");
	LED_INTERRUPT_IP_mWriteReg(XPAR_LED_INTERRUPT_IP_0_BASEADDR,12,0x0000AC55);
	//Rx_data = MINI_UART_mReadReg(XPAR_MINI_UART_0_BASEADDR,4);
	// xil_printf("RxD = %c \n\r",Rx_data);
}


int ScuGicInterrupt_Init(void)
{
	int Status;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 * */
	Xil_ExceptionInit();

	GicConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&InterruptController, GicConfig,
			GicConfig->CpuBaseAddress);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the Interrupt System
	 * */

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			(void *) &InterruptController);

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler performs
	 * the specific interrupt processing for the device
	 * since the xparameters.h file doesnt detect the external interrupts, we have to manually
	 * use the IRQ_F2P port numbers ; 91, 90, ect..
	 */

	Status = XScuGic_Connect(&InterruptController,91,
			(Xil_ExceptionHandler)INTR_Handler_91,
			(void *)&InterruptController);

	XScuGic_Enable(&InterruptController, 91);
	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();
	/*
	 * Set interrupts PriorityTriggerType
	 */
	XScuGic_SetPriorityTriggerType(&InterruptController, 91,
						0xa0, 3);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

}


int main(void)
{
	int xstatus;

    // Interrupt Test

	xstatus = ScuGicInterrupt_Init();

	if (xstatus != XST_SUCCESS)
		return XST_FAILURE;

	// Wait For interrupt;

	print("Wait for the interrupt to trigger \r\n");
	print("########################################\r\n");
	print(" \r\n");
    int i;
/*
    Tx_buffer = 0x30;
	while(1){
		LED_INTERRUPT_IP_mWriteReg(XPAR_LED_INTERRUPT_IP_0_BASEADDR,0,0x00000012);
		 if(++Tx_buffer == 0x3a)
			 Tx_buffer = 0x30;
		 sleep(1);
	}*/

	LED_INTERRUPT_IP_mWriteReg(XPAR_LED_INTERRUPT_IP_0_BASEADDR,0,0x00000019);
	LED_INTERRUPT_IP_mWriteReg(XPAR_LED_INTERRUPT_IP_0_BASEADDR,4,0x0000002F);

	while(1){
	    xil_printf("counter = %x \n\r",LED_INTERRUPT_IP_mReadReg(XPAR_LED_INTERRUPT_IP_0_BASEADDR,8) );
	    	//xil_printf("counter = 111 \n\r" );
	}
	return 0;
}


