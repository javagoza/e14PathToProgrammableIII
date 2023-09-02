`timescale 1ns / 10ps

/**
A triangular wave or triangle wave is a non-sinusoidal waveform 
named for its triangular shape. It is a periodic, piecewise linear, 
continuous real function. Like a square wave, the triangle wave
contains only odd harmonics. 
The higher harmonics roll off much faster than in a square wave.
*/

module triangle_wave
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
      if (addr_r > 8'h7F) begin
           data = {8'h00, 8'hFE - addr_r + 1}; // decaying
      end else begin
           data = {8'h00, addr_r}; // raising
      end    
    end   
    
    assign dout =  $signed(data << 9) - $signed(16'h8000);  // offset biass  
endmodule