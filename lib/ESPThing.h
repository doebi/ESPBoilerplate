/*
 * ESP Boilerplate OS - ESPThing.h
 * (c) by Christoph (doebi) Döberl
 *
 */
#ifndef ESPTHING_H_
#define ESPTHING_H_

#include "PubSubClient.h"
#include <vector>

/*!
 * An Input listens on a specific topic for incoming messages and executes the supplied 
 * callback function once a new message arrives.
 */
class Input {
    public:
        Input(){};
        Input(String t, void (*c)(const MQTT::Publish& pub));
        String topic;
        void (*callback)(const MQTT::Publish& pub);
};

/*!
 * Outputs work the other way round than Inputs. You implement a method, which is executed 
 * in loop(). When this method defines a String at msg, it is sent as message to the outputs 
 * topic. Optionally you can add a third parameter to the Output class. An interval in 
 * milliseconds in which the loop method should be executed. This defaults to 0.
 */
class Output {
    public:
        Output(){};
        Output(String t, void (*l)(String * msg), int i = 0);
        String topic;
        void (*loop)(String * msg);
        int interval;
        int last_run = 0;
};

class ESPThing {
    public:
        ESPThing();
        ~ESPThing();
        void loop();

        /*! equals to calling thingPublish([...]) */
        void addOutput(const Output &o);

        /*! equals to calling thingSubscribe([...]) */
        void addInput(const Input &i);

        /*!
         * Convenience method for subscribing to an internal (device related) topic 
         * for which the given callback is called.
         * 
         * \param t the topic specified as relative path (the configured topic prefix is prepended)
         * \param c the callback function
         */
        void thingSubscribe(String t, void (*c)(const MQTT::Publish& pub));

        /*!
         * Convenience method for publishing to an internal (device related) topic.
         * 
         * \param t the topic specified as relative path (the configured topic prefix is prepended)
         * \param l the callback function which may set a new message to be published
         * \param i the interval in milliseconds after which the callback function is called
         */
        void thingPublish(String t, void (*l)(String * msg), int i = 0);

        /*!
         * Convenience method for subscribing to a certain (device unrelated) topic 
         * for which the given callback is called.
         * 
         * \param t the topic specified as absolute path
         * \param c the callback function
         */
        void subscribe(String t, void (*c)(const MQTT::Publish& pub));

        /*!
         * Convenience method for publishing to a certain (device unrelated) topic.
         * 
         * \param t the topic specified as absolute path
         * \param l the callback function which may set a new message to be published
         * \param i the interval in milliseconds after which the callback function is called
         */
        void publish(String t, void (*l)(String * msg), int i = 0);


        bool wifiConnected();

        bool mqttClientConnected();

        bool mqttThingClientConnected();
        
    private:
        void setup();
        void log(String message);
        
        void server_loop();
        void handleRoot();
        void handleNotFound();


        std::vector<Input> _mqtt_thingInputs;
        std::vector<Input> _mqtt_inputs;
        void mqtt_loop();
        void mqtt_callback(const MQTT::Publish& pub, const std::vector<Input> &inputs);
        void mqtt_publishLoop(std::vector<Output> &outputs, PubSubClient &mqttClient);
        void mqtt_subscribe(String t, void (*c)(const MQTT::Publish& pub), bool internal);
        void mqtt_publish(String t, void (*l)(String * msg), int i, bool internal);
        bool singleMqttClient();
};

#endif /* ESPTHING_H_ */
