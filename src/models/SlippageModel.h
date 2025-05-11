#pragma once

#include <Eigen/Dense>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SlippageModel {
public:
    SlippageModel();
    
    // Calculate expected slippage using linear regression
    double calculateSlippage(double orderSize, const json& orderbookData);
    
    // Update regression model with new data
    void updateModel(const std::vector<double>& features, double actualSlippage);
    
private:
    Eigen::VectorXd m_coefficients;
    bool m_modelInitialized;
    
    // Extract features from orderbook data
    Eigen::VectorXd extractFeatures(double orderSize, const json& orderbookData);
};