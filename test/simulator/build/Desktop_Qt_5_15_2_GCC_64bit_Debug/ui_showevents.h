/********************************************************************************
** Form generated from reading UI file 'showevents.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWEVENTS_H
#define UI_SHOWEVENTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowEvents
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *d_io_chart_widget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ShowEvents)
    {
        if (ShowEvents->objectName().isEmpty())
            ShowEvents->setObjectName(QString::fromUtf8("ShowEvents"));
        ShowEvents->resize(800, 600);
        centralwidget = new QWidget(ShowEvents);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        d_io_chart_widget = new QWidget(centralwidget);
        d_io_chart_widget->setObjectName(QString::fromUtf8("d_io_chart_widget"));

        gridLayout->addWidget(d_io_chart_widget, 0, 0, 1, 1);

        ShowEvents->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ShowEvents);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        ShowEvents->setMenuBar(menubar);
        statusbar = new QStatusBar(ShowEvents);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ShowEvents->setStatusBar(statusbar);

        retranslateUi(ShowEvents);

        QMetaObject::connectSlotsByName(ShowEvents);
    } // setupUi

    void retranslateUi(QMainWindow *ShowEvents)
    {
        ShowEvents->setWindowTitle(QCoreApplication::translate("ShowEvents", "Digital IO Status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowEvents: public Ui_ShowEvents {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWEVENTS_H
