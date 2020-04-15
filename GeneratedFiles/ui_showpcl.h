/********************************************************************************
** Form generated from reading UI file 'showpcl.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPCL_H
#define UI_SHOWPCL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showpcl
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *totalLayout;
    QHBoxLayout *leftLayout;
    QHBoxLayout *rightLayout;

    void setupUi(QWidget *showpcl)
    {
        if (showpcl->objectName().isEmpty())
            showpcl->setObjectName(QString::fromUtf8("showpcl"));
        showpcl->setWindowModality(Qt::NonModal);
        showpcl->resize(946, 648);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(showpcl->sizePolicy().hasHeightForWidth());
        showpcl->setSizePolicy(sizePolicy);
        horizontalLayoutWidget = new QWidget(showpcl);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(9, -1, 931, 641));
        totalLayout = new QHBoxLayout(horizontalLayoutWidget);
        totalLayout->setSpacing(2);
        totalLayout->setObjectName(QString::fromUtf8("totalLayout"));
        totalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        totalLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout = new QHBoxLayout();
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));

        totalLayout->addLayout(leftLayout);

        rightLayout = new QHBoxLayout();
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));

        totalLayout->addLayout(rightLayout);


        retranslateUi(showpcl);

        QMetaObject::connectSlotsByName(showpcl);
    } // setupUi

    void retranslateUi(QWidget *showpcl)
    {
        showpcl->setWindowTitle(QApplication::translate("showpcl", "showpcl", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showpcl: public Ui_showpcl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPCL_H
