#ifndef COAUTOR_H
#define COAUTOR_H

#include <QObject>
#include "person.h"

class CoAutor : public Person
{
    Q_OBJECT
public:
    explicit CoAutor();
    explicit CoAutor(int id, QString firstname, QString lastname, QString pseudonym);

};

#endif // COAUTOR_H
