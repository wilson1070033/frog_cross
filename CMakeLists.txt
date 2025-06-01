cmake_minimum_required(VERSION 3.31)
project(frog)

set(CMAKE_CXX_STANDARD 20)

add_executable(frog main.cpp
        core/GameColors.cpp
        core/GameColors.h
        core/GameUtils.cpp
        core/GameUtils.h
        core/GameTimer.cpp
        core/GameTimer.h
        core/GameConfig.cpp
        core/GameConfig.h
        core/Map.cpp
        core/Map.h
        entities/GameObject.cpp
        entities/GameObject.h
        entities/Obstacle.cpp
        entities/Obstacle.h
        entities/Vehicle.cpp
        entities/Vehicle.h
        entities/Car.cpp
        entities/Car.h
        entities/Moto.cpp
        entities/Moto.h
        entities/Frog.cpp
        entities/Frog.h
        systems/PowerUp.cpp
        systems/PowerUp.h
        systems/GameStats.cpp
        systems/GameStats.h
        systems/EffectSystem.cpp
        systems/EffectSystem.h
        ui/ConfigurationMenu.cpp
        ui/ConfigurationMenu.h
        data/DataStructures.cpp
        data/DataStructures.h
        data/DataManager.cpp
        data/DataManager.h
        data/UserManagementMenu.cpp
        data/UserManagementMenu.h
        core/GameManager.cpp
        core/GameManager.h)
