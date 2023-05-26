#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bird.h"
#include "scene.h"
#include <QTimer>
#include <QFile>
#include <memory>
#include <iomanip>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

//    QFile file(":/style.css");
//    file.open(QFile::ReadOnly);
//    this->setStyleSheet(file.readAll());
    ui->setupUi(this);
    rr = new QWidget(ui->centralwidget);
    size_t num_of_birds = 1000;
    std::vector <bird> birds(num_of_birds);
    make_birds(num_of_birds);
    mainScene = new Scene(birds);
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(5);
}

void MainWindow::make_birds(size_t num) {
    Birds.resize(num);
    for (size_t i = 0; i < num; ++i){
        Birds[i] = new QWidget(ui->centralwidget);
        Birds[i]->setGeometry(100, 200, 30, 30);
        Birds[i]->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 42, 255);"));
        Birds[i]->show();
        Birds[i]->setProperty("class", "bird");
    }
}
void MainWindow::importInfo() {
    auto b = mainScene->getBirds();
    for (size_t i = 0; i < b.size(); ++i) {
        if (!b[i].is_alive)
            Birds[i]->hide();
        else
            Birds[i]->show();
        Birds[i]->move(b[i].x, b[i].y);
    }
    ui->tube1->move(mainScene->get_Tube1x(), mainScene->get_Tube1y());
    ui->tube2->move(mainScene->get_Tube2x(), mainScene->get_Tube2y());
    ms = (mainScene->get_max_score() + 2600) / 4000;
    gn = mainScene->get_gen_num();
    cs = (mainScene->get_cur_score() + 2600) / 4000;
    ui->label->setText(QString::number(ms) + "\n" + QString::number(gn) + "\n" + QString::number(cs));
}
void MainWindow::slotTimerAlarm() {
    importInfo();
    mainScene->do_smth();
}

MainWindow::~MainWindow()
{
    delete ui;
}

