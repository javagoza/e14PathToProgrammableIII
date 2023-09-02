

/***************************** Include Files *******************************/
#include "SynthModule1.h"

/************************** Function Definitions ***************************/

/************************** Constant Definitions ***************************/
#define READ_WRITE_MUL_FACTOR 0x10


/**
 *
 * Initialize the modulecalculates write and read register addresses
 * based on SynthModule base address. SYNTHMODULE1_init() must be call once
 * before any other API call
 *
 * @param   baseAddr is the base address of the SYNTHMODULE instance to be worked on.
 *
 * @return
 *
 *    - void
 *
 */
void SYNTHMODULE1_init(Synth * synth, u32 baseAddr) {

	synth->baseAddr = baseAddr;

	// calculate oscillator 1 write register addresses
   synth->osc_1.fccwOffset       =  OSC1_FCCW_OFFSET;
   synth->osc_1.focwOffset       =  OSC1_FOCW_OFFSET;
   synth->osc_1.phoOffset        =  OSC1_PHO_OFFSET;
   synth->osc_1.waveTypeOffset   =  WAVE_TYPES_OFFSET;
   synth->osc_1.waveTypeIndex  = 0;
   synth->osc_1.amplitudeOffset  =  MIX_LVL_OSC1_OSC2_OFFSET;
   synth->osc_1.amplitudeIndex = 0;

   // calculate oscillator 2 write register addresses
   synth->osc_2.fccwOffset       =  OSC2_FCCW_OFFSET;
   synth->osc_2.focwOffset       =  OSC2_FOCW_OFFSET;
   synth->osc_2.phoOffset        =  OSC2_PHO_OFFSET;
   synth->osc_2.waveTypeOffset   =  WAVE_TYPES_OFFSET;
   synth->osc_2.waveTypeIndex  = 1;
   synth->osc_2.amplitudeOffset  =  MIX_LVL_OSC1_OSC2_OFFSET;
   synth->osc_2.amplitudeIndex = 1;

   // calculate oscillator 3 write register addresses
   synth->osc_3.fccwOffset       =  OSC3_FCCW_OFFSET;
   synth->osc_3.focwOffset       =  OSC3_FOCW_OFFSET;
   synth->osc_3.phoOffset        =  OSC3_PHO_OFFSET;
   synth->osc_3.waveTypeOffset   =  WAVE_TYPES_OFFSET;
   synth->osc_3.waveTypeIndex  = 2;
   synth->osc_3.amplitudeOffset  =  MIX_LVL_OSC3_MIC_OFFSET;
   synth->osc_3.amplitudeIndex = 0;

   // calculate oscillators lfo write register addresses
   synth->osc_lfo.fccwOffset       =  LFOO_FCCW_OFFSET;
   synth->osc_lfo.focwOffset       =  LFOO_FOCW_OFFSET;
   synth->osc_lfo.phoOffset        =  LFOO_PHO_OFFSET;
   synth->osc_lfo.waveTypeOffset   =  WAVE_TYPES_OFFSET;
   synth->osc_lfo.waveTypeIndex  = 3;
   synth->osc_lfo.amplitudeOffset  =  LFO_AMOUNTS_OFFSET;
   synth->osc_lfo.amplitudeIndex = 0;

   // calculate filter lfo write register addresses
   synth->filter_lfo.fccwOffset       =  LFOF_FCCW_OFFSET;
   synth->filter_lfo.focwOffset       =  LFOF_FOCW_OFFSET;
   synth->filter_lfo.phoOffset        =  LFOF_PHO_OFFSET;
   synth->filter_lfo.waveTypeOffset   =  WAVE_TYPES_OFFSET;
   synth->filter_lfo.waveTypeIndex  = 4;
   synth->filter_lfo.amplitudeOffset  =  LFO_AMOUNTS_OFFSET;
   synth->filter_lfo.amplitudeIndex = 1;

   // calculate microphone write register addresses
   synth->mic.micLevelOffset          =  MIX_LVL_OSC3_MIC_OFFSET;
   synth->mic.amplitudeIndex        = 1;

   // calculate amplifier adsr write register addresses
   synth->amplifier_adsr.atackStepsOffset   =  ADSRA_A_OFFSET;
   synth->amplifier_adsr.decayStepsOffset   =  ADSRA_D_OFFSET;
   synth->amplifier_adsr.sustainLevelOffset =  ADSRA_SL_OFFSET;
   synth->amplifier_adsr.sustainTimeOffset  =  ADSRA_ST_OFFSET;
   synth->amplifier_adsr.releaseStepsOffset =  ADSRA_R_OFFSET;

   // calculate filter adsr write register addresses
   synth->filter_adsr.atackStepsOffset   =  ADSRF_A_OFFSET;
   synth->filter_adsr.decayStepsOffset   =  ADSRF_D_OFFSET;
   synth->filter_adsr.sustainLevelOffset =  ADSRF_SL_OFFSET;
   synth->filter_adsr.sustainTimeOffset  =  ADSRF_ST_OFFSET;
   synth->filter_adsr.releaseStepsOffset =  ADSRF_R_OFFSET;

   // calculate amplifier gate addresses
   synth->amplifier_adsr.gateOffset =  ADSR_START_GATES_OFFSET;

   // calculate amplifier adsr read register address
   synth->amplifier_adsr.statusOffset =  AMP_ADSR_STATUS_OFFSET;


}

/**
 * Configure System as:
 * 	- two main sawtooth wave oscillators
 * 	- two sinusoidal waves lfo
 * 	- 1 noise input
 * 	- 1 microphone input
 *
 *
 */
void setBaseConfiguration(Synth * synth) {

	SYNTHMODULE1_osc1_set_carrier_freq(synth, 262);
	SYNTHMODULE1_osc1_set_amp(synth, 0);
	SYNTHMODULE1_osc1_set_phase_degree(synth, 0);
	SYNTHMODULE1_osc1_set_wave_type(synth, SAW);

	SYNTHMODULE1_amplifier_adsr_bypass(synth);

	SYNTHMODULE1_osc2_set_carrier_freq(synth, 262);
	SYNTHMODULE1_osc2_set_phase_degree(synth, 0);
	SYNTHMODULE1_osc2_set_wave_type(synth, SAW);
	SYNTHMODULE1_osc2_set_amp(synth, 0);

	SYNTHMODULE1_set_mix_lvl_osc1_osc2(synth);

	SYNTHMODULE1_osc3_set_wave_type(synth, NOISE);
	SYNTHMODULE1_osc3_set_amp(synth, 0.0);
	SYNTHMODULE1_mic_set_amp(synth, 0.0);

	SYNTHMODULE1_lfoo_set_amp(synth, 0);
	SYNTHMODULE1_lfoo_set_carrier_freq(synth,.01);
	SYNTHMODULE1_lfoo_set_wave_type(synth, TRIANGLE);
	SYNTHMODULE1_lfoo_set_phase_degree(synth, 0);




	SYNTHMODULE1_amplifier_adsr_set_env(synth, 100, 50, 100, 50, 0.9);

	SYNTHMODULE1_amplifier_adsr_start(synth);



}

/************************** Function Definitions ***************************/




/**************************  Support functions ***************************/

/**
 * Calculates frequency control word in steps
 * Only for audio frequencies above 10 Hz
 *
 * @param freq frequency in hertz
 */
u32 SYNTHMODULE1_calc_freq_control_word(int freq) {
	u32 fcw, p2n;
	float tmp;
	p2n = 1 << PHA_WIDTH;
	tmp = ((float) p2n) / (float) (SYS_CLK_FREQ * 1000000.0);
	fcw = (u32) (freq * tmp);
	return fcw;
}

/**
 * Calculate the frequency for lfo oscillators
 * lfo range is from 0.01Hz to 100Hz.
 *
 * @param freq frequency in hertz
 */
u32 SYNTHMODULE1_calc_lfo_freq_control_word(float freq) {
	u32 fcw, p2n;
	float tmp;
	p2n = 1 << PHA_WIDTH;
	tmp = ((float) p2n) / (float) (SYS_CLK_FREQ * 1000000.0);
	fcw = (u32) (freq * tmp);
	return fcw;
}

u32 SYNTHMODULE1_calc_phase_control_word(int phase) {
	u32 pha;
	pha = (SYS_CLK_FREQ * 1000000) * phase / 360;
	return pha;
}

u32 SYNTHMODULE1_convert_amp_to_q214(float amp) {
	// convert floating point to fixed-point Q2.14 format
	u32 q214;
	float max_amp;
	max_amp = (float) (0x4000); // amp * 2^15
	q214 = (u32) (amp * max_amp);

	return q214 & 0x0000ffff;
}

/**********************************************************
 * Setters
 **********************************************************/

/*********************************************************
 * Oscillator 1 setters
 *********************************************************/

void SYNTHMODULE1_osc1_set_carrier_freq(Synth * synth, int freq) {
	synth->osc_1.fccw = freq;
	SYNTHMODULE_mWriteReg (synth->baseAddr, synth->osc_1.fccwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}

void SYNTHMODULE1_osc1_set_offset_freq(Synth * synth, int freq) {
	synth->osc_1.focw = freq;
	SYNTHMODULE_mWriteReg (synth->baseAddr, synth->osc_1.focwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}

void SYNTHMODULE1_osc1_set_phase_degree(Synth * synth, int phase) {
	synth->osc_1.pho = phase;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_1.phoOffset, 	SYNTHMODULE1_calc_phase_control_word(phase));
}

void SYNTHMODULE1_osc1_set_amp(Synth * synth, float amp) {
	synth->osc_1.amplitude = amp;

}

void SYNTHMODULE1_osc1_set_wave_type(Synth * synth, int waveType) {
	synth->osc_1.waveType = waveType;
	SYNTHMODULE1_set_wave_types( synth);
}

/*********************************************************
 * Oscillator 2 setters
 *********************************************************/

void SYNTHMODULE1_osc2_set_carrier_freq(Synth * synth, int freq){
	synth->osc_2.fccw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_2.fccwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}

void SYNTHMODULE1_osc2_set_offset_freq_lvl(Synth * synth, float offset) {
	synth->osc_2.frequencyOffsetLvl = offset;
}
void SYNTHMODULE1_osc2_set_offset_freq(Synth * synth, int freq) {
	synth->osc_2.focw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_2.focwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}
void SYNTHMODULE1_osc2_set_phase_degree(Synth * synth, int phase) {
	synth->osc_2.pho = phase;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_2.phoOffset, SYNTHMODULE1_calc_phase_control_word(phase));
}
void SYNTHMODULE1_osc2_set_amp(Synth * synth, float amp) {
	synth->osc_2.amplitude = amp;

}

void SYNTHMODULE1_osc2_set_wave_type(Synth * synth, int waveType) {
	synth->osc_2.waveType = waveType;
	SYNTHMODULE1_set_wave_types( synth);
}


/*********************************************************
 * Oscillator 3 setters
 *********************************************************/

void SYNTHMODULE1_osc3_set_carrier_freq(Synth * synth, int freq) {
	synth->osc_3.fccw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_3.fccwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}
void SYNTHMODULE1_osc3_set_offset_freq(Synth * synth, int freq) {
	synth->osc_3.focw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_3.focwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}
void SYNTHMODULE1_osc3_set_phase_degree(Synth * synth, int phase) {
	synth->osc_3.pho = phase;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_3.phoOffset, 	SYNTHMODULE1_calc_phase_control_word(phase));
}
void SYNTHMODULE1_osc3_set_amp(Synth * synth, float amp) {
	synth->osc_3.amplitude = amp;
}

void SYNTHMODULE1_osc3_set_wave_type(Synth * synth, int waveType) {
	synth->osc_3.waveType = waveType;
	SYNTHMODULE1_set_wave_types(synth);
}

/*********************************************************
 * Oscillators LFO setters
 *********************************************************/

void SYNTHMODULE1_lfoo_set_carrier_freq(Synth * synth, float freq) {
	synth->osc_lfo.fccw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_lfo.fccwOffset, SYNTHMODULE1_calc_lfo_freq_control_word(freq));
}
void SYNTHMODULE1_lfoo_set_offset_freq(Synth * synth, int freq) {
	synth->osc_lfo.focw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_lfo.focwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}
void SYNTHMODULE1_lfoo_set_phase_degree(Synth * synth, int phase) {
	synth->osc_lfo.pho = phase;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_lfo.phoOffset, 	SYNTHMODULE1_calc_phase_control_word(phase));
}
void SYNTHMODULE1_lfoo_set_amp(Synth * synth, float amp) {
	synth->osc_lfo.amplitude = amp;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_lfo.amplitudeOffset, 	SYNTHMODULE1_convert_amp_to_q214(amp));
}

void SYNTHMODULE1_lfoo_set_wave_type(Synth * synth, int waveType) {
	synth->osc_lfo.waveType = waveType;
	SYNTHMODULE1_set_wave_types(synth);
}

/*********************************************************
 * Filter LFO setters
 *********************************************************/

void SYNTHMODULE1_lfof_set_carrier_freq(Synth * synth, float freq) {
	synth->filter_lfo.fccw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->filter_lfo.fccwOffset, SYNTHMODULE1_calc_lfo_freq_control_word(freq));
}
void SYNTHMODULE1_lfof_set_offset_freq(Synth * synth, int freq) {
	synth->filter_lfo.focw = freq;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->filter_lfo.focwOffset, SYNTHMODULE1_calc_freq_control_word(freq));
}
void SYNTHMODULE1_lfof_set_phase_degree(Synth * synth, int phase) {
	synth->filter_lfo.pho = phase;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->filter_lfo.phoOffset, 	SYNTHMODULE1_calc_phase_control_word(phase));
}
void SYNTHMODULE1_lfof_set_amp(Synth * synth, float amp) {
	synth->filter_lfo.amplitude = amp;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->filter_lfo.amplitudeOffset, 	SYNTHMODULE1_convert_amp_to_q214(amp));
}

void SYNTHMODULE1_lfof_set_wave_type(Synth * synth, int waveType) {
	synth->filter_lfo.waveType = waveType;
	SYNTHMODULE1_set_wave_types(synth);
}



/*********************************************************
 * Wave type register LFO setters
 *********************************************************/

void SYNTHMODULE1_set_wave_types(Synth* synth) {
	u32 waveType;
	waveType = (synth->osc_1.waveType & 7)
			| (synth->osc_2.waveType & 7) << 3
			| (synth->osc_3.waveType & 7) << 6
			| (synth->osc_lfo.waveType & 7) << 9
			| (synth->filter_lfo.waveType & 7) << 12;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_1.waveTypeOffset, 	waveType);
}

/*********************************************************
 * Microphone input setters
 *********************************************************/

void SYNTHMODULE1_mic_set_amp(Synth * synth, float amp) {
	synth->mic.amplitude = amp;
}

/*********************************************************
 * Amplifier ADSR setters
 *********************************************************/

/**
 * Put amplifier ADSR to IDLE status
 *
 * @param   synth is the SynthModule configuration instance
 *
 * @return
 *
 *    - void
 *
 */
int SYNTHMODULE1_amplifier_adsr_idle(Synth * synth){
	return Xil_In32(synth->amplifier_adsr.statusOffset);
}

/**
 * Send an start signal to amplifier ADSR
 *
 * @param   synth is the SynthModule configuration instance
 *
 * @return
 *
 *    - void
 *
 */
void SYNTHMODULE1_amplifier_adsr_start(Synth * synth) {
	//SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.gateOffset, (u32) 0);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.gateOffset, (u32) 3);
	usleep(10);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.gateOffset, (u32) 0);
}

/**
 * Send an stop signal to amplifier ADSR
 *
 * @param   synth is the SynthModule configuration instance
 *
 * @return
 *
 *    - void
 *
 */
void SYNTHMODULE1_amplifier_adsr_abort(Synth * synth) {
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) SYNTHMODULE1_STOP_PATTERN);
}

/**
 * Bypass amplifier ADSR, output is not modulated
 *
 * @param   synth is the SynthModule configuration instance
 *
 * @return
 *
 *    - void
 *
 */
void SYNTHMODULE1_amplifier_adsr_bypass(Synth * synth) {
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) SYNTHMODULE1_BYPASS_PATTERN);
}


/**
 * Configure amplifier ADSR
 *
 * @param   synth is the SynthModule configuration instance
 * @param   attack_ms attack time in milliseconds
 * @param   decay_ms decay time in milliseconds
 * @param   sustain_ms sustain time in milliseconds
 * @param   release_ms release time in milliseconds
 * @param   sus_level in percentage of MAX value
 *
 * @return
 *
 *    - void
 *
 */
void SYNTHMODULE1_amplifier_adsr_set_env(Synth * synth, int attack_ms, int decay_ms, int sustain_ms, int release_ms, float sus_level) {
	synth->amplifier_adsr.ams = attack_ms;
	synth->amplifier_adsr.dms = decay_ms;
	synth->amplifier_adsr.sms = sustain_ms;
	synth->amplifier_adsr.rms = release_ms;
	synth->amplifier_adsr.slevel = sus_level;
	SYNTHMODULE1_amplifier_adsr_write_reg(synth);
}

/**
 * Sets a preset Amplifier ADSR configuration
 */
void SYNTHMODULE1_amplifier_adsr_select_env(Synth * synth, int n) {
	switch(n) {
	case 1:
		SYNTHMODULE1_amplifier_adsr_set_env(synth,100,50,100,50, 0.9);
			break;
	case 2:
		SYNTHMODULE1_amplifier_adsr_set_env(synth,10,50,100,100, 0.9);
			break;
	case 3:
		SYNTHMODULE1_amplifier_adsr_set_env(synth,10,200,100,100, .1);
			break;
	default:
		SYNTHMODULE1_amplifier_adsr_set_env(synth,10,200,100,100, .1);
			break;
	}

}

/**
 * upload amplifier ADSR configuration writing the ADSR registers
 */
void SYNTHMODULE1_amplifier_adsr_write_reg(Synth * synth) {
	u32 nc, step, sus_abs;
	// clocks per ms = 0.001 / (1 / ( SYS_CLK_FREQ * 1000000))
	const u32 clks = SYS_CLK_FREQ * 1000;

	if (synth->amplifier_adsr.ams == SYNTHMODULE1_BYPASS_PATTERN) {
		SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) SYNTHMODULE1_BYPASS_PATTERN);
		return;
	}

	if (synth->amplifier_adsr.ams == SYNTHMODULE1_STOP_PATTERN) {
		SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) SYNTHMODULE1_STOP_PATTERN);
		return;
	}

	// convert sustain value in absolute value
	sus_abs = (u32) SYNTHMODULE1_MAX * synth->amplifier_adsr.slevel;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.sustainLevelOffset, (u32) sus_abs);

	// convert attack time (in ms) into envelope increment steps
	nc = synth->amplifier_adsr.ams * clks;
	step = SYNTHMODULE1_MAX / nc;
	if (step == 0) {
		step = 1;
	}
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) step);

	// convert decay time ( in ms) into envelope decrement step
	nc = synth->amplifier_adsr.dms * clks;
	step = (SYNTHMODULE1_MAX - sus_abs) / nc;
	if (step == 0) {
		step = 1;
	}
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.decayStepsOffset, (u32) step);

	// convert sustain time (in ms)  into clks
	nc = synth->amplifier_adsr.sms * clks;
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.sustainTimeOffset, (u32) nc);

	// convert release time (in ms) into envelope decrement step
	nc = synth->amplifier_adsr.rms * clks;
	step = sus_abs / nc;
	if (step == 0) {
		step = 1;
	}
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.releaseStepsOffset, (u32) step);

}
/**
 *
 *  logic [31:0] attack_step_value  = 32'h0000_00d6;   // precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
    logic [31:0] decay_step_value   = 32'h0000_002a;   // precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
    logic [31:0] sustain_level      = 32'h7333_3332;   // amplitude for the sustain segment
    logic [31:0] sustain_time       = 32'h0098_9680;   // tsustain / t_sys steps for the sustain
    logic [31:0] release_step_value = 32'h0000_0183;   // precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
 */


void SYNTHMODULE1_amplifier_adsr_write_default(Synth * synth) {
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.sustainLevelOffset, (u32) 0x73333332);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.atackStepsOffset, (u32) 0x000000d6);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.decayStepsOffset, (u32) 0x0000002a);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.sustainTimeOffset, (u32) 0x00989680);
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->amplifier_adsr.releaseStepsOffset, (u32) 0x00000183);

}

/************************ Mixer Functions ********   ***********************/

/**
 * Save mix level for oscillators 1 and 2
 */
void SYNTHMODULE1_set_mix_lvl_osc1_osc2(Synth* synth) {

	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_1.amplitudeOffset,
			(u32) (	SYNTHMODULE1_convert_amp_to_q214(synth->osc_2.amplitude) << 16
			| SYNTHMODULE1_convert_amp_to_q214(synth->osc_1.amplitude) ) );

//	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_1.amplitudeOffset,
//			(u32) (	0X18002800 ));

}

/**
 * Save mix level for oscillators 3 and microphone input
 */
void SYNTHMODULE1_set_mix_lvl_osc3_mic(Synth* synth) {
	SYNTHMODULE_mWriteReg(synth->baseAddr, synth->osc_3.amplitudeOffset,
			(u32) (	SYNTHMODULE1_convert_amp_to_q214(synth->mic.amplitude) << 16
			| SYNTHMODULE1_convert_amp_to_q214(synth->osc_3.amplitude)  ) );

}

/************************ Music Related Functions **************************/

int SYNTHMODULE1_calc_note_freq(int octave, int noteIndex) {
	// frequency table for octave 0
	const float NOTES[]={
			16.3516,	// 0 C
			17.3239,	// 1 C#
			18.3541, 	// 2 D
			19.4454,	// 3 D#
			20.6017,	// 4 E
			21.8268,	// 5 F
			23.1247,	// 6 F#
			24.4997,	// 7 G
			25.9565,	// 8 G#
			27.5000, 	// 9 A
			29.1352, 	// 10 A#
			30.8677		// 11 B
	};

	int freq;

	freq = (u32) NOTES[noteIndex] * (1 << octave);
	return freq;
}

void SYNTHMODULE1_play_note(Synth * synth, int note, int octave, int duration){
	int sus_tmp;
	int freq;

	freq = SYNTHMODULE1_calc_note_freq(octave, note);
	SYNTHMODULE1_osc1_set_carrier_freq(synth, freq);
	SYNTHMODULE1_osc2_set_carrier_freq(synth, freq/2 + (u32)(
			synth->osc_2.frequencyOffsetLvl * (float)( freq * 2 - freq /2) ));
	SYNTHMODULE1_osc3_set_carrier_freq(synth, freq);

	sus_tmp = duration - (synth->amplifier_adsr.ams + synth->amplifier_adsr.dms + synth->amplifier_adsr.rms);
	if (sus_tmp <=0) {
		sus_tmp = 10;
	}

	SYNTHMODULE1_amplifier_adsr_set_env(synth,
			synth->amplifier_adsr.ams,
			synth->amplifier_adsr.dms,
			sus_tmp,
			synth->amplifier_adsr.rms,
			synth->amplifier_adsr.slevel);

	SYNTHMODULE1_amplifier_adsr_start(synth);


}

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the SYNTHMODULEinstance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus SYNTHMODULE1_Reg_SelfTest(u32 baseaddr_p)
{
	u32 baseaddr;
	int write_loop_index;
	int read_loop_index;

	baseaddr = (u32) baseaddr_p;

	xil_printf("******************************\n\r");
	xil_printf("* User Peripheral Self Test\n\r");
	xil_printf("******************************\n\n\r");

	/*
	 * Write to user logic slave module register(s) and read back
	 */
	xil_printf("User logic slave module test...\n\r");

	for (write_loop_index = 0 ; write_loop_index < 4; write_loop_index++)
	  SYNTHMODULE_mWriteReg (baseaddr, write_loop_index*4, (write_loop_index+1)*READ_WRITE_MUL_FACTOR);
	for (read_loop_index = 0 ; read_loop_index < 4; read_loop_index++)
	  if ( SYNTHMODULE_mReadReg (baseaddr, read_loop_index*4) != (read_loop_index+1)*READ_WRITE_MUL_FACTOR){
	    xil_printf ("Error reading register value at address %x\n", (int)baseaddr + read_loop_index*4);
	    return XST_FAILURE;
	  }

	xil_printf("   - slave register write/read passed\n\n\r");

	return XST_SUCCESS;
}


