// gets data through a reciver and identifies profitable trades.

#include "receiver.h"

class Trader {
    private:
    
    public:
        Trader();
};

int main(int argc, char** argv) {
    try {
        Receiver receiver;
        while(1) {
            std::string message = receiver.readClient();
            if(message.size() == 0) continue;
            std::cout << message << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}