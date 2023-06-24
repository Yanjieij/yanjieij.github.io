/********************************************************************************
** Form generated from reading UI file 'MCES.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MCES_H
#define UI_MCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MCESClass
{
public:
    QWidget *centralWidget;
    QLabel *text_condition;
    QLabel *text_console;
    QFrame *conditionFrame;
    QWidget *formLayoutWidget;
    QFormLayout *conditionLayout;
    QLabel *text_elevatorNum;
    QLineEdit *elevatorNum;
    QSpacerItem *verticalSpacer_3;
    QLabel *text_maxHeight;
    QSpacerItem *verticalSpacer_2;
    QLabel *text_cabinVolume;
    QLineEdit *cabinVolume;
    QSpacerItem *verticalSpacer;
    QLabel *text_simulateDuration;
    QLineEdit *simulateDuration;
    QLineEdit *maxHeight;
    QFrame *policyFrame;
    QWidget *gridLayoutWidget;
    QGridLayout *policyLayout;
    QComboBox *policySelection;
    QLabel *text_currentPolicy;
    QTextBrowser *policyDescribe;
    QFrame *consoleFrame;
    QWidget *gridLayoutWidget_2;
    QGridLayout *consoleLayout;
    QLabel *leftTime;
    QLabel *text_leftTime;
    QPushButton *startSimulateButton;
    QPushButton *pauseSimulateButton;
    QProgressBar *simulateProcess;
    QLabel *currentStatus;
    QLabel *text_status;
    QFrame *simulateGraphFrame;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *simulateGraph;
    QLabel *text_simulateGraph;
    QFrame *simulateResultFrame;
    QWidget *gridLayoutWidget_4;
    QGridLayout *simulateResultLayout;
    QLabel *text_ARC;
    QLabel *ARC;
    QLabel *AWD;
    QLabel *ATD;
    QLabel *text_ATD;
    QLabel *text_AWD;
    QFrame *modelSelectionFrame;
    QLabel *text_model;
    QComboBox *modelSelection;

    void setupUi(QMainWindow *MCESClass)
    {
        if (MCESClass->objectName().isEmpty())
            MCESClass->setObjectName(QString::fromUtf8("MCESClass"));
        MCESClass->resize(1200, 700);
        centralWidget = new QWidget(MCESClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        text_condition = new QLabel(centralWidget);
        text_condition->setObjectName(QString::fromUtf8("text_condition"));
        text_condition->setGeometry(QRect(30, 6, 111, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(10);
        text_condition->setFont(font);
        text_condition->setTextFormat(Qt::AutoText);
        text_console = new QLabel(centralWidget);
        text_console->setObjectName(QString::fromUtf8("text_console"));
        text_console->setGeometry(QRect(20, 456, 51, 20));
        text_console->setFont(font);
        conditionFrame = new QFrame(centralWidget);
        conditionFrame->setObjectName(QString::fromUtf8("conditionFrame"));
        conditionFrame->setGeometry(QRect(20, 30, 201, 181));
        conditionFrame->setFrameShape(QFrame::Box);
        conditionFrame->setFrameShadow(QFrame::Sunken);
        conditionFrame->setLineWidth(2);
        formLayoutWidget = new QWidget(conditionFrame);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 181, 157));
        conditionLayout = new QFormLayout(formLayoutWidget);
        conditionLayout->setSpacing(6);
        conditionLayout->setContentsMargins(11, 11, 11, 11);
        conditionLayout->setObjectName(QString::fromUtf8("conditionLayout"));
        conditionLayout->setContentsMargins(0, 0, 0, 0);
        text_elevatorNum = new QLabel(formLayoutWidget);
        text_elevatorNum->setObjectName(QString::fromUtf8("text_elevatorNum"));

        conditionLayout->setWidget(0, QFormLayout::LabelRole, text_elevatorNum);

        elevatorNum = new QLineEdit(formLayoutWidget);
        elevatorNum->setObjectName(QString::fromUtf8("elevatorNum"));
        elevatorNum->setAlignment(Qt::AlignCenter);

        conditionLayout->setWidget(0, QFormLayout::FieldRole, elevatorNum);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        conditionLayout->setItem(1, QFormLayout::LabelRole, verticalSpacer_3);

        text_maxHeight = new QLabel(formLayoutWidget);
        text_maxHeight->setObjectName(QString::fromUtf8("text_maxHeight"));

        conditionLayout->setWidget(2, QFormLayout::LabelRole, text_maxHeight);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        conditionLayout->setItem(3, QFormLayout::LabelRole, verticalSpacer_2);

        text_cabinVolume = new QLabel(formLayoutWidget);
        text_cabinVolume->setObjectName(QString::fromUtf8("text_cabinVolume"));

        conditionLayout->setWidget(4, QFormLayout::LabelRole, text_cabinVolume);

        cabinVolume = new QLineEdit(formLayoutWidget);
        cabinVolume->setObjectName(QString::fromUtf8("cabinVolume"));
        cabinVolume->setAlignment(Qt::AlignCenter);

        conditionLayout->setWidget(4, QFormLayout::FieldRole, cabinVolume);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        conditionLayout->setItem(5, QFormLayout::LabelRole, verticalSpacer);

        text_simulateDuration = new QLabel(formLayoutWidget);
        text_simulateDuration->setObjectName(QString::fromUtf8("text_simulateDuration"));
        text_simulateDuration->setAlignment(Qt::AlignCenter);

        conditionLayout->setWidget(6, QFormLayout::LabelRole, text_simulateDuration);

        simulateDuration = new QLineEdit(formLayoutWidget);
        simulateDuration->setObjectName(QString::fromUtf8("simulateDuration"));
        simulateDuration->setAlignment(Qt::AlignCenter);
        simulateDuration->setClearButtonEnabled(false);

        conditionLayout->setWidget(6, QFormLayout::FieldRole, simulateDuration);

        maxHeight = new QLineEdit(formLayoutWidget);
        maxHeight->setObjectName(QString::fromUtf8("maxHeight"));
        maxHeight->setAlignment(Qt::AlignCenter);
        maxHeight->setReadOnly(false);

        conditionLayout->setWidget(2, QFormLayout::FieldRole, maxHeight);

        policyFrame = new QFrame(centralWidget);
        policyFrame->setObjectName(QString::fromUtf8("policyFrame"));
        policyFrame->setGeometry(QRect(10, 300, 291, 141));
        policyFrame->setFrameShape(QFrame::Box);
        policyFrame->setFrameShadow(QFrame::Sunken);
        policyFrame->setLineWidth(2);
        gridLayoutWidget = new QWidget(policyFrame);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 273, 124));
        policyLayout = new QGridLayout(gridLayoutWidget);
        policyLayout->setSpacing(6);
        policyLayout->setContentsMargins(11, 11, 11, 11);
        policyLayout->setObjectName(QString::fromUtf8("policyLayout"));
        policyLayout->setContentsMargins(0, 0, 0, 0);
        policySelection = new QComboBox(gridLayoutWidget);
        policySelection->addItem(QString());
        policySelection->addItem(QString());
        policySelection->addItem(QString());
        policySelection->addItem(QString());
        policySelection->setObjectName(QString::fromUtf8("policySelection"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(9);
        policySelection->setFont(font1);
        policySelection->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        policyLayout->addWidget(policySelection, 0, 1, 1, 1);

        text_currentPolicy = new QLabel(gridLayoutWidget);
        text_currentPolicy->setObjectName(QString::fromUtf8("text_currentPolicy"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        text_currentPolicy->setFont(font2);

        policyLayout->addWidget(text_currentPolicy, 0, 0, 1, 1);

        policyDescribe = new QTextBrowser(gridLayoutWidget);
        policyDescribe->setObjectName(QString::fromUtf8("policyDescribe"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font3.setPointSize(10);
        font3.setUnderline(false);
        policyDescribe->setFont(font3);
        policyDescribe->setFocusPolicy(Qt::StrongFocus);
        policyDescribe->setLayoutDirection(Qt::LeftToRight);

        policyLayout->addWidget(policyDescribe, 1, 0, 1, 2);

        consoleFrame = new QFrame(centralWidget);
        consoleFrame->setObjectName(QString::fromUtf8("consoleFrame"));
        consoleFrame->setGeometry(QRect(10, 480, 221, 191));
        consoleFrame->setFrameShape(QFrame::Box);
        consoleFrame->setFrameShadow(QFrame::Sunken);
        consoleFrame->setLineWidth(2);
        gridLayoutWidget_2 = new QWidget(consoleFrame);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 201, 171));
        consoleLayout = new QGridLayout(gridLayoutWidget_2);
        consoleLayout->setSpacing(6);
        consoleLayout->setContentsMargins(11, 11, 11, 11);
        consoleLayout->setObjectName(QString::fromUtf8("consoleLayout"));
        consoleLayout->setContentsMargins(0, 0, 0, 0);
        leftTime = new QLabel(gridLayoutWidget_2);
        leftTime->setObjectName(QString::fromUtf8("leftTime"));
        leftTime->setFrameShape(QFrame::Box);
        leftTime->setFrameShadow(QFrame::Raised);
        leftTime->setLineWidth(1);
        leftTime->setAlignment(Qt::AlignCenter);
        leftTime->setMargin(1);

        consoleLayout->addWidget(leftTime, 1, 2, 1, 1);

        text_leftTime = new QLabel(gridLayoutWidget_2);
        text_leftTime->setObjectName(QString::fromUtf8("text_leftTime"));
        text_leftTime->setFrameShape(QFrame::Box);
        text_leftTime->setFrameShadow(QFrame::Raised);
        text_leftTime->setAlignment(Qt::AlignCenter);
        text_leftTime->setMargin(0);

        consoleLayout->addWidget(text_leftTime, 1, 1, 1, 1);

        startSimulateButton = new QPushButton(gridLayoutWidget_2);
        startSimulateButton->setObjectName(QString::fromUtf8("startSimulateButton"));
        startSimulateButton->setFont(font);

        consoleLayout->addWidget(startSimulateButton, 3, 1, 1, 1);

        pauseSimulateButton = new QPushButton(gridLayoutWidget_2);
        pauseSimulateButton->setObjectName(QString::fromUtf8("pauseSimulateButton"));
        pauseSimulateButton->setFont(font);

        consoleLayout->addWidget(pauseSimulateButton, 3, 2, 1, 1);

        simulateProcess = new QProgressBar(gridLayoutWidget_2);
        simulateProcess->setObjectName(QString::fromUtf8("simulateProcess"));
        simulateProcess->setValue(0);
        simulateProcess->setAlignment(Qt::AlignCenter);

        consoleLayout->addWidget(simulateProcess, 2, 1, 1, 2);

        currentStatus = new QLabel(gridLayoutWidget_2);
        currentStatus->setObjectName(QString::fromUtf8("currentStatus"));
        currentStatus->setFrameShape(QFrame::Box);
        currentStatus->setFrameShadow(QFrame::Raised);
        currentStatus->setAlignment(Qt::AlignCenter);
        currentStatus->setMargin(1);

        consoleLayout->addWidget(currentStatus, 0, 2, 1, 1);

        text_status = new QLabel(gridLayoutWidget_2);
        text_status->setObjectName(QString::fromUtf8("text_status"));
        text_status->setFrameShape(QFrame::Box);
        text_status->setFrameShadow(QFrame::Raised);
        text_status->setMidLineWidth(0);
        text_status->setScaledContents(false);
        text_status->setAlignment(Qt::AlignCenter);
        text_status->setWordWrap(false);
        text_status->setMargin(0);

        consoleLayout->addWidget(text_status, 0, 1, 1, 1);

        simulateGraphFrame = new QFrame(centralWidget);
        simulateGraphFrame->setObjectName(QString::fromUtf8("simulateGraphFrame"));
        simulateGraphFrame->setGeometry(QRect(320, 30, 861, 551));
        simulateGraphFrame->setFrameShape(QFrame::Box);
        simulateGraphFrame->setFrameShadow(QFrame::Sunken);
        simulateGraphFrame->setLineWidth(2);
        horizontalLayoutWidget = new QWidget(simulateGraphFrame);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 841, 531));
        simulateGraph = new QHBoxLayout(horizontalLayoutWidget);
        simulateGraph->setSpacing(6);
        simulateGraph->setContentsMargins(11, 11, 11, 11);
        simulateGraph->setObjectName(QString::fromUtf8("simulateGraph"));
        simulateGraph->setContentsMargins(0, 0, 0, 0);
        text_simulateGraph = new QLabel(centralWidget);
        text_simulateGraph->setObjectName(QString::fromUtf8("text_simulateGraph"));
        text_simulateGraph->setGeometry(QRect(330, 6, 91, 20));
        text_simulateGraph->setFont(font);
        simulateResultFrame = new QFrame(centralWidget);
        simulateResultFrame->setObjectName(QString::fromUtf8("simulateResultFrame"));
        simulateResultFrame->setGeometry(QRect(260, 590, 931, 101));
        simulateResultFrame->setFrameShape(QFrame::Box);
        simulateResultFrame->setFrameShadow(QFrame::Sunken);
        simulateResultFrame->setLineWidth(2);
        gridLayoutWidget_4 = new QWidget(simulateResultFrame);
        gridLayoutWidget_4->setObjectName(QString::fromUtf8("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(10, 10, 911, 81));
        simulateResultLayout = new QGridLayout(gridLayoutWidget_4);
        simulateResultLayout->setSpacing(6);
        simulateResultLayout->setContentsMargins(11, 11, 11, 11);
        simulateResultLayout->setObjectName(QString::fromUtf8("simulateResultLayout"));
        simulateResultLayout->setContentsMargins(0, 0, 0, 0);
        text_ARC = new QLabel(gridLayoutWidget_4);
        text_ARC->setObjectName(QString::fromUtf8("text_ARC"));
        text_ARC->setFrameShape(QFrame::Box);
        text_ARC->setFrameShadow(QFrame::Raised);
        text_ARC->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(text_ARC, 0, 2, 1, 1);

        ARC = new QLabel(gridLayoutWidget_4);
        ARC->setObjectName(QString::fromUtf8("ARC"));
        ARC->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(ARC, 1, 2, 1, 1);

        AWD = new QLabel(gridLayoutWidget_4);
        AWD->setObjectName(QString::fromUtf8("AWD"));
        AWD->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(AWD, 1, 0, 1, 1);

        ATD = new QLabel(gridLayoutWidget_4);
        ATD->setObjectName(QString::fromUtf8("ATD"));
        ATD->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(ATD, 1, 1, 1, 1);

        text_ATD = new QLabel(gridLayoutWidget_4);
        text_ATD->setObjectName(QString::fromUtf8("text_ATD"));
        text_ATD->setFrameShape(QFrame::Box);
        text_ATD->setFrameShadow(QFrame::Raised);
        text_ATD->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(text_ATD, 0, 1, 1, 1);

        text_AWD = new QLabel(gridLayoutWidget_4);
        text_AWD->setObjectName(QString::fromUtf8("text_AWD"));
        text_AWD->setFrameShape(QFrame::Box);
        text_AWD->setFrameShadow(QFrame::Raised);
        text_AWD->setAlignment(Qt::AlignCenter);

        simulateResultLayout->addWidget(text_AWD, 0, 0, 1, 1);

        modelSelectionFrame = new QFrame(centralWidget);
        modelSelectionFrame->setObjectName(QString::fromUtf8("modelSelectionFrame"));
        modelSelectionFrame->setGeometry(QRect(10, 230, 301, 41));
        modelSelectionFrame->setFrameShape(QFrame::Box);
        modelSelectionFrame->setFrameShadow(QFrame::Sunken);
        modelSelectionFrame->setLineWidth(2);
        text_model = new QLabel(modelSelectionFrame);
        text_model->setObjectName(QString::fromUtf8("text_model"));
        text_model->setGeometry(QRect(10, 10, 101, 21));
        text_model->setFont(font2);
        modelSelection = new QComboBox(modelSelectionFrame);
        modelSelection->addItem(QString());
        modelSelection->addItem(QString());
        modelSelection->setObjectName(QString::fromUtf8("modelSelection"));
        modelSelection->setGeometry(QRect(110, 10, 122, 20));
        modelSelection->setFont(font1);
        modelSelection->setLayoutDirection(Qt::LeftToRight);
        modelSelection->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        MCESClass->setCentralWidget(centralWidget);

        retranslateUi(MCESClass);

        QMetaObject::connectSlotsByName(MCESClass);
    } // setupUi

    void retranslateUi(QMainWindow *MCESClass)
    {
        MCESClass->setWindowTitle(QCoreApplication::translate("MCESClass", "MCES", nullptr));
        text_condition->setText(QCoreApplication::translate("MCESClass", "\345\210\235\345\247\213\346\235\241\344\273\266\346\216\247\345\210\266", nullptr));
        text_console->setText(QCoreApplication::translate("MCESClass", "\346\216\247\345\210\266\345\217\260", nullptr));
        text_elevatorNum->setText(QCoreApplication::translate("MCESClass", "\347\224\265\346\242\257\346\225\260\351\207\217", nullptr));
        elevatorNum->setText(QString());
        elevatorNum->setPlaceholderText(QCoreApplication::translate("MCESClass", "\357\274\2101-4\357\274\211\345\217\260", nullptr));
        text_maxHeight->setText(QCoreApplication::translate("MCESClass", "\346\245\274\345\261\202\346\200\273\351\253\230\345\272\246", nullptr));
        text_cabinVolume->setText(QCoreApplication::translate("MCESClass", "\347\224\265\346\242\257\350\275\275\345\256\242\351\207\217", nullptr));
        cabinVolume->setText(QString());
        cabinVolume->setPlaceholderText(QCoreApplication::translate("MCESClass", "\357\274\2101-20\357\274\211\344\272\272", nullptr));
        text_simulateDuration->setText(QCoreApplication::translate("MCESClass", "\346\250\241\346\213\237\346\227\266\351\227\264", nullptr));
        simulateDuration->setText(QString());
        simulateDuration->setPlaceholderText(QCoreApplication::translate("MCESClass", "\357\274\2101-100\357\274\211\347\247\222", nullptr));
        maxHeight->setText(QString());
        maxHeight->setPlaceholderText(QCoreApplication::translate("MCESClass", "\357\274\2101-30\357\274\211\345\261\202", nullptr));
        policySelection->setItemText(0, QCoreApplication::translate("MCESClass", "\345\271\263\350\241\241\344\272\244\351\200\232\357\274\210Balance\357\274\211", nullptr));
        policySelection->setItemText(1, QCoreApplication::translate("MCESClass", "\344\270\212\350\241\214\351\253\230\345\263\260\346\234\237\357\274\210Up-Peak\357\274\211", nullptr));
        policySelection->setItemText(2, QCoreApplication::translate("MCESClass", "\344\270\213\350\241\214\351\253\230\345\263\260\346\234\237\357\274\210Down-Peak\357\274\211", nullptr));
        policySelection->setItemText(3, QCoreApplication::translate("MCESClass", "\344\272\214\350\267\257\344\272\244\351\200\232\357\274\210Two-Way\357\274\211", nullptr));

        text_currentPolicy->setText(QCoreApplication::translate("MCESClass", "\345\275\223\345\211\215\347\255\226\347\225\245", nullptr));
        leftTime->setText(QString());
        text_leftTime->setText(QCoreApplication::translate("MCESClass", "\345\211\251\344\275\231\346\227\266\351\227\264", nullptr));
        startSimulateButton->setText(QCoreApplication::translate("MCESClass", "\345\274\200\345\247\213\346\250\241\346\213\237", nullptr));
        pauseSimulateButton->setText(QCoreApplication::translate("MCESClass", "\346\232\202\345\201\234\346\250\241\346\213\237", nullptr));
        currentStatus->setText(QString());
        text_status->setText(QCoreApplication::translate("MCESClass", "\345\275\223\345\211\215\347\212\266\346\200\201", nullptr));
        text_simulateGraph->setText(QCoreApplication::translate("MCESClass", "\345\217\257\350\247\206\345\214\226\346\250\241\346\213\237", nullptr));
        text_ARC->setText(QCoreApplication::translate("MCESClass", "\345\271\263\345\235\207\350\275\275\345\256\242\347\216\207", nullptr));
        ARC->setText(QString());
        AWD->setText(QString());
        ATD->setText(QString());
        text_ATD->setText(QCoreApplication::translate("MCESClass", "\345\271\263\345\235\207\344\271\230\346\242\257\346\227\266\351\227\264", nullptr));
        text_AWD->setText(QCoreApplication::translate("MCESClass", "\345\271\263\345\235\207\345\200\231\346\242\257\346\227\266\351\227\264", nullptr));
        text_model->setText(QCoreApplication::translate("MCESClass", "\346\216\247\345\210\266\347\255\226\347\225\245\351\200\211\346\213\251", nullptr));
        modelSelection->setItemText(0, QCoreApplication::translate("MCESClass", "\347\256\200\345\215\225\350\260\203\345\272\246\347\256\227\346\263\225\346\250\241\346\213\237", nullptr));
        modelSelection->setItemText(1, QCoreApplication::translate("MCESClass", "\347\234\237\345\256\236\347\224\265\346\242\257\350\260\203\345\272\246\346\250\241\346\213\237", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MCESClass: public Ui_MCESClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCES_H
