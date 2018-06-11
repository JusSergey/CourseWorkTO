#ifndef FORMS_H
#define FORMS_H

#include <QtWidgets>
#include "form.h"
#include "SystemOverview.h"
//#include "widget.h"
#include "TestHardware.h"
#include "testhw.h"
#include "serverhw.h"

class Forms {

    Form *form = nullptr;
    SystemOverview *widg = nullptr;
    TestHardware *thw  = nullptr;
    ServerHW *serv = nullptr;
    Forms() {
        form = nullptr;
        widg = nullptr;
        thw = nullptr;
    }

public:
    static Forms *inst() {
        static Forms *instance = new Forms();
        return instance;
    }
    inline Form *getForm() const { return form; }
    inline SystemOverview *getWidget() const { return widg; }
    inline TestHardware *getTestHW() const { return thw; }
    inline ServerHW *getServerHW() const { return serv; }

    void showMenu() {
        if (form == nullptr)
            form = new Form();

        if (widg)
            widg->close();

        if (thw)
            thw->close();

        form->show();
    }
    void showConfigure() {
        if (widg == nullptr)
            widg = new SystemOverview();

        if (form)
            form->close();

        if (thw)
            thw->close();

        widg->show();
    }
    void showTestHW() {
        if (thw == nullptr)
            thw = new TestHardware();

        if (form)
            form->close();

        if (widg)
            widg->close();

        thw->show();
    }
    void showServer() {
        if (serv == nullptr)
            serv = new ServerHW;

        serv->show();
    }
    ~Forms() {
        if (form)
            delete form;
        if (widg)
            delete widg;
        if(thw)
            delete thw;
    }
};

#endif // FORMS_H
