/*
 * FP_Command.c
 *
 *  Created on: Feb 8, 2014
 *  Author: Amit Tandel
 *  Description: Contain all functions for FP module
 *
 */

#include<msp430.h>
#include"FP_Command.h"
#include"includes.h"

//**************************************************************************************
// Function			: void Send_Data()
// Input Parameters	: unsigned char *Command
// Return Value		: void
// Description		: Serially transmits 6bytes data received from array
//**************************************************************************************
void Send_Data(unsigned char *Command)
{
	unsigned char i;
	for(i=0;i<12;i++)		// Add junk to Recd_Data[] array
	{ Recd_Data[i]= '*'; }
	for(i=0;i<6;i++)
	{
		while (!(IFG2 & UCA0TXIFG));	// USCI_A0 TX buffer ready?
		UCA0TXBUF = *Command++;			// Transmit data
	}
	Recd_Count=0;
	Delay_Sec(1);
	__bis_SR_register(GIE);	// Enable interrupt
}

//**********************************************************************************************
// Function			: void Command_API_CAPTURE_IMAGE()
// Input Parameters	: void
// Return Value		: void
// Description : An image is captured from the fingerprint sensor. The fingerprint image is placed
//				 in RAM and can be uploaded by the command API_UPLOAD_IMAGE. Calculation is done on
//				 the image to determine if a finger is present or not present on the sensor. No
// 				 payload is sent with this command.
// Response command: API_NO_FINGER_PRESENT = No finger present on sensor
//					 API_FINGER_PRESENT = Finger present on sensor
// No payload is received with the response from this command.
//**********************************************************************************************
void Command_API_CAPTURE_IMAGE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0X00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x80;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**********************************************************************************************
// Function			: Command_API_CAPTURE_AND_VERIFY_FLASH(unsigned char SLOT_NUM)
// Input Parameters	: unsigned char SLOT_NUM
// Return Value		: void
// Description : The FLASH slot number must be given in the IDX bytes. This command
// 				 first captures an image from the fingerprint sensor. The command waits for
// 				 “finger present” before it starts the verification. This means that images
// 				 are captured in a loop from the sensor until a finger is present. The command
// 				 returns with response when the verification is complete or if the verification
// 				 fails for any reason. No payload is sent with this command.
// Response command: API_VERIFICATION_OK = Verification successful
//					 API_VERIFICATION_FAIL = Verification failed
//					 API_NO_TEMPLATE_PRESENT = No template in given FLASH slot
//					 API_INVALID_SLOT_NR = Wrong slot number
// No payload is received with the response from this command.
//**********************************************************************************************
void Command_API_CAPTURE_AND_VERIFY_FLASH(unsigned char SLOT_NUM)
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = SLOT_NUM;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x83;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**********************************************************************************************
// Function			: void Command_API_CAPTURE_AND_ENROL_FLASH(unsigned char IDX_LSB)
// Input Parameters	: unsigned char IDX_LSB
// Return Value		: void
// Description		: This command first captures an image from the fingerprint sensor. The
//					  command waits for “finger present” before it starts the verification.
// 					  This means that images are captured in a loop from the sensor until a
//					  finger is present. The command then returns with a response when the
//					  enrolment is complete or if the enrolment fails for any reason. After
//				      enrolment the template is stored in FLASH and can be uploaded or moved to
//					  FLASH storage. The desired FLASH slot number must be given in the IDX bytes.
//					  No payload is sent with this command.
// Response command: API_ENROL_OK = Enrolment successful
//			   		 API_ENROL_FAIL = Enrolment failed
//					 API_INVALID_SLOT_NR = Incorrect FLASH slot number
// No payload is received with the response from this command.
//**********************************************************************************************
void Command_API_CAPTURE_AND_ENROL_FLASH(unsigned char IDX_LSB)
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = IDX_LSB;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x93;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}
//*********************************************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		: Identification is made against all FLASH slots. This command first captures an
// 					  image from the fingerprint sensor. The command waits for “finger present” before
// 					  it starts the identification. This means that images are captured in a loop from
//					  the sensor until a finger is present. The command returns with response when the
//					  identification is complete or if the identification fails for any reason. No payload
//					  is sent with this command.
// Response command: API_ IDENTIFY _OK = Identification successful
// 					 API_ IDENTIFY _FAIL = Identification fails
// In a successful identification, the slot index is received as payload in two bytes(LSB first) plus the 4 CRC bytes.
//*********************************************************************************************************************
void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x84;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_CAPTURE_AND_VERIFY_RAM()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x82;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_API_IDENTIFY_FLASH()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x88;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_VERIFY_FLASH(unsigned int INDX_Data)
// Input Parameters	: unsigned int INDX_Data
// Return Value		: void
// Description		: A fingerprint image must be present in RAM before starting the verification,
//					  use the command Download Image API_DOWNLOAD_IMAGE) OR the command API_CAPTURE_IMAGE.
//					  The FLASH slot number must be given in the IDX bytes. The command returns with
//					  response when the verification is complete or if the verification fails for any reason.
//					  No payload is sent with this command.
// Response command: API_VERIFICATION_OK = Verification successful
//					 API_VERIFICATION_FAIL = Verification failed
//					 API_NO_TEMPLATE_PRESENT = No template in given FLASH slot
//					 API_INVALID_SLOT_NR = Wrong slot number
// No payload is received with the response from this command.
//**************************************************************************************
void Command_API_VERIFY_FLASH(unsigned char INDX_Data)
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = INDX_Data;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x87;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_API_CAPTURE_IMAGE_FINGERPRESENT()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x89;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_API_ENROL_FLASH()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x92;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//******************************************************************************************************
// Function			: void Command_API_DOWNLOAD_TEMPLATE()
// Input Parameters	: void
// Return Value		: void
// Description		: Before verification the template is downloaded to RAM. The size of the template
//					  is 938 bytes. Payload size of this command must be 938 bytes, plus the 4 CRC-bytes
//					  sent after the payload
//******************************************************************************************************
void Command_API_DOWNLOAD_TEMPLATE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xA1;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_API_DOWNLOAD_IMAGE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x01;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x91;
	Trans_Data[4] = 0xC0;
	Trans_Data[5] = 0x76;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		:
//**************************************************************************************
void Command_API_UPLOAD_IMAGE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0x90;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//*************************************************************************************************
// Function			: void Command_API_DELETE_SLOT_IN_FLASH(unsigned char slot_num)
// Input Parameters	: unsigned char slot_num
// Return Value		: void
// Description		: By using the command Delete slot in FLASH one can choose which slot to delete
//					  (include slot number in index value of command). No payload is sent with this
//					  command.
// Response command: API_ SUCCESS = Template removal successful
//		        	 API_ FAILURE = Template removal failed
//					 API_INVALID_SLOT_NR = Wrong slot number
// No payload is received with the response from this command.
//*************************************************************************************************
void Command_API_DELETE_SLOT_IN_FLASH(unsigned char slot_num)
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = slot_num;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xA5;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_DELETE_ALL_IN_FLASH())
// Input Parameters	: void
// Return Value		: void
// Description		: Delete all templates in FLASH
// Response command: API_ SUCCESS = Template removal successful
// 					 API_ FAILURE = Template removal failed
// No payload is received with the response from this command.
//**************************************************************************************
void Command_API_DELETE_ALL_IN_FLASH()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xA6;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//**************************************************************************************
// Function			: void Command_API_CAPTURE_AND_IDENTIFY_FLASH()
// Input Parameters	: void
// Return Value		: void
// Description		: SLEEP MODE is entered by issuing the command API_ENTER_SLEEP_MODE.
// 					  In SLEEP MODE the device runs on low power. To wake up the device,
//					  a wakeup interrupt must occur. This is triggered by one of three
//					  possible signals: HOST_SI (active low), HOST_SCN (active low) or
//					  SENSOR_IRQ (active high). No payload is sent with this command.
//    				  Before the device enters SLEEP MODE it responds with one of the following:
// Response command: API_ SUCCESS = Request accepted, entering SLEEP MODE
//					 API_ FAILURE = Request failed
// No payload is received with the response from this command.
//**************************************************************************************
void Command_API_ENTER_SLEEP_MODE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xE1;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//*******************************************************************************************************
// Function			: void Command_API_POWER_SAVE_MODE_RAM_DISABLE()
// Input Parameters	: void
// Return Value		: void
// Description		: In POWER SAVE MODE the module reduces the clock frequency of the processor by half
// 					  to lower power consumption. To enter POWER SAVE MODE, issue the command Power Save
//					  Mode with the value 0 in the IDX-LSB byte. To exit POWER SAVE MODE, issue the
//					  command Power Save Mode with the value 1 in the IDX-LSB byte. The setting is stored
//					  in RAM and the setting is lost after reset. No payload is sent with this command.
//					  The factory default setting is that the module is NOT in power save mode (value=1).
//*******************************************************************************************************
void Command_API_POWER_SAVE_MODE_RAM_DISABLE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x01;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xE2;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}

//*******************************************************************************************************
// Function			: void Command_API_POWER_SAVE_MODE_RAM_DISABLE()
// Input Parameters	: void
// Return Value		: void
// Description		: In POWER SAVE MODE the module reduces the clock frequency of the processor by half
// 					  to lower power consumption. To enter POWER SAVE MODE, issue the command Power Save
//					  Mode with the value 0 in the IDX-LSB byte. To exit POWER SAVE MODE, issue the
//					  command Power Save Mode with the value 1 in the IDX-LSB byte. The setting is stored
//					  in RAM and the setting is lost after reset. No payload is sent with this command.
//					  The factory default setting is that the module is NOT in power save mode (value=1).
//*******************************************************************************************************
void Command_API_POWER_SAVE_MODE_RAM_ENABLE()
{
	Trans_Data[0] = 0x02;
	Trans_Data[1] = 0x00;
	Trans_Data[2] = 0x00;
	Trans_Data[3] = 0xE2;
	Trans_Data[4] = 0x00;
	Trans_Data[5] = 0x00;
	Send_Data(Trans_Data);
}
