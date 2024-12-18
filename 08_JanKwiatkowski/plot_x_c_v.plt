reset
set term png size 1600,1200 enhanced font "Arial,16" lw 2

set output "ct_xt_vx_vy.png"
set grid
set multiplot layout 2,2

#------------------------------------------------------------------------
set key left bottom
set title 'srednie polozenie x(t)'
set xlabel 't'
set ylabel 'x_{s}'
plot 'ct_d0.dat' u 1:3 w l lt -1 lw 2 t 'x_{sr}(D=0)', \
     'ct_d01.dat' u 1:3 w l lt 7 lw 2 t 'x_{sr}(D=0.1)'

#------------------------------------------------------------------------
set title 'calka gestosci c(t)'
set xlabel 't'
set ylabel 'c(t)'
plot 'ct_d0.dat' u 1:2 w l lt 4 lw 2 t 'c(D=0)', \
     'ct_d01.dat' u 1:2 w l lt 6 lw 2 t 'c(D=0.1)'

#------------------------------------------------------------------------
set pm3d
set view map
unset surface
set palette rgbformulae 22,13,-31

#------------------------------------------------------------------------
# v_x(x,y)
set cbrange[-5:45]
set title 'v_x(x,y)'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "v.dat" u 1:2:3 notitle

#------------------------------------------------------------------------
# v_y(x,y)
set cbrange[-20:20]
set title 'v_y(x,y)'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "v.dat" u 1:2:4 notitle

unset multiplot
reset
