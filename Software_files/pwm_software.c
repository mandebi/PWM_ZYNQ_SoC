/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * pwm_software.c: pwm software controller
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <xparameters.h>
#include <xgpio.h>
#include <my_pwm.h>


int main()
{
	u32 duty_cycle, pwm_data;
	XGpio leds;
	int counter, turning_off;

    init_platform();

    print("Pulse Width Modulation SoC by Joel Mandebi\n");

    duty_cycle = 100;

    //initialize the LED device
    XGpio_Initialize(&leds, XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&leds, 1, 0x0); // Mask=0 -> output channel

    //we read and print print the duty cycle previously stored in the PWM register
    pwm_data = MY_PWM_mReadReg(XPAR_MY_PWM_0_S00_AXI_BASEADDR, MY_PWM_S00_AXI_SLV_REG0_OFFSET);
    xil_printf("\nPWM Data Reg0 = %d", pwm_data);

    counter = 0;
    turning_off = 1;
    while(1){
    	//Write the new duty cycle to the PWM module
    	    MY_PWM_mWriteReg(XPAR_MY_PWM_0_S00_AXI_BASEADDR, MY_PWM_S00_AXI_SLV_REG0_OFFSET, duty_cycle);

    	//we read PWM data after configuration
    	pwm_data = MY_PWM_mReadReg(XPAR_MY_PWM_0_S00_AXI_BASEADDR, MY_PWM_S00_AXI_SLV_REG1_OFFSET);

    	if(pwm_data == 1)//we turn on/off/dim all four leds
    	  XGpio_DiscreteWrite(&leds, 1, 0xF);
    	else
    		XGpio_DiscreteWrite(&leds, 1, 0);

    	if(counter == (1000000)){//we add some delay before turning off/on leds
    	  if(duty_cycle>0 && turning_off==1){
    		  duty_cycle = duty_cycle / 2;
    	  }
    	  else{
    		  if(turning_off==1)
    			  duty_cycle = 1;

    		  turning_off = 0;
    		  if(duty_cycle<100)
    		   duty_cycle = duty_cycle * 2;
    		  else
    			  turning_off = 1;
    	  }
    		counter = 0;
    	}

    	counter ++;
    }

    cleanup_platform();
    return 0;
}
