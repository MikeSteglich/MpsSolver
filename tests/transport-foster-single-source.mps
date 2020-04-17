* CMPL - Free-MPS - Export
NAME transport-foster-single-source
* OBJNAME cost
* OBJSENSE MIN
ROWS
 N cost
 L supplies[1]
 L supplies[2]
 L supplies[3]
 E demands[1]
 E demands[2]
 E demands[3]
 E demands[4]
COLUMNS
 GVANF 'MARKER' 'INTORG'
 y[1,1] cost 18000 supplies[1] 6000
 y[1,1] demands[1] 1
 y[1,2] cost 8000 supplies[1] 4000
 y[1,2] demands[2] 1
 y[1,3] cost 14000 supplies[1] 2000
 y[1,3] demands[3] 1
 y[1,4] cost 9000 supplies[1] 1500
 y[1,4] demands[4] 1
 y[2,1] cost 42000 supplies[2] 6000
 y[2,1] demands[1] 1
 y[2,2] cost 20000 supplies[2] 4000
 y[2,2] demands[2] 1
 y[2,3] cost 4000 supplies[2] 2000
 y[2,3] demands[3] 1
 y[2,4] cost 4500 supplies[2] 1500
 y[2,4] demands[4] 1
 y[3,1] cost 12000 supplies[3] 6000
 y[3,1] demands[1] 1
 y[3,2] cost 20000 supplies[3] 4000
 y[3,2] demands[2] 1
 y[3,3] cost 8000 supplies[3] 2000
 y[3,3] demands[3] 1
 y[3,4] cost 7500 supplies[3] 1500
 y[3,4] demands[4] 1
 GVEND 'MARKER' 'INTEND'
RHS
 RHS supplies[1] 5000 supplies[2] 6000
 RHS supplies[3] 5000 demands[1] 1
 RHS demands[2] 1 demands[3] 1
 RHS demands[4] 1
BOUNDS
 UP BOUND y[1,1] 1
 UP BOUND y[1,2] 1
 UP BOUND y[1,3] 1
 UP BOUND y[1,4] 1
 UP BOUND y[2,1] 1
 UP BOUND y[2,2] 1
 UP BOUND y[2,3] 1
 UP BOUND y[2,4] 1
 UP BOUND y[3,1] 1
 UP BOUND y[3,2] 1
 UP BOUND y[3,3] 1
 UP BOUND y[3,4] 1
ENDATA
