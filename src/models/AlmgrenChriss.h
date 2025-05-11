#pragma once

#include <Eigen/Dense>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AlmgrenChriss {
public:
    struct Parameters {
        double lambda;  // Temporary market impact parameter
        double sigma;   // Asset volatility
        double epsilon; // Fixed trading cost
        double eta;     // Permanent market impact parameter
        double gamma;   // Risk aversion parameter
        double tau;     // Time interval between trades
    };
    
    AlmgrenChriss(const Parameters& params);
    
    // Calculate market impact using the Almgren-Chriss model
    double calculateMarketImpact(double orderSize, double timeHorizon, const json& orderbookData);
    
    // Calculate optimal trading trajectory
    Eigen::VectorXd calculateTrajectory(double orderSize, double timeHorizon, int steps);
    
private:
    Parameters m_params;
    double m_etaTilda;
    double m_kappaTildaSquared;
    double m_kappa;
    
    void initializeDerivedParameters();
};