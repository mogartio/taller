#include "main_window.h"
#include <QApplication>
#include <iostream>

MainWindow::MainWindow(QWidget *parent, Queue<std::unique_ptr<GenericMsg>>& send_queue, Queue<std::unique_ptr<GenericMsg>>& recv_queue) : QMainWindow(parent), send_queue(send_queue), recv_queue(recv_queue) {
    
    resize(1920, 1080);
    //setWindowFlags(Qt::FramelessWindowHint);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Create screens
    logoScreen = new LogoScreen(send_queue, recv_queue);
    connectionScreen = new ConnectionScreen(send_queue, recv_queue);

    // Add screens to stacked widget
    stackedWidget->addWidget(logoScreen);
    stackedWidget->addWidget(connectionScreen);

    // Ensure the screens are resized to fit the QMainWindow
    logoScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connectionScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Show the logo screen initially
    showLogoScreen();

    // Connect the signal from LogoScreen to switch to ConnectionScreen
    connect(logoScreen, &LogoScreen::switchToConnectionScreen, this, &MainWindow::showConnectionScreen);

    // Connect the quir signal from ConnectionScreen to quit the application
    connect(connectionScreen, &ConnectionScreen::quitApplication, this, &MainWindow::handleQuitApplication);

    // Connect the signal from ConnectionScreen to switch to MainMenuScreen
    connect(connectionScreen, &ConnectionScreen::switchToMainMenuScreen, this, &MainWindow::showMainMenuScreen);
}

void MainWindow::showLogoScreen() {
    stackedWidget->setCurrentWidget(logoScreen);
}

void MainWindow::showConnectionScreen() {
    stackedWidget->setCurrentWidget(connectionScreen);
}

void MainWindow::handleQuitApplication() {
    QApplication::quit();
}

void MainWindow::showMainMenuScreen() {
    stackedWidget->setCurrentWidget(mainMenuScreen);    
}


MainWindow::~MainWindow() {
    delete connectionScreen;
}