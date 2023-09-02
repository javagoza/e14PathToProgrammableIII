`timescale 1ns / 1ps


module ddfs
#(parameter PHASE_ACC_WIDTH = 30) // width of phase accumulator
( 
    input   logic               clk,
    input   logic               reset,
    input   logic   [PHASE_ACC_WIDTH-1:0]    freq_carrier_ctrl_word,     // frequency control word to generate carrier frequency
                                                                         // fccw = fout/fsys * 2^N ; (N = PHASE_ACC_WIDTH)
    input   logic   [PHASE_ACC_WIDTH-1:0]    freq_offset_ctrl_word,      // frequency offset control word
                                                                         // focw = fout/fsys * 2^N  ; (N = PHASE_ACC_WIDTH)
    input   logic   [PHASE_ACC_WIDTH-1:0]    phase_offset,               // phase offset
                                                                         // offset/360 * 2^N ; (N = PHASE_ACC_WIDTH)
    input   logic   [15:0]      envelope,                                // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
    input   logic   [2:0]       wave_type,
        
    output  logic   [15:0]      pcm_out,                                 // pcm signal 
    output  logic               pulse_out                                // pulse out
    );
    
    localparam SINE =     3'b000;                 
    localparam SQUARE =   3'b001;
    localparam SAW =      3'b010;
    localparam TRIANGLE = 3'b011;
    localparam NOISE =    3'b100; 
    
    // signal declaration
    logic   [PHASE_ACC_WIDTH-1:0]    freq_control_word;  // frequency modulation control word
    logic   [PHASE_ACC_WIDTH-1:0]    phase_next;         // next phase
    logic   [PHASE_ACC_WIDTH-1:0]    phase_control_word; // phase control word
    logic   [PHASE_ACC_WIDTH-1:0]    phase_reg;          // actual phase
    logic   [7:0]       p2a_raddr;
    logic   [15:0]      amp;
    logic   signed  [31:0] modulation;
    logic   [15:0] pcm_reg;  // multi-bit PCM (pulse code modulation) t    
    logic   [15:0] pcm_next;  // multi-bit PCM (pulse code modulation) t

    
    logic   [15:0] sine_amp;
    logic   [15:0] square_amp;
    logic   [15:0] triangle_amp;
    logic   [15:0] saw_tooth_amp;
    logic   [15:0] noise_amp;
    
    logic   pulse_noise;
    
    // body
    // instanciate sin() ROM
    sin_rom rom_unit
        (   .clk(clk),
            .addr_r(p2a_raddr),
            .dout(sine_amp));
            
            
   // instantiate square_wave unit
    square_wave square_unit
    (   .clk(clk),
        .addr_r(p2a_raddr),
        .dout(square_amp));
        
    // instantiate triangle wave unit
    triangle_wave triangle_unit
    (   .clk(clk),
        .addr_r(p2a_raddr),
        .dout(triangle_amp));
        
    // instantiate saw tooth wave unit
    saw_tooth saw_tooth_unit
    (   .clk(clk),
        .addr_r(p2a_raddr),
        .dout(saw_tooth_amp));
        
    // instantiate lsfr noise
    lfsr lfsr_unit(.clk(pulse_noise),
             .enable(1'b1),
             .lfsr_data(noise_amp),
             .reset(reset));
                  
    // phase register and output buffer
    // use an output buffer (to shorten crtical path since the o/p feeds dac) 
    // always_ff @(posedge clk)
    //    pcm_reg <= modu[29:14];
    always_ff @(posedge clk, posedge reset)
    begin
        if (reset) 
            begin
                phase_reg <= 0;
                pcm_reg <= 0;
            end
        else
            begin
                phase_reg <= phase_next;
                pcm_reg <= pcm_next;
            end
    end
    
    // frequency modulation
    assign freq_control_word = freq_carrier_ctrl_word + freq_offset_ctrl_word;
    
    // phase accumulation
    assign phase_next = phase_reg + freq_control_word;
    
    assign pcm_next = modulation[29:14];
    
    // phase modulation
    assign phase_control_word = phase_reg + phase_offset;
    
    // phase to amplitude mapping address
    assign p2a_raddr = phase_control_word[PHASE_ACC_WIDTH-1:PHASE_ACC_WIDTH-8]; // 8 bits
    
    // amplitude modulation envelop in Q2.14 
    //    * -1 < env < +1  (between 1100...00 and 0100...00) 
    //    * Q16.0 * Q2.14 => modu is Q18.14
    //    * convert modu back to Q16.0  
    // assign modulation = $signed(envelope) * $signed(amp);
    
    always_comb begin
      case (wave_type)
          SINE    : amp = sine_amp;                 
          SQUARE  : amp = square_amp;
          SAW     : amp = saw_tooth_amp;
          TRIANGLE: amp = triangle_amp;
          NOISE   : amp = noise_amp;  
          default:  amp = sine_amp;       
       endcase 
       modulation = $signed(envelope) * $signed(amp);
    end
    
    assign pcm_out = pcm_reg;
    assign pulse_out = phase_reg[PHASE_ACC_WIDTH-1];
    assign pulse_noise = phase_reg[PHASE_ACC_WIDTH-5];   
    
endmodule