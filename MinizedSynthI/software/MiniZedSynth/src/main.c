/***************************** Include Files ********************************/

#include "xsysmon.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xintc.h"
#include "stdio.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "sleep.h"
#include "xgpio.h"
#include "xgpiops.h"
#include "SynthModule1.h"
#include "xil_cache.h"

/************************** Constant Definitions *****************************/

#define LEDS 0x1F   /* 5 LEDS  */

#define SWITCHES 0xF   /* 4 SWITCHES  */

#define PS_LED_R 52
#define PS_LED_G 53
#define PS_PUSH_BUTTON 0

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */

#ifndef GPIOPS_DEVICE_ID
#define GPIOPS_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#endif

#ifndef GPIOPL_DEVICE_ID
#define GPIOPL_DEVICE_ID		XPAR_GPIO_0_DEVICE_ID
#endif

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY     1

/*
 * channel of the GPIO is used for the LEDs
 */
#define LED_CHANNEL 2

/*
 * channel of the GPIO is used for the SWITCHES
 */
#define SWITCH_CHANNEL 1

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define SYSMON_DEVICE_ID	XPAR_SYSMON_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define INTR_ID			XPAR_INTC_0_SYSMON_0_VEC_ID

#define printf xil_printf 	/* Small foot-print printf function */

/**************************** Type Definitions ******************************/
typedef struct PanelSwitchesType {
	int switch1;
	int switch2;
	int switch3;
	int switch4;
} PanelSwitchesType;

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Function Prototypes *****************************/

static int initXADCMultiplexer(XIntc* IntcInstPtr, XSysMon* SysMonInstPtr,
		u16 SysMonDeviceId, u16 SysMonIntrId);

static void SysMonInterruptHandler(void *CallBackRef);

static int SysMonSetupInterruptSystem(XIntc* IntcInstPtr, XSysMon *SysMonPtr,
		u16 IntrId);

static int initGPIOLedsSwitchesPanel();

static int synthMainLoop(XSysMon* SysMonInstPtr);

void setEnvelopeFromPanel(Synth* synth, u32* AdcData, int releaseSelected);

void play_melody_check(XSysMon* SysMonInstPtr, Synth* synth, u32* AdcData);

void readFrontPanel(XSysMon* SysMonInstPtr, u32* AdcData,
		PanelSwitchesType* panelSwitches);

PanelSwitchesType panelSwitches = { 0, 0, 0, 0 };

/************************** Variable Definitions ****************************/

static XSysMon SysMonInst; /* System Monitor driver instance */
static XIntc IntcInst; /* Instance of the XIntc driver */

volatile static int EosFlag = FALSE; /* EOS interrupt */

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio Gpio; /* The Instance of the PL GPIO Driver */
XGpioPs GpioPS; /* The Instance of the PS GPIO Driver */

Synth synth; /* The instance of the Synth Module */

/****************************************************************************/
/**
 *
 * Main function that invokes the Interrupt example.
 *
 * @param	None.
 *
 * @return
 *		- XST_SUCCESS if the example has completed successfully.
 *		- XST_FAILURE if the example has failed.
 *
 * @note		None.
 *
 *****************************************************************************/
int main(void) {

	int Status;

	Status = initXADCMultiplexer(&IntcInst, &SysMonInst,
	SYSMON_DEVICE_ID, INTR_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("XADC MUltiplexer Initialization Failed\r\n");
		return XST_FAILURE;
	}

	Status = initGPIOLedsSwitchesPanel();
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO Initialization Failed\r\n");
		return XST_FAILURE;
	}
	Status = synthMainLoop(&SysMonInst);
	if (Status != XST_SUCCESS) {
		xil_printf("Synth Main Loop Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran MiniZedSynth\r\n");
	return XST_SUCCESS;

}

void readFrontPanel(XSysMon* SysMonInstPtr, u32* AdcData,
		PanelSwitchesType* panelSwitches) {

	int waveChanged = 0;
	int relesaseSelected = 0;
	int micSelected = 0;

	/* Loop forever blinking the LED and reading switches*/

	u32 switches = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);

	if ((switches & 1) == 0) {
		waveChanged = 1;
		panelSwitches->switch1 = !panelSwitches->switch1;
		printf("switch14 %d \n", panelSwitches->switch1);
	}

	if ((switches & 2) == 0) {
		panelSwitches->switch2 = !panelSwitches->switch2;
		printf("switch 2 %d \n", panelSwitches->switch2);
		waveChanged = 1;
	}

	if ((switches & 4) == 0) {
		panelSwitches->switch3 = !panelSwitches->switch3;
		printf("switch 3 %d \n", panelSwitches->switch3);
		if (panelSwitches->switch3) {
			micSelected = 1 ;
		} else {
			micSelected = 0 ;
		}
	}

	if ((switches & 8) == 0) {
		panelSwitches->switch4 = !panelSwitches->switch4;
		printf("switch 4 %d \n", panelSwitches->switch4);
		if (panelSwitches->switch4) {
			relesaseSelected = 1 ;
		} else {
			relesaseSelected = 0 ;
		}

	}

	if (waveChanged) {
		if (panelSwitches->switch1) {
			SYNTHMODULE1_osc1_set_wave_type(&synth, SQUARE);
		} else {
			SYNTHMODULE1_osc1_set_wave_type(&synth, SAW);
		}
		if (panelSwitches->switch3) {
			SYNTHMODULE1_osc2_set_wave_type(&synth, SQUARE);
		} else {
			SYNTHMODULE1_osc2_set_wave_type(&synth, SAW);
		}
	}

	/*
	 * Read the ADC converted Data from the data registers.
	 */
	/* Read ADC data for channels 0 - 8 */
	for (int Index = 0; Index < 16; Index++) {
		AdcData[Index] = XSysMon_GetAdcData(SysMonInstPtr,
		XSM_CH_AUX_MIN + Index);
		// printf("%d,", (AdcData[Index] )>>8 );

	}

	SYNTHMODULE1_osc1_set_amp(&synth, ((float) (AdcData[0] >> 8)) / 256.0); // osc 1
	SYNTHMODULE1_osc2_set_amp(&synth, ((float) (AdcData[5] >> 8)) / 256.0); // osc 2
	if (micSelected) {
		SYNTHMODULE1_osc3_set_amp(&synth, 0); //osc 3 noise
		SYNTHMODULE1_mic_set_amp(&synth, ((float) (AdcData[15] >> 8)) / 256.0); //mic
	} else {
		SYNTHMODULE1_osc3_set_amp(&synth, ((float) (AdcData[15] >> 8)) / 256.0); //osc 3
		SYNTHMODULE1_mic_set_amp(&synth, 0); //mic
	}

	//SYNTHMODULE1_osc2_set_offset_freq_lvl(&synth, (float) (AdcData[1] >> 8) / 256.0);

	// lfo

	if ((AdcData[4] >> 8) > 10) {
		SYNTHMODULE1_lfoo_set_carrier_freq(&synth,
				0.1 + 100 * ((float) (AdcData[4] >> 8) - 10) / 256.0);
		SYNTHMODULE1_lfoo_set_amp(&synth, (float) (AdcData[2] >> 10) / 256.0);

	} else {
		SYNTHMODULE1_lfoo_set_amp(&synth, 0.0); // lffo oscillators
	}

	SYNTHMODULE1_set_mix_lvl_osc1_osc2(&synth);
	SYNTHMODULE1_set_mix_lvl_osc3_mic(&synth);

	XGpio_DiscreteWrite(&Gpio, LED_CHANNEL,
			16 | (panelSwitches->switch1 & 1)
					| ((panelSwitches->switch2 & 1) << 1)
					| ((panelSwitches->switch3 & 1) << 2)
					| ((panelSwitches->switch4 & 1) << 3));

	setEnvelopeFromPanel(&synth, AdcData, relesaseSelected);
}

static int synthMainLoop(XSysMon* SysMonInstPtr) {
	volatile int Delay;
	Delay = 0;

	SYNTHMODULE1_amplifier_adsr_write_default(&synth);
	u32 AdcData[16];
	while (1) {

		readFrontPanel(SysMonInstPtr, AdcData, &panelSwitches);

		u32 plButton = XGpioPs_ReadPin(&GpioPS, PS_PUSH_BUTTON);

		if (plButton == 1) {
			play_melody_check(SysMonInstPtr, &synth, AdcData);

		}

		if (Delay == LED_DELAY) {
			u32 leds = XGpio_DiscreteRead(&Gpio, LED_CHANNEL);
			XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, leds ^ 16);
		}

		Delay++;

		usleep(200000);
	}
	Xil_DCacheEnable();

	return XST_SUCCESS;
}

static int initGPIOLedsSwitchesPanel() {

	int Status;
	XGpioPs_Config *ConfigPtr;

	// PS GPIOs
	ConfigPtr = XGpioPs_LookupConfig(GPIOPS_DEVICE_ID);

	Status = XGpioPs_CfgInitialize(&GpioPS, ConfigPtr, ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the direction for the LED pins to be output and
	 * Enable the Output enable for the LED Pins.
	 */
	XGpioPs_SetDirectionPin(&GpioPS, PS_LED_R, 1);
	XGpioPs_SetOutputEnablePin(&GpioPS, PS_LED_R, 1);

	/* Set the GPIO output to be low. */
	XGpioPs_WritePin(&GpioPS, PS_LED_R, 0x1);

	XGpioPs_SetDirectionPin(&GpioPS, PS_LED_G, 1);
	XGpioPs_SetOutputEnablePin(&GpioPS, PS_LED_G, 1);

	/* Set the GPIO output to be low. */
	XGpioPs_WritePin(&GpioPS, PS_LED_G, 0x1);

	/* Set the direction for the specified pin to be input. */
	XGpioPs_SetDirectionPin(&GpioPS, PS_PUSH_BUTTON, 0x0);
	/* Read the state of the data so that it can be  verified. */

	/* Initialize the PL GPIO driver */
	Status = XGpio_Initialize(&Gpio, GPIOPL_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0xffff);

	/* Set the direction for all signals as inputs */
	XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0x0);

	Xil_DCacheDisable();

	/* Initialize the Synth Module */
	SYNTHMODULE1_init(&synth, XPAR_SYNTHMODULE_0_S00_AXI_BASEADDR);
	setBaseConfiguration(&synth);

	return XST_SUCCESS;
}

/****************************************************************************/
/**
 *
 * This function runs a test on the XADC device using the driver APIs.
 *
 * The function does the following tasks:
 *	- Initiate the XADC device driver instance
 *	- Run self-test on the device
 *	- Reset the device
 *	- Set up sequencer registers to continuously monitor the auxiliary
 *	  channel pairs avaibale in XADC
 *	- Setup interrupt system
 *	- Enable interrupts
 *	- Set up configuration registers to start the sequencer in simultaneous
 *	  sampling mode

 *
 * @param	IntcInstPtr is a pointer to the Interrupt Controller
 *		driver Instance.
 * @param	SysMonInstPtr is a pointer to the XSysMon driver Instance.
 * @param	SysMonDeviceId is the XPAR_<SYSMON_ADC_instance>_DEVICE_ID value
 *		from xparameters.h.
 * @param	SysMonIntrId is
 *		XPAR_<INTC_instance>_<SYSMON_ADC_instance>_VEC_ID value from
 *		xparameters.h.
 *
 * @return
 *		- XST_SUCCESS if the example has completed successfully.
 *		- XST_FAILURE if the example has failed.
 *
 * @note		This function may never return if no interrupt occurs.
 *
 ****************************************************************************/
static int initXADCMultiplexer(XIntc* IntcInstPtr, XSysMon* SysMonInstPtr,
		u16 SysMonDeviceId, u16 SysMonIntrId) {
	int Status;
	XSysMon_Config *ConfigPtr;
	u32 IntrStatus;
	u32 IntrEnable;

	printf("\r\nXADC External MUX Example. \r\n");

	/*
	 * Initialize the SysMon driver.
	 */
	ConfigPtr = XSysMon_LookupConfig(SysMonDeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	XSysMon_CfgInitialize(SysMonInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

	/*
	 * Self Test the System Monitor/ADC device.
	 */
	Status = XSysMon_SelfTest(SysMonInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Disable the Channel Sequencer before configuring the Sequencer.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SAFE);

	Status = XSysMon_SetSeqChEnables(SysMonInstPtr,
	XSM_SEQ_CH_AUX00 |
	XSM_SEQ_CH_AUX01 |
	XSM_SEQ_CH_AUX02 |
	XSM_SEQ_CH_AUX03 |
	XSM_SEQ_CH_AUX04 |
	XSM_SEQ_CH_AUX05 |
	XSM_SEQ_CH_AUX06 |
	XSM_SEQ_CH_AUX07 |
	XSM_SEQ_CH_AUX08 |
	XSM_SEQ_CH_AUX09 |
	XSM_SEQ_CH_AUX10 |
	XSM_SEQ_CH_AUX11 |
	XSM_SEQ_CH_AUX12 |
	XSM_SEQ_CH_AUX13 |
	XSM_SEQ_CH_AUX14 |
	XSM_SEQ_CH_AUX15);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the ADCCLK frequency equal to 1/32 of System clock for the System
	 * Monitor/ADC in the Configuration Register 2.
	 */
	XSysMon_SetAdcClkDivisor(SysMonInstPtr, 32);

	/*
	 * Setup the interrupt system.
	 */
	Status = SysMonSetupInterruptSystem(IntcInstPtr, SysMonInstPtr,
			SysMonIntrId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable global interrupt of System Monitor.
	 */
	XSysMon_IntrGlobalEnable(SysMonInstPtr);

	/*
	 * Clear any pending interrupts.
	 */
	IntrStatus = XSysMon_IntrGetStatus(SysMonInstPtr);
	XSysMon_IntrClear(SysMonInstPtr, IntrStatus);

	/*
	 * Enable EOS interrupts.
	 */
	XSysMon_IntrEnable(SysMonInstPtr, XSM_IPIXR_EOS_MASK);
	IntrEnable = XSysMon_IntrGetEnabled(SysMonInstPtr);
	if ((IntrEnable & XSM_IPIXR_EOS_MASK) != XSM_IPIXR_EOS_MASK) {
		return XST_FAILURE;
	}

	/*
	 * Enable external Mux and connect to Aux CH0 and Aux CH8.
	 */
	//XSysMon_SetExtenalMux(SysMonInstPtr, 0x10); /* 0b'10000 to CH[4:0] */
	XSysMon_SetExtenalMux(SysMonInstPtr, 0x10); /* 0b'10000 to CH[4:0] */

	/*
	 * Enable simultaneous sequencer mode.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SIMUL);

	XSysMon_GetStatus(SysMonInstPtr); /* Clear the latched status */

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 *
 * This function is the Interrupt Service Routine for the XADC device.
 * It will be called by the processor when an interrupt is asserted by the
 * device.
 *
 * There are 10 different interrupts supported
 *	- Over Temperature
 *	- ALARM 0
 *	- ALARM 1
 *	- ALARM 2
 *	- End of Sequence
 *	- End of Conversion
 *	- JTAG Locked
 *	- JATG Modified
 *	- Over Temperature deactivate
 *	- ALARM 0 deactivate
 *
 * This function only handles EOS interrupts.
 * User of this code may need to modify the code to meet the needs of the
 * application.
 *
 * @param	CallBackRef is the callback reference passed from the Interrupt
 *		controller driver, which in our case is a pointer to the
 *		driver instance.
 *
 * @return	None.
 *
 * @note		This function is called within interrupt context.
 *
 ******************************************************************************/
static void SysMonInterruptHandler(void *CallBackRef) {
	u32 IntrStatusValue;
	XSysMon *SysMonPtr = (XSysMon *) CallBackRef;

	/*
	 * Get the interrupt status from the device and check the value.
	 */
	IntrStatusValue = XSysMon_IntrGetStatus(SysMonPtr);

	if (IntrStatusValue & XSM_IPIXR_EOS_MASK) {
		/*
		 * Set End of Conversion  interrupt flag so the code
		 * in application context can be aware of this interrupt.
		 */
		EosFlag = TRUE;
		XSysMon_GetStatus(SysMonPtr); /* Clear the latched status */
	}

	/*
	 * Clear all bits in Interrupt Status Register.
	 */
	XSysMon_IntrClear(SysMonPtr, IntrStatusValue);
}

/****************************************************************************/
/**
 *
 * This function sets up the interrupt system so interrupts can occur for the
 * System Monitor/ADC.  The function is application-specific since the actual
 * system may or may not have an interrupt controller. The System Monitor/ADC
 * device could be directly connected to a processor without an interrupt
 * controller. The user should modify this function to fit the application.
 *
 * @param	IntcInstPtr is a pointer to the Interrupt Controller driver
 *		Instance.
 * @param	SysMonPtr is a pointer to the driver instance for the System
 * 		Monitor device which is going to be connected to the interrupt
 *		controller.
 * @param	IntrId is XPAR_<INTC_instance>_<SYSMON_ADC_instance>_VEC_ID
 *		value from xparameters.h
 *
 * @return	XST_SUCCESS if successful, or XST_FAILURE.
 *
 * @note		None.
 *
 *
 ****************************************************************************/
static int SysMonSetupInterruptSystem(XIntc* IntcInstPtr, XSysMon *SysMonPtr,
		u16 IntrId) {
	int Status;

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 */
	Status = XIntc_Initialize(IntcInstPtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the handler that will be called when an interrupt
	 * for the device occurs, the handler defined above performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(IntcInstPtr, IntrId,
			(XInterruptHandler) SysMonInterruptHandler, SysMonPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts. Specify real mode so that the System
	 * Monitor/ACD device can cause interrupts through the interrupt
	 * controller.
	 */
	Status = XIntc_Start(IntcInstPtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the System Monitor/ADC device.
	 */
	XIntc_Enable(IntcInstPtr, IntrId);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XIntc_InterruptHandler, IntcInstPtr);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

void setEnvelopeFromPanel(Synth* synth, u32* AdcData, int releaseSelected) {

	u32 attack_ms = (((float) (AdcData[14] >> 8)) / 256.0) * 5000;
	u32 decay_ms = (((float) (AdcData[10] >> 8)) / 256.0) * 5000;

	float sus_level = ((float) (AdcData[8] >> 8)) / 256.0;


	u32 release_ms = 0;
	if (releaseSelected) {
		release_ms = decay_ms;
		decay_ms = 0;
	} else {
		 release_ms = 0;

	}
	u32 sustain_ms = 0;

	SYNTHMODULE1_amplifier_adsr_set_env(synth, attack_ms, decay_ms, sustain_ms,
			release_ms, sus_level);
	SYNTHMODULE1_amplifier_adsr_abort(synth);

}

void play_melody_check(XSysMon* SysMonInstPtr, Synth* synth, u32* AdcData) {
	const int melody[] = { 0, 2, 4, 5, 7, 9, 11 };

	SYNTHMODULE1_amplifier_adsr_bypass(synth);
	for (int i = 0; i < 7; i++) {
		SYNTHMODULE1_play_note(synth, melody[i], 3, 500);
		usleep(500000);
	}
	SYNTHMODULE1_amplifier_adsr_abort(synth);
	//while( XGpioPs_ReadPin(&GpioPS, PS_PUSH_BUTTON)){}
	sleep(1);

	for (int octave = 3; octave < 6; octave++) {
		for (int i = 0; i < 7; i++) {
			readFrontPanel(SysMonInstPtr, AdcData, &panelSwitches);
			SYNTHMODULE1_play_note(synth, melody[i], octave, 600);

			usleep(600000);
		}
	}
	sleep(1);
	for (int i = 0; i < 4; i++) {
		SYNTHMODULE1_play_note(synth, 0, 4, 500 * i);
		usleep(500000 * i + 1000000);

	}

	SYNTHMODULE1_amplifier_adsr_abort(synth);


}

