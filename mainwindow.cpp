#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
    connect(ui->actionOpenModel,&QAction::triggered,this,&MainWindow::openModel);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::exit);
    connect(ui->actionAbout,&QAction::triggered,this,&MainWindow::about);
    //std::cout<<this->width()<<" "<<this->height()<<std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    //std::cout<<"MainWindow: "<<this->width()<<" "<<this->height()<<std::endl;
}

void MainWindow::openModel(){
    std::cout<<"openModel trigger!!!"<<std::endl;
    QString filename = QFileDialog::getOpenFileName(this,tr("选择模型"),tr("C:/Users/73965/Downloads"),tr("*.fbx *.obj"));
    std::cout<<filename.toStdString()<<std::endl;
}

void MainWindow::exit(){
    std::cout<<"exit trigger!!!"<<std::endl;
}

void MainWindow::about(){
    std::cout<<"about trigger!!!"<<std::endl;
}
