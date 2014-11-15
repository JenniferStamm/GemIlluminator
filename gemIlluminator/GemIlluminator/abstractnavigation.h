#ifndef ABSTRACTNAVIGATION_H
#define ABSTRACTNAVIGATION_H

#include <QObject>

class AbstractNavigation : public QObject
{
    Q_OBJECT
public:
    explicit AbstractNavigation(QObject *parent = 0);

signals:

public slots:

};

#endif // ABSTRACTNAVIGATION_H
