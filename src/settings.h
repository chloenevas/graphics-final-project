/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

enum RenderType {
    DEPTH,
    MOTION,
    LENS
};

struct Settings {
    int renderMode = 0;
    float aperture = 0;
    float focalLength = 0;
    float velocity = 0;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H

