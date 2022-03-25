/********************************************************************************
** Form generated from reading UI file 'DataSplit.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATASPLIT_H
#define UI_DATASPLIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataSplitClass
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QPushButton *Rbt_OpenCom;
    QLabel *label_15;
    QComboBox *Rcb_com;
    QComboBox *Rcb_Baudrate;
    QLabel *label_16;
    QLabel *Rlb_state;
    QLabel *Rlb_stateT;
    QCheckBox *Rcb_saveFile;
    QCheckBox *Rcb_showRData;
    QPushButton *pb_clear;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QTextEdit *Log_textEdit;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QTextEdit *Log_textEdit_2;
    QGroupBox *groupBox_2;
    QPushButton *Sbt_OpenCom;
    QLabel *label_17;
    QComboBox *Scb_com;
    QComboBox *Scb_Baudrate;
    QLabel *label_18;
    QLabel *Slb_state;
    QLabel *Slb_stateT;
    QPushButton *Sbt_sendData;
    QCheckBox *Scb_showRData;

    void setupUi(QWidget *DataSplitClass)
    {
        if (DataSplitClass->objectName().isEmpty())
            DataSplitClass->setObjectName(QStringLiteral("DataSplitClass"));
        DataSplitClass->resize(798, 484);
        QIcon icon;
        icon.addFile(QStringLiteral(":/DataSplit/image_1/logo64.ico"), QSize(), QIcon::Normal, QIcon::Off);
        DataSplitClass->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(DataSplitClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(DataSplitClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(201, 0));
        Rbt_OpenCom = new QPushButton(groupBox);
        Rbt_OpenCom->setObjectName(QStringLiteral("Rbt_OpenCom"));
        Rbt_OpenCom->setGeometry(QRect(20, 190, 160, 31));
        Rbt_OpenCom->setStyleSheet(QLatin1String("QPushButton:hover{\n"
" 	background-color:transparent;\n"
" 	border-image: url(:/DataSplit/image_1/bt_hover.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton:pressed{\n"
" 	background-color:transparent; \n"
" 	border-image: url(:/DataSplit/image_1/bt_active.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton {\n"
"  	background-color:transparent;\n"
"  	border-image: url(:/DataSplit/image_1/bt_normal.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(20, 20, 51, 20));
        Rcb_com = new QComboBox(groupBox);
        Rcb_com->setObjectName(QStringLiteral("Rcb_com"));
        Rcb_com->setGeometry(QRect(20, 40, 160, 31));
        QFont font;
        font.setPointSize(12);
        Rcb_com->setFont(font);
        Rcb_com->setContextMenuPolicy(Qt::DefaultContextMenu);
        Rcb_com->setLayoutDirection(Qt::LeftToRight);
        Rcb_com->setStyleSheet(QStringLiteral("border: 1px solid darkgray;"));
        Rcb_com->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        Rcb_Baudrate = new QComboBox(groupBox);
        Rcb_Baudrate->setObjectName(QStringLiteral("Rcb_Baudrate"));
        Rcb_Baudrate->setGeometry(QRect(20, 100, 160, 31));
        Rcb_Baudrate->setFont(font);
        Rcb_Baudrate->setStyleSheet(QStringLiteral("border: 1px solid darkgray;"));
        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(20, 80, 54, 20));
        Rlb_state = new QLabel(groupBox);
        Rlb_state->setObjectName(QStringLiteral("Rlb_state"));
        Rlb_state->setGeometry(QRect(130, 151, 51, 31));
        Rlb_state->setStyleSheet(QStringLiteral("image: url(:/DataSplit/image_1/Connection statuP_red.png);"));
        Rlb_stateT = new QLabel(groupBox);
        Rlb_stateT->setObjectName(QStringLiteral("Rlb_stateT"));
        Rlb_stateT->setGeometry(QRect(30, 150, 81, 31));
        Rcb_saveFile = new QCheckBox(groupBox);
        Rcb_saveFile->setObjectName(QStringLiteral("Rcb_saveFile"));
        Rcb_saveFile->setGeometry(QRect(60, 250, 101, 20));
        Rcb_showRData = new QCheckBox(groupBox);
        Rcb_showRData->setObjectName(QStringLiteral("Rcb_showRData"));
        Rcb_showRData->setGeometry(QRect(60, 290, 91, 21));
        pb_clear = new QPushButton(groupBox);
        pb_clear->setObjectName(QStringLiteral("pb_clear"));
        pb_clear->setGeometry(QRect(60, 380, 75, 23));

        horizontalLayout->addWidget(groupBox);

        widget = new QWidget(DataSplitClass);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 3, 0, 3);
        Log_textEdit = new QTextEdit(groupBox_3);
        Log_textEdit->setObjectName(QStringLiteral("Log_textEdit"));
        Log_textEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        Log_textEdit->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        Log_textEdit->setReadOnly(true);

        gridLayout->addWidget(Log_textEdit, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_3, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(widget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 3, 0, 3);
        Log_textEdit_2 = new QTextEdit(groupBox_4);
        Log_textEdit_2->setObjectName(QStringLiteral("Log_textEdit_2"));
        Log_textEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        Log_textEdit_2->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        Log_textEdit_2->setReadOnly(true);

        gridLayout_2->addWidget(Log_textEdit_2, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_4, 1, 0, 1, 1);


        horizontalLayout->addWidget(widget);

        groupBox_2 = new QGroupBox(DataSplitClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(201, 0));
        Sbt_OpenCom = new QPushButton(groupBox_2);
        Sbt_OpenCom->setObjectName(QStringLiteral("Sbt_OpenCom"));
        Sbt_OpenCom->setGeometry(QRect(20, 190, 160, 31));
        Sbt_OpenCom->setStyleSheet(QLatin1String("QPushButton:hover{\n"
" 	background-color:transparent;\n"
" 	border-image: url(:/DataSplit/image_1/bt_hover.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton:pressed{\n"
" 	background-color:transparent; \n"
" 	border-image: url(:/DataSplit/image_1/bt_active.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton {\n"
"  	background-color:transparent;\n"
"  	border-image: url(:/DataSplit/image_1/bt_normal.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(20, 20, 51, 20));
        Scb_com = new QComboBox(groupBox_2);
        Scb_com->setObjectName(QStringLiteral("Scb_com"));
        Scb_com->setGeometry(QRect(20, 40, 160, 31));
        Scb_com->setFont(font);
        Scb_com->setContextMenuPolicy(Qt::DefaultContextMenu);
        Scb_com->setLayoutDirection(Qt::LeftToRight);
        Scb_com->setStyleSheet(QStringLiteral("border: 1px solid darkgray;"));
        Scb_com->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        Scb_Baudrate = new QComboBox(groupBox_2);
        Scb_Baudrate->setObjectName(QStringLiteral("Scb_Baudrate"));
        Scb_Baudrate->setGeometry(QRect(20, 100, 160, 31));
        Scb_Baudrate->setFont(font);
        Scb_Baudrate->setStyleSheet(QStringLiteral("border: 1px solid darkgray;"));
        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(20, 80, 54, 20));
        Slb_state = new QLabel(groupBox_2);
        Slb_state->setObjectName(QStringLiteral("Slb_state"));
        Slb_state->setGeometry(QRect(120, 151, 51, 31));
        Slb_state->setStyleSheet(QStringLiteral("image: url(:/DataSplit/image_1/Connection statuP_red.png);"));
        Slb_stateT = new QLabel(groupBox_2);
        Slb_stateT->setObjectName(QStringLiteral("Slb_stateT"));
        Slb_stateT->setGeometry(QRect(20, 150, 81, 31));
        Sbt_sendData = new QPushButton(groupBox_2);
        Sbt_sendData->setObjectName(QStringLiteral("Sbt_sendData"));
        Sbt_sendData->setGeometry(QRect(20, 220, 160, 31));
        Sbt_sendData->setStyleSheet(QLatin1String("QPushButton:hover{\n"
" 	background-color:transparent;\n"
" 	border-image: url(:/DataSplit/image_1/bt_hover.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton:pressed{\n"
" 	background-color:transparent; \n"
" 	border-image: url(:/DataSplit/image_1/bt_active.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);}\n"
"QPushButton {\n"
"  	background-color:transparent;\n"
"  	border-image: url(:/DataSplit/image_1/bt_normal.png);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        Scb_showRData = new QCheckBox(groupBox_2);
        Scb_showRData->setObjectName(QStringLiteral("Scb_showRData"));
        Scb_showRData->setGeometry(QRect(60, 250, 91, 21));

        horizontalLayout->addWidget(groupBox_2);


        retranslateUi(DataSplitClass);

        QMetaObject::connectSlotsByName(DataSplitClass);
    } // setupUi

    void retranslateUi(QWidget *DataSplitClass)
    {
        DataSplitClass->setWindowTitle(QApplication::translate("DataSplitClass", "\344\270\200\344\275\223\345\214\226\346\216\247\345\210\266\345\215\225\345\205\203-\346\225\260\346\215\256\346\213\206\345\214\205", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DataSplitClass", "\346\216\245\346\224\266\344\270\262\345\217\243", Q_NULLPTR));
        Rbt_OpenCom->setText(QApplication::translate("DataSplitClass", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        label_15->setText(QApplication::translate("DataSplitClass", "\344\270\262\345\217\243\345\217\267\357\274\232", Q_NULLPTR));
        label_16->setText(QApplication::translate("DataSplitClass", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        Rlb_state->setText(QString());
        Rlb_stateT->setText(QApplication::translate("DataSplitClass", "\347\212\266\346\200\201\357\274\232\346\234\252\346\211\223\345\274\200", Q_NULLPTR));
        Rcb_saveFile->setText(QApplication::translate("DataSplitClass", "\344\277\235\345\255\230\346\216\245\346\224\266\346\225\260\346\215\256", Q_NULLPTR));
        Rcb_showRData->setText(QApplication::translate("DataSplitClass", "\346\230\276\347\244\272\346\216\245\346\224\266\346\225\260\346\215\256", Q_NULLPTR));
        pb_clear->setText(QApplication::translate("DataSplitClass", "\346\270\205\347\251\272\346\230\276\347\244\272", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("DataSplitClass", "\346\216\245\346\224\266\346\225\260\346\215\256", Q_NULLPTR));
#ifndef QT_NO_ACCESSIBILITY
        Log_textEdit->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        groupBox_4->setTitle(QApplication::translate("DataSplitClass", "\345\267\262\345\217\221\346\225\260\346\215\256", Q_NULLPTR));
#ifndef QT_NO_ACCESSIBILITY
        Log_textEdit_2->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        groupBox_2->setTitle(QApplication::translate("DataSplitClass", "\345\217\221\351\200\201\344\270\262\345\217\243", Q_NULLPTR));
        Sbt_OpenCom->setText(QApplication::translate("DataSplitClass", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        label_17->setText(QApplication::translate("DataSplitClass", "\344\270\262\345\217\243\345\217\267\357\274\232", Q_NULLPTR));
        label_18->setText(QApplication::translate("DataSplitClass", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        Slb_state->setText(QString());
        Slb_stateT->setText(QApplication::translate("DataSplitClass", "\347\212\266\346\200\201\357\274\232\346\234\252\346\211\223\345\274\200", Q_NULLPTR));
        Sbt_sendData->setText(QApplication::translate("DataSplitClass", "\345\217\221\351\200\201\346\226\207\344\273\266", Q_NULLPTR));
        Scb_showRData->setText(QApplication::translate("DataSplitClass", "\346\230\276\347\244\272\345\217\221\351\200\201\346\225\260\346\215\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DataSplitClass: public Ui_DataSplitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATASPLIT_H
