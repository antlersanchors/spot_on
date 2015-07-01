// Using liberal bits of the following code:
/*
  MP3 Shield Trigger
  by: Jim Lindblom
      SparkFun Electronics
  date: September 23, 2013

  Much of this code was grabbed from the FilePlayer example
  included with the SFEMP3Shield library. Major thanks to Bill
  Porter and Michael Flaga, again, for this amazing library!
*/

#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

SdFat sd; // Create object to handle SD functions

SFEMP3Shield MP3player; // Create Mp3 library object
// These variables are used in the MP3 initialization to set up
// some stereo options:
uint8_t volume = 0; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max


// Magnet stuff
int sensorVal;
bool playState = false;
const int sensorThreshold = 150;

#define SENSOR_COUNT 6
int sensorPins[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5};

int currentTime;
int previousTime;

// New sound 

void setup()
{
  Serial.begin(9600);

  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield
}

void loop()
{
	for (int i=0; i < SENSOR_COUNT; i++){
	  sensorVal = analogRead(sensorPins[i]);

	  if (sensorVal < sensorThreshold && MP3player.isPlaying(){
	  	if (MP3player.isPlaying(i+1) == false){
	  		MP3player.stopTrack();
	  		MP3player.playTrack(i+1);
		}
	  } else if (sensorVal < sensorThreshold){
	  	MP3player
	  }

	}
  // Serial.println(sensorVal);

  if (sensorVal < 200){
    playState = true;

    if (MP3player.isPlaying() == false){
      uint8_t result = MP3player.playTrack(1);
    }
  } else {
    playState = false;
    if (MP3player.isPlaying()){
      MP3player.stopTrack();
    }
  }


}

// initSD() initializes the SD card and checks for an error.
void initSD()
{
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) 
    sd.initErrorHalt();
  if(!sd.chdir("/")) 
    sd.errorHalt("sd.chdir");
}

// initMP3Player() sets up all of the initialization for the
// MP3 Player Shield. It runs the begin() function, checks
// for errors, applies a patch if found, and sets the volume/
// stero mode.
void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  if(result != 0) // check result, see readme for error codes.
  {
    // Error checking can go here!
  }
  MP3player.setVolume(volume, volume);
  MP3player.setMonoMode(monoMode);
}

