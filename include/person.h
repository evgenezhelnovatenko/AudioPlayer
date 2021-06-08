#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString firstname READ firstname WRITE setFirstname NOTIFY firstnameChanged)
    Q_PROPERTY(QString lastname READ lastname WRITE setLastname NOTIFY lastnameChanged)
    Q_PROPERTY(QString pseudonym READ pseudonym WRITE setPseudonym NOTIFY pseudonymChanged)


public:
    explicit Person(QObject *parent = nullptr);
    explicit Person(const Person& other, QObject *parent = nullptr);
    explicit Person(int id, QString firstname, QString lastname, QString pseudonym);

    void setId(int id);						  // set id.
    int id() const;								  // get id.
    void setFirstname(QString firstname); // set firsname.
    QString firstname() const;				  // get firstname.
    void setLastname(QString lastname);   // set lastname.
    QString lastname() const;					  // get lastname.
    void setPseudonym(QString pseudonym); // set pseudonym.
    QString pseudonym() const;				  // get pseudonym.

    friend bool operator== (const Person& a1, const Person& a2);
    Person& operator=(const Person& other) {

        if (this == &other)
            return *this;

        m_id = other.m_id;
        m_firstname = other.m_firstname;
        m_lastname = other.m_lastname;
        m_pseudonym = other.m_pseudonym;

        return  *this;
    }


signals:
    void firstnameChanged(QString firstname);

    void lastnameChanged(QString lastname);

    void pseudonymChanged(QString pseudonym);

protected:
    int m_id = -1;
    QString m_firstname = "";
    QString m_lastname = "";
    QString m_pseudonym = "";

};

Q_DECLARE_METATYPE(Person);

#endif // PERSON_H
