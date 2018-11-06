//Author: Benoit Coville
//student number: c3316165
//course: ELEC
//project BENDO audio dsp

#include <SFML/Audio.hpp>
#include <iostream>

// custom audio stream that plays a loaded buffer
class MyStream : public sf::SoundStream
{
public:
	void loadFile(std::string fileName);
	void applyEffect();

	void volumeControl();
	void applyLPF();

private:
    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);



    //We need to have the file as member of this class
    sf::InputSoundFile m_file;
    sf::Int16* m_data_in;
    sf::Int16 m_previous_sample;
    sf::Int16* m_data_out;
   	size_t m_sample_number;
    std::size_t m_currentSample;
    
    // 1: volume
    // 2: lpf
    int m_current_effect;
};