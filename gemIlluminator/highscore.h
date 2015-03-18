#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QObject>

#include "fileio.h"

/**
 * @brief The Highscore is only a semantic class to clarify that this fileIO is a Highscore.
 */
class Highscore : public FileIO
{
    Q_OBJECT
public:
    explicit Highscore(QObject *parent = 0);
    ~Highscore();

signals:

};

#endif // HIGHSCORE_H
