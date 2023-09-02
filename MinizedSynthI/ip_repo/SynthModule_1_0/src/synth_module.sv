`timescale 1ns / 1ps


module synth_module #(PHASE_ACC_WIDTH = 30)(
    input logic clk,
    input logic reset,
    // oscillator 1 configuration
    input logic [PHASE_ACC_WIDTH-1:0] osc1_fccw, // Pitch, oscillator 1 frequency carrier ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc1_focw, // Detune, oscillator 1 frequency offset ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc1_pho, // Phase shift, oscillator 1 phase offset ctrl word
    input logic [2:0] osc1_wt, // oscillator 1 wave type
    input logic [15:0] mix_lvl_osc1, // oscillator 1 volume level
   
    // oscillator 2 configuration
    input logic [PHASE_ACC_WIDTH-1:0] osc2_fccw, // Pitch, oscillator 1 frequency carrier ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc2_focw, // Detune, oscillator 1 frequency offset ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc2_pho, // Phase shift, oscillator 1 phase offset ctrl word
    input logic [2:0] osc2_wt, // oscillator 2 wave type
    input logic [15:0] mix_lvl_osc2, // oscillator 2 volume level
    
    // oscillator 3 / noise oscillator configuration
    input logic [PHASE_ACC_WIDTH-1:0] osc3_fccw, // Pitch, oscillator 1 frequency carrier ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc3_focw, // Detune, oscillator 1 frequency offset ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] osc3_pho, // Phase shift, oscillator 1 phase offset ctrl word
    input logic [2:0] osc3_wt, // oscillator 3 wave type
    input logic [15:0] mix_lvl_osc3, // oscillator 3 volume level
   
    // LFO oscillator configuration
    input logic [PHASE_ACC_WIDTH-1:0] lfoo_fccw, // LFO oscillator frequency carrier ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] lfoo_focw, // LFO oscillator frequency offset ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] lfoo_pho, // LFO oscillator phase offset ctrl word
    input logic [2:0] lfoo_wt, // LFO oscillator wave type
    input logic [15:0] lfoo_lvl, // Sets the maximum amount the LFO moves the VCOs pitch up and down, up to +/- 1 octave.
    
    // LFO filter configuration
    input logic [PHASE_ACC_WIDTH-1:0] lfof_fccw, // LFO filter frequency carrier ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] lfof_focw, // LFO filter frequency offset ctrl word
    input logic [PHASE_ACC_WIDTH-1:0] lfof_pho, // LFO filter phase offset ctrl word
    input logic [2:0] lfof_wt, // LFO filter wave type
    input logic [15:0] lfof_lvl, // Sets the maximum amount the LFO moves the Filter Cutoff up and down, up to +/- 5 octaves.
    
    // Amplifier ADSR configuration
    input logic adsra_start, // generate a pulse to start the envelope generation
    input logic [31:0] adsra_a, // precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
    input logic [31:0] adsra_d,  // precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
    input logic [31:0] adsra_sl, // amplitude for the sustain segment
    input logic [31:0] adsra_r,  // precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
    input logic [31:0] adsra_st, // tsustain / t_sys steps for the sustain

    // Filter ADSR configuration
    input logic adsrf_start, // generate a pulse to start the envelope generation
    input logic [31:0] adsrf_a, // precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
    input logic [31:0] adsrf_d,  // precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
    input logic [31:0] adsrf_sl, // amplitude for the sustain segment
    input logic [31:0] adsrf_r,  // precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
    input logic [31:0] adsrf_st, // tsustain / t_sys steps for the sustain
    
    // Filter configuration
    input logic [31:0] fcut, // filter cutoff frequency
    input logic [31:0] fres, // filter resonance level
    input logic [31:0] feg, // filter envelope generator amount


    // PDM Microphone 
    input logic mic_pdm_input, // Input external P1-bit DM MiniZed Signal AUDIO_DAT
    input logic [15:0] mix_lvl_mic, // oscillator 3 volume level
        
    output logic pdm_out, // 1-bit PDM sound output
    output logic mic_clk,  // generated microphone clock signal route to Minized external AUDIO_CLK
    
    output logic adsra_idle,
    output logic [15:0] adsra_amp


    );
    
    // signal declarations
    localparam BIAS = 2 ** (15); // {1'b1, (W-2){1'b0}};
 

    
    // constants for envelopes
    localparam MAX = 32'h8000_0000; // MAX volume
    localparam BYPASS = 32'hffff_ffff; // Bypass envelope
    localparam ZERO = 32'h0000_0000; // mute
    
    // constants for oscillators
    localparam SINE =     3'b000;                 
    localparam SQUARE =   3'b001;
    localparam SAW =      3'b010;
    localparam TRIANGLE = 3'b011;
    localparam NOISE =    3'b100; 
    
    // lfo oscillator outputs
    logic [15:0] lfoo_output; // LFO oscillator output
    logic lfoo_pulse_out; //  LFO Oscillator pulse output 
    
//    // lfo filter outputs
    logic [15:0] lfof_output; // lfo filter output
    logic lfof_pulse_out; //  Oscillator 1 pulse output 
    
    
    // Oscillator 1 outputs
    logic [15:0] osc1_pcm_out; // Oscillator 1 PCM output 
    logic osc1_pulse_out; //  Oscillator 1 pulse output 
    
//    // Oscillator 2 outputs
    logic [15:0] osc2_pcm_out; // Oscillator 2 PCM output 
    logic osc2_pulse_out; //  Oscillator 2 pulse output 
    
//    // Noise / Oscillator 3 outputs
    logic [15:0] osc3_pcm_out; // Oscillator 3 PCM output 
    logic osc3_pulse_out; //  Oscillator 3 pulse output 
    
    // Microphone Outputs
    logic [15:0] mic_pcm_out; // Microphone PCM output 
    logic mic_data_valid; //  Microphone PDM to PCM conversion data valid
    
    // Mixer outputs
    logic [15:0] mix_pcm_out; // Mixer PCM output 
    
    // amplifier ADSR amplitude output
     logic [15:0] adsra_amp;
     
     // filter ADSR amplitude output
     logic [15:0] adsrf_amp;
    
    // Amplifier outputs
    logic [15:0] amp_pcm_out;
    
//    filter outputs
    logic [15:0] filter_pcm_out;
    
    
    logic [PHASE_ACC_WIDTH-1:0] lfoo_extended;
    logic [PHASE_ACC_WIDTH-1:0] lfof_extended;
    
    logic adsrf_idle;
    
    
//    // Keep The range is from 0.01Hz to 100Hz
//    // instantiate lfo oscillator
    ddfs #(.PHASE_ACC_WIDTH(PHASE_ACC_WIDTH)) lfoo(.clk(clk),
        .reset(reset),
        .freq_carrier_ctrl_word(lfoo_fccw),
        .freq_offset_ctrl_word(lfoo_focw),
        .phase_offset(lfoo_pho),
        .envelope(lfoo_lvl), // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
        .pcm_out(lfoo_output),
        .pulse_out(lfoo_pulse_out),
        .wave_type( lfoo_wt)
        );
        
    // extend sign to 30 bits    
    assign lfoo_extended = {{ 14{lfoo_output[15]}},lfoo_output}; 
    
    // extend sign to 30 bits    
    assign lfof_extended = {{ 14{lfof_output[15]}},lfof_output}; 


    
    
//    // Keep The range is from up to +/- 5 octaves. Amounts above 20KHz or below 20 Hz are clipped
//    // instantiate lfo filter
    ddfs #(.PHASE_ACC_WIDTH(PHASE_ACC_WIDTH)) lfof(.clk(clk),
        .reset(reset),
        .freq_carrier_ctrl_word(lfof_fccw),
        .freq_offset_ctrl_word(lfof_focw),
        .phase_offset(lfof_pho),
        .envelope(lfof_lvl), // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
        .pcm_out(lfof_output),
        .pulse_out(lfof_pulse_out),
        .wave_type( lfof_wt)
        );
    
    // instantiate oscillator 1 osc_1
    ddfs #(.PHASE_ACC_WIDTH(PHASE_ACC_WIDTH)) osc1(.clk(clk),
        .reset(reset),
        .freq_carrier_ctrl_word(osc1_fccw),// 4.724 .freq_carrier_ctrl_word(osc1_fccw),
        .freq_offset_ctrl_word(lfoo_extended),  // .freq_offset_ctrl_word(lfoo_output), // LFO moves the VCOs pitch up and down, up to +/- 1 octave
        .phase_offset(osc1_pho), // .phase_offset(osc1_pho),
        .envelope(16'h4000), // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
        .pcm_out(osc1_pcm_out),
        .pulse_out(osc1_pulse_out),
        .wave_type(osc1_wt)//.wave_type( 3'b001)
        );
        
//   // instantiate oscillator 2 osc_2
    ddfs #(.PHASE_ACC_WIDTH(PHASE_ACC_WIDTH)) osc2(.clk(clk),
        .reset(reset),
        .freq_carrier_ctrl_word(osc2_fccw),
        .freq_offset_ctrl_word(lfoo_extended), // LFO moves the VCOs pitch up and down, up to +/- 1 octave
        .phase_offset(osc2_pho),
        .envelope(16'h4000), // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
        .pcm_out(osc2_pcm_out),
        .pulse_out(osc2_pulse_out),
        .wave_type( osc2_wt)
        );
        
    // instantiate oscillator 3 osc_3 usually noise
    ddfs #(.PHASE_ACC_WIDTH(PHASE_ACC_WIDTH)) osc3(.clk(clk),
        .reset(reset),
        .freq_carrier_ctrl_word(osc3_fccw),
        .freq_offset_ctrl_word(lfoo_extended),
        .phase_offset(osc3_pho),
        .envelope(16'h4000), // Amplitude modulation: envelope, digitized value of A(t) in Q2.14 format
        .pcm_out(osc3_pcm_out),
        .pulse_out(osc3_pulse_out),
        .wave_type( osc3_wt)
        );
        
    // instantiate PDM to PCM converter for the microphone input
    pdm_microphone microphone(
        .clk(clk),
        .reset(reset),
        .mic_pdm_data(mic_pdm_input),
        .mic_pcm_data(mic_pcm_out),
        .mic_data_valid(mic_data_valid),
        .mic_clk(mic_clk)
    );    
        
    // instantiate mixer
    mixer mix(.clk(clk),
        .reset(reset),
        .input_1(osc1_pcm_out),
        .lvl_1(mix_lvl_osc1),
        .input_2(osc2_pcm_out),
        .lvl_2(mix_lvl_osc2),
        .input_3(osc3_pcm_out),// .input_3(osc3_pcm_out),
        .lvl_3(mix_lvl_osc3),
        .input_4(mic_pcm_out),
        .lvl_4(mix_lvl_mic),
        .pcm_out(mix_pcm_out)
        );
        
     // instantiate amplifier ADSR amplitude generator
      adsr adsra(
          .clk(clk),
          .reset(reset),
          .start(adsra_start), // generate a pulse to start the envelope generation
          .attack_step_value(adsra_a), // precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
          .decay_step_value(adsra_d),  // precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
          .sustain_level(adsra_sl), // amplitude for the sustain segment
          .release_step_value(adsra_r),  // precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
          .sustain_time(adsra_st), // tsustain / t_sys steps for the sustain
          .envelope(adsra_amp),
          .adsr_idle(adsra_idle)
          );
          
//      // instantiate filter ADSR amplitude generator
      adsr adsrf(
          .clk(clk),
          .reset(reset),
          .start(adsrf_start), // generate a pulse to start the envelope generation
          .attack_step_value(adsrf_a), // precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
          .decay_step_value(adsrf_d),  // precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
          .sustain_level(adsrf_sl), // amplitude for the sustain segment
          .release_step_value(adsrf_r),  // precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
          .sustain_time(adsrf_st), // tsustain / t_sys steps for the sustain
          .envelope(adsrf_amp),
          .adsr_idle(adsrf_idle)
          );
        
      // instantiate Amplifier
      amplifier amp(
          .clk(clk),
          .reset(reset),
          .pcm_input(mix_pcm_out),
          .envelope(adsra_amp),
          .pcm_out(amp_pcm_out));
          
//      // instantiate filter
      filter filter_unit(
          .clk(clk),
          .reset(reset),
          .fccw(osc1_fccw),
          .pcm_in(amp_pcm_out),
          .cutoff_freq(fcut),
          .resonance_lvl(fres),
          .eg_amount(feg), //EG AMOUNT Determines how much the Filter Envelope Generator (EG) adds to or subtracts from the Filter Cutoff control setting.
          .envelope(adsrf_amp),
          .modulation(lfof_output),
          .pcm_out(filter_pcm_out)
      );
          
      // instantiate 1 bit delta sigma DAC
      ds_1bit_dac dac(
        .clk(clk),
        .reset(reset),
        .pcm_in(filter_pcm_out),
        .pdm_out(pdm_out)
        );
      
      
        
    
endmodule
