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
#include <QStackedWidget>
#include <QButtonGroup>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QtCore>
#include "mainwindow.h"

#include <iostream>
#include "utils/sceneparser.h"
#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"

#include <QApplication>
#include <QScreen>
#include <iostream>
#include <QSettings>

MainWindow::MainWindow()
{
    setWindowTitle("Spirit Sliders: Graphics Final Project");

    QHBoxLayout *hLayout = new QHBoxLayout(); // horizontal layout for canvas and controls panel
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical layout for control panel

    vLayout->setAlignment(Qt::AlignTop);

    hLayout->addLayout(vLayout);
    setLayout(hLayout);

    resize(1475, 810);

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

    QWidget *sandGroup = new QWidget();
    QVBoxLayout *sandLayout = new QVBoxLayout();
    sandLayout->setAlignment(Qt::AlignTop);
    sandGroup->setLayout(sandLayout);

    QScrollArea *controlsScroll = new QScrollArea();
    QTabWidget *tabs = new QTabWidget();
    controlsScroll->setWidget(tabs);
    controlsScroll->setWidgetResizable(true);

    tabs->addTab(depthGroup, "Depth of Field");
    tabs->addTab(motionGroup, "Motion Blur");
    tabs->addTab(lensGroup, "Lens Assembly");
    tabs->addTab(sandGroup, "Sandbox");

    connect(tabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    vLayout->addWidget(controlsScroll);

    // depth section

    addLabel(depthLayout, "Depth of Field:");

    QGroupBox *lay1 = new QGroupBox();
    QHBoxLayout *lay11 = new QHBoxLayout();

    depthSlider = new QSlider(Qt::Horizontal);
    depthSlider->setTickInterval(1);
    depthSlider->setMinimum(0);
    depthSlider->setMaximum(100);
    depthSlider->setValue(0);
    // depthLayout->addWidget(depthSlider);

    depthBox = new QSpinBox();
    depthBox->setMinimum(0);
    depthBox->setMaximum(100);
    depthBox->setSingleStep(1);
    depthBox->setValue(0);

    lay11->addWidget(depthSlider);
    lay11->addWidget(depthBox);
    lay1->setLayout(lay11);
    depthLayout->addWidget(lay1);

    connect(depthSlider, &QSlider::valueChanged, this, &MainWindow::depthChanged);
    connect(depthBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::depthChanged);
    connect(depthSlider, &QSlider::valueChanged, this, [this](int value) {
        updateImage("sphere_line", value);
    });

    updateImage("sphere_line", 0);

    // motion section
    QLabel *motionLabel = new QLabel();
    motionLabel->setText("Motion Blur Sensitivity:");
    motionLayout->addWidget(motionLabel);

    QGroupBox *lay2 = new QGroupBox();
    QHBoxLayout *lay22 = new QHBoxLayout();

    motionSlider = new QSlider(Qt::Horizontal);
    motionSlider->setTickInterval(1);
    motionSlider->setMinimum(0);
    motionSlider->setMaximum(100);
    motionSlider->setValue(0);
    motionLayout->addWidget(motionSlider);

    motionBox = new QSpinBox();
    motionBox->setMinimum(0);
    motionBox->setMaximum(100);
    motionBox->setSingleStep(1);
    motionBox->setValue(0);

    lay22->addWidget(motionSlider);
    lay22->addWidget(motionBox);
    lay2->setLayout(lay22);
    motionLayout->addWidget(lay2);

    connect(motionSlider, &QSlider::valueChanged, this, &MainWindow::motionChanged);
    connect(motionBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::motionChanged);
    connect(motionSlider, &QSlider::valueChanged, this, [this](int value) {
        updateImage("falling_spheres", value);
    });

    // lens section
    addLabel(lensLayout, "Choose Lens:");

    QGroupBox *imageBox = new QGroupBox();
    QHBoxLayout *imageLay = new QHBoxLayout();

    addRadioButton(imageLay, "Normal", true, [this]{
        updateImage("andys_room", 0);
    });
    addRadioButton(imageLay, "Fish Eye", false, [this]{
        updateImage("andys_room", 1);
    });
    addRadioButton(imageLay, "Wide", false, [this]{
        updateImage("andys_room", 2);
    });

    imageBox->setLayout(imageLay);
    lensLayout->addWidget(imageBox);

    // sandbox section

    // Create the layout and buttons

    addLabel(sandLayout, "Choose Feature:");

    QHBoxLayout *imageLay2 = new QHBoxLayout;
    QStackedWidget *panelStack = new QStackedWidget;
    QGroupBox *imageBox2 = new QGroupBox();

    QRadioButton *depthButton = new QRadioButton("Depth of Field");
    depthButton->setChecked(settings.renderMode == DEPTH);
    imageLay2->addWidget(depthButton);

    QRadioButton *motionButton = new QRadioButton("Motion");
    motionButton->setChecked(settings.renderMode == MOTION);
    imageLay2->addWidget(motionButton);

    QRadioButton *lensButton = new QRadioButton("Lens");
    lensButton->setChecked(settings.renderMode == LENS);
    imageLay2->addWidget(lensButton);

    imageBox2->setLayout(imageLay2);
    sandLayout->addWidget(imageBox2);

    sandLayout->addWidget(panelStack);

    QWidget *depthSettingsWidget = new QWidget();
    QVBoxLayout *depthSettingsLayout = new QVBoxLayout(depthSettingsWidget);

    addDoubleSpinBox(depthSettingsLayout, "Aperture:", 0.0, 10.0, 0.1, settings.aperture, 2, [this](double value) {
        settings.aperture = value;
    });
    addDoubleSpinBox(depthSettingsLayout, "Focal Length:", 0.0, 200.0, 1.0, settings.focalLength, 1, [this](double value) {
        settings.focalLength = value;
    });

    // Motion Panel
    QWidget *motionSettingsWidget = new QWidget();
    QVBoxLayout *motionSettingsLayout = new QVBoxLayout(motionSettingsWidget);

    addDoubleSpinBox(motionSettingsLayout, "Velocity Factor:", 0.0, 1.0, 0.1, settings.velocity, 2, [this](double value) {
        settings.velocity = value;
    });

    // Lens Panel
    QWidget *lensSettingsWidget = new QWidget();
    QHBoxLayout *lensSettingsLayout = new QHBoxLayout(lensSettingsWidget);

    QGroupBox *imageBox3 = new QGroupBox();
    QHBoxLayout *imageLay3 = new QHBoxLayout();

    addRadioButton(imageLay3, "Fish Eye", false, [this]{
        currLens = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/lenses/fisheye.dat";
    });
    addRadioButton(imageLay3, "Wide", false, [this]{
        currLens = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/lenses/wide.dat";
    });

    imageBox3->setLayout(imageLay3);
    lensSettingsLayout->addWidget(imageBox3);

    panelStack->addWidget(depthSettingsWidget);
    panelStack->addWidget(motionSettingsWidget);
    panelStack->addWidget(lensSettingsWidget);

    connect(depthButton, &QRadioButton::clicked, this, [panelStack]() {
        settings.renderMode = DEPTH;
        panelStack->setCurrentIndex(DEPTH);
    });
    connect(motionButton, &QRadioButton::clicked, this, [panelStack]() {
        settings.renderMode = MOTION;
        panelStack->setCurrentIndex(MOTION);
    });
    connect(lensButton, &QRadioButton::clicked, this, [panelStack]() {
        settings.renderMode = LENS;
        panelStack->setCurrentIndex(LENS);
    });


    addPushButton(sandLayout, "Load Scenefile", &MainWindow::onUploadButtonClick);
}

void MainWindow::render(){
    RenderData metaData;
    bool success = SceneParser::parseScene(currScene.toStdString(), metaData);

    std::cout << currLens.toStdString() << std::endl;
    bool success2 = SceneParser::parseLens(currLens.toStdString(), metaData);

    if (settings.renderMode == DEPTH){
        metaData.cameraData.aperture = settings.aperture;
        metaData.cameraData.focalLength = settings.focalLength;
    }

    if (settings.renderMode == MOTION){
        metaData.globalData.globalVel = settings.velocity;
    }

    // if (!success) {
    //     std::cerr << "Error loading scene: \"" << currScene.toStdString() << "\"" << std::endl;
    // }

    // if (!success2) {
    //     std::cerr << "Error loading scene: \"" << currScene.toStdString() << "\"" << std::endl;
    // }

    int width = 1024;
    int height = 768;

    QImage myImage = QImage(width, height, QImage::Format_RGBX8888);
    myImage.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(myImage.bits());

    RayTracer::Config rtConfig{};

    rtConfig.enableDepthOfField = settings.renderMode == DEPTH ? true : false;
    rtConfig.enableMotionBlur = settings.renderMode == MOTION ? true : false;
    rtConfig.enableLens = settings.renderMode == LENS ? true : false;

    RayTracer raytracer{ rtConfig };

    RayTraceScene rtScene{ width, height, metaData };

    raytracer.render(data, rtScene);

    image->setPixmap(QPixmap::fromImage(myImage));

    // // Saving the image
    // success = image.save(oImagePath);
    // if (!success) {
    //     success = image.save(oImagePath, "PNG");
    // }
    // if (success) {
    //     std::cout << "Saved rendered image to \"" << oImagePath.toStdString() << "\"" << std::endl;
    // } else {
    //     std::cerr << "Error: failed to save image to \"" << oImagePath.toStdString() << "\"" << std::endl;
    // }

    // a.exit();
}


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

    // Convert the image to RGBX8888 format if needed
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888);

    // Display the image directly from the QImage object
    image->setPixmap(QPixmap::fromImage(myImage));
    image->setFixedSize(myImage.width(), myImage.height());
    update();
}

// ------ FUNCTIONS FOR UPDATING SETTINGS ------

void MainWindow::onUploadButtonClick() {
    // Get new image path selected by user
    QString folderPath = QDir::homePath() + "/graphics_final_project/scenefiles";  // Specify the folder path
    QString file = QFileDialog::getOpenFileName(this, tr("Open Image"), folderPath, tr("Image Files (*.json)"));
    if (file.isEmpty()) { return; }

    currScene = file;
    render();
}

void MainWindow::depthChanged(int newValue) {
    depthSlider->setValue(newValue);
    depthBox->setValue(newValue);
}

void MainWindow::motionChanged(int newValue) {
    motionSlider->setValue(newValue);
    motionBox->setValue(newValue);
}

void MainWindow::onTabChanged(int index) {
    switch (index) {
    case 0:
        updateImage("sphere_line", 0);
        depthChanged(0);
        break;
    case 1:
        updateImage("falling_spheres", 0);
        motionChanged(0);
        break;
    case 2:
        updateImage("andys_room", 0);
        break;
    default:
        int width = 1024;
        int height = 768;
        QPixmap whitePixmap(width, height);
        whitePixmap.fill(Qt::white);
        image->setPixmap(whitePixmap);
        break;
    }
}


