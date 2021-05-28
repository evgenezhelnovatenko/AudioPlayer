#include "person.h"

Person::Person(QObject *parent)
    : QObject(parent)
{

}

Person::Person(const Person &other, QObject *parent)
    : QObject(parent)
    , m_id(other.m_id)
    , m_firstname(other.m_firstname)
    , m_lastname(other.m_lastname)
    , m_pseudonym(other.m_pseudonym)
{

}

Person::Person(int id, QString firstname, QString lastname, QString pseudonym) {
    m_id = id;
    m_firstname = firstname;
    m_lastname = lastname;
    m_pseudonym = pseudonym;
}

void Person::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
}

int Person::id() const
{
    return m_id;
}

void Person::setFirstname(QString firstname)
{
    if (m_firstname == firstname)
        return;

    m_firstname = firstname;
}

QString Person::firstname() const
{
    return m_firstname;
}

void Person::setLastname(QString lastname)
{
    if (m_lastname == lastname)
        return;

    m_lastname = lastname;
}

QString Person::lastname() const
{
    return m_lastname;
}

void Person::setPseudonym(QString pseudonym)
{
    if (m_pseudonym == pseudonym)
        return;

    m_pseudonym = pseudonym;
}

QString Person::pseudonym() const
{
    return m_pseudonym;
}

bool operator==(const Person& a1, const Person& a2)
{
    return (a1.m_id == a2.m_id &&
        a1.m_firstname == a2.m_firstname &&
        a1.m_lastname == a2.m_lastname &&
        a1.m_pseudonym == a2.m_pseudonym);
}

