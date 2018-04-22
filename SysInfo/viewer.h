#ifndef VIEWER_H
#define VIEWER_H


class Viewer
{
public:
    Viewer();
    int start() const;
    void show() const;
    void showCPU() const;
    void showRAM() const;
    void showDMI() const;
};

#endif // VIEWER_H
