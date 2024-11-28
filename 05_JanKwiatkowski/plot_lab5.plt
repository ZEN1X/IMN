reset
set term png size 3200, 2400 enhanced font "Verdana, 20" lw 3
set size square

set lmargin  9.5
set bmargin  7.5
set rmargin  7.5
set tmargin  7.0

#MP_LEFT = 0.1
#MP_RIGHT = 0.95
#MP_BOTTOM = 0.1
#MP_TOP = 0.9
#MP_GAP = 0.05

set out "S_V_k.png"
set grid
set multiplot layout 2,3

set grid


###########################################################
set title 'S(it, k)'
set xlabel 'it'
set ylabel 'S(it)'

plot "Sk16.dat" u 1:2 w l t 'k=16 it=1-81',\
 	 "Sk8.dat" u 1:2 w l t 'k=8 it=82-214',\
 	 "Sk4.dat" u 1:2 w l t 'k=4 it=215-372',\
 	 "Sk2.dat" u 1:2 w l t 'k=2 it=373-511',\
 	 "Sk1.dat" u 1:2 w l lc rgb 'red' t 'k=1 it=512-517 '


###########################################################

set pm3d
unset surface
set view map
set palette defined (-1 "blue", 0 "white", 1 "red")
set size ratio -1

###########################################################
set title 'V_{k=16}(x,y)'
set xlabel 'x'
set ylabel 'y'

splot [0:25.6][0:25.6] "Vk16.dat" u 1:2:3 w l notitle
###########################################################
set title 'V_{k=8}(x,y)'
set xlabel 'x'
set ylabel 'y'

splot [0:25.6][0:25.6] "Vk8.dat" u 1:2:3 w l notitle
###########################################################
set title 'V_{k=4}(x,y)'
set xlabel 'x'
set ylabel 'y'

splot [0:25.6][0:25.6] "Vk4.dat" u 1:2:3 w l notitle
###########################################################
set title 'V_{k=2}(x,y)'
set xlabel 'x'
set ylabel 'y'

splot [0:25.6][0:25.6] "Vk2.dat" u 1:2:3 w l notitle
###########################################################
set title 'V_{k=1}(x,y)'
set xlabel 'x'
set ylabel 'y'

splot [0:25.6][0:25.6] "Vk1.dat" u 1:2:3 w l notitle
###########################################################

unset multiplot
reset

