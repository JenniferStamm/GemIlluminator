#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class Painter : public QObject
{
    Q_OBJECT
public:
    explicit Painter(QObject *parent = 0);

signals:

public slots:

};

#endif // PAINTER_H
