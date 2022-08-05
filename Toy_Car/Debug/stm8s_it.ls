   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.12.6 - 16 Dec 2021
   3                     ; Generator (Limited) V4.5.4 - 16 Dec 2021
  43                     ; 45 uint32_t Timer_GetMillis(void)
  43                     ; 46 {
  45                     	switch	.text
  46  0000               _Timer_GetMillis:
  50                     ; 47 	return currentTick;
  52  0000 ae0000        	ldw	x,#L3_currentTick
  53  0003 cd0000        	call	c_ltor
  57  0006 81            	ret
  81                     ; 58 INTERRUPT_HANDLER(NonHandledInterrupt, 25)
  81                     ; 59 {
  83                     	switch	.text
  84  0007               f_NonHandledInterrupt:
  88                     ; 63 }
  91  0007 80            	iret
 113                     ; 71 INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
 113                     ; 72 {
 114                     	switch	.text
 115  0008               f_TRAP_IRQHandler:
 119                     ; 76 }
 122  0008 80            	iret
 144                     ; 83 INTERRUPT_HANDLER(TLI_IRQHandler, 0)
 144                     ; 84 
 144                     ; 85 {
 145                     	switch	.text
 146  0009               f_TLI_IRQHandler:
 150                     ; 89 }
 153  0009 80            	iret
 175                     ; 96 INTERRUPT_HANDLER(AWU_IRQHandler, 1)
 175                     ; 97 {
 176                     	switch	.text
 177  000a               f_AWU_IRQHandler:
 181                     ; 101 }
 184  000a 80            	iret
 206                     ; 108 INTERRUPT_HANDLER(CLK_IRQHandler, 2)
 206                     ; 109 {
 207                     	switch	.text
 208  000b               f_CLK_IRQHandler:
 212                     ; 113 }
 215  000b 80            	iret
 238                     ; 120 INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
 238                     ; 121 {
 239                     	switch	.text
 240  000c               f_EXTI_PORTA_IRQHandler:
 244                     ; 125 }
 247  000c 80            	iret
 270                     ; 132 INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
 270                     ; 133 {
 271                     	switch	.text
 272  000d               f_EXTI_PORTB_IRQHandler:
 276                     ; 137 }
 279  000d 80            	iret
 302                     ; 144 INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
 302                     ; 145 {
 303                     	switch	.text
 304  000e               f_EXTI_PORTC_IRQHandler:
 308                     ; 149 }
 311  000e 80            	iret
 334                     ; 156 INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
 334                     ; 157 {
 335                     	switch	.text
 336  000f               f_EXTI_PORTD_IRQHandler:
 340                     ; 161 }
 343  000f 80            	iret
 366                     ; 168 INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
 366                     ; 169 {
 367                     	switch	.text
 368  0010               f_EXTI_PORTE_IRQHandler:
 372                     ; 173 }
 375  0010 80            	iret
 397                     ; 220 INTERRUPT_HANDLER(SPI_IRQHandler, 10)
 397                     ; 221 {
 398                     	switch	.text
 399  0011               f_SPI_IRQHandler:
 403                     ; 225 }
 406  0011 80            	iret
 429                     ; 232 INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
 429                     ; 233 {
 430                     	switch	.text
 431  0012               f_TIM1_UPD_OVF_TRG_BRK_IRQHandler:
 435                     ; 237 }
 438  0012 80            	iret
 461                     ; 244 INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
 461                     ; 245 {
 462                     	switch	.text
 463  0013               f_TIM1_CAP_COM_IRQHandler:
 467                     ; 249 }
 470  0013 80            	iret
 495                     ; 282  INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 495                     ; 283  {
 496                     	switch	.text
 497  0014               f_TIM2_UPD_OVF_BRK_IRQHandler:
 499  0014 8a            	push	cc
 500  0015 84            	pop	a
 501  0016 a4bf          	and	a,#191
 502  0018 88            	push	a
 503  0019 86            	pop	cc
 504  001a 3b0002        	push	c_x+2
 505  001d be00          	ldw	x,c_x
 506  001f 89            	pushw	x
 507  0020 3b0002        	push	c_y+2
 508  0023 be00          	ldw	x,c_y
 509  0025 89            	pushw	x
 512                     ; 287 	currentTick++;
 514  0026 ae0000        	ldw	x,#L3_currentTick
 515  0029 a601          	ld	a,#1
 516  002b cd0000        	call	c_lgadc
 518                     ; 288 	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
 520  002e a601          	ld	a,#1
 521  0030 cd0000        	call	_TIM2_ClearITPendingBit
 523                     ; 289  }
 526  0033 85            	popw	x
 527  0034 bf00          	ldw	c_y,x
 528  0036 320002        	pop	c_y+2
 529  0039 85            	popw	x
 530  003a bf00          	ldw	c_x,x
 531  003c 320002        	pop	c_x+2
 532  003f 80            	iret
 555                     ; 296  INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 555                     ; 297  {
 556                     	switch	.text
 557  0040               f_TIM2_CAP_COM_IRQHandler:
 561                     ; 301  }
 564  0040 80            	iret
 587                     ; 311  INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 587                     ; 312  {
 588                     	switch	.text
 589  0041               f_TIM3_UPD_OVF_BRK_IRQHandler:
 593                     ; 316  }
 596  0041 80            	iret
 619                     ; 323  INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 619                     ; 324  {
 620                     	switch	.text
 621  0042               f_TIM3_CAP_COM_IRQHandler:
 625                     ; 328  }
 628  0042 80            	iret
 650                     ; 389 INTERRUPT_HANDLER(I2C_IRQHandler, 19)
 650                     ; 390 {
 651                     	switch	.text
 652  0043               f_I2C_IRQHandler:
 656                     ; 394 }
 659  0043 80            	iret
 682                     ; 402  INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 682                     ; 403  {
 683                     	switch	.text
 684  0044               f_UART2_TX_IRQHandler:
 688                     ; 407  }
 691  0044 80            	iret
 714                     ; 414  INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 714                     ; 415  {
 715                     	switch	.text
 716  0045               f_UART2_RX_IRQHandler:
 720                     ; 419  }
 723  0045 80            	iret
 745                     ; 468  INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 745                     ; 469  {
 746                     	switch	.text
 747  0046               f_ADC1_IRQHandler:
 751                     ; 473  }
 754  0046 80            	iret
 777                     ; 494  INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 777                     ; 495  {
 778                     	switch	.text
 779  0047               f_TIM4_UPD_OVF_IRQHandler:
 783                     ; 499  }
 786  0047 80            	iret
 809                     ; 507 INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
 809                     ; 508 {
 810                     	switch	.text
 811  0048               f_EEPROM_EEC_IRQHandler:
 815                     ; 512 }
 818  0048 80            	iret
 841                     	switch	.ubsct
 842  0000               L3_currentTick:
 843  0000 00000000      	ds.b	4
 844                     	xdef	f_EEPROM_EEC_IRQHandler
 845                     	xdef	f_TIM4_UPD_OVF_IRQHandler
 846                     	xdef	f_ADC1_IRQHandler
 847                     	xdef	f_UART2_TX_IRQHandler
 848                     	xdef	f_UART2_RX_IRQHandler
 849                     	xdef	f_I2C_IRQHandler
 850                     	xdef	f_TIM3_CAP_COM_IRQHandler
 851                     	xdef	f_TIM3_UPD_OVF_BRK_IRQHandler
 852                     	xdef	f_TIM2_CAP_COM_IRQHandler
 853                     	xdef	f_TIM2_UPD_OVF_BRK_IRQHandler
 854                     	xdef	f_TIM1_UPD_OVF_TRG_BRK_IRQHandler
 855                     	xdef	f_TIM1_CAP_COM_IRQHandler
 856                     	xdef	f_SPI_IRQHandler
 857                     	xdef	f_EXTI_PORTE_IRQHandler
 858                     	xdef	f_EXTI_PORTD_IRQHandler
 859                     	xdef	f_EXTI_PORTC_IRQHandler
 860                     	xdef	f_EXTI_PORTB_IRQHandler
 861                     	xdef	f_EXTI_PORTA_IRQHandler
 862                     	xdef	f_CLK_IRQHandler
 863                     	xdef	f_AWU_IRQHandler
 864                     	xdef	f_TLI_IRQHandler
 865                     	xdef	f_TRAP_IRQHandler
 866                     	xdef	f_NonHandledInterrupt
 867                     	xdef	_Timer_GetMillis
 868                     	xref	_TIM2_ClearITPendingBit
 869                     	xref.b	c_x
 870                     	xref.b	c_y
 890                     	xref	c_lgadc
 891                     	xref	c_ltor
 892                     	end
