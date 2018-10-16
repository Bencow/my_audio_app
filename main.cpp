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
		// Print the sound attributes
		std::cout << "duration: " << m_file.getDuration().asSeconds() << std::endl;
		std::cout << "channels: " << m_file.getChannelCount() << std::endl;
		std::cout << "sample rate: " << m_file.getSampleRate() << std::endl;
        //total number of audio sample in the file
		std::cout << "sample count: " << m_file.getSampleCount() << std::endl;
		
        //dynamically allocate m_sample
        //The size of m_sample is allways of one second of music
        //so, the number of elements in the array is equal to the sampleRate
        m_samples = (sf::Int16*) malloc(sizeof(sf::Int16) * m_file.getSampleRate());

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
        sf::Uint64 number_sample_read; 
        

        //Param : 1 pointer to the array to fill
        //        2 number of sample to read (if the file is not finished) (i.e. size of the array)
        //          here we send the sampleRate to read exactly 1 second of music
        //return : the number of sample actually read
        
        number_sample_read = m_file.read(m_samples, getSampleRate());//have to know if this works

        //Put the pointer to the new audio sample to be played
        data.samples = m_samples;

        //if(number_sample_read == 0)
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
    std::string        m_fileName;
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



int main()
{
    //LPF_test();
    
    /*
    MyStream str;
    str.loadFile("Track 10.wav");
    str.play();
    */
    
    sf::InputSoundFile file;
    file.openFromFile("start.wav");
    uint channelCount = file.getChannelCount();
    uint sampleRate = file.getSampleRate();
   

    sf::Int16 samples[BUFFER_SIZE];
    //we can't file directly a buffer with this method
    //and inputFile has only one read method... 
    //so i fill an array of samples
    sf::Uint64 samplesActuallyRead = file.read(samples, BUFFER_SIZE);
    /*
    for(uint i = 0 ; i < samplesActuallyRead ; ++i)
    {
        std::cout << samples[i]<< " ";
    }*/


    //then create a buffer with it
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(samples, BUFFER_SIZE, channelCount, sampleRate);

    //then load the buffer in a sound
    sf::Sound mySound;
    mySound.setBuffer(buffer);
    mySound.play();

    int wait;
    std::cin >> wait;

    return 0;
}
  