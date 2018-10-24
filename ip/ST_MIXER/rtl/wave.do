onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iCLK
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iRESET
add wave -noupdate -divider regs
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iAV_ADDRESS
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iAV_WRITE_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iAV_WRITE
add wave -noupdate -divider bg
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iBG_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iBG_VALID
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oBG_READY
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iBG_SOP
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iBG_EOP
add wave -noupdate -divider fg
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iFG_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iFG_ALPHA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iFG_VALID
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oFG_READY
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iFG_SOP
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iFG_EOP
add wave -noupdate -divider out
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oOUT_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oOUT_VALID
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/iOUT_READY
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oOUT_SOP
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/oOUT_EOP
add wave -noupdate -divider states
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rFG_STATE
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rBG_STATE
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rFG_ENABLED
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_FG_ENABLED
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rVPFLAG
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rHCNT_BG
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_BG_HRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_FG_HRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rBG_HRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rHEND
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_HPOS
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rHPOS
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rVCNT_BG
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_BG_VRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_FG_VRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rBG_VRES
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rVEND
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rREG_VPOS
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rVPOS
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rFG_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rBG_DATA
add wave -noupdate -radix hexadecimal /ST_MIXER_TB/dut/rBG_EOP
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {47 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 410
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {736 ps}
