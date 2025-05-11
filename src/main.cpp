#include <QApplication>
#include <boost/asio/io_context.hpp>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>

#include "ui/MainWindow.h"
#include "websocket/WebSocketClient.h"
#include "models/AlmgrenChriss.h"
#include "models/SlippageModel.h"
#include "models/FeeModel.h"
#include "utils/Logger.h"
#include "utils/Performance.h"

using json = nlohmann::json;

class TradeSimulator {
public:
    // In the TradeSimulator constructor, update the WebSocket client initialization
    TradeSimulator::TradeSimulator(boost::asio::io_context& ioc)
        : m_ioc(ioc),
          m_wsClient(ioc, "ws.gomarket-cpp.goquant.io", "443", "/ws/l2-orderbook/okx/BTC-USDT-SWAP"),
          m_running(false),
          m_logger("trade_simulator.log") {
        
        // Initialize models with default parameters
        AlmgrenChriss::Parameters acParams;
        acParams.lambda = 0.1;   // Example values
        acParams.sigma = 0.2;
        acParams.epsilon = 0.01;
        acParams.eta = 0.05;
        acParams.gamma = 1.0;
        acParams.tau = 0.1;
        
        m_acModel = std::make_unique<AlmgrenChriss>(acParams);
        m_slippageModel = std::make_unique<SlippageModel>();
        m_feeModel = std::make_unique<FeeModel>();
    }
    
    void start() {
        if (m_running) return;
        
        m_running = true;
        m_logger.log("Starting trade simulator");
        
        // Connect to WebSocket server
        m_wsClient.connect([this](const std::string& message) {
            this->onMessage(message);
        });
    }
    
    void stop() {
        if (!m_running) return;
        
        m_running = false;
        m_logger.log("Stopping trade simulator");
        m_wsClient.disconnect();
    }
    
    void setParameters(double orderSize, double timeHorizon, int steps) {
        m_orderSize = orderSize;
        m_timeHorizon = timeHorizon;
        m_steps = steps;
    }
    
private:
    void onMessage(const std::string& message) {
        try {
            // Parse incoming market data
            json data = json::parse(message);
            m_logger.log("Received market data: " + message);
            
            // Calculate optimal trading trajectory
            Eigen::VectorXd trajectory = m_acModel->calculateTrajectory(m_orderSize, m_timeHorizon, m_steps);
            
            // Calculate market impact
            double marketImpact = m_acModel->calculateMarketImpact(m_orderSize, m_timeHorizon, data);
            
            // Calculate slippage
            double slippage = m_slippageModel->calculateSlippage(m_orderSize, data);
            
            // Calculate fees
            double fees = m_feeModel->calculateFees(m_orderSize);
            
            // Calculate total cost
            double totalCost = marketImpact + slippage + fees;
            
            // Log results
            m_logger.log("Market impact: " + std::to_string(marketImpact));
            m_logger.log("Slippage: " + std::to_string(slippage));
            m_logger.log("Fees: " + std::to_string(fees));
            m_logger.log("Total cost: " + std::to_string(totalCost));
            
            // Emit results to UI (would need to implement signal/slot mechanism)
        } catch (const std::exception& e) {
            m_logger.log("Error processing message: " + std::string(e.what()));
        }
    }
    
    boost::asio::io_context& m_ioc;
    WebSocketClient m_wsClient;
    std::unique_ptr<AlmgrenChriss> m_acModel;
    std::unique_ptr<SlippageModel> m_slippageModel;
    std::unique_ptr<FeeModel> m_feeModel;
    std::atomic<bool> m_running;
    Logger m_logger;
    
    double m_orderSize = 1000.0;
    double m_timeHorizon = 1.0; // 1 day
    int m_steps = 10;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Create io_context for WebSocket
    boost::asio::io_context ioc;
    
    // Create trade simulator
    auto simulator = std::make_shared<TradeSimulator>(ioc);
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    // Start the simulator
    simulator->start();
    
    // Run the io_context in a separate thread
    std::thread ioc_thread([&ioc]() {
        ioc.run();
    });
    
    // Run the Qt event loop
    int result = app.exec();
    
    // Cleanup
    simulator->stop();
    ioc.stop();
    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }
    
    return result;
}