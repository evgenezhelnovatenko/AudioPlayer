#ifndef GENRE_H
#define GENRE_H

#include <QObject>

class Genre : public QObject
{
    Q_OBJECT
public:
    explicit Genre(QObject *parent = nullptr);
    explicit Genre(const Genre& other);
    explicit Genre(int id, QString name);

    void setId(int id);
    int id() const;
    void setName(QString name);
    QString name() const;

    friend bool operator== (const Genre& g1, const Genre& g2);
    Genre& operator=(const Genre& other) {

        if (this == &other)
            return *this;

        m_id = other.m_id;
        m_name = other.m_name;

        return *this;
    }

private:
    int m_id = -1;
    QString m_name = "";

};

#endif // GENRE_H
