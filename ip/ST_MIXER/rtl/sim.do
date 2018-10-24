vlog -reportprogress 300 -work work C:/rcs/vidorstuff/t/vidor_s/ip/ST_MIXER/rtl/ST_MIXER.sv
vlog -reportprogress 300 -work work C:/rcs/vidorstuff/t/vidor_s/ip/ST_MIXER/rtl/ST_MIXER_TB.sv
vsim -gui work.ST_MIXER_TB
do wave.do
run 1.400 us
