
puts "INFO: The number of arguments passed was: $argc."

if { $argc != 3 } {
  puts "ERROR: The following parameters must be passed to this script in the following sequence:"
#              $argv 0            $argv 1                    $argv 2
  puts "       vitisWorkspacePath platformXsaPathAndFilename platformProjectName"
  gets stdin
  exit
}

set vitisWorkspacePath [lindex $argv 0]
puts "INFO: vitisWorkspacePath=$vitisWorkspacePath"

set platformXsaPathAndFilename [lindex $argv 1]
puts "INFO: platformXsaPathAndFilename=$platformXsaPathAndFilename"

set platformProjectName [lindex $argv 2]
puts "INFO: platformProjectName=$platformProjectName"

set targetHardwareDescriptionFile [file normalize $platformXsaPathAndFilename]
set targetProcessor "psu_cortexr5_0"
set os "standalone"

puts "INFO: targetProcessor=$targetProcessor, os=$os, targetHardwareDescriptionFile=$targetHardwareDescriptionFile"

puts "INFO: Setting Workspace folder to $vitisWorkspacePath"
setws "$vitisWorkspacePath"
cd "$vitisWorkspacePath"

platform list

puts "INFO: platform create..."
# NB: the -fsbl-target option passed to the platform create command is not listed in the help documentation
#     that option is however valid for the platform config command, added below but must be done after the platform active command
platform create -name $platformProjectName -hw $targetHardwareDescriptionFile -proc $targetProcessor -os $os -fsbl-target $targetProcessor

puts "INFO: platform write to $vitisWorkspacePath/$platformProjectName/platform.spr"
platform write

puts "INFO: platform generate..."
# NB: the -domains option passed to the platform generate command is supposed to be followed by one or more comma separated domain names,
#     without the -domains option, all relevant domains are built
#     consider removing the superfluous -domains option
platform generate -domains

puts "INFO: platform active $platformProjectName..."
platform active $platformProjectName

puts "INFO: platform config -fsbl-target $targetProcessor"
platform config -fsbl-target $targetProcessor

puts "INFO: platform report"
platform report

puts "INFO: setting bsp config options for the lwip library..."
# The following configuration settings are all set for the lwip library below
bsp setlib -name lwip211 -ver 1.3
bsp config tcp_queue_ooseq "0"
bsp config lwip_tcp "false"
bsp config memp_n_udp_pcb "32"
bsp config lwip_stats "true"
bsp config mem_size "524288"
bsp config memp_n_pbuf "1024"
bsp config memp_num_netbuf "4096"
bsp config default_udp_recvmbox_size "4096"
bsp config tcpip_mbox_size "4096"
bsp config pbuf_pool_size "20480"
bsp config n_rx_descriptors "512"

puts "INFO: setting bsp config compiler_flags..."
# The following configuration settings are platform project compilation settings
bsp config compiler_flags "-O0 -c -mcpu=cortex-r5"

puts "INFO: bsp write..."
bsp write

puts "INFO: bsp reload..."
# Forces a reload using the now saved settings
bsp reload

puts "INFO: bsp regenerate..."
# Forces platform project source regeneration using the reloaded settings
bsp regenerate
