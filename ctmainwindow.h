#ifndef CTMAINWINDOW_H
#define CTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CTMainWindow;
}

class CTMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CTMainWindow(QWidget *parent = 0);
    ~CTMainWindow();

private:
    Ui::CTMainWindow *ui;
};

#endif // CTMAINWINDOW_H
