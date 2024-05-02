#include "channal.h"
#include "assert.h"

v_spo::v_spo(Pitch v1, Pitch v2, Pitch v3, Pitch v4, int volume) {
    _v1 = v1;
    _v2 = v2;
    _v3 = v3;
    _v4 = v4;
    _volume = volume;
}

Channal::Channal(int _number, int _length, int musicType, int _strength, QObject *parent)
    : QObject{parent}
{
    assert (_number < MAX_HOLD_OF_NOTES);
    this->number = _number;
    for (int i = 0; i < _length; ++i) {
        this->notes[i] = new rest_spo();
    }
    this->length = _length;
    this->strength = _strength;
    this->type = musicType;
}

void Channal::addNote(int addingPlace, v_spo* note) {
    assert(addingPlace >= 0 && addingPlace < length);
    if (this->notes[addingPlace])
        delete this->notes[addingPlace];
    this->notes[addingPlace] = note;
}

void Channal::addNote(int addingPlace, int volumn, Pitch m1, Pitch m2, Pitch m3, Pitch m4) {
    assert(addingPlace >= 0 && addingPlace < length);
    if (this->notes[addingPlace])
        delete this->notes[addingPlace];
    this->notes[addingPlace] = new v_spo(m1, m2, m3, m4, volumn);
}

void Channal::changeType(int _type) {
    this->type = _type;
}

void Channal::changeLength(int _length) {
    if (this->length < _length) {
        for (int i = this->length; i < _length; ++i) {
            this->notes[i] = new rest_spo();
        }
    }
    if (this->length > _length) {
        for (int i = _length - 1; i >= this->length; --i) {
            if (this->notes[i]) {
                delete this->notes[i];
            }
        }
    }
    this->length = _length;
}
