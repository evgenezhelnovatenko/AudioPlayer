#ifndef AUTOR_H
#define AUTOR_H

#include <QObject>
#include "person.h"

class Autor : public Person
{
    Q_OBJECT
public:
    explicit Autor();
    explicit Autor(int id, QString firstname, QString lastname, QString pseudonym);

};

#endif // AUTOR_H
