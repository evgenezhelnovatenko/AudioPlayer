#include "song.h"

Song::Song(QObject *parent) : QObject(parent)
{

}

Song::Song(const Song &other, QObject *parent)
    : QObject(parent)
    , m_id(other.m_id)
    , m_title(other.m_title)
    , m_url(other.m_url)
    , m_year(other.m_year)
    , m_length(other.m_length)
    , m_autor(other.m_autor)
    , m_genres(other.m_genres)
    , m_co_autors(other.m_co_autors)
{

}

Song::Song(int id, QString title, QString url, int year, int length, const Autor& autor, const std::list<Genre>& genres, const std::list<CoAutor>& co_autors)
    : m_id(id)
    , m_title(title)
    , m_url(url)
    , m_year(year)
    , m_length(length)
    , m_autor(autor)
    , m_genres(genres)
    , m_co_autors(co_autors)
{

}

Song::Song(QString title, QString url) : m_title(title), m_url(url)
{

}

void Song::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
}

int Song::id() const
{
    return m_id;
}

void Song::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;

    emit titleChanged(m_title);
}

QString Song::title() const
{
    return m_title;
}

void Song::setUrl(QString url)
{
    if (m_url == url)
        return;

    m_url = url;

    emit urlChanged(m_url);
}

QString Song::url() const
{
    return m_url;
}

void Song::setYear(int year)
{
    if (m_year == year)
        return;

    m_year = year;

    emit yearChanged(m_year);
}

int Song::year() const
{
    return m_year;
}

void Song::setLength(int length)
{
    if (m_length == length)
        return;

    m_length = length;

    emit lengthChanged(m_length);
}

int Song::length() const
{
    return m_length;
}

void Song::setAutor(Autor autor)
{
    if (m_autor == autor)
        return;

    m_autor = autor;
}

Autor Song::autor()
{
    return m_autor;
}

void Song::setGenres(const std::list<Genre>& genres)
{
    if (m_genres == genres)
        return;

    m_genres = genres;
}

std::list<Genre> Song::genres()
{
    return m_genres;
}

void Song::setCo_autors(const std::list<CoAutor>& co_autors)
{
    if (m_co_autors == co_autors)
        return;

    m_co_autors = co_autors;
}

std::list<CoAutor> Song::co_autors()
{
    return m_co_autors;
}
