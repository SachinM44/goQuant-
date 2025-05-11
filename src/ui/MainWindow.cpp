#include "MainWindow.h"
#include "InputPanel.h"
#include "OutputPanel.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("GoQuant Trading System");
    resize(1024, 768);
    
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_layout = new QHBoxLayout(m_centralWidget);
    
    m_inputPanel = new InputPanel(this);
    m_outputPanel = new OutputPanel(this);
    
    m_layout->addWidget(m_inputPanel);
    m_layout->addWidget(m_outputPanel);
}

MainWindow::~MainWindow() {}

void MainWindow::updateSimulation(const QVariantMap& parameters) {
    // Forward parameters to the trade simulator
    emit simulationUpdated(parameters);
}

void MainWindow::updateOutputs(const QVariantMap& outputs) {
    // Update the output panel with new values
    m_outputPanel->updateValues(outputs);
}