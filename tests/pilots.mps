* CMPL - Free-MPS - Export
NAME pilots
* OBJNAME line_1
* OBJSENSE MAX
ROWS
 N line_1
 E line_2
 E line_3
 E line_4
 E line_5
 E line_6
 E line_7
 E line_8
 E line_9
 E line_10
 E line_11
 E line_12
 E line_13
 E line_14
 E line_15
 E line_16
 E line_17
 E line_18
 E line_19
COLUMNS
 x[S,A1] line_2 1 line_3 -1
 x[S,A2] line_2 1 line_4 -1
 x[S,A3] line_2 1 line_5 -1
 x[S,A4] line_2 1 line_6 -1
 x[S,A5] line_2 1 line_7 -1
 x[S,A6] line_2 1 line_8 -1
 x[S,A7] line_2 1 line_9 -1
 x[S,A8] line_2 1 line_10 -1
 x[A1,B1] line_3 1 line_11 -1
 x[A1,B2] line_3 1 line_12 -1
 x[A2,B1] line_4 1 line_11 -1
 x[A2,B2] line_4 1 line_12 -1
 x[A2,B6] line_4 1 line_13 -1
 x[A3,B2] line_5 1 line_12 -1
 x[A3,B6] line_5 1 line_13 -1
 x[A4,B3] line_6 1 line_14 -1
 x[A4,B8] line_6 1 line_15 -1
 x[A5,B6] line_7 1 line_13 -1
 x[A6,B4] line_8 1 line_16 -1
 x[A6,B5] line_8 1 line_17 -1
 x[A7,B1] line_9 1 line_11 -1
 x[A7,B2] line_9 1 line_12 -1
 x[A7,B6] line_9 1 line_13 -1
 x[A8,B1] line_10 1 line_11 -1
 x[A8,B6] line_10 1 line_13 -1
 x[A8,B7] line_10 1 line_18 -1
 x[B1,T] line_11 1 line_19 -1
 x[B2,T] line_12 1 line_19 -1
 x[B6,T] line_13 1 line_19 -1
 x[B3,T] line_14 1 line_19 -1
 x[B8,T] line_15 1 line_19 -1
 x[B4,T] line_16 1 line_19 -1
 x[B5,T] line_17 1 line_19 -1
 x[B7,T] line_18 1 line_19 -1
 f line_1 1 line_2 -1
 f line_19 1
RHS
BOUNDS
 UP BOUND x[S,A1] 1.000000
 UP BOUND x[S,A2] 1.000000
 UP BOUND x[S,A3] 1.000000
 UP BOUND x[S,A4] 1.000000
 UP BOUND x[S,A5] 1.000000
 UP BOUND x[S,A6] 1.000000
 UP BOUND x[S,A7] 1.000000
 UP BOUND x[S,A8] 1.000000
 UP BOUND x[A1,B1] 1.000000
 UP BOUND x[A1,B2] 1.000000
 UP BOUND x[A2,B1] 1.000000
 UP BOUND x[A2,B2] 1.000000
 UP BOUND x[A2,B6] 1.000000
 UP BOUND x[A3,B2] 1.000000
 UP BOUND x[A3,B6] 1.000000
 UP BOUND x[A4,B3] 1.000000
 UP BOUND x[A4,B8] 1.000000
 UP BOUND x[A5,B6] 1.000000
 UP BOUND x[A6,B4] 1.000000
 UP BOUND x[A6,B5] 1.000000
 UP BOUND x[A7,B1] 1.000000
 UP BOUND x[A7,B2] 1.000000
 UP BOUND x[A7,B6] 1.000000
 UP BOUND x[A8,B1] 1.000000
 UP BOUND x[A8,B6] 1.000000
 UP BOUND x[A8,B7] 1.000000
 UP BOUND x[B1,T] 1.000000
 UP BOUND x[B2,T] 1.000000
 UP BOUND x[B6,T] 1.000000
 UP BOUND x[B3,T] 1.000000
 UP BOUND x[B8,T] 1.000000
 UP BOUND x[B4,T] 1.000000
 UP BOUND x[B5,T] 1.000000
 UP BOUND x[B7,T] 1.000000
 PL BOUND f
ENDATA
