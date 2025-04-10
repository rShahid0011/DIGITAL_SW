Version 4
SymbolType CELL
LINE Normal -32 36 32 36
LINE Normal -32 60 32 60
LINE Normal 0 96 0 76
LINE Normal 0 16 0 36
LINE Normal -20 24 -12 24
LINE Normal -16 20 -16 28
RECTANGLE Normal -16 44 16 52
RECTANGLE Normal -16 68 16 76
WINDOW 0 52 -7 Left 0
WINDOW 3 24 104 Left 0
SYMATTR Value LEADACID
SYMATTR Prefix X
SYMATTR Description Lead Acid Battery, either DC, AC, PULSE, SINE, PWL, EXP, or SFFM
SYMATTR ModelFile leadacid.sub
SYMATTR SpiceModel leadacid
SYMATTR SpiceLine CAPACITY=1.3 RESISTANCE=0.06 CELLS=3 INITSOC=1
PIN 0 16 NONE 0
PINATTR PinName +OUTPUT
PINATTR SpiceOrder 1
PIN 0 96 NONE 0
PINATTR PinName -OUTPUT
PINATTR SpiceOrder 2
PIN 32 16 NONE 8
PINATTR PinName SOC
PINATTR SpiceOrder 3
PIN 32 80 NONE 8
PINATTR PinName RATE
PINATTR SpiceOrder 4
