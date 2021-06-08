#include "genre.h"

Genre::Genre(QObject *parent) : QObject(parent)
{

}

Genre::Genre(const Genre &other, QObject *parent)
    : QObject(parent)
    , m_id(other.m_id)
    , m_name(other.m_name)
{

}

Genre::Genre(int id, QString name)
    : m_id(id)
    , m_name(name)
{

}

void Genre::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
}

int Genre::id() const
{
    return m_id;
}

void Genre::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;

    emit nameChanged(m_name);
}

QString Genre::name() const
{
    return m_name;
}

bool operator==(const Genre& g1, const Genre& g2)
{
    return (g1.m_id == g2.m_id &&
            g1.m_name == g2.m_name);
}
