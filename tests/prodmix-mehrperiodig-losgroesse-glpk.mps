* CMPL - Free-MPS - Export
NAME prodmix-mehrperiodig-losgroesse
* OBJNAME gdb
* OBJSENSE MAX
ROWS
 N gdb
 L line_2
 L line_3
 L line_4
 L line_5
 L line_6
 L line_7
 E mBilanz[1,1]
 G x_y[1,1]
 E mBilanz[1,2]
 G x_y[1,2]
 E mBilanz[1,3]
 G x_y[1,3]
 E mBilanz[2,1]
 G x_y[2,1]
 E mBilanz[2,2]
 G x_y[2,2]
 E mBilanz[2,3]
 G x_y[2,3]
COLUMNS
 x[1,1] gdb -100 line_2 15
 x[1,1] line_5 25 mBilanz[1,1] 1
 x[1,1] x_y[1,1] -1
 x[1,2] gdb -97 line_3 14
 x[1,2] line_6 22 mBilanz[1,2] 1
 x[1,2] x_y[1,2] -1
 x[1,3] gdb -95 line_4 13
 x[1,3] line_7 20 mBilanz[1,3] 1
 x[1,3] x_y[1,3] -1
 x[2,1] gdb -90 line_2 25
 x[2,1] line_5 30 mBilanz[2,1] 1
 x[2,1] x_y[2,1] -1
 x[2,2] gdb -89 line_3 25
 x[2,2] line_6 28 mBilanz[2,2] 1
 x[2,2] x_y[2,2] -1
 x[2,3] gdb -88 line_4 25
 x[2,3] line_7 27 mBilanz[2,3] 1
 x[2,3] x_y[2,3] -1
 GVANF 'MARKER' 'INTORG'
 xa[1,1] gdb 235 mBilanz[1,1] -1
 q[1,1] gdb -10 mBilanz[1,1] -1
 q[1,1] mBilanz[1,2] 1
 xa[1,2] gdb 235 mBilanz[1,2] -1
 q[1,2] gdb -10 mBilanz[1,2] -1
 q[1,2] mBilanz[1,3] 1
 xa[1,3] gdb 234 mBilanz[1,3] -1
 q[1,3] gdb -10 mBilanz[1,3] -1
 xa[2,1] gdb 255 mBilanz[2,1] -1
 q[2,1] gdb -14 mBilanz[2,1] -1
 q[2,1] mBilanz[2,2] 1
 xa[2,2] gdb 253 mBilanz[2,2] -1
 q[2,2] gdb -14 mBilanz[2,2] -1
 q[2,2] mBilanz[2,3] 1
 xa[2,3] gdb 250 mBilanz[2,3] -1
 q[2,3] gdb -14 mBilanz[2,3] -1
 y[1,1] gdb -500 x_y[1,1] 1000
 y[1,2] gdb -490 x_y[1,2] 1000
 y[1,3] gdb -480 x_y[1,3] 1000
 y[2,1] gdb -450 x_y[2,1] 1000
 y[2,2] gdb -450 x_y[2,2] 1000
 y[2,3] gdb -450 x_y[2,3] 1000
 GVEND 'MARKER' 'INTEND'
RHS
 RHS line_2 2500 line_3 1800
 RHS line_4 2200 line_5 2500
 RHS line_6 3000 line_7 2000
BOUNDS
 UP BOUND xa[1,1] 40
 UP BOUND q[1,1] 80
 UP BOUND xa[1,2] 50
 UP BOUND q[1,2] 80
 UP BOUND xa[1,3] 50
 UP BOUND q[1,3] 80
 UP BOUND xa[2,1] 40
 UP BOUND q[2,1] 100
 LO BOUND xa[2,2] 10
 UP BOUND xa[2,2] 70
 UP BOUND q[2,2] 100
 LO BOUND xa[2,3] 20
 UP BOUND xa[2,3] 80
 UP BOUND q[2,3] 100
 PL BOUND x[1,1]
 PL BOUND x[1,2]
 PL BOUND x[1,3]
 PL BOUND x[2,1]
 PL BOUND x[2,2]
 PL BOUND x[2,3]
 UP BOUND y[1,1] 1
 UP BOUND y[1,2] 1
 UP BOUND y[1,3] 1
 UP BOUND y[2,1] 1
 UP BOUND y[2,2] 1
 UP BOUND y[2,3] 1
ENDATA
