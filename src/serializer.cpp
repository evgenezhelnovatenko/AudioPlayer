#include "serializer.h"

#include <QVariant>
#include <QJsonParseError>


Serializer::Serializer(QObject *parent) : QObject(parent)
{

}

bool Serializer::JSONDesirealizer(std::list<Song>& songlist, const QString& jsonStr)
{

    QJsonObject jsonObj = ObjectFromString(jsonStr);
    QJsonValue jsonValue = jsonObj["songlist"];
    QJsonArray jsonArray;

    if (jsonValue.isArray()) {
        jsonArray = jsonValue.toArray();

        for (auto&& songJsonVal : jsonArray) {
            Song song;
            if (!fillSong(song, songJsonVal)) {
                qDebug() << "failed filling song.";
                return false;
            }

            qDebug() << song.genres()->size();

            songlist.push_back(song);
        }

    }
    else {
        qDebug() << "jsonValue isn't array.";
        return false;
    }

    return true;
}

bool Serializer::fillSong(Song &song, const QJsonValue& songJsonVal)
{

    QJsonObject songJsonObj;
    if (songJsonVal.isObject()) {
        songJsonObj = songJsonVal.toObject();

        song.setId(songJsonObj["id"].toInt());
        song.setTitle(songJsonObj["title"].toString());
        song.setUrl(songJsonObj["url"].toString());
        song.setYear(songJsonObj["year"].toInt());
        song.setLength(songJsonObj["length"].toInt());

        QJsonValue autorJsonVal = songJsonObj["autor"];
        Autor autor;
        if (!fillAutor(autor, autorJsonVal)) {
            qDebug() << "failed filling autor.";
            return false;
        }

        QJsonValue genresJsonVal = songJsonObj["genres"];
        std::list<Genre> genres;
        if (!fillGenres(genres, genresJsonVal)) {
            qDebug() << "failed filling genres.";
            return false;
        }

        QJsonValue co_autorJsonVal = songJsonObj["co_autor"];
        std::list<CoAutor> co_autors;
        if (!fillCoAutors(co_autors, co_autorJsonVal)) {
            qDebug() << "failed filling co_autors.";
            return false;
        }

        song.setAutor(autor);
        song.setGenres(genres);
        song.setCo_autors(co_autors);

    }
    else {
        qDebug() << "songJsonVal isn't object.";
        return false;
    }

    return true;
}

bool Serializer::fillAutor(Autor& autor, const QJsonValue& autorJsonVal)
{
    QJsonObject autorJsonObj;

    if (autorJsonVal.isObject()) {
        autorJsonObj = autorJsonVal.toObject();

        autor.setId(autorJsonObj["id"].toInt());
        autor.setFirstname(autorJsonObj["firstname"].toString());
        autor.setLastname(autorJsonObj["lastname"].toString());
        autor.setPseudonym(autorJsonObj["pseudonym"].toString());
    }
    else {
        qDebug() << "autorJsonVal isn't object.";
        return false;
    }

    return true;
}

bool Serializer::fillGenres(std::list<Genre>& genres, const QJsonValue& genresJsonVal)
{
    QJsonArray genresJsonArr;

    if (genresJsonVal.isArray()) {
        genresJsonArr = genresJsonVal.toArray();

        for (auto&& genreJsonVal : genresJsonArr) {
            Genre genre;
            if (!fillGenre(genre, genreJsonVal)) {
                qDebug() << "failed filling genre.";
                return false;
            }
            genres.push_back(genre);
        }
    }
    else {
        qDebug() << "genresJsonVal isn't array.";
        return false;
    }

    return true;
}

bool Serializer::fillGenre(Genre& genre, const QJsonValue& genreJsonVal)
{
    QJsonObject genreJsonObj;

    if (genreJsonVal.isObject()) {
        genreJsonObj = genreJsonVal.toObject();

        genre.setId(genreJsonObj["id"].toInt());
        genre.setName(genreJsonObj["name"].toString());
    }
    else {
        qDebug() << "genreJsonVal isn't object.";
        return false;
    }

    return true;
}

bool Serializer::fillCoAutors(std::list<CoAutor> &co_autors, const QJsonValue &co_autorsJsonVal)
{
    QJsonArray co_autorsJsonArr;

    if (co_autorsJsonVal.isArray()) {
        co_autorsJsonArr = co_autorsJsonVal.toArray();

        for (auto&& co_autorJsonVal : co_autorsJsonArr) {
            QJsonObject co_autorJsonObj;
            CoAutor co_autor;

            if (!fillCoAutor(co_autor, co_autorJsonVal)) {
                qDebug() << "failed filling co_autor.";
                return false;
            }
            co_autors.push_back(co_autor);
        }
    }
    else {
        qDebug() << "co_autorsJsonVal isn't array.";
        return false;
    }

    return true;
}

bool Serializer::fillCoAutor(CoAutor& co_autor, const QJsonValue &co_autorJsonVal)
{
    QJsonObject co_autorJsonObj;

    if (co_autorJsonVal.isObject()) {
        co_autorJsonObj = co_autorJsonVal.toObject();

        co_autor.setId(co_autorJsonObj["id"].toInt());
        co_autor.setFirstname(co_autorJsonObj["firstname"].toString());
        co_autor.setLastname(co_autorJsonObj["lastname"].toString());
        co_autor.setPseudonym(co_autorJsonObj["pseudonym"].toString());
    }
    else {
        qDebug() << "co_autorJsonVal isn't object.";
        return false;
    }


    return true;
}

QJsonObject Serializer::ObjectFromString(const QString& in)
{
    QJsonObject obj;
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8(), &parseErr);

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << Qt::endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << parseErr.error << Qt::endl << in;
    }

    return obj;
}
