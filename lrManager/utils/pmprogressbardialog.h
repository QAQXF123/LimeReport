#ifndef PMPROGRESSBARDIALOG_H
#define PMPROGRESSBARDIALOG_H

#include <QProgressDialog>

class PMProgressBarDialog : public QProgressDialog
{
public:
    PMProgressBarDialog(QWidget *parent = nullptr,
                        const QString &title = "",
                        const QString &text = "",
                        int max = 0);
    ~PMProgressBarDialog();

    void setShowCancelButton(bool bShow);
    void progressStep(int step = 1);

protected:
    void keyPressEvent(QKeyEvent *) override;
};

#endif // PMPROGRESSBARDIALOG_H
