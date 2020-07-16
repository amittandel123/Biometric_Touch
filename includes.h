/*
 * includes.h
 *
 *  Created on: Feb 13, 2014
 *  Author: Amit Tandel
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

extern unsigned char Recd_Data[12],Trans_Data[6];
extern unsigned char Recd_Count;

#define GREEN_LED_ON			(P1OUT |= BIT6)
#define GREEN_LED_OFF			(P1OUT &= ~BIT6)
#define RED_LED_ON				(P1OUT |= BIT5)
#define RED_LED_OFF				(P1OUT &= ~BIT5)
#define BUZZER_ON				(P1OUT |= BIT7)
#define BUZZER_OFF				(P1OUT &= ~BIT7)

#define LED1_ON					(P1OUT |= BIT3)
#define LED2_ON					(P1OUT |= BIT4)
#define LED1_OFF				(P1OUT &= ~BIT3)
#define LED2_OFF				(P1OUT &= ~BIT4)

#define Long_Beep_Time			2
#define Short_Beep_Time			1

#define FP_INTR_PIN				BIT6	// P2.6
#define FP_RESET_PIN			BIT7	// P2.7
#define Motor_Port				P2OUT
#define FWD						BIT0	// P2.0
#define REV						BIT1	// P2.1

#define STALL_MOTOR		do{Motor_Port |=  FWD; Motor_Port |=  REV;}while(0)
#define REV_MOTOR		do{Motor_Port |=  FWD; Motor_Port &= ~REV;}while(0)
#define FWD_MOTOR		do{Motor_Port &= ~FWD; Motor_Port |=  REV;}while(0)
#define HALT_MOTOR		do{Motor_Port &= ~FWD; Motor_Port &= ~REV;}while(0)

#define API_FAILURE				0x00
#define API_SUCCESS				0x01
#define API_NO_FINGER_PRESENT	0x02
#define API_FINGER_PRESENT		0x03
#define API_VERIFICATION_OK		0x04
#define API_VERIFICATION_FAIL	0x05
#define API_ENROL_OK			0x06
#define API_ENROL_FAIL		0x07
#define API_HW_TEST_OK		0x08
#define API_HW_TEST_FAIL		0x09
#define API_CRC_FAIL			0x0A
#define API_PAYLOAD_TOO_LONG	0x0B
#define API_PAYLOAD_TOO_SHORT	0x0C
#define API_UNKNOWN_COMMAND		0x0D
#define API_NO_TEMPLATE_PRESENT	0x0E
#define API_IDENTIFY_OK			0x0F
#define API_IDENTIFY_FAIL		0x10
#define API_INVALID_SLOT_NR		0x11
#define API_CANCEL_SUCCESS		0x12
#define API_APPL_CRC_FAIL		0x14
#define API_SYS_CRC_FAILED		0x16
#define API_LOW_VOLTAGE			0x17

#endif /* INCLUDES_H_ */
