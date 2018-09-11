onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iCLOCK
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iRESET
add wave -noupdate -divider Master
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iMST_ADDRESS
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iMST_WRITE
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iMST_READ
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iMST_WRITE_DATA
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/oMST_READ_DATA
add wave -noupdate -divider Slave
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iSLV_ADDRESS
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iSLV_WRITE
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iSLV_READ
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iSLV_WRITE_DATA
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/oSLV_READ_DATA
add wave -noupdate -divider handshake
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/oMST_AK
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/iMST_RQ
add wave -noupdate -divider internals
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/rRD_PTR
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/rWR_PTR
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/rRESYNC_MST_RQ
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/rPERSIST_MST_RQ
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/rRET_AVAIL
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/wRD_USED
add wave -noupdate -radix hexadecimal /mailbox_tb/mbox/wWR_FREE
add wave -noupdate -divider memory
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[772]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[771]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[770]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[769]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[768]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[767]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[1]}
add wave -noupdate -radix hexadecimal {/mailbox_tb/mbox/rDPRAM[0]}
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {21 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 349
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
WaveRestoreZoom {0 ps} {56 ps}
