/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showpcl
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *totalLayout;
    QHBoxLayout *rightLayout;
    QHBoxLayout *leftLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *showpcl)
    {
        if (showpcl->objectName().isEmpty())
            showpcl->setObjectName(QString::fromUtf8("showpcl"));
        showpcl->setWindowModality(Qt::WindowModal);
        showpcl->resize(951, 642);
        centralwidget = new QWidget(showpcl);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 951, 591));
        totalLayout = new QHBoxLayout(centralwidget);
        totalLayout->setSpacing(2);
        totalLayout->setObjectName(QString::fromUtf8("totalLayout"));
        totalLayout->setContentsMargins(0, 0, 0, 0);
        rightLayout = new QHBoxLayout();
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));

        totalLayout->addLayout(rightLayout);

        leftLayout = new QHBoxLayout();
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));

        totalLayout->addLayout(leftLayout);

        showpcl->setCentralWidget(centralwidget);
        menubar = new QMenuBar(showpcl);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 951, 26));
        showpcl->setMenuBar(menubar);
        statusbar = new QStatusBar(showpcl);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        showpcl->setStatusBar(statusbar);

        retranslateUi(showpcl);

        QMetaObject::connectSlotsByName(showpcl);
    } // setupUi

    void retranslateUi(QMainWindow *showpcl)
    {
        showpcl->setWindowTitle(QApplication::translate("showpcl", "pointcloud", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showpcl: public Ui_showpcl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
