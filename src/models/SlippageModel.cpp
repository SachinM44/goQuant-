#include "SlippageModel.h"
#include <cmath>

SlippageModel::SlippageModel() : m_modelInitialized(false) {
    m_coefficients = Eigen::VectorXd::Zero(3); // [intercept, size_coef, depth_coef]
    m_coefficients << 0.0001, 0.0002, 0.0003; // Initial coefficients
}

double SlippageModel::calculateSlippage(double orderSize, const json& orderbookData) {
    Eigen::VectorXd features = extractFeatures(orderSize, orderbookData);
    return features.dot(m_coefficients);
}

void SlippageModel::updateModel(const std::vector<double>& features, double actualSlippage) {
    // Simple online learning update
    if (features.size() == 3) {
        Eigen::VectorXd x(3);
        for (size_t i = 0; i < 3; ++i) x(i) = features[i];
        
        double prediction = x.dot(m_coefficients);
        double error = actualSlippage - prediction;
        
        // Gradient descent update
        double learningRate = 0.01;
        m_coefficients += learningRate * error * x;
        m_modelInitialized = true;
    }
}

Eigen::VectorXd SlippageModel::extractFeatures(double orderSize, const json& orderbookData) {
    Eigen::VectorXd features(3);
    
    // Feature 1: Normalized order size
    features(0) = 1.0; // Intercept
    features(1) = orderSize / 100.0; // Normalize by typical order size
    
    // Feature 2: Market depth
    double depth = 0.0;
    if (!orderbookData["bids"].empty() && !orderbookData["asks"].empty()) {
        double bestBid = std::stod(orderbookData["bids"][0][0].get<std::string>());
        double bestAsk = std::stod(orderbookData["asks"][0][0].get<std::string>());
        depth = (bestAsk - bestBid) / ((bestAsk + bestBid) / 2.0);
    }
    features(2) = depth;
    
    return features;
}