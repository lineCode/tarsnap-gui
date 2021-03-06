#ifndef JOURNAL_H
#define JOURNAL_H

#include "persistentobject.h"

#include <QObject>

struct LogEntry
{
    QDateTime timestamp;
    QString   message;
};

/*!
 * \ingroup persistent
 * \brief The Journal stores the user's log messages.
 */
class Journal : public QObject, public PersistentObject
{
    Q_OBJECT

public:
    //! Constructor.
    explicit Journal(QObject *parent = nullptr);
    ~Journal();

public slots:
    //! Emits the current log.
    void getJournal() { emit journal(_log); }
    //! Adds a new log message to the journal (and PersistentStore)
    //! after stripping HTML commands from the string.
    void log(QString message);

    // From PersistentObject
    //! Does nothing.
    void save() {}
    //! Loads this object from the PersistentStore.
    void load();
    //! Deletes this object from the PersistentStore.
    void purge();
    //! Returns whether an object with this key exists in the PersistentStore.
    bool doesKeyExist(QString key)
    {
        Q_UNUSED(key);
        return false;
    }

signals:
    //! A log entry (including time and HTML-stripped message).
    void logEntry(LogEntry log);
    //! The complete set of log entries.
    void journal(QVector<LogEntry> _log);

private:
    QVector<LogEntry> _log;
};

#endif // JOURNAL_H
