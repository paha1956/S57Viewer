#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include "AstS57Parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

private:
    Ui::CMainWindow *ui;

    QString                       m_sConsoleBuffer;

    CAstS57Parser                 m_S57Parser;

public slots:

    void OpenFileSlot(void);

};
#endif // CMAINWINDOW_H
