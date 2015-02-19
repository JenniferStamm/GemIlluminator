#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>

class soundmanager : public QObject
{
    Q_OBJECT
public:
    explicit soundmanager(QObject *parent = 0);
    ~soundmanager();

signals:

public slots:
};

#endif // SOUNDMANAGER_H
