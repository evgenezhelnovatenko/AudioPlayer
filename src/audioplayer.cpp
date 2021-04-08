#include <QTextStream>
#include <QDebug>
#include <QMediaPlayer>
#include <QDir>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QMetaEnum>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QAbstractListModel {parent}
    , m_currentSongIndex (0)
    , mySongsFile (new QFile(this))
    , reply (nullptr)
    , file (nullptr)
{
    filename = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "/file.txt";

    pathToMySongsFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "/mySongs.txt";
    mySongsFile->setFileName(pathToMySongsFile);

    if (!readingSongsFromMySongsFile()) {
        qDebug() << "Файл з музикою пошкоджено або видалено!";
        return;
    }

    m_filepath = m_playlist[m_currentSongIndex];


    connect(this, &AudioPlayer::newSongsListChanged, this, &AudioPlayer::addNewSongs);

    connect(&qnam, &QNetworkAccessManager::authenticationRequired,
                this, &AudioPlayer::slotAuthenticationRequired);
    #ifndef QT_NO_SSL
        connect(&qnam, &QNetworkAccessManager::sslErrors,
                this, &AudioPlayer::sslErrors);
    #endif
}

AudioPlayer::~AudioPlayer()
{
    delete mySongsFile;
}

int AudioPlayer::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_playlist.size();
}

QVariant AudioPlayer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const auto index_row {static_cast<size_t>(index.row())};

    if (!isPositionValid(index_row)) {
        return {};
    }

    return QVariant::fromValue(m_playlist[index_row]);
}

void AudioPlayer::switchToNextSong()
{
    m_currentSongIndex++;
    songChange();
}

void AudioPlayer::switchToPrevSong()
{
    m_currentSongIndex--;
    songChange();
}

void AudioPlayer::setCurrentSongIndex(int index)
{
    if (index == m_currentSongIndex)
        return;

    m_currentSongIndex = index;
    emit currentSongIndexChanged(m_currentSongIndex);
}

void AudioPlayer::setnewSongsList(QList<QUrl> newSongsList)
{
    if (m_newSongsList == newSongsList)
        return;

    m_newSongsList = newSongsList;
    emit newSongsListChanged(m_newSongsList);
    m_newSongsList.clear();
}

void AudioPlayer::addNewSongs()
{
    if (!mySongsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "Помилка при додаванні нових файлів. Файл з музикою пошкоджено або видалено!";
        return;
    }

    QTextStream out(mySongsFile);
    beginInsertRows(QModelIndex(), m_playlist.size(), m_playlist.size() + m_newSongsList.size() - 1);
    for (int i = 0; i < m_newSongsList.size(); i++) {

        QString newSong = m_newSongsList.at(i).toLocalFile();
        out << newSong << "\n";
        m_playlist.push_back(newSong);
    }
    endInsertRows();

    mySongsFile->close();

}

void AudioPlayer::deleteSong(int songIndex)
{
    std::vector<QString>::const_iterator it;
    it = m_playlist.cbegin() + songIndex;
    beginRemoveRows(QModelIndex(), songIndex, songIndex);
    m_playlist.erase(it);
    endRemoveRows();

    dubbingToSongsFile();
}

void AudioPlayer::downloadJsonData()
{
    const QByteArray urlSpec = "https://musicbrainz.org/ws/2/recording/b9ad642e-b012-41c7-b72a-42cf4911f9ff?inc=artist-credits+isrcs+releases";
    const QUrl newUrl = QUrl::fromEncoded(urlSpec);


    qDebug() << "url: " << newUrl;
    if (!newUrl.isValid()) {
        qDebug() << "url не правильний.";
        return;
    }

    file = openFileForWrite(filename);

    if (!file)
        return;

    startRequest(newUrl);
}

void AudioPlayer::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;

    QNetworkRequest request = createRequest();

    reply = qnam.get(request);

    connect(reply, &QNetworkReply::finished, this, &AudioPlayer::httpFinished);
    connect(reply, &QNetworkReply::readyRead, this, &AudioPlayer::httpReadyRead);

}

QNetworkRequest AudioPlayer::createRequest()
{
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Accept","application/json");
    request.setRawHeader("User-Agent", "AudioPlayer/1.1.0 (mr.zhelnovatenko@gmail.com)");

    return request;
}

std::unique_ptr<QFile> AudioPlayer::openFileForWrite(const QString &fileName)
{
    std::unique_ptr<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        qDebug() << "Файл для результатів не відкривається";
        return nullptr;
    }
    return file;
}

QJsonObject AudioPlayer::parseReply(QNetworkReply *reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    auto replyText = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}

void AudioPlayer::httpReadyRead()
{
    if (file) {
        file->write(reply->readAll());
    }
}

void AudioPlayer::httpFinished()
{
    QFileInfo fi;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        file.reset();
    }

    if (reply->error() != QNetworkReply::NoError) {
        QFile::remove(fi.absoluteFilePath());
        QMetaEnum metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
        qDebug() << "reply->error(): " << metaEnum.valueToKey(reply->error());
        reply->deleteLater();
        reply = nullptr;

        return;
    }

    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    reply->deleteLater();
    reply = nullptr;

    if (!redirectionTarget.isNull()) {
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        file = openFileForWrite(fi.absoluteFilePath());
        if (!file) {
            return;
        }
        startRequest(redirectedUrl);
        return;
    }
}

void AudioPlayer::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    qDebug() << authenticator->realm() << " at " << url.host();

    authenticator->setUser("NeVr0t1k");
    authenticator->setPassword("Zhe1n0v76");
}

bool AudioPlayer::isPositionValid(const size_t position) const
{
    return (position < m_playlist.size() && position >= 0);
}

bool AudioPlayer::readingSongsFromMySongsFile() // Зчитування списку пісень з файлу та запис їх у m_playlist.
{

    if (!mySongsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(mySongsFile);
    while (!in.atEnd()) {
        QString path = in.readLine();
        m_playlist.push_back(path);
    }

    mySongsFile->close();

    return true;
}

void AudioPlayer::songChange()
{
    if (!isPositionValid(m_currentSongIndex)) {
        emit songsAreOver();
        m_currentSongIndex = 0;
    }
    m_filepath = m_playlist[m_currentSongIndex];
    emit currentSongIndexChanged(m_currentSongIndex);
    emit filepathChanged(m_filepath);
}

void AudioPlayer::dubbingToSongsFile()
{
    if (!mySongsFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Помилка при перезаписі файлу. Файл з музикою пошкоджено або видалено!";
        return ;
    }

    QTextStream out(mySongsFile);
    for (size_t i = 0; i < m_playlist.size(); i++) {
        QString line = m_playlist.at(i) + "\n";
        out << line;
    }

    mySongsFile->close();
}

#ifndef QT_NO_SSL
void AudioPlayer::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    QString errorString;
    for (const QSslError &error : errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }
    reply->ignoreSslErrors();
}
#endif
