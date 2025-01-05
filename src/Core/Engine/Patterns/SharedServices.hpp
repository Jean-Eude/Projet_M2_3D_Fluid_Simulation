#pragma once

#include <HeadersBase.hpp>

class SharedServices {
public:
    static SharedServices& GetInstance() {
        static SharedServices instance;
        return instance;
    }

    void RegisterService(const std::string& key, std::shared_ptr<void> service) {
        namedServices[key] = service;
    }

    template<typename Ret, typename... Args>
    void RegisterFunction(const std::string& key, std::function<Ret(Args...)> func) {
        namedServices[key] = std::make_shared<std::function<Ret(Args...)>>(func);
    }

    template<typename T>
    std::shared_ptr<T> GetService(const std::string& key) {
        auto it = namedServices.find(key);
        if (it != namedServices.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        throw std::runtime_error("Service not registered with this key: " + key);
    }

    template<typename T>
    void SetService(const std::string& key, const T& value) {
        auto it = namedServices.find(key);
        if (it != namedServices.end()) {
            auto existingService = std::static_pointer_cast<T>(it->second);
            *existingService = value;
        } else {
            throw std::runtime_error("Service not found to update: " + key);
        }
    }

    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> GetFunction(const std::string& key) {
        auto it = namedServices.find(key);
        if (it != namedServices.end()) {
            return *std::static_pointer_cast<std::function<Ret(Args...)>>(it->second);
        }
        throw std::runtime_error("Function not registered with this key: " + key);
    }

private:
    std::unordered_map<std::string, std::shared_ptr<void>> namedServices;

    SharedServices() = default;
    ~SharedServices() = default;

    SharedServices(const SharedServices&) = delete;
    SharedServices& operator=(const SharedServices&) = delete;
    SharedServices(SharedServices&&) = delete;
    SharedServices& operator=(SharedServices&&) = delete;
};