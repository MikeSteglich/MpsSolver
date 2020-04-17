* CMPL - Free-MPS - Export
NAME prodmix-fc-threshold
* OBJNAME gdb
* OBJSENSE MAX
ROWS
 N gdb
 L maschine[1]
 L maschine[2]
 L y_x[1]
 L y_x[2]
 G __y_x_a[1,2]
 G __y_x_a[2,2]
COLUMNS
 GVANF 'MARKER' 'INTORG'
 x[1] gdb 135 maschine[1] 15
 x[1] maschine[2] 25 y_x[1] -1
 x[1] __y_x_a[1,2] -1
 x[2] gdb 140 maschine[1] 25
 x[2] maschine[2] 30 y_x[2] -1
 x[2] __y_x_a[2,2] -1
 y[1] gdb -200 y_x[1] 30
 y[1] __y_x_a[1,2] 50
 y[2] gdb -300 y_x[2] 30
 y[2] __y_x_a[2,2] 60
 GVEND 'MARKER' 'INTEND'
RHS
 RHS maschine[1] 2000 maschine[2] 2000
BOUNDS
 PL BOUND x[1]
 PL BOUND x[2]
 UP BOUND y[1] 1
 UP BOUND y[2] 1
ENDATA
