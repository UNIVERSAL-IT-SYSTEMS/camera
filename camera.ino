
/*

  BLANC <==> GND
  
  JAUNE <==> VCC = 5V
  
  ROUGE <==> RX ou TX
  
  NOIR <==> RX ou TX
  
  
*/

#include <avr/pgmspace.h>
#include "uCamlib.h"
#include <SoftwareSerial.h>

SoftwareSerial Uart(2, 3);
uCamlib camera = uCamlib(&Uart);
static const uint8_t TEENSY_LED = 13;
static const uint8_t PACKAGE_SIZE = 60;
static const uint8_t BUFFER_SIZE = 4800;
static const uint8_t SKIP_FRAMES = 10;
uint8_t RxBuffer[BUFFER_SIZE];
uint16_t pictureSizeCount = 0;
uint16_t pictureSize = 4800;
byte cmd = '0';



void setup() 
{ 
    pinMode(TEENSY_LED, OUTPUT);

    Serial.begin(115200); // initialize USB communication w/host computer 
    Uart.begin(115200);  // initialize TTL serial communication w/camera
    delay(3000);
    digitalWrite( TEENSY_LED, HIGH ); // indicate power on teensy
    Serial.println(" Test de la camera 4D System");
    
    for(int y = 0; y <10; y++)
    {
    digitalWrite(TEENSY_LED, HIGH);  // set the LED on
    delay(100);
    digitalWrite(TEENSY_LED, LOW);  // set the LED on
    delay(100);
    }
    //setupCam();
}


void loop() 
{
  //digitalWrite(TEENSY_LED, HIGH);  // set the LED on  
  //getRAW();
  
  if(Serial.available () > 0)
  {
    cmd = Serial.read();
    
    if(cmd == '2')
    {
      cmd = '0';
      if(camera.sync())
        Serial.println(" Syncronisation de la camera avec succes");
       else
         Serial.println(" Probleme de Syncronisation");
    }
    
    if(cmd == '3')
    {
      cmd = '0';
      if(camera.initial(uCamlib::CT_GRAYSCALE_8, uCamlib::RR_80x60, uCamlib::JR_160x128))
        Serial.println(" Initialisation de la camera");
      else
        Serial.println(" Probleme d'Initialisation de la camera");
    }
    
    if(cmd == '5')
    {
      cmd = '0';
      
      if(camera.getPicture( uCamlib::PT_RAW, PROCESS_DELAY, pictureSize ))
      {
        Serial.print(" Picture size = ");Serial.println(pictureSize);
        if(pictureSize > 4800)
        {
          Serial.println(" Picture size bigger than the picture buffer");
        }
        else
        {
          // Wait for the package
          if( camera.waitForResponse( PROCESS_DELAY, RxBuffer, pictureSize ) ) 
          {
            camera.sendACK( 0x0A );
            Serial.println(" Get picture succed");
            
            Serial.println(" print picture start");
            for(int i = 0; i < pictureSize; i++)
              Serial.write(byte(RxBuffer[i]));
            Serial.println(" print picture end");
          }
        }
        
      }
      else
        Serial.println(" Get picture failed");
        
      Serial.print(camera._command[0], HEX);
      Serial.print(camera._command[1], HEX);
      Serial.print(camera._command[2], HEX);
      Serial.print(camera._command[3], HEX);
      Serial.print(camera._command[4], HEX);
      Serial.println(camera._command[5], HEX);
        
      //getRAW();
    }
    
    if(cmd == 'r')
    {
      cmd = '0';
      camera.reset( true );
      delay(2000);
      Serial.println("Reset de la camera");
    }
  }
  
  //digitalWrite(TEENSY_LED, LOW);   // set the LED off
  //getJPEG();
  
  
  
}


boolean setupCam()
{
    // synch camera
    if( !camera.sync() ) 
    {
      Serial.println(F( "Sync failed.") );
      return false;
    }
    else
    {
      Serial.println(F( "Sync successful.") );
    }
    // end synch
    // initialize camera
    if( !camera.initial( uCamlib::CT_COLOR_8, uCamlib::RR_80x60, uCamlib::JR_640x480 ) )
    {
      Serial.println( F("Initial failed.") ); 
      return false;
    }
    else 
    {
    Serial.println(F( "Initial successful.") );
    }
    // end initialize
    // setPackageSize
    if( !camera.setPackageSize( PACKAGE_SIZE ) )
    {
      Serial.println(F( "Package size failed.") ); 
      return false;
    }
    else
    {
      Serial.println(F( "Package size successful." ));
    }
    // end Package size
    // set external light frequency 50Hz or 60Hz for flicker compensation
    if( !camera.setLightFrequency( uCamlib::FT_60Hz ) )
    {
      Serial.println(F( "Light frequency failed." ));
      return false;
    }
    else
    {
      Serial.println(F( "Light frequency successful." ));
    }
    // end set external light frequency
    // Let camera settle, per manual
    delay(2000);
}

boolean getRAW()
{
  /*
    // snapshot, holds image in camera buffer
    if( !camera.snapshot( uCamlib::ST_UNCOMPRESSED, SKIP_FRAMES ) )
    {
      Serial.println(F( "RAW snapshot failed." ));
      return false;
    }
    else
    {
      Serial.println(F( "RAW snapshot successful.") );
    }
    // end snapshot
    */
    
    // get RAW picture
    if( !camera.getRawPicture(uCamlib::PT_RAW, RxBuffer, BUFFER_SIZE, PROCESS_DELAY ))
    {
      Serial.println(F( "Get RAW picture failed.") );
      return false;
    }
	else
	{
      Serial.println(F( "Get RAW picture successful." ));
    }
    // end RAW picture
    
}	


boolean getJPEG()
{
    if( !camera.snapshot(uCamlib::ST_COMPRESSED, SKIP_FRAMES))
	{
	  Serial.println(F( "JPEG snapshot failed." ));
      return false;
	}
	else
    {
      Serial.println(F( "JPEG snapshot successful.") );
    }
	
	if( !camera.getJPEGPicture(uCamlib::PT_JPEG, PROCESS_DELAY,  &getJPEGPicture_callback))
	{
	  Serial.println(F( "Get JPEG failed.") );
      return false;
	}
	else
    {
      Serial.println(F( "Get JPEG successful." ));
    }
}


	
void getJPEGPicture_callback( uint16_t pictureSize, uint16_t packageSize, uint16_t packageCount, byte* package )
{//sends JPEG to computer over USB
  // packageSize is the size of the picture part of the package
  pictureSizeCount += packageSize;

  for( uint16_t i = 0; i < packageSize; i++ )
  {
    //Serial.print (package[i], BYTE);
    Serial.write (byte (package[i]));

  }

  if( pictureSizeCount == pictureSize )
  {

    Serial.flush();
    //Serial.print(char(69) , BYTE); // E
    //Serial.print(char(79) , BYTE); //O
    //Serial.print(char(70) , BYTE); //F
    Serial.write(byte (char(69))); // E
    Serial.write(byte (char(79))); //O
    Serial.write(byte (char(70))); //F
    pictureSizeCount = 0;
  }
} 
 
