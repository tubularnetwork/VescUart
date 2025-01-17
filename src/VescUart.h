#ifndef _VESCUART_h
#define _VESCUART_h

#include <Arduino.h>
#include "datatypes.h"
#include "buffer.h"
#include "crc.h"

class VescUart
{
	/** Struct to store the telemetry data returned by the VESC */
	struct dataPackage {
		float avgMotorCurrent;
		float avgInputCurrent;
		float dutyCycleNow;
		long rpm;
		float inpVoltage;
		float ampHours;
		float ampHoursCharged;
		long tachometer;
		long tachometerAbs;
	};

	/** Struct to hold the nunchuck values to send over UART */
	struct nunchuckPackage {
		int	valueX;
		int	valueY;
		bool upperButton; // valUpperButton
		bool lowerButton; // valLowerButton
	};


	public:
		/**
		 * @brief      Class constructor
		 */
		VescUart(void);

		/** Variabel to hold measurements returned from VESC */
		dataPackage data; 

		/** Variabel to hold nunchuck values */
		nunchuckPackage nunchuck; 

		/**
		 * @brief      Set the serial port for uart communication
		 * @param      port  - Reference to Serial port (pointer) 
		 */
		void setSerialPort(HardwareSerial* port);

		/**
		 * @brief      Set the serial port for debugging
		 * @param      port  - Reference to Serial port (pointer) 
		 */
		void setDebugPort(Stream* port);

		/**
		 * @brief      Sends a command to VESC and stores the returned data, data is replaced each call even if for different controllers
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 *
		 * @return     True if successfull otherwise false
		 */
		bool getVescValues(int controllerId=-1);

		/**
		 * @brief      Find any CAN connected controllers
		 */
		bool scanCAN(void);

		/**
		 * @brief      Set motor to run in FOC Openloop mode. This will only run for about 5 sec and is hard on the motors
		 * @param	   current - The current to run at. Low current and low RPM will cause vibration/cogging
		 * @param	   eRPM - The eRPM (RPM * Pole-pair count) to run the motor at
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void focOpenloop(float current, int eRPM, int controllerId=-1);

		/**
		 * @brief      Stop motor
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void stopCmd(int controllerId=-1);

		/**
		 * @brief      Sends values for joystick and buttons to the nunchuck app
		 */
		void setNunchuckValues(void);

		/**
		 * @brief      Set the current to drive the motor
		 * @param      current  - The current to apply
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void setCurrent(float current, int controllerId=-1);

		/**
		 * @brief      Set the current to brake the motor
		 * @param      brakeCurrent  - The current to apply
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void setBrakeCurrent(float brakeCurrent, int controllerId=-1);

		/**
		 * @brief      Set the rpm of the motor
		 * @param      rpm  - The desired RPM (actually eRPM = RPM * poles)
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void setRPM(float rpm, int controllerId=-1);

		/**
		 * @brief      Set the duty of the motor
		 * @param      duty  - The desired duty (0.0-1.0)
		 * @param      controllerId  - If specified the command will be sent over CAN to the specified controller ID
		 */
		void setDuty(float duty, int controllerId=-1);

		/**
		 * @brief      Help Function to print struct dataPackage over Serial for Debug
		 */
		void printVescValues(void);

	private: 

		/** Variabel to hold the reference to the Serial object to use for UART */
		HardwareSerial* serialPort = NULL;

		/** Variabel to hold the reference to the Serial object to use for debugging. 
		  * Uses the class Stream instead of HarwareSerial */
		Stream* debugPort = NULL;

		/**
		 * @brief      Packs the payload and sends it over Serial
		 *
		 * @param      payload  - The payload as a unit8_t Array with length of int lenPayload
		 * @param      lenPay   - Length of payload
		 * @return     The number of bytes send
		 */
		int packSendPayload(uint8_t * payload, int lenPay);

		/**
		 * @brief      Receives the message over Serial
		 *
		 * @param      payloadReceived  - The received payload as a unit8_t Array
		 * @return     The number of bytes receeived within the payload
		 */
		int receiveUartMessage(uint8_t * payloadReceived);

		/**
		 * @brief      Verifies the message (CRC-16) and extracts the payload
		 *
		 * @param      message  - The received UART message
		 * @param      lenMes   - The lenght of the message
		 * @param      payload  - The final payload ready to extract data from
		 * @return     True if the process was a success
		 */
		bool unpackPayload(uint8_t * message, int lenMes, uint8_t * payload);

		/**
		 * @brief      Extracts the data from the received payload
		 *
		 * @param      message  - The payload to extract data from
		 * @return     True if the process was a success
		 */
		bool processReadPacket(uint8_t * message);

		/**
		 * @brief      Help Function to print uint8_t array over Serial for Debug
		 *
		 * @param      data  - Data array to print
		 * @param      len   - Lenght of the array to print
		 */
		void serialPrint(uint8_t * data, int len);

};

#endif
