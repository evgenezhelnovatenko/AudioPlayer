#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "song.h"

class Serializer : public QObject
{
    Q_OBJECT
public:
    explicit Serializer(QObject *parent = nullptr);

    static bool JSONDesirealizer(std::list<Song>& songlist, const QString& jsonStr);

    static bool fillSong(Song& song, const QJsonValue& songJsonVal);
    static bool fillAutor(Autor& autor, const QJsonValue& autorJsonVal);
    static bool fillGenres(std::list<Genre>& genres, const QJsonValue& genresJsonVal);
    static bool fillGenre(Genre& genre, const QJsonValue& genreJsonVal);
    static bool fillCoAutors(std::list<CoAutor>& co_autors, const QJsonValue& co_autorsJsonVal);
    static bool fillCoAutor(CoAutor& co_autor, const QJsonValue& co_autorJsonVal);

    static QJsonObject ObjectFromString(const QString& in);
};

#endif // SERIALIZER_H
