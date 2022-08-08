   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.12.6 - 16 Dec 2021
   3                     ; Generator (Limited) V4.5.4 - 16 Dec 2021
  44                     ; 26 static void Bluetooth_Init(void)
  44                     ; 27 {
  46                     	switch	.text
  47  0000               L3_Bluetooth_Init:
  51                     ; 29 	UART2_Init(9600,UART2_WORDLENGTH_8D,
  51                     ; 30 						 UART2_STOPBITS_1,UART2_PARITY_NO,
  51                     ; 31 						 UART2_SYNCMODE_CLOCK_DISABLE,
  51                     ; 32 						 UART2_MODE_RX_ENABLE);
  53  0000 4b08          	push	#8
  54  0002 4b80          	push	#128
  55  0004 4b00          	push	#0
  56  0006 4b00          	push	#0
  57  0008 4b00          	push	#0
  58  000a ae2580        	ldw	x,#9600
  59  000d 89            	pushw	x
  60  000e ae0000        	ldw	x,#0
  61  0011 89            	pushw	x
  62  0012 cd0000        	call	_UART2_Init
  64  0015 5b09          	addw	sp,#9
  65                     ; 33 	UART2_Cmd(ENABLE);
  67  0017 a601          	ld	a,#1
  68  0019 cd0000        	call	_UART2_Cmd
  70                     ; 34 }
  73  001c 81            	ret
 109                     ; 36 static char Bluetooth_Receive(void)
 109                     ; 37 {
 110                     	switch	.text
 111  001d               L32_Bluetooth_Receive:
 113  001d 88            	push	a
 114       00000001      OFST:	set	1
 117                     ; 38 	char rxData = '\0';
 119  001e 0f01          	clr	(OFST+0,sp)
 121                     ; 39 	if(UART2_GetFlagStatus(UART2_FLAG_RXNE) == SET)
 123  0020 ae0020        	ldw	x,#32
 124  0023 cd0000        	call	_UART2_GetFlagStatus
 126  0026 a101          	cp	a,#1
 127  0028 2605          	jrne	L34
 128                     ; 41 		rxData = (char)UART2_ReceiveData8();
 130  002a cd0000        	call	_UART2_ReceiveData8
 132  002d 6b01          	ld	(OFST+0,sp),a
 134  002f               L34:
 135                     ; 43 	return rxData;
 137  002f 7b01          	ld	a,(OFST+0,sp)
 140  0031 5b01          	addw	sp,#1
 141  0033 81            	ret
 165                     ; 47 static void Motors_Init(void)
 165                     ; 48 {
 166                     	switch	.text
 167  0034               L54_Motors_Init:
 171                     ; 50 	GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_SLOW);
 173  0034 4bc0          	push	#192
 174  0036 4b10          	push	#16
 175  0038 ae5005        	ldw	x,#20485
 176  003b cd0000        	call	_GPIO_Init
 178  003e 85            	popw	x
 179                     ; 51 	GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW);
 181  003f 4bc0          	push	#192
 182  0041 4b20          	push	#32
 183  0043 ae5005        	ldw	x,#20485
 184  0046 cd0000        	call	_GPIO_Init
 186  0049 85            	popw	x
 187                     ; 52 }
 190  004a 81            	ret
 214                     ; 54 static void Motors_Move(void)
 214                     ; 55 {
 215                     	switch	.text
 216  004b               L75_Motors_Move:
 220                     ; 57 	GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
 222  004b 4b10          	push	#16
 223  004d ae5005        	ldw	x,#20485
 224  0050 cd0000        	call	_GPIO_WriteHigh
 226  0053 84            	pop	a
 227                     ; 58 	GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
 229  0054 4b20          	push	#32
 230  0056 ae5005        	ldw	x,#20485
 231  0059 cd0000        	call	_GPIO_WriteHigh
 233  005c 84            	pop	a
 234                     ; 59 }
 237  005d 81            	ret
 261                     ; 61 static void Motors_Stop(void)
 261                     ; 62 {
 262                     	switch	.text
 263  005e               L17_Motors_Stop:
 267                     ; 63 	GPIO_WriteLow(GPIOC,GPIO_PIN_3);
 269  005e 4b08          	push	#8
 270  0060 ae500a        	ldw	x,#20490
 271  0063 cd0000        	call	_GPIO_WriteLow
 273  0066 84            	pop	a
 274                     ; 64 	GPIO_WriteLow(GPIOC,GPIO_PIN_4);	
 276  0067 4b10          	push	#16
 277  0069 ae500a        	ldw	x,#20490
 278  006c cd0000        	call	_GPIO_WriteLow
 280  006f 84            	pop	a
 281                     ; 65 }
 284  0070 81            	ret
 331                     ; 68 static void Servo_Init(void)
 331                     ; 69 {
 332                     	switch	.text
 333  0071               L301_Servo_Init:
 335  0071 5204          	subw	sp,#4
 336       00000004      OFST:	set	4
 339                     ; 72 	const uint16_t prescaler = 8-1;
 341  0073 ae0007        	ldw	x,#7
 342  0076 1f01          	ldw	(OFST-3,sp),x
 344                     ; 73 	const uint16_t period = 20000-1;
 346  0078 ae4e1f        	ldw	x,#19999
 347  007b 1f03          	ldw	(OFST-1,sp),x
 349                     ; 74 	TIM1_TimeBaseInit(prescaler,TIM1_COUNTERMODE_UP,period,0);
 351  007d 4b00          	push	#0
 352  007f 1e04          	ldw	x,(OFST+0,sp)
 353  0081 89            	pushw	x
 354  0082 4b00          	push	#0
 355  0084 1e05          	ldw	x,(OFST+1,sp)
 356  0086 cd0000        	call	_TIM1_TimeBaseInit
 358  0089 5b04          	addw	sp,#4
 359                     ; 76 	TIM1_OC1Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,
 359                     ; 77 							 TIM1_OUTPUTNSTATE_ENABLE,MID_POS,
 359                     ; 78 						   TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_HIGH,
 359                     ; 79 						   TIM1_OCIDLESTATE_SET,
 359                     ; 80 						   TIM1_OCNIDLESTATE_RESET);
 361  008b 4b00          	push	#0
 362  008d 4b55          	push	#85
 363  008f 4b00          	push	#0
 364  0091 4b22          	push	#34
 365  0093 ae05dc        	ldw	x,#1500
 366  0096 89            	pushw	x
 367  0097 4b44          	push	#68
 368  0099 ae7011        	ldw	x,#28689
 369  009c cd0000        	call	_TIM1_OC1Init
 371  009f 5b07          	addw	sp,#7
 372                     ; 82 	TIM1_Cmd(ENABLE);
 374  00a1 a601          	ld	a,#1
 375  00a3 cd0000        	call	_TIM1_Cmd
 377                     ; 84 	TIM1_CtrlPWMOutputs(ENABLE);					 
 379  00a6 a601          	ld	a,#1
 380  00a8 cd0000        	call	_TIM1_CtrlPWMOutputs
 382                     ; 85 }
 385  00ab 5b04          	addw	sp,#4
 386  00ad 81            	ret
 421                     ; 87 static void Servo_SetPosition(uint16_t pos)
 421                     ; 88 {
 422                     	switch	.text
 423  00ae               L721_Servo_SetPosition:
 427                     ; 89 	TIM1_SetCompare1(pos);
 429  00ae cd0000        	call	_TIM1_SetCompare1
 431                     ; 90 }
 434  00b1 81            	ret
 472                     ; 92 static void BackgroundTimer_Init(void)
 472                     ; 93 {
 473                     	switch	.text
 474  00b2               L741_BackgroundTimer_Init:
 476  00b2 89            	pushw	x
 477       00000002      OFST:	set	2
 480                     ; 96 	const uint16_t period = 125-1;
 482  00b3 ae007c        	ldw	x,#124
 483  00b6 1f01          	ldw	(OFST-1,sp),x
 485                     ; 97 	TIM2_TimeBaseInit(TIM2_PRESCALER_64,period);
 487  00b8 1e01          	ldw	x,(OFST-1,sp)
 488  00ba 89            	pushw	x
 489  00bb a606          	ld	a,#6
 490  00bd cd0000        	call	_TIM2_TimeBaseInit
 492  00c0 85            	popw	x
 493                     ; 98 	TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
 495  00c1 ae0101        	ldw	x,#257
 496  00c4 cd0000        	call	_TIM2_ITConfig
 498                     ; 99 	TIM2_Cmd(ENABLE);	
 500  00c7 a601          	ld	a,#1
 501  00c9 cd0000        	call	_TIM2_Cmd
 503                     ; 100 }
 506  00cc 85            	popw	x
 507  00cd 81            	ret
 510                     .const:	section	.text
 511  0000               L761_servoPos:
 512  0000 03e8          	dc.w	1000
 513  0002 07d0          	dc.w	2000
 594                     	switch	.const
 595  0004               L62:
 596  0004 000007d1      	dc.l	2001
 597                     ; 102 int main(void)
 597                     ; 103 {
 598                     	switch	.text
 599  00ce               _main:
 601  00ce 520d          	subw	sp,#13
 602       0000000d      OFST:	set	13
 605                     ; 105 	const uint16_t servoPos[] = {EXTREME_POS1,EXTREME_POS2};
 607  00d0 96            	ldw	x,sp
 608  00d1 1c0003        	addw	x,#OFST-10
 609  00d4 90ae0000      	ldw	y,#L761_servoPos
 610  00d8 a604          	ld	a,#4
 611  00da cd0000        	call	c_xymov
 613                     ; 106 	uint8_t servoPosIndex = 0;
 615  00dd 0f07          	clr	(OFST-6,sp)
 617                     ; 107 	uint8_t sweepShaft = 0;
 619  00df 0f09          	clr	(OFST-4,sp)
 621                     ; 108 	char appData = '\0';
 623                     ; 109 	uint32_t currentMillis = 0;
 625  00e1 ae0000        	ldw	x,#0
 626  00e4 1f0c          	ldw	(OFST-1,sp),x
 627  00e6 ae0000        	ldw	x,#0
 628  00e9 1f0a          	ldw	(OFST-3,sp),x
 630                     ; 111 	disableInterrupts();
 633  00eb 9b            sim
 635                     ; 112 	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2); //8MHz
 638  00ec a608          	ld	a,#8
 639  00ee cd0000        	call	_CLK_HSIPrescalerConfig
 641                     ; 113 	Bluetooth_Init();
 643  00f1 cd0000        	call	L3_Bluetooth_Init
 645                     ; 114 	Motors_Init();
 647  00f4 cd0034        	call	L54_Motors_Init
 649                     ; 115 	Servo_Init();
 651  00f7 cd0071        	call	L301_Servo_Init
 653                     ; 116 	BackgroundTimer_Init();
 655  00fa adb6          	call	L741_BackgroundTimer_Init
 657                     ; 117 	enableInterrupts();
 660  00fc 9a            rim
 662  00fd               L332:
 663                     ; 122 		appData = Bluetooth_Receive();
 665  00fd cd001d        	call	L32_Bluetooth_Receive
 667  0100 6b08          	ld	(OFST-5,sp),a
 669                     ; 123 		switch(appData)
 671  0102 7b08          	ld	a,(OFST-5,sp)
 673                     ; 133 				break;
 674  0104 a030          	sub	a,#48
 675  0106 2705          	jreq	L171
 676  0108 4a            	dec	a
 677  0109 2715          	jreq	L371
 678  010b 2018          	jra	L142
 679  010d               L171:
 680                     ; 125 			case '0':
 680                     ; 126 				Motors_Move();
 682  010d cd004b        	call	L75_Motors_Move
 684                     ; 127 				sweepShaft = 1;
 686  0110 a601          	ld	a,#1
 687  0112 6b09          	ld	(OFST-4,sp),a
 689                     ; 128 				currentMillis = Timer_GetMillis();
 691  0114 cd0000        	call	_Timer_GetMillis
 693  0117 96            	ldw	x,sp
 694  0118 1c000a        	addw	x,#OFST-3
 695  011b cd0000        	call	c_rtol
 698                     ; 129 				break;
 700  011e 2005          	jra	L142
 701  0120               L371:
 702                     ; 130 			case '1':
 702                     ; 131 				Motors_Stop();
 704  0120 cd005e        	call	L17_Motors_Stop
 706                     ; 132 				sweepShaft = 0;
 708  0123 0f09          	clr	(OFST-4,sp)
 710                     ; 133 				break;
 712  0125               L142:
 713                     ; 136 		if(sweepShaft)
 715  0125 0d09          	tnz	(OFST-4,sp)
 716  0127 2736          	jreq	L342
 717                     ; 138 			if(((Timer_GetMillis() - currentMillis) > 2000))
 719  0129 cd0000        	call	_Timer_GetMillis
 721  012c 96            	ldw	x,sp
 722  012d 1c000a        	addw	x,#OFST-3
 723  0130 cd0000        	call	c_lsub
 725  0133 ae0004        	ldw	x,#L62
 726  0136 cd0000        	call	c_lcmp
 728  0139 25c2          	jrult	L332
 729                     ; 140 				Servo_SetPosition(servoPos[servoPosIndex]);
 731  013b 96            	ldw	x,sp
 732  013c 1c0003        	addw	x,#OFST-10
 733  013f 1f01          	ldw	(OFST-12,sp),x
 735  0141 7b07          	ld	a,(OFST-6,sp)
 736  0143 5f            	clrw	x
 737  0144 97            	ld	xl,a
 738  0145 58            	sllw	x
 739  0146 72fb01        	addw	x,(OFST-12,sp)
 740  0149 fe            	ldw	x,(x)
 741  014a cd00ae        	call	L721_Servo_SetPosition
 743                     ; 141 				servoPosIndex ^= 1;
 745  014d 7b07          	ld	a,(OFST-6,sp)
 746  014f a801          	xor	a,#1
 747  0151 6b07          	ld	(OFST-6,sp),a
 749                     ; 142 				currentMillis = Timer_GetMillis();
 751  0153 cd0000        	call	_Timer_GetMillis
 753  0156 96            	ldw	x,sp
 754  0157 1c000a        	addw	x,#OFST-3
 755  015a cd0000        	call	c_rtol
 758  015d 209e          	jra	L332
 759  015f               L342:
 760                     ; 147 			Servo_SetPosition(MID_POS);
 762  015f ae05dc        	ldw	x,#1500
 763  0162 cd00ae        	call	L721_Servo_SetPosition
 765  0165 2096          	jra	L332
 778                     	xdef	_main
 779                     	xref	_Timer_GetMillis
 780                     	xref	_UART2_GetFlagStatus
 781                     	xref	_UART2_ReceiveData8
 782                     	xref	_UART2_Cmd
 783                     	xref	_UART2_Init
 784                     	xref	_TIM2_ITConfig
 785                     	xref	_TIM2_Cmd
 786                     	xref	_TIM2_TimeBaseInit
 787                     	xref	_TIM1_SetCompare1
 788                     	xref	_TIM1_CtrlPWMOutputs
 789                     	xref	_TIM1_Cmd
 790                     	xref	_TIM1_OC1Init
 791                     	xref	_TIM1_TimeBaseInit
 792                     	xref	_GPIO_WriteLow
 793                     	xref	_GPIO_WriteHigh
 794                     	xref	_GPIO_Init
 795                     	xref	_CLK_HSIPrescalerConfig
 796                     	xref.b	c_x
 815                     	xref	c_lcmp
 816                     	xref	c_lsub
 817                     	xref	c_rtol
 818                     	xref	c_xymov
 819                     	end
