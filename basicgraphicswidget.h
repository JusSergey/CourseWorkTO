#ifndef BASICGRAPHICSWIDGET_H
#define BASICGRAPHICSWIDGET_H

#include <QWidget>

class BasicGraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BasicGraphicsWidget(QWidget *parent = nullptr);

protected:
    //virtual void paintEvent(QPaintEvent *unused_event) override;

signals:

public slots:
};

#endif // BASICGRAPHICSWIDGET_H
