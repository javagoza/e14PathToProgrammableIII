#ifndef SYNTHMODULE1_H
#define SYNTHMODULE1_H


/****************** Include Files ********************/
#include <stdio.h>
#include "xil_types.h"
#include "xstatus.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "sleep.h"


#define PHA_WIDTH 30 // bits in DDFS phase register
#define SYS_CLK_FREQ 100
// MAX is 0x80000000
#define SYNTHMODULE1_MAX            0x7fffffff
#define SYNTHMODULE1_BYPASS_PATTERN 0xffffffff
#define SYNTHMODULE1_STOP_PATTERN   0x00000000

#define SINE     0x00
#define SQUARE   0x01
#define SAW      0x02
#define TRIANGLE 0x03
#define NOISE    0x04

#define SYNTHMODULE_S00_AXI_SLV_REG0_OFFSET 0
#define SYNTHMODULE_S00_AXI_SLV_REG1_OFFSET 4
#define SYNTHMODULE_S00_AXI_SLV_REG2_OFFSET 8
#define SYNTHMODULE_S00_AXI_SLV_REG3_OFFSET 12
#define SYNTHMODULE_S00_AXI_SLV_REG4_OFFSET 16
#define SYNTHMODULE_S00_AXI_SLV_REG5_OFFSET 20
#define SYNTHMODULE_S00_AXI_SLV_REG6_OFFSET 24
#define SYNTHMODULE_S00_AXI_SLV_REG7_OFFSET 28
#define SYNTHMODULE_S00_AXI_SLV_REG8_OFFSET 32
#define SYNTHMODULE_S00_AXI_SLV_REG9_OFFSET 36
#define SYNTHMODULE_S00_AXI_SLV_REG10_OFFSET 40
#define SYNTHMODULE_S00_AXI_SLV_REG11_OFFSET 44
#define SYNTHMODULE_S00_AXI_SLV_REG12_OFFSET 48
#define SYNTHMODULE_S00_AXI_SLV_REG13_OFFSET 52
#define SYNTHMODULE_S00_AXI_SLV_REG14_OFFSET 56
#define SYNTHMODULE_S00_AXI_SLV_REG15_OFFSET 60
#define SYNTHMODULE_S00_AXI_SLV_REG16_OFFSET 64
#define SYNTHMODULE_S00_AXI_SLV_REG17_OFFSET 68
#define SYNTHMODULE_S00_AXI_SLV_REG18_OFFSET 72
#define SYNTHMODULE_S00_AXI_SLV_REG19_OFFSET 76
#define SYNTHMODULE_S00_AXI_SLV_REG20_OFFSET 80
#define SYNTHMODULE_S00_AXI_SLV_REG21_OFFSET 84
#define SYNTHMODULE_S00_AXI_SLV_REG22_OFFSET 88
#define SYNTHMODULE_S00_AXI_SLV_REG23_OFFSET 92
#define SYNTHMODULE_S00_AXI_SLV_REG24_OFFSET 96
#define SYNTHMODULE_S00_AXI_SLV_REG25_OFFSET 100
#define SYNTHMODULE_S00_AXI_SLV_REG26_OFFSET 104
#define SYNTHMODULE_S00_AXI_SLV_REG27_OFFSET 108
#define SYNTHMODULE_S00_AXI_SLV_REG28_OFFSET 112
#define SYNTHMODULE_S00_AXI_SLV_REG29_OFFSET 116
#define SYNTHMODULE_S00_AXI_SLV_REG30_OFFSET 120
#define SYNTHMODULE_S00_AXI_SLV_REG31_OFFSET 124
#define SYNTHMODULE_S00_AXI_SLV_REG32_OFFSET 128
#define SYNTHMODULE_S00_AXI_SLV_REG33_OFFSET 132

/**
* Register 0: Gates Register {30'b0, adsrf_start,adsra_start}
* bit 0: adsra_start, generate a pulse to start the oscillator envelope generation
* bit 1: adsrf_start, generate a pulse to start the filter envelope generation
*/
#define ADSR_START_GATES_OFFSET  SYNTHMODULE_S00_AXI_SLV_REG0_OFFSET

/**
* Register 1 :osc1_fccw
* bits 29 to 0: osc1_fccw 30-bits Pitch, oscillator 1 frequency carrier ctrl word
*/
#define OSC1_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG1_OFFSET

/**
* Register 2 :osc1_focw
* bits 29 to 0: osc1_focw 30-bits Detune, oscillator 1 frequency offset ctrl word
*/
#define OSC1_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG2_OFFSET

/**
* Register 3: osc1_pho:
* bits 29 to 0: osc1_pho, Phase shift, oscillator 1 phase offset ctrl word
*/
#define OSC1_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG3_OFFSET

/**
* Register 4 :osc2_fccw:
* bits 29 to 0: osc2_fccw 30-bits Pitch, oscillator 2 frequency carrier ctrl word
*/
#define OSC2_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG4_OFFSET

/**
* Register 5 :osc2_focw
* bits 29 to 0: osc2_focw 30-bits Detune, oscillator 2 frequency offset ctrl word
*/
#define OSC2_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG5_OFFSET

/**
* Register 6: osc2_pho
* bits 29 to 0: osc2_pho, Phase shift, oscillator 2 phase offset ctrl word
*/
#define OSC2_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG6_OFFSET

/**
* Register 7 :osc3_fccw
* bits 29 to 0: osc3_fccw 30-bits Pitch, oscillator 3 frequency carrier ctrl word
*/
#define OSC3_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG7_OFFSET

/**
* Register 8 :osc3_focw
* bits 29 to 0: osc3_focw 30-bits Detune, oscillator 3 frequency offset ctrl word
*/
#define OSC3_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG8_OFFSET

/**
* Register 9: osc3_pho
* bits 29 to 0: osc3_pho, Phase shift, oscillator 3 phase offset ctrl word
*/
#define OSC3_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG9_OFFSET

/**
* Register 10 :lfoo_fccw
* bits 29 to 0: lfoo_fccw 30-bits Pitch, LFO oscillator frequency carrier ctrl word
*/
#define LFOO_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG10_OFFSET

/**
* Register 11 :lfoo_focw
* bits 29 to 0: lfoo_focw 30-bits Detune, LFO oscillator frequency offset ctrl word
*/
#define LFOO_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG11_OFFSET

/**
* Register 12: lfoo_pho
* bits 29 to 0: lfoo_pho, Phase shift, LFO oscillator phase offset ctrl word
*/
#define LFOO_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG12_OFFSET

/**
* Register 13 :lfof_fccw
* bits 29 to 0: lfof_fccw 30-bits Pitch, LFO filter frequency carrier ctrl word
*/
#define LFOF_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG13_OFFSET

/**
* Register 14 :lfof_focw
* bits 29 to 0: lfof_focw 30-bits Detune, LFO filter frequency offset ctrl word
*/
#define LFOF_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG14_OFFSET

/**
* Register 15: lfof_pho
* bits 29 to 0: lfof_pho, Phase shift, LFO filter phase offset ctrl word
*/
#define LFOF_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG15_OFFSET

/**
* Register 16: adsra_a, Amplifier ADSR configuration attack step
* bits 31 to 0: adsra_a, precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
*/
#define ADSRA_A_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG16_OFFSET

/**
* Register 17: adsra_d, Amplifier ADSR configuration decay step
* bits 31 to 0: adsra_d, precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
*/
#define ADSRA_D_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG17_OFFSET

/**
* Register 18: adsra_sl, Amplifier ADSR configuration sustain level
* bits 31 to 0: adsra_sl, amplitude for the sustain segment
*/
#define ADSRA_SL_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG18_OFFSET

/**
* Register 19: adsra_r, Amplifier ADSR configuration, steps for the release segment
* bits 31 to 0: adsra_r, precalculated (A_sus - 0)/(t_release - t_sys) steps fot the release segment
*/
#define ADSRA_R_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG19_OFFSET

/**
* Register 20: adsra_st, Amplifier ADSR configuration, time for the sustain segment
* bits 31 to 0: adsra_st, tsustain / t_sys steps for the sustain
*/
#define ADSRA_ST_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG20_OFFSET

/**
* Register 21: adsrf_a, Filter ADSR configuration attack step
* bits 31 to 0: adsrf_a, precalculated (Amax - 0)/(t_attack - t_sys) steps for the attack segment
*/
#define ADSRF_A_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG21_OFFSET

/**
* Register 22: adsrf_d, Filter ADSR configuration decay step
* bits 31 to 0: adsrf_d, precalculated (A_max-A_sus) / (t_sustain / t_sys) steps for the decay segment
*/
#define ADSRF_D_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG22_OFFSET

/**
* Register 23: adsrf_sl, Filter ADSR configuration sustain level
* bits 31 to 0: adsrf_sl, amplitude for the sustain segment
*/
#define ADSRF_SL_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG23_OFFSET

/**
* Register 24: adsrf_r, Filter ADSR configuration, steps for the release segment
* bits 31 to 0: adsrf_r, precalculated (A_sus - 0)/(t_release - t_sys) steps for the release segment
*/
#define ADSRF_R_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG24_OFFSET

/**
* Register 25: adsrf_st, Filter ADSR configuration, time for the sustain segment
* bits 31 to 0: adsrf_st, tsustain / t_sys steps for the sustain
*/
#define ADSRF_ST_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG25_OFFSET

/**
* Register 26: fcut filter cutoff frequency
* bits 31 to 0: fcut, filter cutoff frequency
*/
#define FCUT_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG26_OFFSET

/**
* Register 27: fres, filter resonance level
* bits 31 to 0: fres, filter resonance level
*/
#define FRES_OFFSET 	         SYNTHMODULE_S00_AXI_SLV_REG27_OFFSET

/**
* Register 28: feg, filter envelope generator amount
* bits 31 to 0: feg, filter envelope generator amount
*/
#define FEG_OFFSET 	             SYNTHMODULE_S00_AXI_SLV_REG28_OFFSET

/**
* Register 29: wave_types:
* bits 2 to 0: osc1_wt, oscillator 1 wave type
* bits 5 to 3: osc2_wt, oscillator 2 wave type
* bits 8 to 6: osc3_wt, oscillator 3 wave type
* bits 11 to 9: lfoo_wt, LFO oscillator wave type
* bits 14 to 12: lfof_wt, LFO filter wave type
*/
#define WAVE_TYPES_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG29_OFFSET


/**
* Register 30: Mix_levels_osc1_osc2
* bits 15 to 0: mix_lvl_osc1, oscillator 1 volume level
* bits 31 to 16: mix_lvl_osc2, oscillator 2 volume level
*/
#define MIX_LVL_OSC1_OSC2_OFFSET SYNTHMODULE_S00_AXI_SLV_REG30_OFFSET

/**
* Register 31: Mix_levels_osc3_mic
* bits 15 to 0: mix_lvl_osc3, oscillator 3 volume level
* bits 31 to 16: mix_lvl_mic, mic volume level
*/
#define MIX_LVL_OSC3_MIC_OFFSET  SYNTHMODULE_S00_AXI_SLV_REG31_OFFSET

/**
* Register 32: LFO amounts configurations
* bits 15 to 0: lfoo_lvl, Sets the maximum amount the LFO moves the Oscillators pitch up and down, up to +/- 1 octave.
* bits 31 to 16: lfof_lvl, Sets the maximum amount the LFO moves the Filter Cutoff up and down, up to +/- 5 octaves.
*/
#define LFO_AMOUNTS_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG32_OFFSET

/**
* Read Registers
* Register 33: idle status and amplifier envelope amplitude data
* bits 31 to 16: 16 bit envelope data
* bit 0: idle status
*/
#define AMP_ADSR_STATUS_OFFSET 	 SYNTHMODULE_S00_AXI_SLV_REG33_OFFSET



/**
* Register 1 :osc1_fccw
* bits 29 to 0: osc1_fccw 30-bits Pitch, oscillator 1 frequency carrier ctrl word
*/
#define OSC1_FCCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG1_OFFSET

/**
* Register 2 :osc1_focw
* bits 29 to 0: osc1_focw 30-bits Detune, oscillator 1 frequency offset ctrl word
*/
#define OSC1_FOCW_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG2_OFFSET

/**
* Register 3: osc1_pho:
* bits 29 to 0: osc1_pho, Phase shift, oscillator 1 phase offset ctrl word
*/
#define OSC1_PHO_OFFSET 	     SYNTHMODULE_S00_AXI_SLV_REG3_OFFSET


typedef struct OscillatorConfig {
   u32 fccwOffset;
   int fccw;  // Pitch, oscillator frequency carrier ctrl word

   u32 focwOffset;
   float frequencyOffsetLvl;
   int focw;  // Detune, oscillator frequency offset ctrl word

   u32 phoOffset;
   int pho;  // Phase shift, oscillator phase offset ctrl word

   u32 waveTypeOffset; //
   u8  waveTypeIndex;
   u8  waveType;

   u32 amplitudeOffset;
   u8  amplitudeIndex;
   // mix level
   float amplitude;
} OscillatorConfig;

typedef struct AdsrConfig {
	u32 atackStepsOffset;
	u32 ams;

	u32 decayStepsOffset;
	u32 dms;

	u32 sustainLevelOffset;
	float slevel;

	u32 sustainTimeOffset;
	u32 sms;

	u32 releaseStepsOffset;
	u32 rms;

	u32 gateOffset;

	u32 statusOffset;
	u32 status;
} AdsrConfig;

typedef struct MicConfig {
	u32 micLevelOffset;
	u32 amplitudeIndex;
	float amplitude;
} MicConfig;

typedef struct Synth{
	u32 			 baseAddr;
	OscillatorConfig osc_1;
	OscillatorConfig osc_2;
	OscillatorConfig osc_3;
	OscillatorConfig osc_lfo;
	OscillatorConfig filter_lfo;
    MicConfig        mic;
	AdsrConfig       amplifier_adsr;
	AdsrConfig       filter_adsr;
} Synth;



/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a SYNTHMODULE register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the SYNTHMODULEdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void SYNTHMODULE_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define SYNTHMODULE_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a SYNTHMODULE register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the SYNTHMODULE device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 SYNTHMODULE_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define SYNTHMODULE_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))




/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the SYNTHMODULE instance to be worked on.
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
XStatus SYNTHMODULE1_Reg_SelfTest(u32 baseaddr_p);


void SYNTHMODULE1_init(Synth * synth, u32 baseAddr);

void SYNTHMODULE1_osc1_set_carrier_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc1_set_offset_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc1_set_phase_degree(Synth * synth, int phase);
void SYNTHMODULE1_osc1_set_amp(Synth * synth, float amp);
void SYNTHMODULE1_osc1_set_wave_type(Synth * synth, int waveType);

void SYNTHMODULE1_osc2_set_carrier_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc2_set_offset_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc2_set_phase_degree(Synth * synth, int phase);
void SYNTHMODULE1_osc2_set_amp(Synth * synth, float amp);
void SYNTHMODULE1_osc2_set_wave_type(Synth * synth, int waveType);

void SYNTHMODULE1_osc3_set_carrier_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc3_set_offset_freq(Synth * synth, int freq);
void SYNTHMODULE1_osc3_set_phase_degree(Synth * synth, int phase);
void SYNTHMODULE1_osc3_set_amp(Synth * synth, float amp);
void SYNTHMODULE1_osc3_set_wave_type(Synth * synth, int waveType);

void SYNTHMODULE1_lfoo_set_carrier_freq(Synth * synth, float freq);
void SYNTHMODULE1_lfoo_set_offset_freq(Synth * synth, int freq);
void SYNTHMODULE1_lfoo_set_phase_degree(Synth * synth, int phase);
void SYNTHMODULE1_lfoo_set_amp(Synth * synth, float amp);
void SYNTHMODULE1_lfoo_set_wave_type(Synth * synth, int waveType);

void SYNTHMODULE1_lfof_set_carrier_freq(Synth * synth, float freq);
void SYNTHMODULE1_lfof_set_offset_freq(Synth * synth, int freq);
void SYNTHMODULE1_lfof_set_phase_degree(Synth * synth, int phase);
void SYNTHMODULE1_lfof_set_amp(Synth * synth, float amp);
void SYNTHMODULE1_lfof_set_wave_type(Synth * synth, int waveType);



int SYNTHMODULE1_amplifier_adsr_idle(Synth * synth);
void SYNTHMODULE1_amplifier_adsr_start(Synth * synth);
void SYNTHMODULE1_amplifier_adsr_abort(Synth * synth);
void SYNTHMODULE1_amplifier_adsr_bypass(Synth * synth);
void SYNTHMODULE1_amplifier_adsr_set_env(Synth * synth, int attack_ms, int decay_ms, int sustain_ms, int release_ms, float sus_level);
void SYNTHMODULE1_amplifier_adsr_select_env(Synth * synth, int n);

int SYNTHMODULE1_filter_adsr_idle(Synth * synth);
void SYNTHMODULE1_filter_adsr_start(Synth * synth);
void SYNTHMODULE1_filter_adsr_abort(Synth * synth);
void SYNTHMODULE1_filter_adsr_bypass(Synth * synth);
void SYNTHMODULE1_filter_adsr_set_env(Synth * synth, int attack_ms, int decay_ms, int sustain_ms, int release_ms, float sus_level);
void SYNTHMODULE1_filter_adsr_select_env(Synth * synth, int n);

void SYNTHMODULE1_amplifier_adsr_write_reg(Synth * synth);

int SYNTHMODULE1_calc_note_freq(int octave, int noteIndex);
void SYNTHMODULE1_play_note(Synth * synth, int note, int octave, int duration);

u32 SYNTHMODULE1_calc_phase_control_word(int phase);

u32 SYNTHMODULE1_convert_amp_to_q214(float amp) ;

void SYNTHMODULE1_set_wave_types(Synth* synth);

void SYNTHMODULE1_set_mix_lvl_osc1_osc2(Synth* synth);


void SYNTHMODULE1_set_mix_lvl_osc3_mic(Synth* synth);

u32 SYNTHMODULE1_calc_lfo_freq_control_word(float freq);

void SYNTHMODULE1_mic_set_amp(Synth* synth, float amp);

void setBaseConfiguration(Synth * synth);

void SYNTHMODULE1_amplifier_adsr_write_default(Synth * synth);

void SYNTHMODULE1_osc2_set_offset_freq_lvl(Synth * synth, float offset);

#endif // SYNTHMODULE1_H
