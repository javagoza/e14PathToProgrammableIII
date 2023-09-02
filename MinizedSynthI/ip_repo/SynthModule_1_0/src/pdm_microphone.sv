`timescale 1ns / 1ps

module pdm_microphone
  #(
    parameter INPUT_FREQ = 100000000,
    parameter PDM_FREQ = 2400000

    )
   (
    input         clk,
    input         reset,

    output [15:0] mic_pcm_data,
    output        mic_data_valid,


    output        mic_clk,
    input         mic_pdm_data,
    output        mic_lrsel

    );

   logic [2:0]      data_reg; 
   logic            clk_rising;


   always_ff@(posedge clk) begin
      if (reset) begin
         data_reg       <= 0;

      end
      else begin
         data_reg[0]   <= mic_pdm_data;
         data_reg[2:1] <= data_reg[1:0];

      end
   end

   // clock gen
   pdm_clk_gen
     #(
       .INPUT_FREQ(INPUT_FREQ),
       .OUTPUT_FREQ(PDM_FREQ)

       )
   pdm_clk_gen_i
     (
      .clk(clk),
      .reset(reset),

      .mic_clk(mic_clk),
      .clk_rising(clk_rising)

      );

   logic [31:0] cic_out_data;
   logic        cic_out_valid;

   cic_compiler_0 pdm_to_pcm
     (
      .aclk(clk),                                // input wire aclk
      .s_axis_data_tdata({7'b0,data_reg[2]}),    // input wire [7 : 0] s_axis_data_tdata
      .s_axis_data_tvalid(clk_rising),           // input wire s_axis_data_tvalid
      .s_axis_data_tready(),                     // output wire s_axis_data_tready

      .m_axis_data_tdata(cic_out_data),         // output wire [15 : 0] m_axis_data_tdata
      .m_axis_data_tvalid(cic_out_valid)        // output wire m_axis_data_tvalid
      );


   assign mic_lrsel = 0;
   assign mic_pcm_data = cic_out_data;
   assign mic_data_valid = cic_out_valid;

endmodule