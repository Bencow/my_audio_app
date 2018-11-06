#include "MyStream.h"


void MyStream::loadFile(std::string fileName)
{
	//if we cannot open the file :
	if (!m_file.openFromFile(fileName))
	std::cout << "error occured while opening audio file";


   m_sample_number = m_file.getSampleRate();
   //dynamically allocate m_sample
   //The size of m_sample is allways of one second of music
   //so, the number of elements in the array is equal to the sampleRate
   m_data_in = (sf::Int16*) malloc(sizeof(sf::Int16) * m_sample_number);
   m_data_out = (sf::Int16*) malloc(sizeof(sf::Int16) * m_sample_number);

   //initialize the channel count and sampleRate
   initialize(m_file.getChannelCount(), m_file.getSampleRate());

   //other initialization should not be there after...
   m_current_effect = 2;//LPF

}

bool MyStream::onGetData(Chunk& data)
{
   uint number_sample_read;
   //Param : 1 pointer to the array to fill
   //        2 number of sample to read (if the file is not finished) (i.e. size of the array)
   //          here we send the sampleRate to read exactly 1 second of music
   //return : the number of sample actually read

   m_previous_sample = m_data_in[m_sample_number];

   number_sample_read = m_file.read(m_data_in, m_sample_number);

   //apply the current effect on m_data_in
   applyEffect();

   //Put the pointer to the new audio sample to be played
   data.samples = m_data_out;
   data.sampleCount = m_sample_number;

    
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

void MyStream::applyEffect()
{
   switch(m_current_effect)
   {
      case 1:
         volumeControl();
         break;
      case 2:
         applyLPF();
         break;

   }
}
void MyStream::volumeControl()
{

}

void MyStream::applyLPF()
{
   float Beta = 0.025;//should not be harcoded

   //calculate the first case
   m_data_out[0] = Beta*m_data_in[0] + (1 - Beta)*m_previous_sample;

   //start at one because we are using i-1 index
   for(unsigned int i = 1 ; i < m_sample_number ; ++i)
   {
      // m_data_in[i] = Beta*x[i] + (1-Beta)*m_data_in[i-1];
      m_data_out[i] = Beta*m_data_in[i] + (1 - Beta)*m_data_in[i-1];
      
      // m_data_out[i] = m_data_in[i]; 
   }
}