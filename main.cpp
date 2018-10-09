#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

#define VOL_MAX 100

// custom audio stream that plays a loaded buffer
class MyStream : public sf::SoundStream
{
public:

    void load(const sf::SoundBuffer& buffer)
    {
        // extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position
        m_currentSample = 0;

        // initialize the base class
        initialize(buffer.getChannelCount(), buffer.getSampleRate());
    }

    void displayExtremsValuesBuffer()
    {
      int maxi = 0;
      int mini = 0;
      for(unsigned int i = 0 ; i < m_samples.size() ; i++)
      {
        if(maxi < m_samples[i] )
          maxi = m_samples[i];
        if(mini > m_samples[i])
          mini = m_samples[i];
      }
      std::cout << "max : " << maxi << " min : " << mini << std::endl;
    }

    void applyLPF()
    {
      //int maxi = 16384;
      float Beta = 0.025;
      std::vector<sf::Int16> x;//this is the input data and m_samples the exit

      for(unsigned int i = 0 ; i < m_samples.size() ; i++)
      {
        x.push_back(m_samples[i]);
      }

      for(unsigned int i = 1 ; i < m_samples.size() ; i++)
      {
        m_samples[i] = Beta*x[i] + (1-Beta)*m_samples[i-1];
        //m_samples[i] = 0.5 * ( x[i] + m_samples[i] );
      }
    }


private:

    virtual bool onGetData(Chunk& data)
    {
        // number of samples to stream every time the function is called;
        // in a more robust implementation, it should be a fixed
        // amount of time rather than an arbitrary number of samples
        const int samplesToStream = 50000;

        // set the pointer to the next audio samples to be played
        data.samples = &m_samples[m_currentSample];

        // have we reached the end of the sound?
        if (m_currentSample + samplesToStream <= m_samples.size())
        {
            // end not reached: stream the samples and continue
            data.sampleCount = samplesToStream;
            m_currentSample += samplesToStream;
            return true;
        }
        else
        {
            // end of stream reached: stream the remaining samples and stop playback
            data.sampleCount = m_samples.size() - m_currentSample;
            //m_currentSample = m_samples.size();
            //if we want to loop
            m_currentSample = 0;
            return false;
        }
    }

    virtual void onSeek(sf::Time timeOffset)
    {
        // compute the corresponding sample index according to the sample rate and channel count
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
    std::string m_fileName;
};


//This works !
void LPF_test()
{
  // load an audio buffer from a sound file
  sf::SoundBuffer buffer;
  buffer.loadFromFile("Track 10.wav");

  // initialize and play the custom stream
  MyStream stream;
  stream.load(buffer);
  stream.applyLPF();
  stream.play();

  // let it play until it is finished
  while (stream.getStatus() == MyStream::Playing)
      sf::sleep(sf::seconds(0.1f));
}

//
void generate_squareWave()
{
  sf::SoundBuffer squareWave;

  std::vector<sf::Int16> samples;


  sf::Uint64 sampleRate = 44000;
  int channelCount = 2;
  unsigned int n_sample_per_period;
  int freq = 440;

  //Initialization of the array of samples
  //Note : the array contain 44000 numbers = sampleRate, thus it corresponds to one second
  n_sample_per_period = sampleRate/freq;
  for(unsigned int i = 0 ; i < sampleRate ; i++)
  {
    if(i % n_sample_per_period < n_sample_per_period / 2)
      samples.push_back(0);
    else
      samples.push_back(VOL_MAX);
  }

  //Loading the array into the buffer
  if( squareWave.loadFromSamples(samples.data(), sampleRate, channelCount, sampleRate) )//it looks like this works
  {
    std::cout << "good" << std::endl;
  }
  else
    std::cout << "bad" << std::endl;

  MyStream stream;
  stream.load(squareWave);
  std::cout << "turfu5"<< std::endl;

  stream.play();
  //int wait;
  //std::cin >> wait;
  std::cout << "turfu6"<< std::endl;

  // let it play until it is finished
  while (stream.getStatus() == MyStream::Playing)
      sf::sleep(sf::seconds(0.1f));


}



int main()
{
    //generate_squareWave();

	LPF_test();

    return 0;
}
