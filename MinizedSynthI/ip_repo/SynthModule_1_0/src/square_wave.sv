`timescale 1ns / 1ps

/**
A square wave is a non-sinusoidal periodic waveform 
in which the amplitude alternates at a steady frequency between
fixed minimum and maximum values, with the same duration at minimum and maximum. 
n musical terms, they are often described as sounding hollow, and are therefore 
used as the basis for wind instrument sounds created using subtractive synthesis.
Additionally, the distortion effect used on electric guitars clips the outermost
regions of the waveform, causing it to increasingly resemble 
a square wave as more distortion is applied.
*/
module square_wave
#( parameter DATA_WIDTH = 16,  // number of bits
             ADDR_WIDTH = 8    // number od address bits

)
(
    input logic clk,
    input logic [ADDR_WIDTH-1:0] addr_r,
    output logic [DATA_WIDTH-1:0] dout
    );
    
    // signal declaration
    logic [DATA_WIDTH-1:0] data_reg;
    logic [DATA_WIDTH-1:0] data;
    
    
    // read operation
    always_ff @(posedge clk)
    begin
        data_reg <= data;
    end
    
    always_comb
    begin
       data = addr_r[7]?16'b1100_0000_0000_0000:16'b0100_0000_0000_0000;
    end
    
    assign dout = data_reg;    
endmodule