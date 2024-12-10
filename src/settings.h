/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

enum ImageType {
    IMAGE1,
    IMAGE2,
    IMAGE3
};

struct Settings {
    QString currImagePath;
    bool depth = false;
    bool motion = false;
    bool lens = false;
    bool sandbox = false;

    int currImage = -1;

    // bool image1;
    // bool image2;
    // bool image3;


    // int shapeParameter1 = 1;
    // int shapeParameter2 = 1;
    // float nearPlane = 1;
    // float farPlane = 1;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H

