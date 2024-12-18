reset
set term png size 2600,3000 enhanced font "Arial,24" lw 2

set output "u_t_D0_D01.png"
set grid
set multiplot layout 5,2

set pm3d
set view map
unset surface
set palette rgbformulae 22,13,-31

# Row 1: it=2000, t=0.1139
set cbrange[0:18]
set title 'D=0 u(x,y) it=2000, t=0.1139'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d0.dat" u 1:2:3 i 1 notitle

set cbrange[0:4]
set title 'D=0.1 u(x,y) it=2000, t=0.1139'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d01.dat" u 1:2:3 i 1 notitle

# Row 2: it=4000, t=0.2279
set cbrange[0:18]
set title 'D=0 u(x,y) it=4000, t=0.2279'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d0.dat" u 1:2:3 i 3 notitle

set cbrange[0:2.5]
set title 'D=0.1 u(x,y) it=4000, t=0.2279'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d01.dat" u 1:2:3 i 3 notitle

# Row 3: it=6000, t=0.3418
set cbrange[0:18]
set title 'D=0 u(x,y) it=6000, t=0.3418'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d0.dat" u 1:2:3 i 5 notitle

set cbrange[0:1.2]
set title 'D=0.1 u(x,y) it=6000, t=0.3418'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d01.dat" u 1:2:3 i 5 notitle

# Row 4: it=8000, t=0.4558
set cbrange[0:18]
set title 'D=0 u(x,y) it=8000, t=0.4558'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d0.dat" u 1:2:3 i 7 notitle

set cbrange[0:0.8]
set title 'D=0.1 u(x,y) it=8000, t=0.4558'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d01.dat" u 1:2:3 i 7 notitle

# Row 5: it=10000, t=0.5697
set cbrange[0:18]
set title 'D=0 u(x,y) it=10000, t=0.5697'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d0.dat" u 1:2:3 i 9 notitle

set cbrange[0:0.5]
set title 'D=0.1 u(x,y) it=10000, t=0.5697'
set xlabel 'x'
set ylabel 'y'
splot [0:400][0:90] "u_t_d01.dat" u 1:2:3 i 9 notitle

unset multiplot
reset
