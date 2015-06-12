
/**
*
* @file		ucam.h
* @brief	Prototype function for the 4D system uCAM  
* @author	Theodore ATEBA, tf.ateba@gmail.com	
* @version	1.0.
* @date		11 June 2015
*
*/

	#ifndef UCAM_H
	#define	UCAM_H
	
		//=====================================================================
		//	macro for command set
		//=====================================================================
			#define	STAR_BYTE				0xAA
			#define CMD_INITIAL				0x01
			#define CMD_GET_PICTURE			0x04
			#define CMD_SNAPSHOT			0x05
			#define CMD_SET_PACKAGE_SIZE	0x06
			#define CMD_SET_BAUD_RATE		0x07
			#define CMD_RESET				0x08
			#define CMD_DATA				0x0A
			#define CMD_SYNC				0x0D
			#define CMD_ACK					0x0E
			#define CMD_NAK					0x0F
			#define CMD_LIGHT				0x13
			
			
		//=====================================================================
		//	macro for color types, page 8 of the data sheet
		//=====================================================================
			#define COLOR_TYPE1	0x01	// 2-bit Gray Scale (RAW, 2-bit for Y only) 
			#define COLOR_TYPE2	0x02	// 4-bit Gray Scale (RAW, 4-bit for Y only) 
			#define COLOR_TYPE3	0x03	// 8-bit Gray Scale (RAW, 8-bit for Y only)
			#define COLOR_TYPE4	0x04	// 8-bit Colour (RAW, 332(RGB))
			#define COLOR_TYPE5	0x05	// 12-bit Colour (RAW, 444(RGB))
			#define COLOR_TYPE6	0x06	// 16-bit Colour (RAW, 565(RGB)) 
			#define COLOR_TYPE7	0x07	// JPEG
			
		//=====================================================================
		//	macro for RAW Resolution, page 9 of the data sheet
		//=====================================================================			
			#define RAW_80x60	0x01
			#define RAW_160x120	0x03
			#define	RAW_320x240	0x05
			#define	RAW_640x480	0x07
			#define	RAW_128x128	0x09
			#define RAW_128x96	0x0B
		
		//=====================================================================
		//	macro for JPEG Resolution, page 9 of the data sheet
		//=====================================================================
			#define JPEG_80x64		0x01
			#define JPEG_160x128	0x03
			#define JPEG_320x240	0x05
			#define JPEG_640x480	0x07
			
		//=====================================================================
		//	macro for Picture type, page 9 of the data sheet
		//=====================================================================
			#define SNAPSHOT_PICTURE	0x01
			#define RAW_PICTURE			0x02
			#define JPEG_PICTURE		0x05
			
		//=====================================================================
		//	macro for Picture type, page 9 of the data sheet
		//=====================================================================
			#define CMD_RESET_SYSTEM		0x00
			#define CMD_RESET_STATE_MACHINE	0x01
			
		//=====================================================================
		//	macro for Picture type, page 9 of the data sheet
		//=====================================================================
			#define PICTURE_COMPRESSED		0x00	// (JPEG)
			#define PICTURE_UNCOMPRESSED	0x01	// (RAW)
		//=====================================================================
		//	Include Libraries
		//=====================================================================

		//=====================================================================
		//	Prototypes des fonction locales
		//=====================================================================
		
			/** 
			* @fn		void initial();
			* @brief	Initialisation of the Camera
			* 
			*/
			void initial (image_size, color_type);
			
			void getPicture ();
			void snapshot ();
			void setPackageSize ();
			void setBaudRate ( int baud_rate );
			void reset ();
			void data();
			void sync ();
			void ack ();
			void nack ();
			void ligth ();
		
	#endif	/* UCAM_H */