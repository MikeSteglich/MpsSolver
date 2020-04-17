* CMPL - Free-MPS - Export
NAME prodmix-verfahren
* OBJNAME gdb
* OBJSENSE MAX
ROWS
 N gdb
 L line_2
 L line_3
 L line_4
 L line_5
 E sos1
 G line_7
 G line_8
 G line_9
 G line_10
 G line_11
 G line_12
COLUMNS
 GVANF 'MARKER' 'INTORG'
 x[1,1] gdb 150 line_2 15
 x[1,1] line_3 25 line_7 -1
 x[1,2] gdb 145 line_4 10
 x[1,2] line_5 24 line_10 -1
 x[2,1] gdb 120 line_2 25
 x[2,1] line_3 30 line_8 -1
 x[2,2] gdb 135 line_4 22
 x[2,2] line_5 28 line_11 -1
 x[3,1] gdb 165 line_2 28
 x[3,1] line_3 32 line_9 -1
 x[3,2] gdb 166 line_4 25
 x[3,2] line_5 34 line_12 -1
 y[1] line_2 -2000 line_3 -2000
 y[1] sos1 1 line_7 50
 y[1] line_8 60 line_9 35
 y[2] line_4 -1800 line_5 -2200
 y[2] sos1 1 line_10 50
 y[2] line_11 60 line_12 35
 GVEND 'MARKER' 'INTEND'
RHS
 RHS sos1 1
BOUNDS
 PL BOUND x[1,1]
 PL BOUND x[1,2]
 PL BOUND x[2,1]
 PL BOUND x[2,2]
 PL BOUND x[3,1]
 PL BOUND x[3,2]
 UP BOUND y[1] 1
 UP BOUND y[2] 1
ENDATA
