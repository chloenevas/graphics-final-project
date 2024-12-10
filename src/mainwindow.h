#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

#include "raytracer/raytracer.h"

class MainWindow : public QLabel
{
    Q_OBJECT

public:
    MainWindow();

private:
    void setupCanvas2D();
    QLabel *image;

    QSlider *depthSlider;
    QSlider *motionSlider;
    QSpinBox *depthBox;
    QSpinBox *motionBox;

    void addHeading(QBoxLayout *layout, QString text);
    void addLabel(QBoxLayout *layout, QString text);
    void addRadioButton(QBoxLayout *layout, QString text, bool value, auto function);
    void addSpinBox(QBoxLayout *layout, QString text, int min, int max, int step, int val, auto function);
    void addDoubleSpinBox(QBoxLayout *layout, QString text, double min, double max, double step, double val, int decimal, auto function);
    void addPushButton(QBoxLayout *layout, QString text, auto function);
    void addCheckBox(QBoxLayout *layout, QString text, bool value, auto function);
    void addSlider(QSlider *slider, QSpinBox *box, QBoxLayout *layout, QString text, float tick, int min, int max, int value);
    void onTabChanged(int index);
    void updateImage(int value);

private slots:
    void setImage(int type);
    void setFilterType(int type);

    void setUIntVal(std::uint8_t &setValue, int newValue);
    void setIntVal(int &setValue, int newValue);
    void setFloatVal(float &setValue, float newValue);
    void setBoolVal(bool &setValue, bool newValue);

    void depthChanged(int newValue);
    void connectWidgets(QSlider *slider, QSpinBox *box);

    // void onClearButtonClick();
    // void onFilterButtonClick();
    // void onRevertButtonClick();
    // void onUploadButtonClick();
    // void onSaveButtonClick();
};
#endif // MAINWINDOW_H
