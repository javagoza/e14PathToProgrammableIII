`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/19/2023 07:43:59 PM
// Design Name: 
// Module Name: filter
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


module filter (
    input logic clk,
    input logic reset,
    input logic [29:0] fccw, // Pitch, oscillator 1 frequency carrier ctrl word
    input logic [15:0] pcm_in, // sound signal
    input logic [15:0] cutoff_freq, // cut-off frequency
    input logic [15:0] resonance_lvl, // resonance level
    input logic [15:0] eg_amount, // envelope generator level
    input logic [15:0] envelope, // adsr envelope
    input logic [15:0] modulation, // lfo signal 
    output logic [15:0] pcm_out
    );
    
    logic [15:0] lpf_pcm_out;

    
    moving_average_lpf lpf_uut(
        .clk(clk),
        .reset(reset),
        .order(cutoff_freq),
        .signal_in(pcm_in),
        .signal_out(lpf_pcm_out)
    );
    
        
    always begin
        if (eg_amount >0) begin
            pcm_out = lpf_pcm_out;
        end else begin
           pcm_out = pcm_in ; // bypass
       end  
    end
    
    
endmodule


