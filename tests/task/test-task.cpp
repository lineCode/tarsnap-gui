#include <QtTest/QtTest>

#include "tarsnaptask.h"

class TestTask : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void sleep_ok();
    void sleep_fail();
    void sleep_crash();
    void sleep_filenotfound();
    void cmd_filenotfound();

private:
    QString get_script(QString scriptname);
};

void TestTask::initTestCase()
{
    QCoreApplication::setOrganizationName(TEST_NAME);
}

QString TestTask::get_script(QString scriptname)
{
    QDir dir = QDir(QCoreApplication::applicationDirPath());

#ifdef Q_OS_OSX
    // Handle the OSX app bundle.
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
#endif

    return dir.absolutePath() + "/" + scriptname;
}

#define RUN_SCRIPT(scriptname)                                                 \
    TarsnapTask *task = new TarsnapTask();                                     \
    QSignalSpy   sig_started(task, SIGNAL(started(QVariant)));                 \
    QSignalSpy   sig_fin(task,                                                 \
                       SIGNAL(finished(QVariant, int, QString, QString)));     \
    QSignalSpy sig_dequeue(task, SIGNAL(dequeue()));                           \
                                                                               \
    task->setCommand("/bin/sh");                                               \
    task->setArguments(QStringList(get_script(scriptname)));                   \
    task->run();                                                               \
    QVERIFY(sig_started.count() == 1);                                         \
    QVERIFY(sig_dequeue.count() == 1);                                         \
    delete task;

void TestTask::sleep_ok()
{
    RUN_SCRIPT("sleep-1-exit-0.sh");
    // We got a "finished" signal, with exit code 0.
    QVERIFY(sig_fin.count() == 1);
    QVERIFY(sig_fin.takeFirst().at(1).toInt() == 0);
}

void TestTask::sleep_fail()
{
    RUN_SCRIPT("sleep-1-exit-1.sh");
    // We got a "finished" signal, with exit code 1.
    QVERIFY(sig_fin.count() == 1);
    QVERIFY(sig_fin.takeFirst().at(1).toInt() == 1);
}

void TestTask::sleep_crash()
{
    RUN_SCRIPT("sleep-1-crash.sh");
    // Despite the crash, we should still get a "finished" signal.
    QVERIFY(sig_fin.count() == 1);
    QVERIFY(sig_fin.takeFirst().at(1).toInt() == EXIT_CRASHED);
}

void TestTask::sleep_filenotfound()
{
    // This script should not exist.
    RUN_SCRIPT("sleep-1-filenotfound.sh");
    // We got a "finished" signal, with sh exit code 127 ("command not found").
    QVERIFY(sig_fin.count() == 1);
    QVERIFY(sig_fin.takeFirst().at(1).toInt() == 127);
}

void TestTask::cmd_filenotfound()
{
    TarsnapTask *task = new TarsnapTask();
    QSignalSpy   sig_started(task, SIGNAL(started(QVariant)));
    QSignalSpy sig_fin(task, SIGNAL(finished(QVariant, int, QString, QString)));
    QSignalSpy sig_dequeue(task, SIGNAL(dequeue()));

    task->setCommand("/fake/dir/fake-cmd");
    task->run();
    // We got a "finished" signal, with exit code EXIT_DID_NOT_START.
    QVERIFY(sig_fin.count() == 1);
    QVERIFY(sig_fin.takeFirst().at(1).toInt() == EXIT_DID_NOT_START);
}

QTEST_MAIN(TestTask)
#include "test-task.moc"
