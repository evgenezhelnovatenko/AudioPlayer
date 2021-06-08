#ifndef AUTOR_H
#define AUTOR_H

#include <QObject>
#include "person.h"

class Autor : public Person
{
    Q_OBJECT
//    Q_PROPERTY(QString firstname READ firstname WRITE setFirstname NOTIFY firstnameChanged)
//    Q_PROPERTY(QString lastname READ lastname WRITE setLastname NOTIFY lastnameChanged)
//    Q_PROPERTY(QString pseudonym READ pseudonym WRITE setPseudonym NOTIFY pseudonymChanged)

public:
    explicit Autor();
    explicit Autor(const Autor &other, QObject *parent);
    explicit Autor(int id, QString firstname, QString lastname, QString pseudonym);



//signals:
//    void firstnameChanged(QString firstname);

//    void lastnameChanged(QString lastname);

//    void pseudonymChanged(QString pseudonym);


    // Person interface
//public:
//    void setFirstname(QString firstname) override;
//    void setLastname(QString lastname) override;
//    void setPseudonym(QString pseudonym) override;
};

Q_DECLARE_METATYPE(Autor);

#endif // AUTOR_H
