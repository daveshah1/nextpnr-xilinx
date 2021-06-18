#!/usr/bin/env bash
set -ex
yosys -p "synth_xilinx -flatten -arch xcup -nobram -top top; write_json blinky_synth.json; write_verilog blinky_synth.v" top.v -l yosys.log
../../../nextpnr-xilinx --chipdb ../../xczu3eg.bin --json blinky_synth.json --xdc ultra96.xdc --write blinky_routed.json --log nextpnr.log
java -jar ../../../rapidwright_json2dcp.jar xczu3eg-sbva484-1-e blinky_routed.json blinky_routed.dcp | tee rapidwright.log
vivado -mode batch -nojournal -source blinky.tcl -log vivado.log
