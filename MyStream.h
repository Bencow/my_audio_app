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

private:
    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);



    //We need to have the file as member of this class
    sf::InputSoundFile m_file;
    sf::Int16*         m_samples;
    std::size_t        m_currentSample;
};