`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08/19/2023 05:27:47 PM
// Design Name: 
// Module Name: amplifier
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


module amplifier(
    input logic clk,
    input logic reset,
    input logic [15:0] pcm_input,
    input logic [15:0] envelope, // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
    output logic [15:0] pcm_out
    );
    
    logic   signed  [31:0] modulation;
        
    // amplitude modulation in Q2.14 
    //    * -1 < env < +1  (between 1100...00 and 0100...00) 
    //    * Q16.0 * Q2.14 => modu is Q18.14
    //    * convert modu back to Q16.0  
    // assign modulation = $signed(envelope) * $signed(amp);   
       
    assign modulation = $signed(envelope) * $signed(pcm_input);
    
    // convert back to Q16.0
    assign pcm_out = modulation[29:14];

endmodule
