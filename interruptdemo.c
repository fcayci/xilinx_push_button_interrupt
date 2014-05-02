#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xintc.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "xil_exception.h"

/******************************************************
 * Customize these based on your design
 * Can be found in xparameters.h
******************************************************/
#define PUSH_MASK XPAR_PUSH_BUTTONS_4BITS_IP2INTC_IRPT_MASK
#define LEDS_DEVICE_ID XPAR_LEDS_8BITS_DEVICE_ID
#define PUSH_DEVICE_ID XPAR_PUSH_BUTTONS_4BITS_DEVICE_ID
#define INTC_DEVICE_ID XPAR_MICROBLAZE_0_INTC_DEVICE_ID
#define PUSH_VEC_ID XPAR_INTC_0_GPIO_1_VEC_ID

XGpio led, push;

// Interrupt service routine for the push button.
void pushISR(void * CallBackRef) {

	u32 push_check;

	// Setup call back reference for the push buttons
	XGpio *InstancePtr = (XGpio *)CallBackRef;

	// Read the push button status
	push_check = XGpio_DiscreteRead(InstancePtr, 1);
	xil_printf("Button value is: %x\n\r", push_check);
	XGpio_DiscreteWrite(&led, 1, push_check);

	// Clear the interrupt
	(void)XGpio_InterruptClear(InstancePtr, PUSH_MASK);
}

int main()
{
	static XIntc intc;

	xil_printf("Starting Main....\r\n");
	// Initialize LEDs
	XGpio_Initialize(&led, LEDS_DEVICE_ID);
	XGpio_SetDataDirection(&led, 1, 0x0);
	XGpio_DiscreteWrite(&led, 1, 0x80); // Turn on one LED by default

	// Initialize Push Buttons
	XGpio_Initialize(&push, PUSH_DEVICE_ID);
	XGpio_SetDataDirection(&push, 1, 0xFF);

	// Initialize Interrupt Controller
	XIntc_Initialize(&intc, INTC_DEVICE_ID);

	XIntc_Connect(&intc, PUSH_VEC_ID,
			      (Xil_ExceptionHandler)pushISR, &push);

	// Start the interrupt controller
	XIntc_Start(&intc, XIN_REAL_MODE);

	// Enable the interrupt
	XIntc_Enable(&intc, PUSH_VEC_ID);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
		(Xil_ExceptionHandler)XIntc_InterruptHandler, &intc);

	// Enable non-critical exceptions
	Xil_ExceptionEnable();

	// Enable the Interrupt
	XGpio_InterruptEnable(&push, PUSH_MASK);
	XGpio_InterruptGlobalEnable(&push);


	while(1){
		// Do nothing. Let the interrupt handled take care of everything!
	}

	return 0;
}
