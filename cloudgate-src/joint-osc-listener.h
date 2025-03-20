#include <iostream>
#include <cstring>
#include "oscpack/osc/OscReceivedElements.h"
#include "oscpack/osc/OscPacketListener.h"
#include "oscpack/ip/UdpSocket.h"

class JointListener: public osc::OscPacketListener  {
    public:
    // TODO: Reference to circular buffer.

    virtual void ProcessMessage(const osc::ReceivedMessage& m,
        const IpEndpointName& remoteEndpoint ) {
            (void) remoteEndpoint;
        try {
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            float x1, y1;
            float x2, y2;
            float x3, y3;
            float x4, y4;
            float x5, y5;
            float x6, y6;
            float x7, y7;

            args >> x1 >> y1
                >> x2 >> y2
                >> x3 >> y3
                >> x4 >> y4
                >> x5 >> y5
                >> x6 >> y6
                >> x7 >> y7
                >> osc::EndMessage;

            std::cout << m.AddressPattern()
                << " [" << x1 << ", " << y1 << "],"
                << "[" << x2 << ", " << y2 << "],"
                << "[" << x3 << ", " << y3 << "],"
                << "[" << x4 << ", " << y4 << "],"
                << "[" << x5 << ", " << y5 << "],"
                << "[" << x6 << ", " << y6 << "],"
                << "[" << x7 << ", " << y7 << "]"
                << std::endl;

            // TODO: Write the motor positions to the queue.
            // This will require a refactoring of puppet_demo.cpp
            // to factor out the custom circular buffer you've got
            // so that we can hold a reference to it here and be able to
            // call push_back() on it.

        } catch(osc::Exception& e) {
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};
