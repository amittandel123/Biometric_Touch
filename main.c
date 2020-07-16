/******************************************************************************************************************************
 *  main.c
 *  Modified by: Amit Tandel
 *  Date : 18 Mar 2014
 *  Version 1.4
 *  Description: There are 2 switches. One for FP identification & other for RESET.
 * 				When 1st switch is pressed, Capture finger-print & check if it is present. If yes Drive motor else RED LED.
 * 				If it is Master finger, sensor will wait for another finger to get added.
 * 				If RESET s/w pressed, delete all slots in the memory & wait for adding new finger at 0th location.
 *
 *  Backlogs: EEPROM required to store current USER_SLOT number.
 *
 *  Pin Connection: P1.0 = RED LED, P1.1 = RX, P1.2 = TX, P1.5 = RED LED, P1.6 = GREEN LED, P1.7 = BUZZER
 *  				P2.6 = FingerSense S/W, P2.7 = RESET S/W, P1.3 = Scan LED
 ******************************************************************************************************************************/

#include<msp430.h>
#include"FP_Command.h"
#include"includes.h"

void System_Init();
void Short_Single_Beep();
void Short_Double_Beep();
void Short_Triple_Beep();
void Long_Single_Beep();
void Long_Double_Beep();
void Long_Triple_Beep();
void Drive_Motor(void);
void Delay_Sec(unsigned char val);
void Panic_Alarm();

unsigned char Recd_Count=0;
volatile unsigned char RECD_FLAG=0x00, FP_INTR_FLAG=0x00, FP_RESET_FLAG=0x00;
unsigned char Recd_Data[12],Trans_Data[6];
unsigned char WRONG_COUNT=0;
static unsigned char MASTER_SLOT=0,USER_SLOT;
long unsigned int ENROLL_TIME=0,SCAN_TIME=0,WAIT_TIME=0, RESET_TIME=0;

void main(void)
{
	System_Init();
	while(1)
	{
		__bis_SR_register(LPM3_bits + GIE);		// Interrupts enabled for Switch

//---< Check Finger Scanning switch >---------------------------------------------------
		if(FP_INTR_FLAG==0xAA)
		{
			FP_INTR_FLAG=0x00;
			Command_API_CAPTURE_AND_IDENTIFY_FLASH();
			FP_INTR_FLAG=0x00;
			SCAN_TIME=200000;
			LED1_ON;
			while(SCAN_TIME--)
			{
				if(Recd_Data[1] == API_IDENTIFY_OK)
				{
					GREEN_LED_ON;
					WRONG_COUNT=0;
					LED1_OFF;
					Short_Single_Beep();
					Drive_Motor();
					GREEN_LED_OFF;
					if(Recd_Data[4] == 0x00)	// Template is of 0th location i.e. Master
					{
						Add_User();
					}
					break;
				}
				else if(Recd_Data[1] == API_IDENTIFY_FAIL)
				{
					RED_LED_ON;
					LED1_OFF;
					Short_Double_Beep();
					RED_LED_OFF;
					WRONG_COUNT++;
					if(WRONG_COUNT >= 3)
					{
						Panic_Alarm();
					}
					break;
				}

				if(SCAN_TIME <= 1)
				{
					Short_Triple_Beep();
					break;
				}
			}
			LED1_OFF;
			RED_LED_OFF;
			GREEN_LED_OFF;
		}

//---< Check RESET switch >---------------------------------------------------
		if(FP_RESET_FLAG == 0xBB)
		{
			Command_API_DELETE_ALL_IN_FLASH();
			FP_RESET_FLAG = 0x00;
			RESET_TIME=400000;			// At least 8 to 12 Sec reqd for response
			while(RESET_TIME--)
			{
				RED_LED_ON;
				if(Recd_Data[1] == API_SUCCESS)
				{
					Command_API_CAPTURE_AND_ENROL_FLASH(MASTER_SLOT);	// Enroll MAster finger
					Long_Triple_Beep();		// Reset indication
					RED_LED_OFF;
					LED1_ON;
					USER_SLOT=1;
					SCAN_TIME=400000;	// At least 6 to 7 Sec reqd for response
					while(SCAN_TIME--)
					{
						if(Recd_Data[1] == API_ENROL_OK)
						{
							GREEN_LED_ON;
//							Recd_Data[1] = 0x22;	// Add Junk data
							LED1_OFF;
							Short_Single_Beep();	// Success Indication
							GREEN_LED_OFF;
							break;
						}
						if((Recd_Data[1] == API_ENROL_FAIL)||(Recd_Data[1] == API_INVALID_SLOT_NR))
						{
							RED_LED_ON;
							LED1_OFF;
							Recd_Data[1] = 0x22;	// Add Junk data
							Short_Double_Beep();	// Failure indication
							RED_LED_OFF;
							break;
						}
						if(SCAN_TIME <= 1)
						{
							Short_Triple_Beep();
							break;
						}
						GREEN_LED_OFF;
					}
					RED_LED_OFF;
					GREEN_LED_OFF;
					break;
				}

				if(Recd_Data[1] == API_FAILURE)
				{
					RED_LED_ON;
					LED1_OFF;
					Short_Double_Beep();
					RED_LED_OFF;
					break;
				}

				if(RESET_TIME <= 1)
				{
					Short_Triple_Beep();
					break;
				}
			}
			LED1_OFF;
			RED_LED_OFF;
			GREEN_LED_OFF;
		}
	}
}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	CCR0 = 50000;
	LPM0_EXIT;
}

// ISR for reception
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	Recd_Data[Recd_Count++] = UCA0RXBUF;
}

// Port 2 interrupt service routine for Switches
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	if(P2IFG & FP_RESET_PIN)	// RESET Interrupt
	{
//		Short_Single_Beep();
		if((P2IN & FP_RESET_PIN)==0)
		{
			__delay_cycles(100000);
			if((P2IN & FP_RESET_PIN)==0)
			{
				FP_RESET_FLAG=0xBB;
			}
		}
	}

	if(P2IFG & FP_INTR_PIN)		// Finger Interrupt
	{
//		Short_Single_Beep();
		if((P2IN & FP_INTR_PIN)==0)
		{
			__delay_cycles(100000);
			if((P2IN & FP_INTR_PIN)==0)
			{
				FP_INTR_FLAG=0xAA;
			}
		}
	}
	P2IFG = 0x00; 					// IFG cleared
	LPM3_EXIT;
}

void System_Init()
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop WDT
	DCOCTL = 0x00;				// Set DCOCLK to 1MHz
	BCSCTL1 = CALBC1_1MHZ;		// Freq @ 1MHz
	DCOCTL = CALDCO_1MHZ;
	/********************* Port setting *************************/
	P1DIR = (0xFF & ~BIT1);		// All P1.x outputs except P1.1
	P1OUT = 0;					// All P1.x reset
	P2DIR = (0xFF & ~(FP_RESET_PIN + FP_INTR_PIN));		// All P2.x outputs except P2.7
	P2OUT = 0;					// All P2.x reset
	P1SEL = BIT1 + BIT2;		// P1.1 = RXD, P1.2=TXD
	P1SEL2= BIT1 + BIT2;		// P1.1 = RXD, P1.2=TXD
	/********************* Port setting *************************/

	/********************* UART Configuration *************************/
	UCA0CTL1 |= UCSSEL_2;		// CLK = SMCLK
	UCA0CTL0 = UCPEN;			// Parity enabled, ODD parity, LSB first, 1 Stop bit, UART mode
	UCA0BR0 = 104;				// 1MHz/9600 = 104 = 68H
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS0;			// Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;		// Start UART
	IE2 |= UCA0RXIE;			// Enable RX interrupt
	/********************* UART Configuration *************************/

	/***************** RESET S/W Interrupt setting *****************/
	P2SEL = 0;
	P2REN |= (FP_RESET_PIN + FP_INTR_PIN);	// P2.7 PullUp/PullDown resistor enabled
	P2OUT |= (FP_RESET_PIN + FP_INTR_PIN);	// P2.7 PullUp enabled
	P2IE  |= (FP_RESET_PIN + FP_INTR_PIN); 	// P2.7 interrupt enabled
	P2IES |= (FP_RESET_PIN + FP_INTR_PIN);	// P2.7 Hi to Low edge
	P2IFG=0;								// Clear Intr flag
	/***************** RESET S/W Interrupt setting *****************/

	/*************** Initialization Indication ******************/
//	Short_Triple_Beep();
//	for(i=3;i>0;i--)
	{
		GREEN_LED_ON;
		RED_LED_ON;
		__delay_cycles(500000);
		GREEN_LED_OFF;
		RED_LED_OFF;
		__delay_cycles(200000);
	}
	/*************** Initialization Indication ******************/
}

void Short_Single_Beep()
{
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Short_Beep_Time);
}

void Short_Double_Beep()
{
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Short_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
}

void Short_Triple_Beep()
{
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Short_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Short_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Short_Beep_Time);
	BUZZER_OFF;
}

void Drive_Motor()	// function to drive motor
{
//	Short_Double_Beep();
	FWD_MOTOR;
	Delay_Sec(2);
	HALT_MOTOR;
	Delay_Sec(30);
	REV_MOTOR;
	Delay_Sec(2);
	HALT_MOTOR;
	Delay_Sec(1);
//	Short_Single_Beep();
}

void Long_Single_Beep()
{
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Long_Beep_Time);
}

void Long_Double_Beep()
{
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Long_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
}

void Long_Triple_Beep()
{
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Long_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
	Delay_Sec(Long_Beep_Time);
	BUZZER_ON;
	Delay_Sec(Long_Beep_Time);
	BUZZER_OFF;
}

void Delay_Sec(unsigned char val)
{
	unsigned int loop,i;
	loop=val*2;
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 50000;
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
	for(i=0;i<loop;i++)
	{
		_BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
	}
}

void Add_User()
{
	if(USER_SLOT < 20)
	{
		Command_API_CAPTURE_AND_ENROL_FLASH(USER_SLOT);	// Enroll MAster finger
		LED1_ON;
		SCAN_TIME=200000;	// At least 6 to 7 Sec reqd for response
		while(SCAN_TIME--)
		{
			if(Recd_Data[1] == API_ENROL_OK)
			{
				GREEN_LED_ON;
				LED1_OFF;
				USER_SLOT++;
				Long_Single_Beep();	// Success Indication
				GREEN_LED_OFF;
				break;
			}
			if((Recd_Data[1] == API_ENROL_FAIL)||(Recd_Data[1] == API_INVALID_SLOT_NR))
			{
				RED_LED_ON;
				LED1_OFF;
				Short_Double_Beep();	// Failure indication
				RED_LED_OFF;
				break;
			}
			if(SCAN_TIME <= 1)
			{
				Short_Triple_Beep();
				break;
			}
			GREEN_LED_OFF;
		}
		RED_LED_OFF;
		GREEN_LED_OFF;
	}

	else
	{
		Short_Double_Beep();
	}
}

void Panic_Alarm()
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<3;j++)
		{
			Short_Single_Beep();
		}
		Long_Single_Beep();
	}
}
/*
void main()
{
	System_Init();
	while(1)
	{
		SCAN_TIME=300000;
		while(SCAN_TIME--)	// 6Sec loop
		{
			GREEN_LED_ON;
		}
		GREEN_LED_OFF;
		__delay_cycles(100000);
	}
}
*/
