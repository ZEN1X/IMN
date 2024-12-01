reset
set term png size 3200, 2400 enhanced font "Verdana, 20" lw 3
set size square

set lmargin  7.5
set bmargin  7.5
set rmargin  7.5
set tmargin  7.0

set out "V.png"
set grid
set multiplot layout 2,3

###########################################################

set pm3d
unset surface
set view map

set palette defined (-10 'blue',0 'white',10 'red')

set xlabel 'x'
set ylabel 'y'

###########################################################
set title 'V(x,y)   nx=ny=50'
splot 'v50.dat' u 1:2:3 w pm3d notitle
###########################################################
set title 'V(x,y)   nx=ny=100'
splot 'v100.dat' u 1:2:3 w pm3d notitle
###########################################################
set title 'V(x,y)   nx=ny=200'
splot 'v200.dat' u 1:2:3 w pm3d notitle
###########################################################

set cbrange [-0.8:0.8]

set title 'V(x,y)   nx=ny=100, e1=e2=1'
splot 'g_e11.dat' u 1:2:3 w pm3d notitle


set title 'V(x,y)   nx=ny=100, e1=1, e2=2'
splot 'g_e12.dat' u 1:2:3 w pm3d notitle

set title 'V(x,y)   nx=ny=100, e1=1, e2=10'
splot 'g_e110.dat' u 1:2:3 w pm3d notitle

unset multiplot
reset

