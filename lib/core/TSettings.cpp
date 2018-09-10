#include <TSettings.h>

#include <QMutex>
#include <QSettings>

// Filename to use (if we don't want the default).
static QString filename_next = NULL;

// The actual settings.
static QSettings *settings = NULL;

// For safety
static QMutex mutex;

void TSettings::setFilename(QString filename)
{
    filename_next = filename;
}

void TSettings::destroy()
{
    // Bail if we've already cleaned up, including after a mutex (in case a
    // different thread called this function already).
    if(settings == NULL)
        return;
    mutex.lock();
    if(settings == NULL)
    {
        mutex.unlock();
        return;
    }

    // Clean up, release mutex.
    delete settings;
    settings = NULL;
    mutex.unlock();
}

TSettings::TSettings()
{
    // Bail if we already have an object, including after a mutex (in case a
    // different thread created this object already).
    if(settings != NULL)
        return;
    mutex.lock();
    if(settings != NULL)
    {
        mutex.unlock();
        return;
    }

    // If we don't have a custom filename, get the default one.
    if(filename_next == NULL)
    {
        QSettings settings_default;
        filename_next = settings_default.fileName();
    }

    // Create the QSettings.
    settings = new QSettings(filename_next, QSettings::IniFormat);

    // Clean up.
    mutex.unlock();
}

QSettings *TSettings::getQSettings()
{
    return settings;
}

bool TSettings::contains(const QString &key)
{
    return (settings->contains(key));
}

QVariant TSettings::value(const QString &key, const QVariant &defaultValue)
{
    return (settings->value(key, defaultValue));
}

void TSettings::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

void TSettings::sync()
{
    settings->sync();
}
