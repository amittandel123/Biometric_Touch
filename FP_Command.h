/*
 * FP_Command.h
 *
 *  Created on: Feb 8, 2014
 *      Author: Administrator
 */

#ifndef FP_COMMAND_H_
#define FP_COMMAND_H_

extern void Send_Data(unsigned char *);
extern void Add_User();
extern void Delay_Sec(unsigned char);
extern void Command_API_CAPTURE_IMAGE();
extern void Command_API_CAPTURE_AND_VERIFY_FLASH(unsigned char);
extern void Command_API_CAPTURE_AND_ENROL_FLASH(unsigned char);
extern void Command_CAPTURE_AND_VERIFY_RAM();
extern void Command_API_CAPTURE_AND_IDENTIFY_FLASH();
extern void Command_API_IDENTIFY_FLASH();
extern void Command_API_VERIFY_FLASH(unsigned char);
extern void Command_API_CAPTURE_IMAGE_FINGERPRESENT();
extern void Command_API_ENROL_FLASH();
extern void Command_API_DOWNLOAD_TEMPLATE();
extern void Command_API_DOWNLOAD_IMAGE();
extern void Command_API_UPLOAD_IMAGE();
extern void Command_API_DELETE_SLOT_IN_FLASH(unsigned char);
extern void Command_API_DELETE_ALL_IN_FLASH();
extern void Command_API_ENTER_SLEEP_MODE();
extern void Command_API_POWER_SAVE_MODE_RAM_DISABLE();
extern void Command_API_POWER_SAVE_MODE_RAM_ENABLE();


#endif /* FP_COMMAND_H_ */
