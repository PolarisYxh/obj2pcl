/********************************************************************************
** Form generated from reading UI file 'osgqt.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSGQT_H
#define UI_OSGQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_osgqt
{
public:
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QToolButton *open;
    QComboBox *directorycomboBox;
    QGroupBox *groupBox_5;
    QComboBox *comboBox;
    QToolButton *toolButton;
    QLabel *saved;
    QGroupBox *groupBox_6;
    QPushButton *group_button;
    QLineEdit *input_group_name;
    QPushButton *Ungroup;
    QLineEdit *input_attach_name;
    QPushButton *attach;
    QPushButton *rename;
    QLineEdit *input_Rename;
    QPushButton *Delete_node;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_6;
    QPushButton *select;
    QPushButton *unselect;
    QGroupBox *groupBox_3;
    QTextBrowser *group_error_message;
    QPushButton *Reset;
    QLabel *copyright1;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QToolButton *open2;
    QComboBox *comboBox_2;
    QGroupBox *groupBox_7;
    QComboBox *comboBox_3;
    QToolButton *toolButton_2;
    QLabel *label;
    QGroupBox *groupBox_8;
    QPushButton *Animation_test;
    QPushButton *resetanimation;
    QGroupBox *groupBox_9;
    QPushButton *cancel;
    QPushButton *ok;
    QGroupBox *groupBox_10;
    QRadioButton *translation;
    QRadioButton *rotation;
    QRadioButton *spiral;
    QGroupBox *groupBox_13;
    QTextEdit *dof_add_info;
    QGroupBox *groupBox_4;
    QGroupBox *groupBox_11;
    QPushButton *tp;
    QRadioButton *tp_1;
    QRadioButton *tp_2;
    QGroupBox *groupBox_12;
    QPushButton *opof;
    QRadioButton *opof_1;
    QRadioButton *opof_2;
    QLabel *mode_info;
    QTextEdit *done_name_info;
    QPushButton *reset2;
    QLabel *copyright2;

    void setupUi(QWidget *osgqt)
    {
        if (osgqt->objectName().isEmpty())
            osgqt->setObjectName(QString::fromUtf8("osgqt"));
        osgqt->resize(1391, 878);
        gridLayout_3 = new QGridLayout(osgqt);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget = new QTabWidget(osgqt);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayoutWidget = new QWidget(tab);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(220, 0, 891, 831));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(1120, 590, 241, 81));
        open = new QToolButton(groupBox);
        open->setObjectName(QString::fromUtf8("open"));
        open->setGeometry(QRect(180, 50, 51, 18));
        directorycomboBox = new QComboBox(groupBox);
        directorycomboBox->setObjectName(QString::fromUtf8("directorycomboBox"));
        directorycomboBox->setGeometry(QRect(10, 20, 221, 22));
        groupBox_5 = new QGroupBox(tab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(1120, 680, 241, 111));
        comboBox = new QComboBox(groupBox_5);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 20, 221, 22));
        toolButton = new QToolButton(groupBox_5);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(180, 50, 51, 18));
        saved = new QLabel(groupBox_5);
        saved->setObjectName(QString::fromUtf8("saved"));
        saved->setGeometry(QRect(10, 80, 221, 20));
        groupBox_6 = new QGroupBox(tab);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(1120, 40, 241, 461));
        group_button = new QPushButton(groupBox_6);
        group_button->setObjectName(QString::fromUtf8("group_button"));
        group_button->setGeometry(QRect(150, 80, 75, 23));
        input_group_name = new QLineEdit(groupBox_6);
        input_group_name->setObjectName(QString::fromUtf8("input_group_name"));
        input_group_name->setGeometry(QRect(10, 80, 113, 20));
        Ungroup = new QPushButton(groupBox_6);
        Ungroup->setObjectName(QString::fromUtf8("Ungroup"));
        Ungroup->setGeometry(QRect(150, 140, 75, 23));
        input_attach_name = new QLineEdit(groupBox_6);
        input_attach_name->setObjectName(QString::fromUtf8("input_attach_name"));
        input_attach_name->setGeometry(QRect(10, 200, 113, 20));
        attach = new QPushButton(groupBox_6);
        attach->setObjectName(QString::fromUtf8("attach"));
        attach->setGeometry(QRect(150, 200, 75, 23));
        rename = new QPushButton(groupBox_6);
        rename->setObjectName(QString::fromUtf8("rename"));
        rename->setGeometry(QRect(150, 260, 75, 23));
        input_Rename = new QLineEdit(groupBox_6);
        input_Rename->setObjectName(QString::fromUtf8("input_Rename"));
        input_Rename->setGeometry(QRect(10, 260, 113, 20));
        Delete_node = new QPushButton(groupBox_6);
        Delete_node->setObjectName(QString::fromUtf8("Delete_node"));
        Delete_node->setGeometry(QRect(150, 320, 75, 23));
        label_2 = new QLabel(groupBox_6);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 240, 151, 16));
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 180, 131, 16));
        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 60, 151, 16));
        select = new QPushButton(groupBox_6);
        select->setObjectName(QString::fromUtf8("select"));
        select->setGeometry(QRect(30, 20, 75, 23));
        unselect = new QPushButton(groupBox_6);
        unselect->setObjectName(QString::fromUtf8("unselect"));
        unselect->setGeometry(QRect(150, 20, 75, 23));
        groupBox_3 = new QGroupBox(groupBox_6);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 350, 221, 101));
        group_error_message = new QTextBrowser(groupBox_3);
        group_error_message->setObjectName(QString::fromUtf8("group_error_message"));
        group_error_message->setGeometry(QRect(10, 20, 201, 71));
        Reset = new QPushButton(tab);
        Reset->setObjectName(QString::fromUtf8("Reset"));
        Reset->setGeometry(QRect(1260, 800, 91, 23));
        copyright1 = new QLabel(tab);
        copyright1->setObjectName(QString::fromUtf8("copyright1"));
        copyright1->setGeometry(QRect(1150, 10, 211, 21));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(219, -1, 891, 831));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(1120, 590, 241, 81));
        open2 = new QToolButton(groupBox_2);
        open2->setObjectName(QString::fromUtf8("open2"));
        open2->setGeometry(QRect(180, 50, 51, 18));
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(10, 20, 221, 22));
        groupBox_7 = new QGroupBox(tab_2);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(1120, 680, 241, 111));
        comboBox_3 = new QComboBox(groupBox_7);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setGeometry(QRect(10, 20, 221, 22));
        toolButton_2 = new QToolButton(groupBox_7);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setGeometry(QRect(180, 50, 51, 18));
        label = new QLabel(groupBox_7);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 80, 221, 16));
        groupBox_8 = new QGroupBox(tab_2);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(1120, 490, 241, 91));
        Animation_test = new QPushButton(groupBox_8);
        Animation_test->setObjectName(QString::fromUtf8("Animation_test"));
        Animation_test->setGeometry(QRect(50, 20, 151, 23));
        resetanimation = new QPushButton(groupBox_8);
        resetanimation->setObjectName(QString::fromUtf8("resetanimation"));
        resetanimation->setGeometry(QRect(50, 50, 151, 23));
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(1120, 40, 241, 431));
        cancel = new QPushButton(groupBox_9);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(140, 310, 75, 23));
        ok = new QPushButton(groupBox_9);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setGeometry(QRect(20, 310, 75, 23));
        groupBox_10 = new QGroupBox(groupBox_9);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 220, 221, 81));
        translation = new QRadioButton(groupBox_10);
        translation->setObjectName(QString::fromUtf8("translation"));
        translation->setGeometry(QRect(30, 20, 89, 16));
        rotation = new QRadioButton(groupBox_10);
        rotation->setObjectName(QString::fromUtf8("rotation"));
        rotation->setGeometry(QRect(30, 40, 89, 16));
        spiral = new QRadioButton(groupBox_10);
        spiral->setObjectName(QString::fromUtf8("spiral"));
        spiral->setGeometry(QRect(30, 60, 89, 16));
        groupBox_13 = new QGroupBox(groupBox_9);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        groupBox_13->setGeometry(QRect(10, 340, 221, 81));
        dof_add_info = new QTextEdit(groupBox_13);
        dof_add_info->setObjectName(QString::fromUtf8("dof_add_info"));
        dof_add_info->setGeometry(QRect(10, 20, 201, 51));
        dof_add_info->setFocusPolicy(Qt::StrongFocus);
        groupBox_4 = new QGroupBox(groupBox_9);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 20, 221, 191));
        groupBox_11 = new QGroupBox(groupBox_4);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 20, 201, 61));
        tp = new QPushButton(groupBox_11);
        tp->setObjectName(QString::fromUtf8("tp"));
        tp->setGeometry(QRect(10, 10, 181, 23));
        tp_1 = new QRadioButton(groupBox_11);
        tp_1->setObjectName(QString::fromUtf8("tp_1"));
        tp_1->setGeometry(QRect(10, 40, 91, 16));
        tp_2 = new QRadioButton(groupBox_11);
        tp_2->setObjectName(QString::fromUtf8("tp_2"));
        tp_2->setGeometry(QRect(100, 40, 91, 16));
        groupBox_12 = new QGroupBox(groupBox_4);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        groupBox_12->setGeometry(QRect(10, 90, 201, 71));
        opof = new QPushButton(groupBox_12);
        opof->setObjectName(QString::fromUtf8("opof"));
        opof->setGeometry(QRect(10, 10, 181, 23));
        opof_1 = new QRadioButton(groupBox_12);
        opof_1->setObjectName(QString::fromUtf8("opof_1"));
        opof_1->setGeometry(QRect(10, 40, 71, 16));
        opof_2 = new QRadioButton(groupBox_12);
        opof_2->setObjectName(QString::fromUtf8("opof_2"));
        opof_2->setGeometry(QRect(100, 40, 89, 16));
        mode_info = new QLabel(groupBox_4);
        mode_info->setObjectName(QString::fromUtf8("mode_info"));
        mode_info->setGeometry(QRect(10, 170, 201, 16));
        done_name_info = new QTextEdit(tab_2);
        done_name_info->setObjectName(QString::fromUtf8("done_name_info"));
        done_name_info->setGeometry(QRect(0, 630, 211, 201));
        done_name_info->setFocusPolicy(Qt::StrongFocus);
        reset2 = new QPushButton(tab_2);
        reset2->setObjectName(QString::fromUtf8("reset2"));
        reset2->setGeometry(QRect(1260, 800, 91, 23));
        copyright2 = new QLabel(tab_2);
        copyright2->setObjectName(QString::fromUtf8("copyright2"));
        copyright2->setGeometry(QRect(1150, 10, 311, 21));
        tabWidget->addTab(tab_2, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(osgqt);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(osgqt);
    } // setupUi

    void retranslateUi(QWidget *osgqt)
    {
        osgqt->setWindowTitle(QApplication::translate("osgqt", "osgqt", nullptr));
        groupBox->setTitle(QApplication::translate("osgqt", "Open File", nullptr));
        open->setText(QApplication::translate("osgqt", "open", nullptr));
        groupBox_5->setTitle(QApplication::translate("osgqt", "Save File", nullptr));
        toolButton->setText(QApplication::translate("osgqt", "save", nullptr));
        saved->setText(QString());
        groupBox_6->setTitle(QApplication::translate("osgqt", "Motion Part Annotation", nullptr));
        group_button->setText(QApplication::translate("osgqt", "Group", nullptr));
        Ungroup->setText(QApplication::translate("osgqt", "Ungroup", nullptr));
        attach->setText(QApplication::translate("osgqt", "Attach...", nullptr));
        rename->setText(QApplication::translate("osgqt", "Rename", nullptr));
        Delete_node->setText(QApplication::translate("osgqt", "Delete", nullptr));
        label_2->setText(QApplication::translate("osgqt", "New Group Name", nullptr));
        label_3->setText(QApplication::translate("osgqt", "Group Name", nullptr));
        label_6->setText(QApplication::translate("osgqt", "Name New Group", nullptr));
        select->setText(QApplication::translate("osgqt", "Show", nullptr));
        unselect->setText(QApplication::translate("osgqt", "Hide", nullptr));
        groupBox_3->setTitle(QApplication::translate("osgqt", "Warning Message", nullptr));
        Reset->setText(QApplication::translate("osgqt", "Reset Camera", nullptr));
        copyright1->setText(QApplication::translate("osgqt", "@Copyright Shape2Motion", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("osgqt", "Motion Part Annotation", nullptr));
        groupBox_2->setTitle(QApplication::translate("osgqt", "Open File", nullptr));
        open2->setText(QApplication::translate("osgqt", "open", nullptr));
        groupBox_7->setTitle(QApplication::translate("osgqt", "Save File", nullptr));
        toolButton_2->setText(QApplication::translate("osgqt", "save", nullptr));
        label->setText(QString());
        groupBox_8->setTitle(QApplication::translate("osgqt", "Motion Verification", nullptr));
        Animation_test->setText(QApplication::translate("osgqt", "Animate", nullptr));
        resetanimation->setText(QApplication::translate("osgqt", "Stop/Reset", nullptr));
        groupBox_9->setTitle(QApplication::translate("osgqt", "Motion Attribute Annotation", nullptr));
        cancel->setText(QApplication::translate("osgqt", "Cancel", nullptr));
        ok->setText(QApplication::translate("osgqt", "OK", nullptr));
        groupBox_10->setTitle(QApplication::translate("osgqt", "Motion Type", nullptr));
        translation->setText(QApplication::translate("osgqt", "Translation", nullptr));
        rotation->setText(QApplication::translate("osgqt", "Rotation", nullptr));
        spiral->setText(QApplication::translate("osgqt", "Spiral", nullptr));
        groupBox_13->setTitle(QApplication::translate("osgqt", "Message", nullptr));
        groupBox_4->setTitle(QApplication::translate("osgqt", "Motion Parameters", nullptr));
        groupBox_11->setTitle(QString());
        tp->setText(QApplication::translate("osgqt", "Two Points Mode", nullptr));
        tp_1->setText(QApplication::translate("osgqt", "First Point", nullptr));
        tp_2->setText(QApplication::translate("osgqt", "Second Point", nullptr));
        groupBox_12->setTitle(QString());
        opof->setText(QApplication::translate("osgqt", "One Point and One Face Mode", nullptr));
        opof_1->setText(QApplication::translate("osgqt", "One Point", nullptr));
        opof_2->setText(QApplication::translate("osgqt", "One Face", nullptr));
        mode_info->setText(QString());
        reset2->setText(QApplication::translate("osgqt", "Reset Camera", nullptr));
        copyright2->setText(QApplication::translate("osgqt", "@Copyright Shape2Motion", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("osgqt", "Motion Attribute Annotation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class osgqt: public Ui_osgqt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSGQT_H
