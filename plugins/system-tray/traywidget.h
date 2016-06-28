#ifndef TRAYWIDGET_H
#define TRAYWIDGET_H

#include <QWidget>

class TrayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrayWidget(quint32 winId, QWidget *parent = 0);

private:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void wrapWindow();
    void updateIcon();
    void hideIcon();
    void sendClick(uint8_t mouseButton, int x, int y);
    QImage getImageNonComposite();

private:
    WId m_windowId;
    WId m_containerWid;
    QImage m_image;
};

#endif // TRAYWIDGET_H
