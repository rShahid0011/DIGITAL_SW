*FDC3512 at Temp. Electrical Model
*-------------------------------------
.SUBCKT FDC3512 20 10 30 50
*20=DRAIN 10=GATE 30=SOURCE 50=VTEMP
Rg 10 11x 1
Rdu 12x 1 1u
M1 2 1 4x 4x DMOS L=1u W=1u
.MODEL DMOS NMOS(VTO=2.95 KP=2.3E+1
+THETA=.06667 VMAX=3E5 LEVEL=3)
Cgs 1 5x 500p
Rd 20 4 2.4E-2 
Dds 5x 4 DDS
.MODEL DDS D(M=6.5E-1 VJ=1.7 CJO=158p)
Dbody 5x 20 DBODY
.MODEL DBODY D(IS=4.52E-13 N=1.017211 RS=.00064 TT=34.35n)
Ra 4 2 2.4E-2 
Rs 5x 5 0.5m
Ls 5 30 0.5n
M2 1 8 6 6 INTER
E2 8 6 4 1 2
.MODEL INTER NMOS(VTO=0 KP=10 LEVEL=1)
Cgdmax 7 4 735p
Rcgd 7 4 10meg
Dgd 6 4 DGD
Rdgd 6 4 10meg
.MODEL DGD D(M=3.01E-1 VJ=4.27E-4 CJO=735p)
M3 7 9 1 1 INTER
E3 9 1 4 1 -2
*ZX SECTION
EOUT 4x 6x poly(2) (1x,0) (3x,0) 0 0 0 0 1
FCOPY 0 3x VSENSE 1
RIN 1x 0 1G
VSENSE 6x 5x 0
RREF 3x 0 10m
*TEMP SECTION
ED 101 0 VALUE {V(50,100)}
VAMB 100 0 25
EKP 1x 0 101 0 .04
*VTO TEMP SECTION
EVTO 102 0 101 0 .006
EVT 12x 11x 102 0 1
*DIODE THEMO BREAKDOWN SECTION
EBL VB1 VB2 101 0 .08
VBLK VB2 0 80
D 20 DB1 DBLK
.MODEL DBLK D(IS=1E-14 CJO=.1p RS=.1)
EDB DB1 0 VB1 0 1
.ENDS FDC3512
*FDC3512 (Rev.A) 2/4/04 **ST
