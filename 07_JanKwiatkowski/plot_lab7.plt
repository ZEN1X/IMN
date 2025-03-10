reset
set term png size 2400,3000 enhanced font "Verdana,24" lw 2

set lmargin 1.5
set bmargin 1.5
set rmargin 1.5
set tmargin 1.0

set out "maps.png"
set grid
set multiplot layout 5,2

# przeszkoda
set grid
unset grid
set object 1 rect from 0,0 to 0.5,0.55 fs solid noborder fc rgb "black" front

unset surface
set view map
set size ratio -1

###########################################################
set title 'Funkcja strumienia Psi(x,y) Q=-1000'
set xlabel 'x'
set ylabel 'y'
unset ztics
set contour base
set cntrparam level 50
set grid
set palette defined (-55 "blue", -50 "red")
splot [0:2][0:0.9] "wyn.dat" u 1:2:3 w l palette notitle

###########################################################
set title 'Funkcja wirowosci ksi(x,y) Q=-1000'
set xlabel 'x'
set ylabel 'y'
set grid
set palette defined (-200 "blue", 350 "red")
splot [0:2][0:0.9] "wyn.dat" u 1:2:4 w l palette notitle

###########################################################
unset pm3d
set pm3d
set pm3d depthorder
unset surface
unset contour

set title 'Pozioma skladowa predkosci u(x,y) Q=-1000'
set xlabel 'x'
set ylabel 'y'
set grid
set palette defined (-2 "blue", 16 "red")
splot [0:2][0:0.9] "wyn.dat" u 1:2:5 palette notitle

###########################################################
set palette defined (-6 "blue", 1 "red")
set title 'Pionowa skladowa predkosci v(x,y) Q=-1000'
set xlabel 'x'
set ylabel 'y'
splot [0:2][0:0.9] "wyn.dat" u 1:2:6 palette notitle

###########################################################
unset pm3d
set surface
unset surface
set palette rgbformulae 33,13,10
set view map
set size ratio -1

###########################################################
set title 'Funkcja strumienia Psi(x,y) Q=-4000'
set xlabel 'x'
set ylabel 'y'
unset ztics
set contour base
set cntrparam level 50
set palette defined (-219 "blue", -202 "red")
splot [0:2][0:0.9] "wyn_Qm4000.dat" u 1:2:3 w l palette notitle

###########################################################
set title 'Funkcja wirowosci ksi(x,y) Q=-4000'
set xlabel 'x'
set ylabel 'y'
set palette defined (-800 "blue", 1200 "red")
splot [0:2][0:0.9] "wyn_Qm4000.dat" u 1:2:4 w l palette notitle

###########################################################
unset pm3d
set pm3d
unset surface
unset contour
set palette defined (-10 "blue", 70 "red")

set title 'Pozioma skladowa predkosci u(x,y) Q=-4000'
set xlabel 'x'
set ylabel 'y'
splot [0:2][0:0.9] "wyn_Qm4000.dat" u 1:2:5 notitle

###########################################################
set palette defined (-14 "blue", 4 "red")
set title 'Pionowa skladowa predkosci v(x,y) Q=-4000'
set xlabel 'x'
set ylabel 'y'
splot [0:2][0:0.9] "wyn_Qm4000.dat" u 1:2:6 notitle

###########################################################
unset pm3d
set palette rgbformulae 33,13,10

set title 'Funkcja strumienia Psi(x,y) Q=4000'
set xlabel 'x'
set ylabel 'y'
unset ztics
set contour base
set cntrparam level 50
set palette defined (200 "blue", 220 "red")
splot [0:2][0:0.9] "wyn_Q4000.dat" u 1:2:3 w l palette notitle

unset multiplot
reset
