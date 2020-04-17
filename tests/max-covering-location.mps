* CMPL - Free-MPS - Export
NAME max-covering-location
* OBJNAME line_1
* OBJSENSE MAX
ROWS
 N line_1
 G line_2
 G line_3
 G line_4
 G line_5
 G line_6
 G line_7
 L line_8
COLUMNS
 GVANF 'MARKER' 'INTORG'
 x[S1] line_2 1 line_4 1
 x[S1] line_8 1
 x[S2] line_3 1 line_8 1
 x[S3] line_2 1 line_4 1
 x[S3] line_5 1 line_8 1
 x[S4] line_4 1 line_5 1
 x[S4] line_8 1
 x[S5] line_6 1 line_7 1
 x[S5] line_8 1
 x[S6] line_6 1 line_7 1
 x[S6] line_8 1
 y[K1] line_1 5.000000 line_2 -1
 y[K2] line_1 10.000000 line_3 -1
 y[K3] line_1 8.000000 line_4 -1
 y[K4] line_1 12.000000 line_5 -1
 y[K5] line_1 20.000000 line_6 -1
 y[K6] line_1 15.000000 line_7 -1
 GVEND 'MARKER' 'INTEND'
RHS
 RHS line_8 2.000000
BOUNDS
 UP BOUND x[S1] 1
 UP BOUND x[S2] 1
 UP BOUND x[S3] 1
 UP BOUND x[S4] 1
 UP BOUND x[S5] 1
 UP BOUND x[S6] 1
 UP BOUND y[K1] 1
 UP BOUND y[K2] 1
 UP BOUND y[K3] 1
 UP BOUND y[K4] 1
 UP BOUND y[K5] 1
 UP BOUND y[K6] 1
ENDATA
