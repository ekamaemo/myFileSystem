/********************************************************************************
** Form generated from reading UI file 'fileSystemAppLgPCjz.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FILESYSTEMAPPLGPCJZ_H
#define FILESYSTEMAPPLGPCJZ_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *nameCreateDirectory;
    QLineEdit *nameChangeDirectory;
    QLineEdit *lineEdit_3;
    QPushButton *btnListOfFiles;
    QListView *listOfFiles;
    QPushButton *btnCreatePath;
    QPushButton *btnChangePath;
    QPushButton *pushButton_3;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *nameWriteFile;
    QLineEdit *nameReadFile;
    QPushButton *btnWriteFile;
    QPushButton *btnReadFile;
    QLineEdit *outLine;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(596, 616);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        nameCreateDirectory = new QLineEdit(centralwidget);
        nameCreateDirectory->setObjectName(QString::fromUtf8("nameCreateDirectory"));
        nameCreateDirectory->setGeometry(QRect(30, 50, 113, 22));
        nameChangeDirectory = new QLineEdit(centralwidget);
        nameChangeDirectory->setObjectName(QString::fromUtf8("nameChangeDirectory"));
        nameChangeDirectory->setGeometry(QRect(30, 90, 113, 22));
        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setEnabled(false);
        lineEdit_3->setGeometry(QRect(70, 170, 131, 21));
        btnListOfFiles = new QPushButton(centralwidget);
        btnListOfFiles->setObjectName(QString::fromUtf8("btnListOfFiles"));
        btnListOfFiles->setGeometry(QRect(30, 450, 121, 28));
        listOfFiles = new QListView(centralwidget);
        listOfFiles->setObjectName(QString::fromUtf8("listOfFiles"));
        listOfFiles->setGeometry(QRect(30, 250, 256, 192));
        btnCreatePath = new QPushButton(centralwidget);
        btnCreatePath->setObjectName(QString::fromUtf8("btnCreatePath"));
        btnCreatePath->setGeometry(QRect(150, 50, 101, 21));
        btnChangePath = new QPushButton(centralwidget);
        btnChangePath->setObjectName(QString::fromUtf8("btnChangePath"));
        btnChangePath->setGeometry(QRect(150, 90, 101, 21));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(50, 130, 171, 21));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 20, 121, 21));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(430, 10, 55, 16));
        nameWriteFile = new QLineEdit(centralwidget);
        nameWriteFile->setObjectName(QString::fromUtf8("nameWriteFile"));
        nameWriteFile->setGeometry(QRect(340, 50, 113, 22));
        nameReadFile = new QLineEdit(centralwidget);
        nameReadFile->setObjectName(QString::fromUtf8("nameReadFile"));
        nameReadFile->setGeometry(QRect(340, 90, 113, 22));
        btnWriteFile = new QPushButton(centralwidget);
        btnWriteFile->setObjectName(QString::fromUtf8("btnWriteFile"));
        btnWriteFile->setGeometry(QRect(470, 50, 101, 21));
        btnReadFile = new QPushButton(centralwidget);
        btnReadFile->setObjectName(QString::fromUtf8("btnReadFile"));
        btnReadFile->setGeometry(QRect(470, 90, 101, 21));
        outLine = new QLineEdit(centralwidget);
        outLine->setObjectName(QString::fromUtf8("outLine"));
        outLine->setEnabled(false);
        outLine->setGeometry(QRect(20, 520, 541, 31));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 500, 131, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 596, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        nameCreateDirectory->setInputMask(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\277\320\260\320\277\320\272\320\270", nullptr));
        nameCreateDirectory->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\277\320\260\320\277\320\272\320\270", nullptr));
        nameChangeDirectory->setInputMask(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\277\320\260\320\277\320\272\320\270", nullptr));
        btnListOfFiles->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\262\320\265\321\201\321\202\320\270 \321\204\320\260\320\271\320\273\321\213", nullptr));
        btnCreatePath->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\277\320\260\320\277\320\272\321\203", nullptr));
        btnChangePath->setText(QCoreApplication::translate("MainWindow", "\320\241\320\274\320\265\320\275\320\270\321\202\321\214 \320\277\320\260\320\277\320\272\321\203", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\262\320\265\321\201\321\202\320\270 \321\202\320\265\320\272\321\203\321\211\321\203\321\216 \320\277\320\260\320\277\320\272\321\203", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\320\277\320\272\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        nameWriteFile->setInputMask(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \321\204\320\260\320\271\320\273\320\260", nullptr));
        nameReadFile->setInputMask(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \321\204\320\260\320\271\320\273\320\260", nullptr));
        btnWriteFile->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \321\204\320\260\320\271\320\273", nullptr));
        btnReadFile->setText(QCoreApplication::translate("MainWindow", "\320\241\321\207\320\270\321\202\320\260\321\202\321\214 \321\204\320\260\320\271\320\273", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\230\321\202\320\276\320\263 \320\276\320\277\320\265\321\200\320\260\321\206\320\270\320\271:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FILESYSTEMAPPLGPCJZ_H
