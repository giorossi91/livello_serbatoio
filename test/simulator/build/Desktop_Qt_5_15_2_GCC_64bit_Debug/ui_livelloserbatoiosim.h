/********************************************************************************
** Form generated from reading UI file 'livelloserbatoiosim.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIVELLOSERBATOIOSIM_H
#define UI_LIVELLOSERBATOIOSIM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LivelloSerbatoioSim
{
public:
    QAction *actionPause;
    QAction *actionResume;
    QAction *actionShow_Digital_I_O;
    QAction *actionReset;
    QWidget *sim_widget;
    QGridLayout *gridLayout;
    QTextEdit *lcd_textedit;
    QTextBrowser *serial_textbrowser;
    QSlider *distance_vslider;
    QHBoxLayout *horizontalLayout;
    QLabel *led_label;
    QSpacerItem *horizontalSpacer;
    QComboBox *timescale_combobox;
    QPushButton *show_button;
    QDoubleSpinBox *distance_spinbox;
    QLabel *distance_label;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *lineardischarge_button;
    QPushButton *linearcharge_button;
    QPushButton *random_button;
    QPushButton *manual_button;
    QPushButton *tankMax_button;
    QPushButton *sensMax_button;
    QMenuBar *menuBar;
    QMenu *menuControls;
    QMenu *menuD_IO;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LivelloSerbatoioSim)
    {
        if (LivelloSerbatoioSim->objectName().isEmpty())
            LivelloSerbatoioSim->setObjectName(QString::fromUtf8("LivelloSerbatoioSim"));
        LivelloSerbatoioSim->resize(622, 454);
        LivelloSerbatoioSim->setMinimumSize(QSize(550, 0));
        actionPause = new QAction(LivelloSerbatoioSim);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        actionResume = new QAction(LivelloSerbatoioSim);
        actionResume->setObjectName(QString::fromUtf8("actionResume"));
        actionShow_Digital_I_O = new QAction(LivelloSerbatoioSim);
        actionShow_Digital_I_O->setObjectName(QString::fromUtf8("actionShow_Digital_I_O"));
        actionReset = new QAction(LivelloSerbatoioSim);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        sim_widget = new QWidget(LivelloSerbatoioSim);
        sim_widget->setObjectName(QString::fromUtf8("sim_widget"));
        gridLayout = new QGridLayout(sim_widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lcd_textedit = new QTextEdit(sim_widget);
        lcd_textedit->setObjectName(QString::fromUtf8("lcd_textedit"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        font.setPointSize(8);
        lcd_textedit->setFont(font);
        lcd_textedit->setReadOnly(true);

        gridLayout->addWidget(lcd_textedit, 0, 0, 4, 1);

        serial_textbrowser = new QTextBrowser(sim_widget);
        serial_textbrowser->setObjectName(QString::fromUtf8("serial_textbrowser"));
        serial_textbrowser->setFont(font);
        serial_textbrowser->setLineWrapMode(QTextEdit::NoWrap);
        serial_textbrowser->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        serial_textbrowser->setOpenLinks(false);

        gridLayout->addWidget(serial_textbrowser, 4, 0, 1, 1);

        distance_vslider = new QSlider(sim_widget);
        distance_vslider->setObjectName(QString::fromUtf8("distance_vslider"));
        distance_vslider->setMaximum(500);
        distance_vslider->setValue(2);
        distance_vslider->setOrientation(Qt::Vertical);
        distance_vslider->setInvertedAppearance(true);
        distance_vslider->setInvertedControls(true);
        distance_vslider->setTickPosition(QSlider::TicksBothSides);
        distance_vslider->setTickInterval(10);

        gridLayout->addWidget(distance_vslider, 1, 2, 4, 1, Qt::AlignHCenter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        led_label = new QLabel(sim_widget);
        led_label->setObjectName(QString::fromUtf8("led_label"));
        led_label->setMinimumSize(QSize(40, 40));
        led_label->setMaximumSize(QSize(16777215, 16777215));
        led_label->setStyleSheet(QString::fromUtf8("border: 1px solid black;\n"
"border-radius:20px;\n"
""));

        horizontalLayout->addWidget(led_label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        timescale_combobox = new QComboBox(sim_widget);
        timescale_combobox->addItem(QString());
        timescale_combobox->addItem(QString());
        timescale_combobox->addItem(QString());
        timescale_combobox->addItem(QString());
        timescale_combobox->addItem(QString());
        timescale_combobox->setObjectName(QString::fromUtf8("timescale_combobox"));

        horizontalLayout->addWidget(timescale_combobox);

        show_button = new QPushButton(sim_widget);
        show_button->setObjectName(QString::fromUtf8("show_button"));
        show_button->setMaximumSize(QSize(200, 16777215));

        horizontalLayout->addWidget(show_button);


        gridLayout->addLayout(horizontalLayout, 7, 0, 1, 1);

        distance_spinbox = new QDoubleSpinBox(sim_widget);
        distance_spinbox->setObjectName(QString::fromUtf8("distance_spinbox"));
        distance_spinbox->setDecimals(0);
        distance_spinbox->setMaximum(500.000000000000000);
        distance_spinbox->setSingleStep(1.000000000000000);
        distance_spinbox->setValue(2.000000000000000);

        gridLayout->addWidget(distance_spinbox, 7, 2, 1, 1);

        distance_label = new QLabel(sim_widget);
        distance_label->setObjectName(QString::fromUtf8("distance_label"));

        gridLayout->addWidget(distance_label, 0, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineardischarge_button = new QPushButton(sim_widget);
        lineardischarge_button->setObjectName(QString::fromUtf8("lineardischarge_button"));

        horizontalLayout_2->addWidget(lineardischarge_button);

        linearcharge_button = new QPushButton(sim_widget);
        linearcharge_button->setObjectName(QString::fromUtf8("linearcharge_button"));

        horizontalLayout_2->addWidget(linearcharge_button);

        random_button = new QPushButton(sim_widget);
        random_button->setObjectName(QString::fromUtf8("random_button"));

        horizontalLayout_2->addWidget(random_button);

        manual_button = new QPushButton(sim_widget);
        manual_button->setObjectName(QString::fromUtf8("manual_button"));

        horizontalLayout_2->addWidget(manual_button);

        tankMax_button = new QPushButton(sim_widget);
        tankMax_button->setObjectName(QString::fromUtf8("tankMax_button"));

        horizontalLayout_2->addWidget(tankMax_button);

        sensMax_button = new QPushButton(sim_widget);
        sensMax_button->setObjectName(QString::fromUtf8("sensMax_button"));

        horizontalLayout_2->addWidget(sensMax_button);


        gridLayout->addLayout(horizontalLayout_2, 5, 0, 1, 3);

        LivelloSerbatoioSim->setCentralWidget(sim_widget);
        menuBar = new QMenuBar(LivelloSerbatoioSim);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 622, 24));
        menuControls = new QMenu(menuBar);
        menuControls->setObjectName(QString::fromUtf8("menuControls"));
        menuD_IO = new QMenu(menuBar);
        menuD_IO->setObjectName(QString::fromUtf8("menuD_IO"));
        LivelloSerbatoioSim->setMenuBar(menuBar);
        statusBar = new QStatusBar(LivelloSerbatoioSim);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        LivelloSerbatoioSim->setStatusBar(statusBar);

        menuBar->addAction(menuControls->menuAction());
        menuBar->addAction(menuD_IO->menuAction());
        menuControls->addAction(actionPause);
        menuControls->addAction(actionResume);
        menuD_IO->addAction(actionShow_Digital_I_O);

        retranslateUi(LivelloSerbatoioSim);

        timescale_combobox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(LivelloSerbatoioSim);
    } // setupUi

    void retranslateUi(QMainWindow *LivelloSerbatoioSim)
    {
        LivelloSerbatoioSim->setWindowTitle(QCoreApplication::translate("LivelloSerbatoioSim", "LivelloSerbatoioSim", nullptr));
        actionPause->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Pause", nullptr));
        actionResume->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Resume", nullptr));
        actionShow_Digital_I_O->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Show Digital I/O", nullptr));
        actionReset->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Reset", nullptr));
        lcd_textedit->setHtml(QCoreApplication::translate("LivelloSerbatoioSim", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Monospace'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">test</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
#if QT_CONFIG(tooltip)
        serial_textbrowser->setToolTip(QCoreApplication::translate("LivelloSerbatoioSim", "Serial Monitor", nullptr));
#endif // QT_CONFIG(tooltip)
        led_label->setText(QString());
        timescale_combobox->setItemText(0, QCoreApplication::translate("LivelloSerbatoioSim", "0.01", nullptr));
        timescale_combobox->setItemText(1, QCoreApplication::translate("LivelloSerbatoioSim", "0.1", nullptr));
        timescale_combobox->setItemText(2, QCoreApplication::translate("LivelloSerbatoioSim", "1", nullptr));
        timescale_combobox->setItemText(3, QCoreApplication::translate("LivelloSerbatoioSim", "10", nullptr));
        timescale_combobox->setItemText(4, QCoreApplication::translate("LivelloSerbatoioSim", "100", nullptr));

#if QT_CONFIG(tooltip)
        timescale_combobox->setToolTip(QCoreApplication::translate("LivelloSerbatoioSim", "Time scale", nullptr));
#endif // QT_CONFIG(tooltip)
        show_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "LCD on / Stats", nullptr));
        distance_spinbox->setSuffix(QCoreApplication::translate("LivelloSerbatoioSim", " cm", nullptr));
        distance_label->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Distance (cm)", nullptr));
        lineardischarge_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Lin. Discharge", nullptr));
        linearcharge_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Lin. Charge", nullptr));
        random_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Random Val", nullptr));
        manual_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Manual", nullptr));
        tankMax_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Tank Max", nullptr));
        sensMax_button->setText(QCoreApplication::translate("LivelloSerbatoioSim", "Sensor Distance", nullptr));
        menuControls->setTitle(QCoreApplication::translate("LivelloSerbatoioSim", "Controls", nullptr));
        menuD_IO->setTitle(QCoreApplication::translate("LivelloSerbatoioSim", "D/IO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LivelloSerbatoioSim: public Ui_LivelloSerbatoioSim {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIVELLOSERBATOIOSIM_H
