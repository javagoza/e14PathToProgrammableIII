`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/19/2023 04:13:08 PM
// Design Name: 
// Module Name: pdm_clk_gen
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

module pdm_clk_gen
  #(
    parameter INPUT_FREQ = 100000000,
    parameter OUTPUT_FREQ = 2400000

    )
   (
    input  clk,
    input  reset,
    output mic_clk,
    output clk_rising
    );

   logic   clk_rising_reg;
   logic   mic_clk_reg;

   localparam CLK_DIVIDER = INPUT_FREQ/OUTPUT_FREQ;

   // M-bit counter  M = LOG2(CLK_DIVIDER)
   logic [$clog2(CLK_DIVIDER)-1:0] clk_counter_reg;

   always_ff@(posedge clk) begin
      if (reset) begin
         clk_counter_reg    <= 0;
         mic_clk_reg    <= 0;
         clk_rising_reg <= 0;
      end
      else begin
         clk_rising_reg <= 0;
         if (clk_counter_reg < (CLK_DIVIDER/2)-1) begin
            clk_counter_reg <= clk_counter_reg + 1;
         end
         else begin
            clk_counter_reg    <= 0;
            mic_clk_reg        <= ~mic_clk_reg;
            clk_rising_reg     <= ~mic_clk_reg;

         end
      end
   end

   assign mic_clk = mic_clk_reg; // Microphone clock signal
   assign clk_rising = clk_rising_reg; //  output pulse on the rising edge of the clock for s_axis_data_tvalid


endmodule