#include "FeeModel.h"

FeeModel::FeeModel() : m_currentTier(FeeTier::TIER_1) {
    updateFeeRates();
}

void FeeModel::setFeeTier(FeeTier tier) {
    m_currentTier = tier;
    updateFeeRates();
}

void FeeModel::updateFeeRates() {
    switch (m_currentTier) {
        case FeeTier::TIER_1:
            m_makerFeeRate = 0.0002; // 0.02%
            m_takerFeeRate = 0.0005; // 0.05%
            break;
        case FeeTier::TIER_2:
            m_makerFeeRate = 0.00016;
            m_takerFeeRate = 0.00045;
            break;
        case FeeTier::TIER_3:
            m_makerFeeRate = 0.00014;
            m_takerFeeRate = 0.0004;
            break;
        case FeeTier::TIER_4:
            m_makerFeeRate = 0.00012;
            m_takerFeeRate = 0.00035;
            break;
        case FeeTier::TIER_5:
            m_makerFeeRate = 0.0001;
            m_takerFeeRate = 0.0003;
            break;
    }
}

double FeeModel::calculateFees(double orderSize, FeeType feeType) {
    return orderSize * (feeType == FeeType::MAKER ? m_makerFeeRate : m_takerFeeRate);
}