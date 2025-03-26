;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"Dado_L1.c"
	list	p=12f683
	radix dec
	include "p12f683.inc"
;--------------------------------------------------------
; config word(s)
;--------------------------------------------------------
	__config 0x3ff7
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_state
	global	_handleLEDOutput
	global	_customDelayAndSeed
	global	_rotateLeft
	global	_generateRandomByte
	global	_generateDiceNumber

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0070
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_Dado_L1_0	udata
r0x101C	res	1
r0x1011	res	1
r0x1010	res	1
r0x1012	res	1
r0x1013	res	1
r0x1014	res	1
r0x1015	res	1
r0x1016	res	1
r0x1017	res	1
r0x1018	res	1
r0x1019	res	1
r0x101A	res	1
r0x101B	res	1
r0x1002	res	1
r0x1004	res	1
r0x1005	res	1
r0x1006	res	1
r0x1007	res	1
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x100E	res	1
r0x100F	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

IDD_Dado_L1_0	idata
_state
	db	0x00	; 0
	db	0xa3	; 163

;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_Dado_L1	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _generateDiceNumber
;   _handleLEDOutput
;   _customDelayAndSeed
;   _generateDiceNumber
;   _handleLEDOutput
;   _customDelayAndSeed
;4 compiler assigned registers:
;   r0x101D
;   STK00
;   r0x101E
;   r0x101F
;; Starting pCode block
S_Dado_L1__main	code
_main:
; 2 exit points
;	.line	31; "Dado_L1.c"	TRISIO = 0b00100000;
	MOVLW	0x20
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	32; "Dado_L1.c"	GPIO = 0x00;
	BANKSEL	_GPIO
	CLRF	_GPIO
_00109_DS_:
;	.line	39; "Dado_L1.c"	if (GP5) {
	BANKSEL	_GPIObits
	BTFSS	_GPIObits,5
	GOTO	_00106_DS_
;	.line	41; "Dado_L1.c"	diceResult = generateDiceNumber();
	PAGESEL	_generateDiceNumber
	CALL	_generateDiceNumber
	PAGESEL	$
;;1	MOVWF	r0x101D
	MOVF	STK00,W
;;1	MOVWF	r0x101E
;;1	MOVWF	r0x101F
;	.line	44; "Dado_L1.c"	handleLEDOutput(diceResult);
	PAGESEL	_handleLEDOutput
	CALL	_handleLEDOutput
	PAGESEL	$
;	.line	47; "Dado_L1.c"	customDelayAndSeed(waitTime);
	MOVLW	0x64
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_customDelayAndSeed
	CALL	_customDelayAndSeed
	PAGESEL	$
	GOTO	_00109_DS_
_00106_DS_:
;	.line	50; "Dado_L1.c"	GPIO = 0x00;
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00109_DS_
;	.line	53; "Dado_L1.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _generateRandomByte
;   _generateRandomByte
;4 compiler assigned registers:
;   r0x100D
;   r0x100E
;   r0x100F
;   STK00
;; Starting pCode block
S_Dado_L1__generateDiceNumber	code
_generateDiceNumber:
; 2 exit points
_00192_DS_:
;	.line	119; "Dado_L1.c"	randomVal = generateRandomByte();
	PAGESEL	_generateRandomByte
	CALL	_generateRandomByte
	PAGESEL	$
;;1	MOVWF	r0x100D
	BANKSEL	r0x100E
	MOVWF	r0x100E
	CLRF	r0x100F
;;swapping arguments (AOP_TYPEs 1/2)
;;signed compare: left >= lit(0x1=1), size=2, mask=ffff
	MOVLW	0x00
	ADDLW	0x80
	ADDLW	0x80
	BTFSS	STATUS,2
	GOTO	_00208_DS_
	MOVLW	0x01
	SUBWF	r0x100E,W
_00208_DS_:
	BTFSS	STATUS,0
	GOTO	_00192_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
;;signed compare: left < lit(0x7=7), size=2, mask=ffff
	BANKSEL	r0x100F
	MOVF	r0x100F,W
	ADDLW	0x80
	ADDLW	0x80
	BTFSS	STATUS,2
	GOTO	_00209_DS_
	MOVLW	0x07
	SUBWF	r0x100E,W
_00209_DS_:
	BTFSC	STATUS,0
	GOTO	_00192_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
;	.line	121; "Dado_L1.c"	return randomVal;
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	MOVWF	STK00
	MOVF	r0x100F,W
;	.line	124; "Dado_L1.c"	}
	RETURN	
; exit point of _generateDiceNumber

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _rotateLeft
;   _rotateLeft
;   _rotateLeft
;   _rotateLeft
;7 compiler assigned registers:
;   r0x1008
;   r0x1009
;   r0x100A
;   STK01
;   STK00
;   r0x100B
;   r0x100C
;; Starting pCode block
S_Dado_L1__generateRandomByte	code
_generateRandomByte:
; 2 exit points
;	.line	104; "Dado_L1.c"	uint8_t temp0 = state[0];
	BANKSEL	_state
	MOVF	(_state + 0),W
	BANKSEL	r0x1008
	MOVWF	r0x1008
;	.line	105; "Dado_L1.c"	uint8_t temp1 = state[1];
	BANKSEL	_state
	MOVF	(_state + 1),W
;	.line	106; "Dado_L1.c"	uint8_t result = temp0 + temp1;
	BANKSEL	r0x1009
	MOVWF	r0x1009
	ADDWF	r0x1008,W
	MOVWF	r0x100A
;	.line	108; "Dado_L1.c"	temp1 ^= temp0;
	MOVF	r0x1008,W
	XORWF	r0x1009,F
;	.line	109; "Dado_L1.c"	state[0] = rotateLeft(temp0, 6) ^ temp1 ^ (temp1 << 1);
	MOVLW	0x06
	MOVWF	STK01
	MOVLW	0x00
	MOVWF	STK00
	MOVF	r0x1008,W
	PAGESEL	_rotateLeft
	CALL	_rotateLeft
	PAGESEL	$
	BANKSEL	r0x1008
	MOVWF	r0x1008
	MOVF	r0x1009,W
	XORWF	r0x1008,F
	MOVF	r0x1009,W
	MOVWF	r0x100B
	BCF	STATUS,0
	RLF	r0x100B,W
;;1	MOVWF	r0x100C
	MOVWF	r0x100B
	XORWF	r0x1008,F
;;/home/sdcc-builder/build/sdcc-build/orig/sdcc/src/pic14/gen.c:6828: size=0, offset=0, AOP_TYPE(res)=8
	MOVF	r0x1008,W
	BANKSEL	_state
	MOVWF	(_state + 0)
;	.line	110; "Dado_L1.c"	state[1] = rotateLeft(temp1, 3);
	MOVLW	0x03
	MOVWF	STK01
	MOVLW	0x00
	MOVWF	STK00
	BANKSEL	r0x1009
	MOVF	r0x1009,W
	PAGESEL	_rotateLeft
	CALL	_rotateLeft
	PAGESEL	$
	BANKSEL	r0x1008
	MOVWF	r0x1008
	BANKSEL	_state
	MOVWF	(_state + 1)
;	.line	112; "Dado_L1.c"	return result;
	BANKSEL	r0x100A
	MOVF	r0x100A,W
;	.line	113; "Dado_L1.c"	}
	RETURN	
; exit point of _generateRandomByte

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;8 compiler assigned registers:
;   r0x1002
;   STK00
;   r0x1003
;   STK01
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;; Starting pCode block
S_Dado_L1__rotateLeft	code
_rotateLeft:
; 2 exit points
	BANKSEL	r0x1007
	MOVWF	r0x1007
	MOVWF	r0x1005
;	.line	98; "Dado_L1.c"	uint8_t rotateLeft(uint8_t value, int shift) {
	MOVWF	r0x1002
	MOVF	STK00,W
;;1	MOVWF	r0x1003
	MOVF	STK01,W
	MOVWF	r0x1006
	MOVWF	r0x1004
;;103	MOVF	r0x1002,W
;;104	MOVF	r0x1004,W
;;105	MOVF	r0x1005,W
;	.line	99; "Dado_L1.c"	return (value << shift) | (value >> (8 - shift));
	MOVF	r0x1006,W
	BTFSC	r0x1006,7
	GOTO	_00177_DS_
	SUBLW	0x00
	BTFSC	STATUS,2
	GOTO	_00175_DS_
_00174_DS_:
	BANKSEL	r0x1007
	RLF	r0x1007,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00174_DS_
	GOTO	_00175_DS_
_00177_DS_:
	BCF	STATUS,0
	BANKSEL	r0x1007
	BTFSC	r0x1007,7
	BSF	STATUS,0
	RRF	r0x1007,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00177_DS_
_00175_DS_:
	BANKSEL	r0x1004
	MOVF	r0x1004,W
	MOVWF	r0x1005
	SUBLW	0x08
	MOVWF	r0x1004
	MOVF	r0x1002,W
	MOVWF	r0x1005
	MOVF	r0x1004,W
	SUBLW	0x00
	BTFSC	STATUS,2
	GOTO	_00179_DS_
_00178_DS_:
	BANKSEL	r0x1005
	RRF	r0x1005,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00178_DS_
_00179_DS_:
	BANKSEL	r0x1007
	MOVF	r0x1007,W
	MOVWF	r0x1002
	IORWF	r0x1005,W
	MOVWF	r0x1007
;	.line	100; "Dado_L1.c"	}
	RETURN	
; exit point of _rotateLeft

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;13 compiler assigned registers:
;   r0x1010
;   STK00
;   r0x1011
;   r0x1012
;   r0x1013
;   r0x1014
;   r0x1015
;   r0x1016
;   r0x1017
;   r0x1018
;   r0x1019
;   r0x101A
;   r0x101B
;; Starting pCode block
S_Dado_L1__customDelayAndSeed	code
_customDelayAndSeed:
; 2 exit points
;	.line	87; "Dado_L1.c"	void customDelayAndSeed(uint16_t timeDelay) {
	BANKSEL	r0x1010
	MOVWF	r0x1010
	MOVF	STK00,W
	MOVWF	r0x1011
;	.line	88; "Dado_L1.c"	for (uint16_t i = 0; i < timeDelay; i++) {
	CLRF	r0x1012
	CLRF	r0x1013
_00144_DS_:
	BANKSEL	r0x1010
	MOVF	r0x1010,W
	SUBWF	r0x1013,W
	BTFSS	STATUS,2
	GOTO	_00166_DS_
	MOVF	r0x1011,W
	SUBWF	r0x1012,W
_00166_DS_:
	BTFSC	STATUS,0
	GOTO	_00146_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
;	.line	89; "Dado_L1.c"	for (uint16_t j = 0; j < 1275; j++) {
	BANKSEL	r0x1014
	CLRF	r0x1014
	CLRF	r0x1015
_00141_DS_:
	BANKSEL	r0x1014
	MOVF	r0x1014,W
	MOVWF	r0x1016
	MOVF	r0x1015,W
	MOVWF	r0x1017
;;unsigned compare: left < lit(0x4FB=1275), size=2
	MOVLW	0x04
	SUBWF	r0x1017,W
	BTFSS	STATUS,2
	GOTO	_00167_DS_
	MOVLW	0xfb
	SUBWF	r0x1016,W
_00167_DS_:
	BTFSC	STATUS,0
	GOTO	_00145_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
;	.line	91; "Dado_L1.c"	state[0] ^= (j & 0xFF);
	BANKSEL	_state
	MOVF	(_state + 0),W
	BANKSEL	r0x1019
	MOVWF	r0x1019
	MOVWF	r0x1018
	CLRF	r0x1017
;;102	MOVF	r0x1018,W
	CLRF	r0x101A
	MOVF	r0x1016,W
	XORWF	r0x1019,W
	MOVWF	r0x1018
	MOVLW	0x00
	XORWF	r0x101A,W
	MOVWF	r0x101B
	MOVF	r0x1018,W
	MOVWF	r0x1016
	BANKSEL	_state
	MOVWF	(_state + 0)
;	.line	92; "Dado_L1.c"	state[1] ^= (i & 0xFF);
	MOVF	(_state + 1),W
	BANKSEL	r0x1019
	MOVWF	r0x1019
	MOVWF	r0x1016
;;101	MOVF	r0x1012,W
	MOVF	r0x1013,W
	MOVWF	r0x1018
	CLRF	r0x1018
;;99	MOVF	r0x1016,W
	CLRF	r0x101A
;;100	MOVF	r0x1017,W
	MOVF	r0x1012,W
	MOVWF	r0x1017
	XORWF	r0x1019,W
	MOVWF	r0x1016
	MOVLW	0x00
	XORWF	r0x101A,W
	MOVWF	r0x101B
	MOVF	r0x1016,W
	MOVWF	r0x1017
	BANKSEL	_state
	MOVWF	(_state + 1)
;	.line	89; "Dado_L1.c"	for (uint16_t j = 0; j < 1275; j++) {
	BANKSEL	r0x1014
	INCF	r0x1014,F
	BTFSC	STATUS,2
	INCF	r0x1015,F
	GOTO	_00141_DS_
_00145_DS_:
;	.line	88; "Dado_L1.c"	for (uint16_t i = 0; i < timeDelay; i++) {
	BANKSEL	r0x1012
	INCF	r0x1012,F
	BTFSC	STATUS,2
	INCF	r0x1013,F
	GOTO	_00144_DS_
_00146_DS_:
;	.line	95; "Dado_L1.c"	}
	RETURN	
; exit point of _customDelayAndSeed

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _customDelayAndSeed
;   _customDelayAndSeed
;2 compiler assigned registers:
;   r0x101C
;   STK00
;; Starting pCode block
S_Dado_L1__handleLEDOutput	code
_handleLEDOutput:
; 2 exit points
;	.line	56; "Dado_L1.c"	void handleLEDOutput(uint8_t diceValue) {
	BANKSEL	r0x101C
	MOVWF	r0x101C
;;unsigned compare: left < lit(0x1=1), size=1
;	.line	58; "Dado_L1.c"	switch (diceValue) {
	MOVLW	0x01
	SUBWF	r0x101C,W
	BTFSS	STATUS,0
	GOTO	_00120_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x7=7), size=1
	MOVLW	0x07
	SUBWF	r0x101C,W
	BTFSC	STATUS,0
	GOTO	_00120_DS_
;;genSkipc:3307: created from rifx:0x7ffdc86207e0
	DECF	r0x101C,F
	MOVLW	HIGH(_00133_DS_)
	BANKSEL	PCLATH
	MOVWF	PCLATH
	MOVLW	_00133_DS_
	BANKSEL	r0x101C
	ADDWF	r0x101C,W
	BTFSS	STATUS,0
	GOTO	_00001_DS_
	BANKSEL	PCLATH
	INCF	PCLATH,F
_00001_DS_:
	MOVWF	PCL
_00133_DS_:
	GOTO	_00114_DS_
	GOTO	_00115_DS_
	GOTO	_00116_DS_
	GOTO	_00117_DS_
	GOTO	_00118_DS_
	GOTO	_00119_DS_
_00114_DS_:
;	.line	60; "Dado_L1.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	61; "Dado_L1.c"	break;
	GOTO	_00120_DS_
_00115_DS_:
;	.line	63; "Dado_L1.c"	GP0 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
;	.line	64; "Dado_L1.c"	break;
	GOTO	_00120_DS_
_00116_DS_:
;	.line	66; "Dado_L1.c"	GPIO = 0b00000011;
	MOVLW	0x03
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	67; "Dado_L1.c"	break;
	GOTO	_00120_DS_
_00117_DS_:
;	.line	69; "Dado_L1.c"	GPIO = 0b00000101;
	MOVLW	0x05
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	70; "Dado_L1.c"	break;
	GOTO	_00120_DS_
_00118_DS_:
;	.line	72; "Dado_L1.c"	GPIO = 0b00000111;
	MOVLW	0x07
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	73; "Dado_L1.c"	break;
	GOTO	_00120_DS_
_00119_DS_:
;	.line	75; "Dado_L1.c"	GPIO = 0b00010101;
	MOVLW	0x15
	BANKSEL	_GPIO
	MOVWF	_GPIO
_00120_DS_:
;	.line	80; "Dado_L1.c"	customDelayAndSeed(200);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_customDelayAndSeed
	CALL	_customDelayAndSeed
	PAGESEL	$
;	.line	81; "Dado_L1.c"	GPIO = 0x00;
	BANKSEL	_GPIO
	CLRF	_GPIO
;	.line	82; "Dado_L1.c"	GP0 = 0;
	BCF	_GPIObits,0
;	.line	83; "Dado_L1.c"	GP1 = 0;
	BCF	_GPIObits,1
;	.line	84; "Dado_L1.c"	}
	RETURN	
; exit point of _handleLEDOutput


;	code size estimation:
;	  234+   59 =   293 instructions (  704 byte)

	end
