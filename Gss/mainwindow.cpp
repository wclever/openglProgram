#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, &QAction::triggered, ui->openGLWidget, &MyWidget::Open);
    connect(ui->actionOpenTexture, &QAction::triggered, ui->openGLWidget, &MyWidget::OpenTexture);
    connect(ui->actionOpenTexturePos, &QAction::triggered, ui->openGLWidget, &MyWidget::OpenTexturePos);
    connect(ui->lingting_switch, &QCheckBox::stateChanged, ui->openGLWidget, &MyWidget::lighting_switch);
    connect(ui->normal_switch, &QCheckBox::stateChanged, ui->openGLWidget, &MyWidget::normal_switch);
    connect(ui->axis_switch, &QCheckBox::stateChanged, ui->openGLWidget, &MyWidget::axis_switch);
    connect(ui->texture_switch, &QCheckBox::stateChanged, ui->openGLWidget, &MyWidget::texture_switch);
    connect(ui->piont_mode, &QRadioButton::toggled, ui->openGLWidget, &MyWidget::point_mode);
    connect(ui->line_mode, &QRadioButton::toggled, ui->openGLWidget, &MyWidget::line_mode);
    connect(ui->fill_mode, &QRadioButton::toggled, ui->openGLWidget, &MyWidget::fill_mode);
}

MainWindow::~MainWindow()
{
    delete ui;
}
