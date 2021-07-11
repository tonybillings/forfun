set projDir "{FPGA_DIR}/planAhead"
set projName "FPGA_CPU"
set topName top_module
set device xc6slx9-2tqg144
if {[file exists "$projDir/$projName"]} { file delete -force "$projDir/$projName" }
create_project $projName "$projDir/$projName" -part $device
set_property design_mode RTL [get_filesets sources_1]
set verilogSources [list "{FPGA_DIR}/verilog/globals.txt" "{FPGA_DIR}/verilog/top_module.v" "{FPGA_DIR}/verilog/custom_clock.v" "{FPGA_DIR}/verilog/reset_conditioner.v" "{FPGA_DIR}/verilog/processor.v" "{FPGA_DIR}/verilog/timers.v" "{FPGA_DIR}/verilog/synchronizer.v" "{FPGA_DIR}/verilog/block_memory.v" "{FPGA_DIR}/verilog/block_memory.ngc" "{FPGA_DIR}/verilog/core.v" "{FPGA_DIR}/verilog/prog_roms.v" "{FPGA_DIR}/verilog/segment_display_rom.v" "{FPGA_DIR}/verilog/segment_display_decoder.v"]
import_files -fileset [get_filesets sources_1] -force -norecurse $verilogSources
set ucfSources [list "{FPGA_DIR}/constraints.ucf"]
import_files -fileset [get_filesets constrs_1] -force -norecurse $ucfSources
set_property -name {steps.bitgen.args.More Options} -value {-g Binary:Yes -g Compress} -objects [get_runs impl_1]
set_property steps.map.args.mt on [get_runs impl_1]
set_property steps.map.args.pr b [get_runs impl_1]
set_property steps.par.args.mt on [get_runs impl_1]
update_compile_order -fileset sources_1
launch_runs -runs synth_1
wait_on_run synth_1
launch_runs -runs impl_1
wait_on_run impl_1
launch_runs impl_1 -to_step Bitgen
wait_on_run impl_1
