#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QScrollArea>
#include <QCheckBox>
#include <QPainter>
#include <QMessageBox>

MainWindow::MainWindow()
{
    setWindowTitle("Spirit Sliders: Graphics Final Project");

    // loads in settings from last run or uses default values
    // settings.loadSettingsOrDefaults();

    QHBoxLayout *hLayout = new QHBoxLayout(); // horizontal layout for canvas and controls panel
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical layout for control panel

    vLayout->setAlignment(Qt::AlignTop);

    hLayout->addLayout(vLayout);
    setLayout(hLayout);

    // setupCanvas2D();
    resize(2000, 2000);

    // makes the canvas into a scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    hLayout->addWidget(scrollArea, 1);

    image = new QLabel();
    image->setAlignment(Qt::AlignCenter); // Center the image in the label

    // Set the QLabel as the widget of the scroll area
    scrollArea->setWidget(image);

    // groupings by project
    QWidget *depthGroup = new QWidget();
    QVBoxLayout *depthLayout = new QVBoxLayout();
    depthLayout->setAlignment(Qt::AlignTop);
    depthGroup->setLayout(depthLayout);

    QWidget *motionGroup = new QWidget();
    QVBoxLayout *motionLayout = new QVBoxLayout();
    motionLayout->setAlignment(Qt::AlignTop);
    motionGroup->setLayout(motionLayout);

    QWidget *lensGroup = new QWidget();
    QVBoxLayout *lensLayout = new QVBoxLayout();
    lensLayout->setAlignment(Qt::AlignTop);
    lensGroup->setLayout(lensLayout);

    QWidget *freeGroup = new QWidget();
    QVBoxLayout *freeLayout = new QVBoxLayout();
    freeLayout->setAlignment(Qt::AlignTop);
    freeGroup->setLayout(freeLayout);

    QScrollArea *controlsScroll = new QScrollArea();
    QTabWidget *tabs = new QTabWidget();
    controlsScroll->setWidget(tabs);
    controlsScroll->setWidgetResizable(true);

    tabs->addTab(depthGroup, "Depth of Field");
    tabs->addTab(motionGroup, "Motion Blur");
    tabs->addTab(lensGroup, "Lens Assembly");
    tabs->addTab(freeGroup, "Sandbox");

    connect(tabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    vLayout->addWidget(controlsScroll);

    // depth section

    addLabel(depthLayout, "Choose Image:");

    QGroupBox *imageBox = new QGroupBox();
    QHBoxLayout *imageLay = new QHBoxLayout();

    addRadioButton(imageLay, "Image 1", settings.currImage == IMAGE1, [this]{
        depthSlider->setValue(0);
        updateImage("primsalad", 0);
    });
    addRadioButton(imageLay, "Image 2", settings.currImage == IMAGE2, [this]{
        depthSlider->setValue(0);
        updateImage("image2", 0);
    });
    addRadioButton(imageLay, "Image 3", settings.currImage == IMAGE3, [this]{
        depthSlider->setValue(0);
        updateImage("image3", 0);
    });

    imageBox->setLayout(imageLay);
    depthLayout->addWidget(imageBox);

    addLabel(depthLayout, "Depth of Field:");

    QGroupBox *lay1 = new QGroupBox();
    QHBoxLayout *lay11 = new QHBoxLayout();

    depthSlider = new QSlider(Qt::Horizontal);
    depthSlider->setTickInterval(1);
    depthSlider->setMinimum(0);
    depthSlider->setMaximum(20);
    depthSlider->setValue(0);
    depthLayout->addWidget(depthSlider);

    depthBox = new QSpinBox();
    depthBox->setMinimum(0);
    depthBox->setMaximum(20);
    depthBox->setSingleStep(1);
    depthBox->setValue(0);

    lay11->addWidget(depthSlider);
    lay11->addWidget(depthBox);
    lay1->setLayout(lay11);
    depthLayout->addWidget(lay1);

    connectWidgets(depthSlider, depthBox);

    // Connect the slider to the updateImage slot
    connect(depthSlider, &QSlider::valueChanged, this, [this](int value) {
        updateImage(settings.currImagePath, value);
    });

    // motion section
    // addPushButton(motionLayout, "Load Scenefile", &MainWindow::onUploadButtonClick);

    QLabel *motionLabel = new QLabel();
    motionLabel->setText("Motion Blur Sensitivity:");
    motionLayout->addWidget(motionLabel);

    QGroupBox *lay2 = new QGroupBox();
    QHBoxLayout *lay22 = new QHBoxLayout();

    motionSlider = new QSlider(Qt::Horizontal);
    motionSlider->setTickInterval(1);
    motionSlider->setMinimum(0);
    motionSlider->setMaximum(20);
    motionSlider->setValue(0);
    motionLayout->addWidget(motionSlider);

    motionBox = new QSpinBox();
    motionBox->setMinimum(0);
    motionBox->setMaximum(20);
    motionBox->setSingleStep(1);
    motionBox->setValue(0);

    lay22->addWidget(motionSlider);
    lay22->addWidget(motionBox);
    lay2->setLayout(lay22);
    motionLayout->addWidget(lay2);

    connectWidgets(motionSlider, motionBox);

    // lens section

    // addPushButton(lensLayout, "Load Scenefile", &MainWindow::onUploadButtonClick);

    // sandbox section

    // addPushButton(freeLayout, "Load Scenefile", &MainWindow::onUploadButtonClick);


    // // brush selection
    // addHeading(brushLayout, "Brush");
    // addRadioButton(brushLayout, "Constant", settings.brushType == BRUSH_CONSTANT, [this]{ setBrushType(BRUSH_CONSTANT); });
    // addRadioButton(brushLayout, "Linear", settings.brushType == BRUSH_LINEAR, [this]{ setBrushType(BRUSH_LINEAR); });
    // addRadioButton(brushLayout, "Quadratic", settings.brushType == BRUSH_QUADRATIC, [this]{ setBrushType(BRUSH_QUADRATIC); });
    // addRadioButton(brushLayout, "Smudge", settings.brushType == BRUSH_SMUDGE, [this]{ setBrushType(BRUSH_SMUDGE); });

    // // brush parameters
    // addSpinBox(brushLayout, "red", 0, 255, 1, settings.brushColor.r, [this](int value){ setUIntVal(settings.brushColor.r, value); });
    // addSpinBox(brushLayout, "green", 0, 255, 1, settings.brushColor.g, [this](int value){ setUIntVal(settings.brushColor.g, value); });
    // addSpinBox(brushLayout, "blue", 0, 255, 1, settings.brushColor.b, [this](int value){ setUIntVal(settings.brushColor.b, value); });
    // addSpinBox(brushLayout, "alpha", 0, 255, 1, settings.brushColor.a, [this](int value){ setUIntVal(settings.brushColor.a, value); });
    // addSpinBox(brushLayout, "radius", 0, 100, 1, settings.brushRadius, [this](int value){ setIntVal(settings.brushRadius, value); });

    // // extra credit brushes
    // addHeading(brushLayout, "Extra Credit Brushes");
    // addRadioButton(brushLayout, "Spray", settings.brushType == BRUSH_SPRAY, [this]{ setBrushType(BRUSH_SPRAY); });
    // addSpinBox(brushLayout, "density", 0, 100, 1, settings.brushDensity, [this](int value){ setIntVal(settings.brushDensity, value); });
    // addRadioButton(brushLayout, "Speed", settings.brushType == BRUSH_SPEED, [this]{ setBrushType(BRUSH_SPEED); });
    // addRadioButton(brushLayout, "Fill", settings.brushType == BRUSH_FILL, [this]{ setBrushType(BRUSH_FILL); });
    // addRadioButton(brushLayout, "Custom", settings.brushType == BRUSH_CUSTOM, [this]{ setBrushType(BRUSH_CUSTOM); });
    // addCheckBox(brushLayout, "Fix alpha blending", settings.fixAlphaBlending, [this](bool value){ setBoolVal(settings.fixAlphaBlending, value); });

    // // clearing canvas
    // addPushButton(brushLayout, "Clear canvas", &MainWindow::onClearButtonClick);

    // // save canvas as image
    // addPushButton(brushLayout, "Save Image", &MainWindow::onSaveButtonClick);

    // filters
    // addHeading(filterLayout, "Filter");
    // addRadioButton(filterLayout, "Edge detect", settings.filterType == FILTER_EDGE_DETECT,  [this]{ setFilterType(FILTER_EDGE_DETECT); });
    // addDoubleSpinBox(filterLayout, "sensitivity", 0.01, 1, 0.01, settings.edgeDetectSensitivity, 2, [this](float value){ setFloatVal(settings.edgeDetectSensitivity, value); });

    // addRadioButton(filterLayout, "Blur", settings.filterType == FILTER_BLUR, [this]{ setFilterType(FILTER_BLUR); });
    // addSpinBox(filterLayout, "radius", 0, 100, 1, settings.blurRadius, [this](int value){ setIntVal(settings.blurRadius, value); });

    // addRadioButton(filterLayout, "Scale", settings.filterType == FILTER_SCALE, [this]{ setFilterType(FILTER_SCALE); });
    // addDoubleSpinBox(filterLayout, "x", 0.1, 10, 0.1, settings.scaleX, 2, [this](float value){ setFloatVal(settings.scaleX, value); });
    // addDoubleSpinBox(filterLayout, "y", 0.1, 10, 0.1, settings.scaleY, 2, [this](float value){ setFloatVal(settings.scaleY, value); });

    // // extra credit filters
    // addHeading(filterLayout, "Extra Credit Filters");
    // addRadioButton(filterLayout, "Median", settings.filterType == FILTER_MEDIAN,  [this]{ setFilterType(FILTER_MEDIAN); });
    // addSpinBox(filterLayout, "radius", 1, 100, 1, settings.medianRadius, [this](int value){ setIntVal(settings.medianRadius, value); });

    // addRadioButton(filterLayout, "Chromatic aberration", settings.filterType == FILTER_CHROMATIC,  [this]{ setFilterType(FILTER_CHROMATIC); });
    // addSpinBox(filterLayout, "red shift", -100, 100, 0, settings.rShift, [this](int value){ setIntVal(settings.rShift, value); });
    // addSpinBox(filterLayout, "green shift", -100, 100, 0, settings.gShift, [this](int value){ setIntVal(settings.gShift, value); });
    // addSpinBox(filterLayout, "blue shift", -100, 100, 0, settings.bShift, [this](int value){ setIntVal(settings.bShift, value); });

    // addRadioButton(filterLayout, "Tone mapping", settings.filterType == FILTER_MAPPING,  [this]{ setFilterType(FILTER_MAPPING); });
    // addCheckBox(filterLayout, "Non linear function", settings.nonLinearMap, [this](bool value){ setBoolVal(settings.nonLinearMap, value); });
    // addDoubleSpinBox(filterLayout, "gamma", 0.1, 2, 0.1, settings.gamma, 2, [this](float value){ setFloatVal(settings.gamma, value); });

    // addRadioButton(filterLayout, "Rotation", settings.filterType == FILTER_ROTATION,  [this]{ setFilterType(FILTER_ROTATION); });
    // addDoubleSpinBox(filterLayout, "angle", -360, 360, 0.1, settings.rotationAngle, 2, [this](float value){ setFloatVal(settings.rotationAngle, value); });

    // addRadioButton(filterLayout, "Bilteral smooth", settings.filterType == FILTER_BILATERAL,  [this]{ setFilterType(FILTER_BILATERAL); });
    // addSpinBox(filterLayout, "radius", 1, 100, 1, settings.bilateralRadius, [this](int value){ setIntVal(settings.bilateralRadius, value); });

    // filter push buttons
    // addPushButton(filterLayout, "Load Image", &MainWindow::onUploadButtonClick);
    // addPushButton(filterLayout, "Apply Filter", &MainWindow::onFilterButtonClick);
    // addPushButton(filterLayout, "Revert Image", &MainWindow::onRevertButtonClick);
    // addPushButton(filterLayout, "Save Image", &MainWindow::onSaveButtonClick);
}

/**
 * @brief Sets up Canvas2D
 */
// void MainWindow::setupCanvas2D() {
//     m_canvas = new Canvas2D();
//     m_canvas->init();

//     if (!settings.imagePath.isEmpty()) {
//         m_canvas->loadImageFromFile(settings.imagePath);
//     }
// }


// ------ FUNCTIONS FOR ADDING UI COMPONENTS ------

void MainWindow::addHeading(QBoxLayout *layout, QString text) {
    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    QLabel *label = new QLabel(text);
    label->setFont(font);
    layout->addWidget(label);
}

void MainWindow::addLabel(QBoxLayout *layout, QString text) {
    layout->addWidget(new QLabel(text));
}

void MainWindow::addRadioButton(QBoxLayout *layout, QString text, bool value, auto function) {
    QRadioButton *button = new QRadioButton(text);
    button->setChecked(value);
    layout->addWidget(button);
    connect(button, &QRadioButton::clicked, this, function);
}

void MainWindow::addSpinBox(QBoxLayout *layout, QString text, int min, int max, int step, int val, auto function) {
    QSpinBox *box = new QSpinBox();
    box->setMinimum(min);
    box->setMaximum(max);
    box->setSingleStep(step);
    box->setValue(val);
    QHBoxLayout *subLayout = new QHBoxLayout();
    addLabel(subLayout, text);
    subLayout->addWidget(box);
    layout->addLayout(subLayout);
    connect(box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, function);
}

void MainWindow::addDoubleSpinBox(QBoxLayout *layout, QString text, double min, double max, double step, double val, int decimal, auto function) {
    QDoubleSpinBox *box = new QDoubleSpinBox();
    box->setMinimum(min);
    box->setMaximum(max);
    box->setSingleStep(step);
    box->setValue(val);
    box->setDecimals(decimal);
    QHBoxLayout *subLayout = new QHBoxLayout();
    addLabel(subLayout, text);
    subLayout->addWidget(box);
    layout->addLayout(subLayout);
    connect(box, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, function);
}

void MainWindow::addPushButton(QBoxLayout *layout, QString text, auto function) {
    QPushButton *button = new QPushButton(text);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, function);
}

void MainWindow::addCheckBox(QBoxLayout *layout, QString text, bool val, auto function) {
    QCheckBox *box = new QCheckBox(text);
    box->setChecked(val);
    layout->addWidget(box);
    connect(box, &QCheckBox::clicked, this, function);
}

void MainWindow::updateImage(const QString &folder, int value) {
    // Construct the file path based on the folder and image index
    QString filePath = QString("/Users/efratavigdor/Desktop/CS1230/graphics-final-project/outputs/%1/output_%2.png")
                           .arg(folder)
                           .arg(value + 1);

    // Create a QImage object and attempt to load the image
    QImage myImage;
    if (!myImage.load(filePath)) {
        std::cout << "Failed to load image: " << filePath.toStdString() << std::endl;
        return;
    }

    settings.currImagePath = folder;

    // settings.currImage = value;

    // Convert the image to RGBX8888 format if needed
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);

    // Display the image directly from the QImage object
    image->setPixmap(QPixmap::fromImage(myImage));
    image->setFixedSize(myImage.width(), myImage.height());
    update();
}



// void MainWindow::addSlider(QSlider *slider, QSpinBox *box, QBoxLayout *layout, QString text, float tick, int min, int max, int value){
//     QLabel *label = new QLabel();
//     label->setText(text);
//     layout->addWidget(label);

//     QGroupBox *sliderLayout = new QGroupBox();
//     QHBoxLayout *layoutLocal = new QHBoxLayout();

//     slider = new QSlider(Qt::Horizontal);
//     slider->setTickInterval(tick);
//     slider->setMinimum(min);
//     slider->setMaximum(max);
//     slider->setValue(value);
//     layout->addWidget(slider);

//     box = new QSpinBox();
//     box->setMinimum(min);
//     box->setMaximum(max);
//     box->setSingleStep(tick);
//     box->setValue(value);

//     layoutLocal->addWidget(slider);
//     layoutLocal->addWidget(box);
//     sliderLayout->setLayout(layoutLocal);
//     layout->addWidget(sliderLayout);
// }

// ------ FUNCTIONS FOR UPDATING SETTINGS ------

void MainWindow::depthChanged(int newValue) {
    depthSlider->setValue(newValue);
    depthBox->setValue(newValue);
    // settings.shapeParameter1 = p1Slider->value();
    // realtime->settingsChanged();
}

void MainWindow::connectWidgets(QSlider *slider, QSpinBox *box) {
    // Synchronize slider value to spinbox
    connect(slider, &QSlider::valueChanged, this, &MainWindow::depthChanged);

    // Synchronize spinbox value to slider
    connect(box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::depthChanged);
}

void MainWindow::onTabChanged(int index) {
    // switch (index) {
    // case 0:
    //     settings.depth = true;
    //     settings.motion = false;
    //     settings.lens = false;
    //     settings.sandbox = false;
    //     depthSlider->setValue(10);
    //     depthBox->setValue(10);
    //     break;
    // case 1:
    //     settings.depth = false;
    //     settings.motion = true;
    //     settings.lens = false;
    //     settings.sandbox = false;
    //     motionSlider->setValue(5);
    //     motionBox->setValue(5);
    //     break;
    // case 2:
    //     settings.depth = false;
    //     settings.motion = false;
    //     settings.lens = true;
    //     settings.sandbox = false;
    //     break;
    // case 3:
    //     settings.depth = false;
    //     settings.motion = false;
    //     settings.lens = false;
    //     settings.sandbox = true;
    //     break;

    // default:
    //     break;
    // }
}

