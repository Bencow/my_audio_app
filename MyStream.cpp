#include "MyStream.h"


void MyStream::loadFile(std::string fileName)
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

bool MyStream::onGetData(Chunk& data)
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
}

void MyStream::onSeek(sf::Time timeOffset)
{
    // compute the corresponding sample index according to the sample rate and channel count
    m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}