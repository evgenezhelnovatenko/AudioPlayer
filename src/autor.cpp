#include "autor.h"

Autor::Autor()
{

}

Autor::Autor(const Autor &other, QObject *parent)
    : Person(other, parent)
{

}

Autor::Autor(int id, QString firstname, QString lastname, QString pseudonym)
    : Person(id, firstname, lastname, pseudonym) {

}

//void Autor::setFirstname(QString firstname)
//{
//    if (m_firstname == firstname)
//        return;

//    m_firstname = firstname;

//    emit firstnameChanged(m_firstname);
//}

//void Autor::setLastname(QString lastname)
//{
//    if (m_lastname == lastname)
//        return;

//    m_lastname = lastname;

//    emit lastnameChanged(m_lastname);
//}

//void Autor::setPseudonym(QString pseudonym)
//{
//    if (m_pseudonym == pseudonym)
//        return;

//    m_pseudonym = pseudonym;

//    emit pseudonymChanged(m_pseudonym);
//}
