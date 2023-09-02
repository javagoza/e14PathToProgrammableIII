`timescale 1ns / 10ps

module sin_rom
#( parameter DATA_WIDTH = 16,  // number of bits
             ADDR_WIDTH = 8    // number od address bits

)
(
    input logic clk,
    input logic [ADDR_WIDTH-1:0] addr_r,
    output logic [DATA_WIDTH-1:0] dout
    );
    
    // signal declaration
    logic [DATA_WIDTH-1:0] ram [0:2**ADDR_WIDTH-1];  // ascending range
    logic [DATA_WIDTH-1:0] data_reg;
    
    initial
        $readmemh("sin_table.mem", ram);
    
    // read operation
    always_ff @(posedge clk)
    begin
        data_reg <= ram[addr_r];
    end
    
    assign dout = data_reg;    
endmodule