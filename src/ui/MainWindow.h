#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <memory>

class InputPanel;
class OutputPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
public slots:
    void updateSimulation(const QVariantMap& parameters);
    void updateOutputs(const QVariantMap& outputs);
    
private:
    QWidget* m_centralWidget;
    QHBoxLayout* m_layout;
    InputPanel* m_inputPanel;
    OutputPanel* m_outputPanel;
};


// Create input panel for:
// - Exchange selection
// - Spot Asset
// - Order Type
// - Quantity
// - Volatility
// - Fee Tier

// Create output panel for:
// - Expected Slippage
// - Expected Fees
// - Expected Market Impact
// - Net Cost
// - Maker/Taker proportion
// - Internal Latency