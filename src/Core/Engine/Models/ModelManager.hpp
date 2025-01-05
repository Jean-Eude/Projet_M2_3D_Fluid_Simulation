#pragma once

#include <Models.hpp>

class Models;

class ModelManager {
public:
    static ModelManager& getInstance() {
        static ModelManager instance;
        return instance;
    }

    // Enregistrer un modèle
    template <typename T, typename... Args>
    static bool registerModel(const std::string& modelName) {
        auto& manager = getInstance();

        using FunctionType = std::function<std::unique_ptr<Models>(std::tuple<Args...>)>;

        manager.ModelLists[modelName] = FunctionType([](std::tuple<Args...> params) -> std::unique_ptr<Models> {
            return std::apply([](Args... unpackedArgs) {
                return std::make_unique<T>(std::forward<Args>(unpackedArgs)...);
            }, params);
        });

        return true;
    }

    template <typename... Args>
    std::unique_ptr<Models> createModel(const std::string& modelName, Args&&... args) {
        auto it = ModelLists.find(modelName);
        if (it != ModelLists.end()) {
            using FunctionType = std::function<std::unique_ptr<Models>(std::tuple<Args...>)>;
            auto& creator = std::any_cast<FunctionType&>(it->second);
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            return creator(argsTuple);
        }
        throw std::invalid_argument("Modèle non enregistré : " + modelName);
    }

private:
    std::unordered_map<std::string, std::any> ModelLists;

    ModelManager() = default;
    ~ModelManager() = default;
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;
};