#include "InputPanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>

InputPanel::InputPanel(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Input Parameters", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(titleLabel);
    
    // Form layout for parameters
    QFormLayout* formLayout = new QFormLayout();
    
    // Exchange selection
    m_exchangeCombo = new QComboBox(this);
    m_exchangeCombo->addItem("OKX");
    formLayout->addRow("Exchange:", m_exchangeCombo);
    
    // Asset selection
    m_assetCombo = new QComboBox(this);
    m_assetCombo->addItem("BTC-USDT-SWAP");
    m_assetCombo->addItem("ETH-USDT-SWAP");
    formLayout->addRow("Asset:", m_assetCombo);
    
    // Order type
    m_orderTypeCombo = new QComboBox(this);
    m_orderTypeCombo->addItem("Market");
    formLayout->addRow("Order Type:", m_orderTypeCombo);
    
    // Quantity
    m_quantitySpin = new QDoubleSpinBox(this);
    m_quantitySpin->setRange(1.0, 1000.0);
    m_quantitySpin->setValue(100.0);
    m_quantitySpin->setSuffix(" USD");
    formLayout->addRow("Quantity:", m_quantitySpin);
    
    // Volatility
    m_volatilitySpin = new QDoubleSpinBox(this);
    m_volatilitySpin->setRange(0.01, 1.0);
    m_volatilitySpin->setValue(0.2);
    m_volatilitySpin->setSingleStep(0.01);
    formLayout->addRow("Volatility:", m_volatilitySpin);
    
    // Fee tier
    m_feeTierCombo = new QComboBox(this);
    m_feeTierCombo->addItem("Tier 1 (VIP)");
    m_feeTierCombo->addItem("Tier 2");
    m_feeTierCombo->addItem("Tier 3");
    m_feeTierCombo->addItem("Tier 4");
    m_feeTierCombo->addItem("Tier 5 (Regular)");
    m_feeTierCombo->setCurrentIndex(2); // Default to Tier 3
    formLayout->addRow("Fee Tier:", m_feeTierCombo);
    
    mainLayout->addLayout(formLayout);
    
    // Simulate button
    QPushButton* simulateButton = new QPushButton("Simulate Trade", this);
    mainLayout->addWidget(simulateButton);
    
    // Connect signals
    connect(simulateButton, &QPushButton::clicked, this, &InputPanel::onSimulateClicked);
    
    // Add stretch to push everything to the top
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void InputPanel::onSimulateClicked() {
    QVariantMap parameters;
    parameters["exchange"] = m_exchangeCombo->currentText();
    parameters["asset"] = m_assetCombo->currentText();
    parameters["orderType"] = m_orderTypeCombo->currentText();
    parameters["quantity"] = m_quantitySpin->value();
    parameters["volatility"] = m_volatilitySpin->value();
    parameters["feeTier"] = m_feeTierCombo->currentIndex() + 1;
    
    emit simulationRequested(parameters);
}