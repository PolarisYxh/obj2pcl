/********************************************************************************
** Form generated from reading UI file 'showpic.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPIC_H
#define UI_SHOWPIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showpic
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *totalLayout;
    QHBoxLayout *leftLayout;
    QLabel *rgb;
    QHBoxLayout *rightLayout;
    QLabel *depth;

    void setupUi(QWidget *showpic)
    {
        if (showpic->objectName().isEmpty())
            showpic->setObjectName(QString::fromUtf8("showpic"));
        showpic->setWindowModality(Qt::NonModal);
        showpic->resize(991, 648);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(showpic->sizePolicy().hasHeightForWidth());
        showpic->setSizePolicy(sizePolicy);
        horizontalLayoutWidget = new QWidget(showpic);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(9, -1, 931, 641));
        totalLayout = new QHBoxLayout(horizontalLayoutWidget);
        totalLayout->setSpacing(2);
        totalLayout->setObjectName(QString::fromUtf8("totalLayout"));
        totalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        totalLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout = new QHBoxLayout();
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));
        rgb = new QLabel(horizontalLayoutWidget);
        rgb->setObjectName(QString::fromUtf8("rgb"));

        leftLayout->addWidget(rgb);


        totalLayout->addLayout(leftLayout);

        rightLayout = new QHBoxLayout();
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        depth = new QLabel(horizontalLayoutWidget);
        depth->setObjectName(QString::fromUtf8("depth"));

        rightLayout->addWidget(depth);


        totalLayout->addLayout(rightLayout);


        retranslateUi(showpic);

        QMetaObject::connectSlotsByName(showpic);
    } // setupUi

    void retranslateUi(QWidget *showpic)
    {
        showpic->setWindowTitle(QApplication::translate("showpic", "showpic", nullptr));
        rgb->setText(QApplication::translate("showpic", "rgb", nullptr));
        depth->setText(QApplication::translate("showpic", "depth", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showpic: public Ui_showpic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPIC_H
