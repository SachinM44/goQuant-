#pragma once

#include <string>

class FeeModel {
public:
    enum class FeeType {
        MAKER,
        TAKER
    };
    
    enum class FeeTier {
        TIER_1,  // Lowest fees
        TIER_2,
        TIER_3,
        TIER_4,
        TIER_5   // Highest fees
    };
    
    FeeModel();
    
    // Set the fee tier based on trading volume or VIP level
    void setFeeTier(FeeTier tier);
    
    // Calculate fees based on order size and fee type (maker/taker)
    double calculateFees(double orderSize, FeeType feeType = FeeType::TAKER);
    
private:
    FeeTier m_currentTier;
    double m_makerFeeRate;
    double m_takerFeeRate;
    
    // Update fee rates based on the current tier
    void updateFeeRates();
};