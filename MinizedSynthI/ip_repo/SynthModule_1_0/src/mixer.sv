`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/19/2023 11:21:59 AM
// Design Name: 
// Module Name: mixer
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


module mixer(
    input logic clk,
    input logic reset,
    input logic [15:0] input_1, // input signal 1
    input logic [15:0] lvl_1, // volume level 1
    input logic [15:0] input_2, // input signal 2
    input logic [15:0] lvl_2, // volume level 2
    input logic [15:0] input_3, // input signal 3
    input logic [15:0] lvl_3, // volume level 3
    input logic [15:0] input_4, // input signal 4
    input logic [15:0] lvl_4, // volume level 4
    output logic [15:0] pcm_out
    );
    
    logic   signed  [31:0] modulation1;
    logic   signed  [31:0] modulation2;
    logic   signed  [31:0] modulation3;
    logic   signed  [31:0] modulation4;
    logic   signed  [31:0] sum; 
    
    // amplitude modulation in Q2.14 
    //    * -1 < env < +1  (between 1100...00 and 0100...00) 
    //    * Q16.0 * Q2.14 => modu is Q18.14
    //    * convert modu back to Q16.0  
    // assign modulation = $signed(envelope) * $signed(amp);   
       
    assign modulation1 = $signed(lvl_1) * $signed(input_1);
    assign modulation2 = $signed(lvl_2) * $signed(input_2);
    assign modulation3 = $signed(lvl_3) * $signed(input_3);
    assign modulation4 = $signed(lvl_4) * $signed(input_4);    
    
    // Q18.14 + Q18.14 = Q18.14
    assign sum = $signed( modulation1 + modulation2 + modulation3 + modulation4);
    // TODO overflow treatment with clipping
    
    // convert back to Q16.0
    assign pcm_out = sum[29:14];
 
endmodule
