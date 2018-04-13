/********************************************************************************
** Form generated from reading UI file 'ui_mainframe.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QComboBox *comboBox;
    QPushButton *cfmdevbutton;
    QPushButton *startsniff;
    QPushButton *stopsniff;
    QPushButton *resniff;
    QTableView *tableView;
    QLabel *label;
    QLabel *label_3;
    QPlainTextEdit *hexEdit;
    QLabel *label_4;
    QTableView *tableView_2;
    QPlainTextEdit *hexEdit_2;
    QLabel *label_5;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1440, 855);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(280, 30, 411, 41));
        cfmdevbutton = new QPushButton(centralwidget);
        cfmdevbutton->setObjectName(QStringLiteral("cfmdevbutton"));
        cfmdevbutton->setGeometry(QRect(700, 40, 81, 21));
        startsniff = new QPushButton(centralwidget);
        startsniff->setObjectName(QStringLiteral("startsniff"));
        startsniff->setGeometry(QRect(10, 40, 81, 21));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startsniff->sizePolicy().hasHeightForWidth());
        startsniff->setSizePolicy(sizePolicy);
        stopsniff = new QPushButton(centralwidget);
        stopsniff->setObjectName(QStringLiteral("stopsniff"));
        stopsniff->setGeometry(QRect(100, 40, 81, 21));
        resniff = new QPushButton(centralwidget);
        resniff->setObjectName(QStringLiteral("resniff"));
        resniff->setGeometry(QRect(190, 40, 81, 21));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(10, 90, 1421, 331));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 70, 81, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 440, 71, 20));
        hexEdit = new QPlainTextEdit(centralwidget);
        hexEdit->setObjectName(QStringLiteral("hexEdit"));
        hexEdit->setGeometry(QRect(10, 460, 411, 361));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(850, 440, 71, 20));
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(850, 460, 581, 361));
        hexEdit_2 = new QPlainTextEdit(centralwidget);
        hexEdit_2->setObjectName(QStringLiteral("hexEdit_2"));
        hexEdit_2->setGeometry(QRect(430, 460, 411, 361));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(430, 440, 71, 20));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(cfmdevbutton, SIGNAL(clicked()), MainWindow, SLOT(setDevice()));
        QObject::connect(stopsniff, SIGNAL(clicked()), MainWindow, SLOT(stopSniff()));
        QObject::connect(resniff, SIGNAL(clicked()), MainWindow, SLOT(restartSniff()));
        QObject::connect(startsniff, SIGNAL(clicked()), MainWindow, SLOT(startSniff()));
        QObject::connect(tableView, SIGNAL(clicked(QModelIndex)), MainWindow, SLOT(packetDetail(QModelIndex)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        cfmdevbutton->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244\350\256\276\345\244\207", nullptr));
        startsniff->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\215\225\350\216\267", nullptr));
        stopsniff->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\346\215\225\350\216\267", nullptr));
        resniff->setText(QApplication::translate("MainWindow", "\351\207\215\346\226\260\346\215\225\350\216\267", nullptr));
        label->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\214\205", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\345\215\201\345\205\255\350\277\233\345\210\266", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "IP\346\265\201\351\207\217\347\273\237\350\256\241", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "ASCII\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H
