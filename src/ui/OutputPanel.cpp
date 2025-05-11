#include "OutputPanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QDateTime>

OutputPanel::OutputPanel(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Simulation Results", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(titleLabel);
    
    // Grid layout for results
    QGridLayout* gridLayout = new QGridLayout();
    
    // Create result labels
    int row = 0;
    
    // Expected Slippage
    gridLayout->addWidget(new QLabel("Expected Slippage:", this), row, 0);
    m_slippageLabel = new QLabel("0.00 USD", this);
    gridLayout->addWidget(m_slippageLabel, row++, 1);
    
    // Expected Fees
    gridLayout->addWidget(new QLabel("Expected Fees:", this), row, 0);
    m_feesLabel = new QLabel("0.00 USD", this);
    gridLayout->addWidget(m_feesLabel, row++, 1);
    
    // Expected Market Impact
    gridLayout->addWidget(new QLabel("Market Impact:", this), row, 0);
    m_marketImpactLabel = new QLabel("0.00 USD", this);
    gridLayout->addWidget(m_marketImpactLabel, row++, 1);
    
    // Net Cost
    gridLayout->addWidget(new QLabel("Net Cost:", this), row, 0);
    m_netCostLabel = new QLabel("0.00 USD", this);
    m_netCostLabel->setStyleSheet("font-weight: bold;");
    gridLayout->addWidget(m_netCostLabel, row++, 1);
    
    // Maker/Taker proportion
    gridLayout->addWidget(new QLabel("Maker/Taker Proportion:", this), row, 0);
    m_makerTakerLabel = new QLabel("0% / 100%", this);
    gridLayout->addWidget(m_makerTakerLabel, row++, 1);
    
    // Internal Latency
    gridLayout->addWidget(new QLabel("Internal Latency:", this), row, 0);
    m_latencyLabel = new QLabel("0 ms", this);
    gridLayout->addWidget(m_latencyLabel, row++, 1);
    
    // Last Update Time
    gridLayout->addWidget(new QLabel("Last Update:", this), row, 0);
    m_lastUpdateLabel = new QLabel("-", this);
    gridLayout->addWidget(m_lastUpdateLabel, row++, 1);
    
    mainLayout->addLayout(gridLayout);
    
    // Add stretch to push everything to the top
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void OutputPanel::updateResults(const QVariantMap& results) {
    // Update the result labels
    m_slippageLabel->setText(QString("%1 USD").arg(results["slippage"].toDouble(), 0, 'f', 4));
    m_feesLabel->setText(QString("%1 USD").arg(results["fees"].toDouble(), 0, 'f', 4));
    m_marketImpactLabel->setText(QString("%1 USD").arg(results["marketImpact"].toDouble(), 0, 'f', 4));
    m_netCostLabel->setText(QString("%1 USD").arg(results["netCost"].toDouble(), 0, 'f', 4));
    
    double makerPct = results["makerProportion"].toDouble() * 100.0;
    m_makerTakerLabel->setText(QString("%1% / %2%").arg(makerPct, 0, 'f', 1).arg(100.0 - makerPct, 0, 'f', 1));
    
    m_latencyLabel->setText(QString("%1 ms").arg(results["latency"].toDouble(), 0, 'f', 2));
    
    // Update the last update time
    m_lastUpdateLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}