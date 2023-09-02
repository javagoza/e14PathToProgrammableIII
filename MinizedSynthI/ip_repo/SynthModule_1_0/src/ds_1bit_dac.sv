`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/19/2023 06:41:03 PM
// Design Name: 
// Module Name: ds_1bit_dac
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


module ds_1bit_dac
#(parameter W = 16)  // input width
(
input logic clk,
input logic reset,
input logic [W-1:0] pcm_in,
output logic pdm_out
    );
    
    // signal declarations
    localparam BIAS = 2 ** (W-1); // {1'b1, (W-2){1'b0}};
    logic [W:0] pcm_biased;
    logic [W:0] acc_next;
    logic [W:0] acc_reg;
    
    // shift the range from [-2^(W-1) -1 , 2^(W-1)-1] to [0, 2^W-1]
    assign pcm_biased = {pcm_in[W - 1], pcm_in} + BIAS;
    
    // signal treated as unsigned number in delta-sigma modulation
    assign acc_next = {1'b0, acc_reg[W-1:0]} + pcm_biased;
    
    // accumulation register
    always_ff @(posedge clk, posedge reset)
    begin
        if (reset)
        begin
            acc_reg <= 0;
        end
        else
        begin
            acc_reg <= acc_next;
        end
    end
    
    assign pdm_out = acc_reg[W];
    
    
endmodule
