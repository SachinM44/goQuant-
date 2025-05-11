#include "AlmgrenChriss.h"
#include <cmath>

AlmgrenChriss::AlmgrenChriss(const Parameters& params)
    : m_params(params) {
    initializeDerivedParameters();
}

void AlmgrenChriss::initializeDerivedParameters() {
    m_etaTilda = m_params.eta * m_params.sigma * m_params.sigma;
    m_kappaTildaSquared = m_params.lambda / (m_params.eta * m_params.tau);
    m_kappa = std::sqrt(m_params.gamma / m_params.eta);
}

double AlmgrenChriss::calculateMarketImpact(double orderSize, double timeHorizon, const json& orderbookData) {
    // Extract market depth from orderbook
    double marketDepth = 0.0;
    if (!orderbookData["bids"].empty() && !orderbookData["asks"].empty()) {
        double bestBid = std::stod(orderbookData["bids"][0][0].get<std::string>());
        double bestAsk = std::stod(orderbookData["asks"][0][0].get<std::string>());
        marketDepth = (bestAsk - bestBid) / ((bestAsk + bestBid) / 2.0);
    }
    
    // Calculate temporary impact
    double tempImpact = m_params.lambda * orderSize / timeHorizon;
    
    // Calculate permanent impact
    double permImpact = m_params.eta * orderSize;
    
    // Adjust impact based on market depth
    double totalImpact = (tempImpact + permImpact) * (1.0 + marketDepth);
    
    return totalImpact;
}

Eigen::VectorXd AlmgrenChriss::calculateTrajectory(double orderSize, double timeHorizon, int steps) {
    Eigen::VectorXd trajectory = Eigen::VectorXd::Zero(steps);
    double dt = timeHorizon / steps;
    
    for (int i = 0; i < steps; ++i) {
        double t = i * dt;
        trajectory(i) = orderSize * (std::cosh(m_kappa * (timeHorizon - t)) / std::cosh(m_kappa * timeHorizon));
    }
    
    return trajectory;
}