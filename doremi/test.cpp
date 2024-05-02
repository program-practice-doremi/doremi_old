#include "test.h"

Test::Test(QObject *parent)
    : QObject{parent}
{}

void Test::Test1() {
    Music* DoReMi = new Music("DoReMi", 1, 80, 9);
    DoReMi->setType(0, Acoustic_Grand_Piano);
    DoReMi->addNote(0, 0, 5, C5);
    DoReMi->addNote(0, 2, 7, D5);
    DoReMi->addNote(0, 4, 9, E5);

    MediaPlayer *player = new MediaPlayer();
    player->SetMusic(DoReMi);
    player->PlayMusic();
}

void Test::Test2() {
    Music* TwinkleTwinkleLittleStar = new Music("TwinkleTwinkleLittleStar", 2, 60, 35);
    TwinkleTwinkleLittleStar->setType(0, Music_Box);
    TwinkleTwinkleLittleStar->setType(1, Acoustic_Grand_Piano);
    TwinkleTwinkleLittleStar->setAdditionalStrength(0, 4);
    TwinkleTwinkleLittleStar->setAdditionalStrength(1, -2);

    TwinkleTwinkleLittleStar->addNote(0, 0, 6, C5);
    TwinkleTwinkleLittleStar->addNote(0, 2, 6, C5);
    TwinkleTwinkleLittleStar->addNote(0, 4, 6, G5);
    TwinkleTwinkleLittleStar->addNote(0, 6, 6, G5);
    TwinkleTwinkleLittleStar->addNote(0, 8, 6, A5);
    TwinkleTwinkleLittleStar->addNote(0, 10, 6, A5);
    TwinkleTwinkleLittleStar->addNote(0, 12, 6, G5);

    TwinkleTwinkleLittleStar->addNote(0, 16, 6, F5);
    TwinkleTwinkleLittleStar->addNote(0, 18, 6, F5);
    TwinkleTwinkleLittleStar->addNote(0, 20, 6, E5);
    TwinkleTwinkleLittleStar->addNote(0, 22, 6, E5);
    TwinkleTwinkleLittleStar->addNote(0, 24, 6, D5);
    TwinkleTwinkleLittleStar->addNote(0, 26, 6, D5);
    TwinkleTwinkleLittleStar->addNote(0, 28, 6, C5);

    TwinkleTwinkleLittleStar->addNote(1, 0, 6, C4);
    TwinkleTwinkleLittleStar->addNote(1, 1, 6, G4);
    TwinkleTwinkleLittleStar->addNote(1, 2, 6, E4);
    TwinkleTwinkleLittleStar->addNote(1, 3, 6, G4);

    TwinkleTwinkleLittleStar->RepeatSingleChannal(1, 0, 4, 4);
    TwinkleTwinkleLittleStar->RepeatSingleChannal(1, 0, 8, 8);

    TwinkleTwinkleLittleStar->addNote(1, 9, 6, A4);
    TwinkleTwinkleLittleStar->addNote(1, 10, 6, F4);
    TwinkleTwinkleLittleStar->addNote(1, 11, 6, A4);

    TwinkleTwinkleLittleStar->addNote(1, 16, 6, G3);
    TwinkleTwinkleLittleStar->addNote(1, 17, 6, F4);
    TwinkleTwinkleLittleStar->addNote(1, 18, 6, D4);
    TwinkleTwinkleLittleStar->addNote(1, 19, 6, F4);

    TwinkleTwinkleLittleStar->RepeatSingleChannal(1, 16, 20, 20);
    TwinkleTwinkleLittleStar->RepeatSingleChannal(1, 16, 20, 24);
    TwinkleTwinkleLittleStar->addNote(1, 21, 6, E4);
    TwinkleTwinkleLittleStar->addNote(1, 22, 6, C4);
    TwinkleTwinkleLittleStar->addNote(1, 23, 6, E4);

    TwinkleTwinkleLittleStar->addNote(1, 28, 6, C3, E3, G3, C4);


    MediaPlayer *player = new MediaPlayer();
    player->SetMusic(TwinkleTwinkleLittleStar);
    player->PlayMusic();
}
