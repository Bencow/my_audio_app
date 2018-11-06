#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>


#include "MyStream.h"

#define VOL_MAX 100
#define BUFFER_SIZE 100000





//This works !
/*
void LPF_test()
{
	
  // load an audio buffer from a sound file
  sf::SoundBuffer buffer;
  buffer.loadFromFile("Track 10.wav");

  // initialize and play the custom stream
  MyStream stream();
  stream.load(buffer);
  stream.applyLPF();
  stream.play();

  // let it play until it is finished
  while (stream.getStatus() == MyStream::Playing)
      sf::sleep(sf::seconds(0.1f));
      
}*/

void stream_audio()
{
	//stream.play();
}

void playSongWithEvents()
{

    sf::InputSoundFile file;
    file.openFromFile("start.wav");
    uint channelCount = file.getChannelCount();
    uint sampleRate = file.getSampleRate();
    
    //we can't file directly a buffer with this method
    //and inputFile has only one read method... 
    //so i fill an array of samples
    sf::Int16 samples[BUFFER_SIZE];
    sf::SoundBuffer buffer;
    sf::Sound mySound;
    int wait;

    while(1)
    {
        //read the file and fill raw samples array
        file.read(samples, BUFFER_SIZE);


        //WHY "class sf::InputSoundFile’ has no member named ‘getSampleOffset’"
        ////////////////////////////
        file.getSampleCount();
        //file.getSampleOffset();   //
        ////////////////////////////


        //fill the buffer with the samples
        buffer.loadFromSamples(samples, BUFFER_SIZE, channelCount, sampleRate);
        //link the buffer to the Sound object
        mySound.setBuffer(buffer);
        //play
        mySound.play();


        std::cin >> wait;
    }
}

int main()
{
    /*MyStream str;
    str.loadFile("start.wav");
    str.play();
    //WARNING : if there's no wait function (ie the main thread finish before the playing one) the program crash 
    //          "pure virtual method called (probably the destructor...) 
    //          terminate called without an exception
    //          Aborted (core dumped)"
    int wait;
    std::cin >> wait;
    */

    int* arr = (int*)malloc(sizeof(int)* 5);
    for(int i = 0 ; i < 5 ; ++i)
    {
      arr[i] = 1043;
    }
    for(int i = 0 ; i < 6 ; ++i)
    {
      std::cout << arr[i] << " ";
    }
    return 0;
}
  