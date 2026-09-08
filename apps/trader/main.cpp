#include "trader/traderFactory.h"

int main(int argc, char** argv) {
    try {
        Receiver receiver;
        TraderPhase1 trader;
        while(true) {
            std::string message = receiver.readClient();
            if(message.size() == 0) continue;
            trader.digestAndProcess(message);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}