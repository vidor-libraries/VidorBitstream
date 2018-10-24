vlog -reportprogress 300 -work work C:/rcs/vidorstuff/t/vidor_s/ip/MAILBOX/rtl/MAILBOX.sv
vlog -reportprogress 300 -work work C:/rcs/vidorstuff/t/vidor_s/ip/MAILBOX/rtl/mailbox_tb.sv
vsim -gui work.mailbox_tb
do wave.do
run 150 ps