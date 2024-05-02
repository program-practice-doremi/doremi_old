#ifndef TEST_H
#define TEST_H

#include "music.h"
#include "mediaplayer.h"
#include "instruments.h"
#include "pitch.h"

/**
 * @brief These are all the tests when debugging.
 */
class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);

    void Test1();
    void Test2();

signals:
};

#endif // TEST_H
