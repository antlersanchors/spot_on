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
const maxVolume = 10;
uint8_t volume = maxVolume; // MP3 Player volume 0=max, 255=lowest (off)
const uint16_t monoMode = 1;  // Mono setting 0=off, 3=max

// Magnet stuff
int sensorVal;
bool playState = false;
const int sensorThreshold = 150;

#define SENSOR_COUNT 6
int sensorPins[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5};
int sensorStatus[SENSOR_COUNT] = {0, 0, 0, 0, 0, 0};
int ringCount[3] = {0, 0, 0};
unsigned int moodPlaying;

unsigned int lastUpdated;

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

	  if (sensorVal < sensorThreshold && sensorStatus[i] == 0) {
	  	if (MP3player.isPlaying(i+1) == false){
	  		MP3player.stopTrack();
	  		MP3player.playTrack(i+1);
		}
	  } else if (sensorVal > sensorThreshold){
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

// Let's check the traps!
void checkSensors() {
	for (int i=0; i < SENSOR_COUNT; i++){
	  sensorVal = analogRead(sensorPins[i]);

	  // If there's something present, was it there before?
	  if (sensorVal < sensorThreshold && sensorStatus[i] == 0) {
	  	sensorStatus[i] = 1;
	  	lastUpdated = i;
	  }

	  // If there's something missing, was it gone before?
	  if (sensorVal > sensorThreshold && sensorStatus[i] == 1){
	  	sensorStatus[i] = 0;
	  	lastUpdated = i;
	  }
	}
}

// Let's add up what we got!
void tallyRings() {
	// zero them out first? or not?
	for (int i=0; i < SENSOR_COUNT; i++) {
		while (i < 3 && sensorStatus[i] == 1){
			ringCount[0] ++;
		}
		while (i > 2 && i < 5 && sensorStatus[i] == 1){
			ringCount[1] ++;
		}
		while (i > 4 && sensorStatus[i] == 1){
			ringCount[2] ++;
		}
	}
}

int evaluateMood() {
 	int maxIndex = 0;
 	int maxCount = ringCount[maxIndex];

	for (int i=0; i < 3; i++){
		if (maxCount < ringCount[i]){
			maxCount = ringCount[i];
			maxIndex = i;
		}
	}
	// pass this to updateMusic()
	return maxIndex;

}

void updateMusic(int tempMoodSelected){
	int moodSelected = tempMoodSelected;

	if (moodSelected !! moodPlaying) {

		// pick a new track
		// *** some bullshit code to pick a new track based on mood here
		int trackToPlay = 1;

		// fade out the old one
		if (volume < 254 && (millis() % 10 == 0)){
			volume ++;
		} else if (volume == 254){
			MP3player.stopTrack();
			MP3player.playTrack(trackToPlay);
			moodPlaying = moodSelected;
		}
	}

	// fade in the new one
	if (moodSelected == moodPlaying && volume > maxVolume){
		if (volume > maxVolume && (millis() % 10 == 0)){
			volume --;
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

