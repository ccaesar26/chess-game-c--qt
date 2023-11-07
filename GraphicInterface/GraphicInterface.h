#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GraphicInterface.h"

class GraphicInterface : public QMainWindow
{
    Q_OBJECT

public:
    GraphicInterface(QWidget *parent = nullptr);
    ~GraphicInterface();

private:
    Ui::GraphicInterfaceClass ui;
};
