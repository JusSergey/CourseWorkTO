#ifndef TESTHARDWARE_H
#define TESTHARDWARE_H

#include <QWebEngineView>
#include <QtWidgets>
#include "types.h"
#include "Utils/HtmlUtils.h"
#include "Threads/Runnable.h"

class TestHardware : public QWidget
{
    Q_OBJECT
public:
    explicit TestHardware(QWidget *parent = nullptr);

private:
    class RunAsyncTests : public Runnable {
        TestHardware *_ptr;
    public:
        RunAsyncTests(TestHardware *testObjPtr) : _ptr(testObjPtr)
        {}
        virtual void run() override {
            if (_ptr)
                _ptr->htmlPage = _ptr->startTest();
        }
    };

    friend class RunAsyncTests;

private:
    void init();
    QLayout *initCheckboxes();
    QLayout *initButtons();
    QLayout *initWebview();
    void connectWatcherTimer();
    void disconnectWatcherTimer();
    void initConnections();
    void saveToFile(QString namefile);

private:
    void startTestCPU(HtmlUtils::IGF inputDataForHtmlPage);
    void startTestRAM(HtmlUtils::IGF inputDataForHtmlPage);
    void startTestHardDrive(HtmlUtils::IGF inputDataForHtmlPage);
    void startTestQueen(HtmlUtils::IGF inputDataForHtmlPage);
    void startTestZIP(HtmlUtils::IGF inputDataForHtmlPage);
    bool isAllTestsComplete() const;

public:
    ///@return string of html page
    string startTest();

private:
    QWebEngineView *webView         = nullptr;
    QCheckBox *checkCPU             = nullptr;
    QCheckBox *checkRAM             = nullptr;
    QCheckBox *checkHardDrive       = nullptr;
    QCheckBox *checkQueenTest       = nullptr;
    QCheckBox *checkZIPTest         = nullptr;
    QPushButton *buttonStartTest    = nullptr;
    QPushButton *buttonGoToMenu     = nullptr;
    QPushButton *buttonSendToServer = nullptr;
    QTimer *timerForWatcher         = nullptr;

    std::atomic_bool cpuTestComplete;
    std::atomic_bool ramTestComplete;
    std::atomic_bool hdiskTestComplete;
    std::atomic_bool queenTestComplete;
    std::atomic_bool zipTestComplete;

    string htmlPage;

private slots:
    void slotStartTest();
    void slotGoToMenu();
    void slotSendToServer();
    void slotWatcherOnCompleteTests();

};

#endif // TESTHARDWARE_H
