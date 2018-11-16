set xtics rotate
set xtics noenhanced
set style histogram rowstacked
set style data histogram
set style fill solid border -1
plot fn using 2:xtic(1) ti "code" lc rgb "#0000ff", '' u 3 ti "data" lc rgb "#ffff00", '' u 4 ti "stack" lc rgb "#ff0000"
