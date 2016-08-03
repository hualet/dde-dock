#ifndef SOUNDAPPLET_H
#define SOUNDAPPLET_H

#include "componments/volumeslider.h"
#include "dbus/dbusaudio.h"
#include "dbus/dbussink.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>

#include <dimagebutton.h>

class SoundApplet : public QScrollArea
{
    Q_OBJECT

public:
    explicit SoundApplet(QWidget *parent = 0);

signals:
    void defaultSinkChanged(DBusSink *sink) const;

private slots:
    void defaultSinkChanged();
    void onVolumeChanged();
    void volumeSliderValueChanged();
    void toggleMute();

private:
    QWidget *m_centeralWidget;
    QWidget *m_appControlWidget;
    Dtk::Widget::DImageButton *m_volumeBtn;
    VolumeSlider *m_volumeSlider;
    QVBoxLayout *m_centeralLayout;

    DBusAudio *m_audioInter;
    DBusSink *m_defSinkInter;
};

#endif // SOUNDAPPLET_H
