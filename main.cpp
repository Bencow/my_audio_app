#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

#define VOL_MAX 100
#define BUFFER_SIZE 100000

// custom audio stream that plays a loaded buffer
class MyStream : public sf::SoundStream
{
public:

	void loadFile(std::string fileName)
	{
		//if we cannot open the file :
		if (!m_file.openFromFile(fileName))
			std::cout << "error occured while opening audio file";
		
        //dynamically allocate m_sample
        //The size of m_sample is allways of one second of music
        //so, the number of elements in the array is equal to the sampleRate
        m_samples = (sf::Int16*) malloc(sizeof(sf::Int16) * m_file.getSampleRate());
        //initialize the channel count and sampleRate
        initialize(m_file.getChannelCount(), m_file.getSampleRate());
	}
    void displayStream()
    {
        Chunk data;
        onGetData(data);

        //for(int i = 0 ; i < data.
    }

private:

    virtual bool onGetData(Chunk& data)
    {
        uint number_sample_read;
        //Param : 1 pointer to the array to fill
        //        2 number of sample to read (if the file is not finished) (i.e. size of the array)
        //          here we send the sampleRate to read exactly 1 second of music
        //return : the number of sample actually read
        
        number_sample_read = m_file.read(m_samples, getSampleRate());
        //Put the pointer to the new audio sample to be played
        data.samples = m_samples;
        data.sampleCount = getSampleRate();

        
        if(number_sample_read == 0)
        {
            return false;
        }

        return true;
        
        
        //Have we reached the end of the file
        /*
        if(m_file.getSampleOffset() + getSampleRate() <= m_file.getSampleCount())
        {
            data.sampleCount = getSampleRate();
            return true;
        }
        else//the end of the file is reached
        {
            //read what's left
            data.sampleCount = m_file.getSampleCount() - m_file.getSampleOffset();
            return false;//stop playback
        }
        */
        /*
        // have we reached the end of the sound?
        if (m_currentSample + m_sampleRate <= m_samples.size())
        {
            // end not reached: stream the samples and continue
            data.sampleCount = m_sampleRate;
            m_currentSample += m_sampleRate;
            return true;
        }
        else//we haven't reached the end
        {
            // end of stream reached: stream the remaining samples and stop playback
            data.sampleCount = m_samples.size() - m_currentSample;
            //m_currentSample = m_samples.size();
            //if we want to loop
            m_currentSample = 0;
            return false;//playback will stop
        }
        */
    }

    virtual void onSeek(sf::Time timeOffset)
    {
        // compute the corresponding sample index according to the sample rate and channel count
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

    //We need to have the file as member of this class
    sf::InputSoundFile m_file;
    sf::Int16*         m_samples;
    std::size_t        m_currentSample;
};


//This works !
void LPF_test()
{
	/*
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
      */
}



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
    //LPF_test();
    
    
    MyStream str;
    str.loadFile("start.wav");
    str.play();
    //WARNING : if there's no wait function (ie the main thread finish before the playing one) the program crash 
    //          "pure virtual method called (probably the destructor...) 
    //          terminate called without an exception
    //          Aborted (core dumped)"
    int wait;
    std::cin >> wait;

    return 0;
}
  