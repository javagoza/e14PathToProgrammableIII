`timescale 1ns / 10ps


module saw_tooth
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
    logic [DATA_WIDTH-1:0] amp;
    
    
    // read operation
    always_ff @(posedge clk)
    begin
        data_reg <= data;
    end
    
    
    always_comb
    begin
      amp = {8'h00, addr_r} << 8;
      data = $signed(amp) - $signed(16'h8000);   
    end  

    
    assign dout = data_reg;    
endmodule
