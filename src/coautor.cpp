#include "coautor.h"

CoAutor::CoAutor()
{

}

CoAutor::CoAutor(const CoAutor &other, QObject *parent)
    : Person(other, parent)
{

}

CoAutor::CoAutor(int id, QString firstname, QString lastname, QString pseudonym)
    : Person(id, firstname, lastname, pseudonym) {

}

//void CoAutor::setFirstname(QString firstname)
//{
//    if (m_firstname == firstname)
//        return;

//    m_firstname = firstname;

//    emit firstnameChanged(m_firstname);
//}

//void CoAutor::setLastname(QString lastname)
//{
//    if (m_lastname == lastname)
//        return;

//    m_lastname = lastname;

//    emit lastnameChanged(m_lastname);
//}

//void CoAutor::setPseudonym(QString pseudonym)
//{
//    if (m_pseudonym == pseudonym)
//        return;

//    m_pseudonym = pseudonym;

//    emit pseudonymChanged(m_pseudonym);
//}
