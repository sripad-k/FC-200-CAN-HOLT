# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\SripadKulkarni\workspace\FC-200-CAN-HOLT\can_holt_system\_ide\scripts\fc-1.tcl
# 
# 
# Usage with xsct:
# In an external shell use the below command and launch symbol server.
# symbol_server.bat -S -s tcp::1534
# To debug using xsct, launch xsct and run below command
# source C:\Users\SripadKulkarni\workspace\FC-200-CAN-HOLT\can_holt_system\_ide\scripts\fc-1.tcl
# 
connect -path [list tcp::1534 tcp:10.0.0.2:3121]
source C:/Xilinx/Vitis/2020.2/scripts/vitis/util/zynqmp_utils.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
rst -system
after 3000
targets -set -nocase -filter {name =~"RPU*" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
enable_split_mode
targets -set -filter {jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && level==0 && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-14711093-0"}
fpga -file C:/Users/SripadKulkarni/workspace/FC-200-CAN-HOLT/can_holt/_ide/bitstream/fcu_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
loadhw -hw C:/Users/SripadKulkarni/workspace/FC-200-CAN-HOLT/136T-2200-113050-001-F11-02/export/136T-2200-113050-001-F11-02/hw/136T-2200-113050-001-F11-02.xsa -mem-ranges [list {0x80000000 0xbfffffff} {0x400000000 0x5ffffffff} {0x1000000000 0x7fffffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
set mode [expr [mrd -value 0xFF5E0200] & 0xf]
targets -set -nocase -filter {name =~ "*R5*#0" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
rst -processor
dow C:/Users/SripadKulkarni/workspace/FC-200-CAN-HOLT/136T-2200-113050-001-F11-02/export/136T-2200-113050-001-F11-02/sw/136T-2200-113050-001-F11-02/boot/fsbl.elf
set bp_27_31_fsbl_bp [bpadd -addr &XFsbl_Exit]
con -block -timeout 60
bpremove $bp_27_31_fsbl_bp
targets -set -nocase -filter {name =~ "*R5*#0" && jtag_cable_name =~ "JTAG Cable 2020.2 AAo1HToQ0" && jtag_device_ctx=="jsn-XSC0-AAo1HToQ0-5ba00477-0"}
rst -processor
dow C:/Users/SripadKulkarni/workspace/FC-200-CAN-HOLT/can_holt/Debug/can_holt.elf
configparams force-mem-access 0
bpadd -addr &main
