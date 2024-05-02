#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>
#include "channal.h"
#include <QTimer>
#include <string>
#include "pitch.h"

/**
 * @brief The Music class represents a piece of music, including at most 10 channals.
 * @note Directly using "allChannals[]" is not recommended (except for saving and loading files). If you are doing so, there is probably an alternative way.
 * @author Cao Yu
 */
class Music : public QObject
{
    Q_OBJECT
public:
    std::string name;
    int speed = 80;
    int channal_num = 0;
    Channal* allChannals[10] = {};
    int length = 0;

public:
    explicit Music(std::string _name = "", int _channal_num = 1, int _speed = 80, int _length = 32, QObject *parent = nullptr);

    /**
     * @brief change the length of the music to newLength.
     * @note if newLength is less than current length, then delete the redundant notes.
     */
    void changeLength(int newLength);

    /**
     * @brief set or change the type of channal[numOfChannal].
     * @param musicType: refer to "instruments.h".
     */
    void setType(int numOfChannal, int musicType);

    /**
     * @brief Add or change a note at certain place.
     * @note addingPlace can only be less than channal's current length!
     */
    void addNote(int channalNum, int addingPlace, v_spo* note);
    void addNote(int channalNum, int addingPlace, int volumn, Pitch m1, Pitch m2 = _REST, Pitch m3 = _REST, Pitch m4 = _REST);

    /**
     * @brief give channal[channalNum] some additional strength.
     */
    void setAdditionalStrength(int channalNum, int additionalStrength);

    /**
     * @brief RepeatSingleChannal: Copy notes in channal[channalNum], from copyStart(include) to copyEnd(not include).
     * @example
     * Channal_0: {C4}, {C5}, r, r, r
     * RepeatSingleChannal(0, 0, 2, 3)
     * Channal_0: {C4}, {C5}, r, {C4}, {C5}
     */
    void RepeatSingleChannal(int channalNum, int copyStart, int copyEnd, int targetStart);

    void RepeatAllChannals(int copyStart, int copyEnd, int targetStart);
};

#endif // MUSIC_H
