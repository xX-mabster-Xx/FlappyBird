#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QRandomGenerator>

#include <iostream>
#include <random>
#include <scene.h>
//#include <random.h>
#include <ctime>
#include <memory>
//#include <vector>
//#include <algorithm>
//
//#include <cmath>
//#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QRandomGenerator;
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotTimerAlarm();

private:
    void importInfo();
    Ui::MainWindow *ui;
    Scene *mainScene;
    QTimer *timer;
    std::vector <QWidget*> Birds;
    QWidget * rr;
    void make_birds(size_t num);
    int ms = 0;
    int gn = 0;
    int cs = 0;
};
#endif // MAINWINDOW_H
