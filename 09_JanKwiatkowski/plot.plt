reset
set term png size 2500, 5000 enhanced font "Arial, 32" lw 2
set size square

set lmargin  1.5
set bmargin  1.5
set rmargin  1.5
set tmargin  1.0


set out "T_D2T_it.png"
set grid
set multiplot layout 5,2

set grid


#------------------------------------------------------------------------

set pm3d
set view map
unset surface

#------------------------------------------------------------------------
set title "T(x,y) it=100"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "100.dat" u 1:2:3 notitle


#------------------------------------------------------------------------
set title "{/Symbol \321}^2T(x,y) it=100"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "100.dat" u 1:2:4 notitle

#------------------------------------------------------------------------
set title "T(x,y) it=200"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "200.dat" u 1:2:3 notitle


#------------------------------------------------------------------------
set title "{/Symbol \321}^2T(x,y) it=200"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "200.dat" u 1:2:4 notitle

#------------------------------------------------------------------------
set title "T(x,y) it=500"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "500.dat" u 1:2:3 notitle


#------------------------------------------------------------------------
set title "{/Symbol \321}^2T(x,y) it=500"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "500.dat" u 1:2:4 notitle

#------------------------------------------------------------------------
#set cbrange[0:18]
set title "T(x,y) it=1000"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "1000.dat" u 1:2:3 notitle


#------------------------------------------------------------------------
set title "{/Symbol \321}^2T(x,y) it=1000"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "1000.dat" u 1:2:4 notitle

#------------------------------------------------------------------------
set title "T(x,y) it=2000"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "2000.dat" u 1:2:3 notitle


#------------------------------------------------------------------------
set title "{/Symbol \321}^2T(x,y) it=2000"
set xlabel "x"
set ylabel "y"

splot [1:39][1:39] "2000.dat" u 1:2:4 notitle

#------------------------------------------------------------------------

unset multiplot
reset

