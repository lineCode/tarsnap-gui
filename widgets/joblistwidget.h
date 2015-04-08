#ifndef JOBSLISTWIDGET_H
#define JOBSLISTWIDGET_H

#include "job.h"

#include <QListWidget>

class JobListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit JobListWidget(QWidget *parent = 0);
    ~JobListWidget();

signals:
    void displayJobDetails(JobPtr job);

public slots:
    void backupItem();
    void inspectItem();
    void restoreItem();

};

#endif // JOBSLISTWIDGET_H