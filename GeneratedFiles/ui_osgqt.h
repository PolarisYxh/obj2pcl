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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_osgqt
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *totalLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *middleLayout;
    QVBoxLayout *rightLayout;
    QLabel *copyright1;
    QGroupBox *groupBox_6;
    QLabel *label_3;
    QLabel *label_6;
    QPushButton *motion_confirm;
    QPushButton *motion_delete;
    QPushButton *motion_play;
    QLabel *label_9;
    QLineEdit *input_motion_name;
    QLineEdit *motion_amp;
    QLineEdit *input_motion_fre;
    QTextEdit *setting_message;
    QLineEdit *motion_amp_2;
    QLabel *label_7;
    QGroupBox *groupBox_14;
    QPushButton *camera_confirm;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *input_shooting_times;
    QPushButton *camera_delete;
    QPushButton *camera_play;
    QLineEdit *input_shooting_radius;
    QLineEdit *input_camera_height;
    QPushButton *camera_play_2;
    QGroupBox *groupBox;
    QToolButton *open;
    QToolButton *remove;
    QLineEdit *xcoord;
    QLineEdit *ycoord;
    QLineEdit *zcoord;
    QToolButton *modelrotate;
    QToolButton *modeltranslate;
    QPushButton *scaleButton;
    QPushButton *saveButton;
    QGroupBox *groupBox_5;
    QToolButton *startButton;
    QPushButton *generateButton;
    QPushButton *showButton;
    QPushButton *showButton_1;
    QPushButton *Reset;

    void setupUi(QWidget *osgqt)
    {
        if (osgqt->objectName().isEmpty())
            osgqt->setObjectName(QString::fromUtf8("osgqt"));
        osgqt->setWindowModality(Qt::NonModal);
        osgqt->setEnabled(true);
        osgqt->resize(1449, 946);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(osgqt->sizePolicy().hasHeightForWidth());
        osgqt->setSizePolicy(sizePolicy);
        osgqt->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout = new QHBoxLayout(osgqt);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(osgqt);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayoutWidget = new QWidget(tab);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 1421, 893));
        totalLayout = new QHBoxLayout(horizontalLayoutWidget);
        totalLayout->setSpacing(3);
        totalLayout->setContentsMargins(11, 11, 11, 11);
        totalLayout->setObjectName(QString::fromUtf8("totalLayout"));
        totalLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout = new QVBoxLayout();
        leftLayout->setSpacing(6);
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));

        totalLayout->addLayout(leftLayout);

        middleLayout = new QVBoxLayout();
        middleLayout->setSpacing(6);
        middleLayout->setObjectName(QString::fromUtf8("middleLayout"));

        totalLayout->addLayout(middleLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(2);
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        rightLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        rightLayout->setContentsMargins(0, -1, -1, -1);
        copyright1 = new QLabel(horizontalLayoutWidget);
        copyright1->setObjectName(QString::fromUtf8("copyright1"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(copyright1->sizePolicy().hasHeightForWidth());
        copyright1->setSizePolicy(sizePolicy1);
        copyright1->setMinimumSize(QSize(221, 21));
        copyright1->setMaximumSize(QSize(260, 30));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        copyright1->setFont(font);
        copyright1->setLayoutDirection(Qt::LeftToRight);
        copyright1->setAutoFillBackground(false);
        copyright1->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        rightLayout->addWidget(copyright1);

        groupBox_6 = new QGroupBox(horizontalLayoutWidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy2);
        groupBox_6->setMinimumSize(QSize(280, 300));
        groupBox_6->setMaximumSize(QSize(300, 310));
        groupBox_6->setLayoutDirection(Qt::RightToLeft);
        groupBox_6->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 110, 131, 16));
        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 80, 111, 21));
        motion_confirm = new QPushButton(groupBox_6);
        motion_confirm->setObjectName(QString::fromUtf8("motion_confirm"));
        motion_confirm->setGeometry(QRect(40, 260, 75, 23));
        motion_delete = new QPushButton(groupBox_6);
        motion_delete->setObjectName(QString::fromUtf8("motion_delete"));
        motion_delete->setGeometry(QRect(140, 260, 75, 23));
        motion_play = new QPushButton(groupBox_6);
        motion_play->setObjectName(QString::fromUtf8("motion_play"));
        motion_play->setGeometry(QRect(90, 230, 75, 23));
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(30, 20, 141, 21));
        input_motion_name = new QLineEdit(groupBox_6);
        input_motion_name->setObjectName(QString::fromUtf8("input_motion_name"));
        input_motion_name->setEnabled(true);
        input_motion_name->setGeometry(QRect(140, 20, 101, 20));
        input_motion_name->setReadOnly(true);
        motion_amp = new QLineEdit(groupBox_6);
        motion_amp->setObjectName(QString::fromUtf8("motion_amp"));
        motion_amp->setEnabled(false);
        motion_amp->setGeometry(QRect(140, 80, 101, 20));
        input_motion_fre = new QLineEdit(groupBox_6);
        input_motion_fre->setObjectName(QString::fromUtf8("input_motion_fre"));
        input_motion_fre->setEnabled(false);
        input_motion_fre->setGeometry(QRect(140, 110, 101, 20));
        setting_message = new QTextEdit(groupBox_6);
        setting_message->setObjectName(QString::fromUtf8("setting_message"));
        setting_message->setGeometry(QRect(10, 140, 241, 81));
        motion_amp_2 = new QLineEdit(groupBox_6);
        motion_amp_2->setObjectName(QString::fromUtf8("motion_amp_2"));
        motion_amp_2->setEnabled(false);
        motion_amp_2->setGeometry(QRect(140, 50, 101, 20));
        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(0, 50, 141, 21));

        rightLayout->addWidget(groupBox_6);

        groupBox_14 = new QGroupBox(horizontalLayoutWidget);
        groupBox_14->setObjectName(QString::fromUtf8("groupBox_14"));
        groupBox_14->setEnabled(true);
        sizePolicy1.setHeightForWidth(groupBox_14->sizePolicy().hasHeightForWidth());
        groupBox_14->setSizePolicy(sizePolicy1);
        groupBox_14->setMinimumSize(QSize(280, 200));
        groupBox_14->setMaximumSize(QSize(300, 220));
        groupBox_14->setLayoutDirection(Qt::RightToLeft);
        groupBox_14->setAlignment(Qt::AlignCenter);
        camera_confirm = new QPushButton(groupBox_14);
        camera_confirm->setObjectName(QString::fromUtf8("camera_confirm"));
        camera_confirm->setGeometry(QRect(40, 170, 75, 23));
        label_2 = new QLabel(groupBox_14);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 30, 111, 16));
        label_4 = new QLabel(groupBox_14);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 70, 101, 16));
        label_5 = new QLabel(groupBox_14);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 110, 141, 16));
        input_shooting_times = new QLineEdit(groupBox_14);
        input_shooting_times->setObjectName(QString::fromUtf8("input_shooting_times"));
        input_shooting_times->setGeometry(QRect(140, 110, 101, 20));
        camera_delete = new QPushButton(groupBox_14);
        camera_delete->setObjectName(QString::fromUtf8("camera_delete"));
        camera_delete->setGeometry(QRect(140, 170, 75, 23));
        camera_play = new QPushButton(groupBox_14);
        camera_play->setObjectName(QString::fromUtf8("camera_play"));
        camera_play->setGeometry(QRect(40, 140, 75, 23));
        input_shooting_radius = new QLineEdit(groupBox_14);
        input_shooting_radius->setObjectName(QString::fromUtf8("input_shooting_radius"));
        input_shooting_radius->setGeometry(QRect(140, 70, 101, 20));
        input_camera_height = new QLineEdit(groupBox_14);
        input_camera_height->setObjectName(QString::fromUtf8("input_camera_height"));
        input_camera_height->setGeometry(QRect(140, 30, 101, 20));
        camera_play_2 = new QPushButton(groupBox_14);
        camera_play_2->setObjectName(QString::fromUtf8("camera_play_2"));
        camera_play_2->setGeometry(QRect(140, 140, 75, 23));

        rightLayout->addWidget(groupBox_14);

        groupBox = new QGroupBox(horizontalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(280, 120));
        groupBox->setMaximumSize(QSize(300, 120));
        groupBox->setLayoutDirection(Qt::RightToLeft);
        groupBox->setAlignment(Qt::AlignCenter);
        open = new QToolButton(groupBox);
        open->setObjectName(QString::fromUtf8("open"));
        open->setGeometry(QRect(40, 20, 75, 23));
        remove = new QToolButton(groupBox);
        remove->setObjectName(QString::fromUtf8("remove"));
        remove->setGeometry(QRect(140, 20, 71, 23));
        xcoord = new QLineEdit(groupBox);
        xcoord->setObjectName(QString::fromUtf8("xcoord"));
        xcoord->setGeometry(QRect(30, 50, 51, 21));
        ycoord = new QLineEdit(groupBox);
        ycoord->setObjectName(QString::fromUtf8("ycoord"));
        ycoord->setGeometry(QRect(100, 50, 51, 21));
        zcoord = new QLineEdit(groupBox);
        zcoord->setObjectName(QString::fromUtf8("zcoord"));
        zcoord->setGeometry(QRect(170, 50, 51, 21));
        modelrotate = new QToolButton(groupBox);
        modelrotate->setObjectName(QString::fromUtf8("modelrotate"));
        modelrotate->setGeometry(QRect(10, 80, 51, 23));
        modeltranslate = new QToolButton(groupBox);
        modeltranslate->setObjectName(QString::fromUtf8("modeltranslate"));
        modeltranslate->setGeometry(QRect(60, 80, 51, 23));
        scaleButton = new QPushButton(groupBox);
        scaleButton->setObjectName(QString::fromUtf8("scaleButton"));
        scaleButton->setGeometry(QRect(110, 80, 51, 21));
        saveButton = new QPushButton(groupBox);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(160, 80, 41, 21));

        rightLayout->addWidget(groupBox);

        groupBox_5 = new QGroupBox(horizontalLayoutWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(280, 180));
        groupBox_5->setMaximumSize(QSize(300, 200));
        groupBox_5->setLayoutDirection(Qt::RightToLeft);
        groupBox_5->setAlignment(Qt::AlignCenter);
        startButton = new QToolButton(groupBox_5);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(10, 20, 231, 31));
        generateButton = new QPushButton(groupBox_5);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));
        generateButton->setGeometry(QRect(10, 62, 231, 31));
        showButton = new QPushButton(groupBox_5);
        showButton->setObjectName(QString::fromUtf8("showButton"));
        showButton->setGeometry(QRect(10, 100, 231, 31));
        showButton_1 = new QPushButton(groupBox_5);
        showButton_1->setObjectName(QString::fromUtf8("showButton_1"));
        showButton_1->setGeometry(QRect(10, 140, 231, 31));

        rightLayout->addWidget(groupBox_5);

        Reset = new QPushButton(horizontalLayoutWidget);
        Reset->setObjectName(QString::fromUtf8("Reset"));
        Reset->setMinimumSize(QSize(105, 28));
        Reset->setMaximumSize(QSize(110, 32));
        Reset->setLayoutDirection(Qt::RightToLeft);

        rightLayout->addWidget(Reset);


        totalLayout->addLayout(rightLayout);

        totalLayout->setStretch(0, 200);
        totalLayout->setStretch(1, 800);
        totalLayout->setStretch(2, 280);
        tabWidget->addTab(tab, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(osgqt);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(osgqt);
    } // setupUi

    void retranslateUi(QWidget *osgqt)
    {
        osgqt->setWindowTitle(QApplication::translate("osgqt", "osgqt", nullptr));
        copyright1->setText(QApplication::translate("osgqt", "@Copyright Shape2PointCloud", nullptr));
        groupBox_6->setTitle(QApplication::translate("osgqt", "Dof Animate Setting", nullptr));
        label_3->setText(QApplication::translate("osgqt", "motion frequency", nullptr));
        label_6->setText(QApplication::translate("osgqt", "rotation range", nullptr));
        motion_confirm->setText(QApplication::translate("osgqt", "add", nullptr));
        motion_delete->setText(QApplication::translate("osgqt", "delete", nullptr));
        motion_play->setText(QApplication::translate("osgqt", "animate", nullptr));
        label_9->setText(QApplication::translate("osgqt", "dof name", nullptr));
        input_motion_name->setPlaceholderText(QApplication::translate("osgqt", "_r _t _s", nullptr));
        motion_amp->setPlaceholderText(QString());
        input_motion_fre->setPlaceholderText(QApplication::translate("osgqt", ">0", nullptr));
        setting_message->setPlaceholderText(QApplication::translate("osgqt", "motion info", nullptr));
        motion_amp_2->setPlaceholderText(QString());
        label_7->setText(QApplication::translate("osgqt", "translation range", nullptr));
        groupBox_14->setTitle(QApplication::translate("osgqt", "Camera Setting", nullptr));
        camera_confirm->setText(QApplication::translate("osgqt", "confirm", nullptr));
        label_2->setText(QApplication::translate("osgqt", "camera height", nullptr));
        label_4->setText(QApplication::translate("osgqt", "track radius", nullptr));
        label_5->setText(QApplication::translate("osgqt", "number of cameras", nullptr));
        camera_delete->setText(QApplication::translate("osgqt", "cancel", nullptr));
        camera_play->setText(QApplication::translate("osgqt", "animate", nullptr));
        camera_play_2->setText(QApplication::translate("osgqt", "monitor", nullptr));
        groupBox->setTitle(QApplication::translate("osgqt", "Model", nullptr));
        open->setText(QApplication::translate("osgqt", "add", nullptr));
        remove->setText(QApplication::translate("osgqt", "remove", nullptr));
        xcoord->setPlaceholderText(QApplication::translate("osgqt", "x", nullptr));
        ycoord->setPlaceholderText(QApplication::translate("osgqt", "y", nullptr));
        zcoord->setPlaceholderText(QApplication::translate("osgqt", "z", nullptr));
        modelrotate->setText(QApplication::translate("osgqt", "rotate", nullptr));
        modeltranslate->setText(QApplication::translate("osgqt", "translate", nullptr));
        scaleButton->setText(QApplication::translate("osgqt", "scale", nullptr));
        saveButton->setText(QApplication::translate("osgqt", "save", nullptr));
        groupBox_5->setTitle(QApplication::translate("osgqt", "Generate and Save", nullptr));
        startButton->setText(QApplication::translate("osgqt", "take photo", nullptr));
        generateButton->setText(QApplication::translate("osgqt", "generate PointCloud", nullptr));
        showButton->setText(QApplication::translate("osgqt", "show photoes", nullptr));
        showButton_1->setText(QApplication::translate("osgqt", "show PointCloud", nullptr));
        Reset->setText(QApplication::translate("osgqt", "Reset Camera", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("osgqt", "Motion Model2PCL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class osgqt: public Ui_osgqt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSGQT_H
