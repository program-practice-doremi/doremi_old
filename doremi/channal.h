#ifndef CHANNAL_H
#define CHANNAL_H

#include <QObject>
#include "pitch.h"

#define MAX_HOLD_OF_NOTES 3000

/**
 * @brief A channal is composed of a lot of v_spo("voice_sample"), like notes, which can only hold 4 pitches.
 * @author Cao Yu
 */
class v_spo {
public:
    Pitch _v1 = _REST, _v2 = _REST, _v3 = _REST, _v4 = _REST;
    int _volume = 0;
public:
    v_spo(Pitch v1 = _REST, Pitch v2 = _REST, Pitch v3 = _REST, Pitch v4 = _REST, int volume = 0);
};

class rest_spo: public v_spo {
public:
    rest_spo(): v_spo(_REST) { }
};

class stop_spo: public v_spo {
public:
    stop_spo(): v_spo(_STOP, _STOP, _STOP, _STOP) { }
};

/**
 * @brief The Channal class holds not only a vector of notes(v_spo), but its length and music_type.
 * @author Cao Yu
 */
class Channal : public QObject
{
    Q_OBJECT
public:
    int number;
    int length;
    int type; // Type of instrument.


    /**
     * @brief strength is added to each note while playing. It can be negative.
     * @example
     * Channal_0: { {c4(7)}, {c4(6)}, {g4(7)}, {g4(8)}, strength = 1 }
     *                  8        7        8        9    // actual volume
     */
    int strength = 0;

    v_spo* notes[MAX_HOLD_OF_NOTES] = {};

public:
    explicit Channal(int _number, int _length = 8, int musicType = 0, int _strength = 0, QObject *parent = nullptr);

    /**
     * @brief Add or change a note at certain place.
     * @note addingPlace can only be less than channal's current length!
     * @author Cao Yu
     */
    void addNote(int addingPlace, v_spo* note);
    void addNote(int addingPlace, int volumn, Pitch m1, Pitch m2 = _REST, Pitch m3 = _REST, Pitch m4 = _REST);

    /**
     * @brief Change the type of instrument.
     * @param _type: 0-127, representing instruments.
     */
    void changeType(int _type);

    void changeLength(int _length);

signals:
};

#endif // CHANNAL_H
