reset
set terminal pngcairo size 2500,5000 enhanced font "Arial,32" lw 2
set output "T_D2T_it.png"

set size square
set grid
set pm3d
set view map
unset surface

set multiplot layout 5,2

# list of iterations
iterations = "100 200 500 1000 2000"

do for [it in iterations] {
    # T(x,y)
    set title sprintf("T(x,y) it=%s", it)
    set xlabel "x"
    set ylabel "y"
    splot [1:39][1:39] sprintf("%s.dat", it) u 1:2:3 notitle

    # Δ²T(x,y)
    set title sprintf("Δ²T(x,y) it=%s", it)
    set xlabel "x"
    set ylabel "y"
    splot [1:39][1:39] sprintf("%s.dat", it) u 1:2:4 notitle
}

unset multiplot
reset
