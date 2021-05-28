#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <list>
#include "autor.h"
#include "genre.h"
#include "coautor.h"

class Song : public QObject
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = nullptr);
    explicit Song(const Song& other, QObject* parent = nullptr);
    explicit Song(int id, QString title, QString url, int year, int length, const Autor& autor);

    void setId(int id);
    int id() const;
    void setTitle(QString title);
    QString title() const;
    void setUrl(QString url);
    QString url() const;
    void setYear(int year);
    int year() const;
    void setLength(int length);
    int length() const;
    void setAutor(Autor autor);
    Autor autor();
    void setGenres(const std::list<Genre>& genres);
    std::list<Genre>* genres();
    void setCo_autors(const std::list<CoAutor>& co_autors);
    std::list<CoAutor>* co_autors();

    Song& operator=(const Song& other) {
        if (this == &other)
            return *this;

        m_id = other.m_id;
        m_title = other.m_title;
        m_url = other.m_url;
        m_year = other.m_year;
        m_length = other.m_length;
        m_autor = other.m_autor;
        m_genres = other.m_genres;
        m_co_autors = other.m_co_autors;

        return *this;
    }

private:
    int m_id = -1;
    QString m_title = "";
    QString m_url = "";
    int m_year = 0;
    int m_length = 0;
    Autor m_autor;
    std::list<Genre> m_genres;
    std::list<CoAutor> m_co_autors;

};

#endif // SONG_H
