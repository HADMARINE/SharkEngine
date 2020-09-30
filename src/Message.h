//
// Created by EunwooSong on 2020-09-30.
//

#ifndef VULKAN_ENGINE_MESSAGE_H
#define VULKAN_ENGINE_MESSAGE_H

#include <string>

namespace SharkEngine::Core {
    enum MessageType {
        PROCESS_VALUE_MESSAGE, GET_VALUE_MESSAGE, SEND_VALUE_MESSAGE
    };

    template <typename T>
    class Message {
    public:
        MessageType type;
        std::string method;
        T value;

        Message(MessageType type, std::string method, T value) : type(type), method(method), value(value) {}
        Message(const Message & obj) {
            this->type = obj.type;
            this->method = obj.method;
            this->value = obj.value;
        }
    };
}

#endif//VULKAN_ENGINE_MESSAGE_H
