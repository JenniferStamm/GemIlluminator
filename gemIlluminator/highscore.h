#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QObject>

#include "fileio.h"

class Highscore : public FileIO
{
    Q_OBJECT
public:
    explicit Highscore(QObject *parent = 0);
    ~Highscore();

signals:

};

#endif // HIGHSCORE_H
