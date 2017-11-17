#include <HardwareSerial.h>

class Client {
  public:
    Stream & serial;

    /**
     * Constructor
     *
     * @param serial Serial port to use to communicate with the client
     */
    Client(Stream & serial);

    /**
     * [sendStatus description]
     */
    void sendStatus(void);

    /**
     * Send a position message (slide, pan, tilt, focus, zoom) via Serial.
     */
    void sendPosition(void);

    /**
     * Sends a handshake greeting message to the client
     */
    void sendHandshakeGreetingMessage(void);

  private:
    /**
     * Send a message via Serial to the client.
     *
     * @param command       [description]
     * @param payload       [description]
     * @param payloadLength [description]
     */
    void sendMessage(const unsigned char command,
                     const unsigned char * payload,
                     const unsigned int payloadLength);
};
