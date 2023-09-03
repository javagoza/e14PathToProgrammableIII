`timescale 1ns / 1ps
///////////////////////////////////////////////////////////////////////


module moving_average_lpf(
    input logic clk,
    input logic reset,
    input logic [7:0] order,
    input logic  [15:0] signal_in,
    output logic  [15:0] signal_out
);


    // signal declarations
    localparam BIAS = 2 ** 15; // {1'b1, (W-2){1'b0}};

    logic [31:0] ma_new, ma_new_reg;
    logic [31:0] ma_old, ma_old_reg;
    logic [31:0] signal_n;
    logic [16:0] unbiased_signal_out;
    logic [16:0] biased_signal;
    logic [16:0] biased_trucated_output;
    
        
    // shift the range from [-2^(15) -1 , 2^(15)-1] to [0, 2^32-1]
    assign biased_signal = {signal_in[15], signal_in} + BIAS;
    
    assign signal_n = {biased_signal, 15'b0};
    
    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            ma_new_reg <=0;
            ma_old_reg <=0;
        end else begin
            ma_new_reg <= ma_new;
            ma_old_reg <= ma_old;
        end
    end
    

    assign ma_old = ma_new_reg;
    assign ma_new = $signed(signal_n  >> order) + $signed(ma_old_reg) - $signed(ma_old_reg >> order) ;
    
    assign biased_trucated_output = ma_new_reg[31:15];
    
    

    assign unbiased_signal_out = $signed(biased_trucated_output - BIAS) ;
    
    assign signal_out = unbiased_signal_out[15:0];

endmodule
