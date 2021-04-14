`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/13/2021 09:59:45 PM
// Design Name: 
// Module Name: customPWM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module customPWM(
    input clk,
    input rst_n,
    input duty_cycle_in,
    input [6:0] duty_cycle, //we take in duty cycles between 0-100% (0% <-> pwm_out = 1 0% of the time; 100% <-> pwm_out = 1 100% of the time)
    output reg pwm_out
    );
    
    reg [6:0] duty_cycle_reg;
    reg [6:0] count;
    //updating the duty_cycle
    always@(posedge clk)
    begin
    if(rst_n == 0)
      duty_cycle_reg <= 0;
    else
       if(duty_cycle_in)
         duty_cycle_reg <= duty_cycle;
    end
    
    //counter
    always@(posedge clk)
    begin
    if(rst_n == 0)
     begin
      count <= 1;
      pwm_out <= 0;
     end 
    else
     begin
       if(count == 100) //counting from 1 to 100
          count <= 1;
       else
          count <= count + 1; 
          
        if(count <= duty_cycle_reg)  
              pwm_out <= 1;            
        else
              pwm_out <= 0;
            
      end   
    end
    
    
endmodule
