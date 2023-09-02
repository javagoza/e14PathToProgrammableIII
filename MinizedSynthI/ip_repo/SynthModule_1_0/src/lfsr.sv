`timescale 1ns / 1ps

// Description: 
//  This module creates an 16-bit LFSR. A LFSR or Linear Feedback Shift Register
//  is a quick and easy way to generate pseudo-random data inside of an FPGA.

module lfsr #(parameter NUM_BITS = 31)
  (
   input logic clk,
   input logic reset,
   input logic enable,
   output logic [NUM_BITS-1:0] lfsr_data
   );
 
  logic [NUM_BITS:1] lfsr_reg = 0;
  logic polinomial; 

  // run LFSR when enabled.
  always @(posedge clk, posedge reset)
    begin
      if (enable == 1'b1)
        begin
            if (reset == 1'b1) begin
                lfsr_reg <= 1'b0;
            end else begin
                lfsr_reg <= {lfsr_reg[NUM_BITS-1:1], polinomial};
            end
        end else begin
            lfsr_reg <= lfsr_reg;
        end
    end
 
  // Feedback Polynomial based on Xilinx Application Note:
  // http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
  // for 16 bits
  // assign  polinomial = lfsr_reg[16] ^~ lfsr_reg[15] ^~ lfsr_reg[13] ^~ lfsr_reg[4];
  // for 31 bits
   assign  polinomial = lfsr_reg[31] ^~ lfsr_reg[28];
 
  assign lfsr_data = lfsr_reg[16:1];
 
endmodule