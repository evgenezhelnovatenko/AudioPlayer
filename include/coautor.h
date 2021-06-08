#ifndef COAUTOR_H
#define COAUTOR_H

#include <QObject>
#include "person.h"

class CoAutor : public Person
{
    Q_OBJECT
//    Q_PROPERTY(QString firstname READ firstname WRITE setFirstname NOTIFY firstnameChanged)
//    Q_PROPERTY(QString lastname READ lastname WRITE setLastname NOTIFY lastnameChanged)
//    Q_PROPERTY(QString pseudonym READ pseudonym WRITE setPseudonym NOTIFY pseudonymChanged)


public:
    explicit CoAutor();
    explicit CoAutor(const CoAutor& other, QObject *parent = nullptr);
    explicit CoAutor(int id, QString firstname, QString lastname, QString pseudonym);


    // Person interface
//public:
//    void setFirstname(QString firstname) override;
//    void setLastname(QString lastname) override;
//    void setPseudonym(QString pseudonym) override;


//signals:
//    void firstnameChanged(QString m_firstname);
//    void lastnameChanged(QString lastname);
//    void pseudonymChanged(QString pseudonym);
};

Q_DECLARE_METATYPE(CoAutor)

#endif // COAUTOR_H
