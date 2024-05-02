#include "music.h"

Music::Music(std::string _name, int _channal_num, int _speed, int _length, QObject *parent)
    : QObject{parent}, name(_name)
{
    this->channal_num = _channal_num;
    this->speed = _speed;
    this->length = _length;
    for (int i = 0; i < channal_num; ++i) {
        this->allChannals[i] = new Channal(i, length);
    }
}

void Music::changeLength(int newLength) {
    this->length = newLength;
    for (int i = 0; i < channal_num; ++i) {
        this->allChannals[i]->changeLength(newLength);
    }
}

void Music::setType(int numOfChannal, int musicType) {
    assert (numOfChannal >= 0 && numOfChannal < this->channal_num);
    this->allChannals[numOfChannal]->type = musicType;
}

void Music::addNote(int channalNum, int addingPlace, int volumn, Pitch m1, Pitch m2, Pitch m3, Pitch m4) {
    this->allChannals[channalNum]->addNote(addingPlace, volumn, m1, m2, m3, m4);
}

void Music::addNote(int channalNum, int addingPlace, v_spo *note) {
    this->allChannals[channalNum]->addNote(addingPlace, note);
}

void Music::setAdditionalStrength(int channalNum, int additionalStrength) {
    this->allChannals[channalNum]->strength = additionalStrength;
}

void Music::RepeatSingleChannal(int channalNum, int copyStart, int copyEnd, int targetStart) {
    for (int i = 0; i < copyEnd - copyStart; ++i) {
        this->allChannals[channalNum]->notes[i + targetStart] = new v_spo(*this->allChannals[channalNum]->notes[i + copyStart]);
    }
}

void Music::RepeatAllChannals(int copyStart, int copyEnd, int targetStart) {
    for (int i = 0; i < this->channal_num; ++i) {
        this->RepeatSingleChannal(i, copyStart, copyEnd, targetStart);
    }
}
