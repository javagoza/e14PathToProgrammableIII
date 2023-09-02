# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\training\ZynqSoundSynthesizer\Software\MiniZedSynth_system\_ide\scripts\debugger_minizedsynth-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\training\ZynqSoundSynthesizer\Software\MiniZedSynth_system\_ide\scripts\debugger_minizedsynth-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Avnet MiniZed V1 1234-oj1A" && level==0 && jtag_device_ctx=="jsn-MiniZed V1-1234-oj1A-13723093-0"}
fpga -file C:/training/ZynqSoundSynthesizer/Software/MiniZedSynth/_ide/bitstream/ZynqSoundSynth_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/training/ZynqSoundSynthesizer/Software/ZynqSynthPlatform/export/ZynqSynthPlatform/hw/ZynqSoundSynth_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/training/ZynqSoundSynthesizer/Software/MiniZedSynth/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/training/ZynqSoundSynthesizer/Software/MiniZedSynth/Debug/MiniZedSynth.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
