cmake_minimum_required(VERSION 3.16)

project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(../src/ ../src/ThirdParty/include/ ../src/ThirdParty/include/glad ../src/ThirdParty/include/glm ../src/ThirdParty/include/IconFont ../src/ThirdParty/include/ImGui ../src/ThirdParty/include/stb ../src/ThirdParty/include/msgpack ../src/ThirdParty/include/ImGuiFileDialog ../src/Core/Engine/ ../src/Core/Engine/Buffers/ ../src/Core/Engine/BuffersGPU ../src/Core/Engine/Events/ ../src/Core/Engine/FileSystem ../src/Core/Engine/Inputs/ ../src/Core/Engine/Layer/ ../src/Core/Engine/Logs/ ../src/Core/Engine/Memory/ ../src/Core/Engine/Models/ ../src/Core/Engine/Parser/ ../src/Core/Engine/Patterns ../src/Core/Engine/Profiling/ ../src/Core/Engine/Threads/ ../src/Core/Engine/Shaders/ ../src/Core/Engine/Textures/ ../src/Core/Engine/Timer/ ../src/Core/Engine/Window/ ../src/Core/Engine/Layer/ ../src/Core/Engine/Camera/   ../src/Core/Editor/ ../src/Core/Editor/EditorLayers/ ../src/Core/Editor/UI/) 
link_directories(../src/ThirdParty/lib)
file(GLOB SOURCES ../src/ThirdParty/include/glad/*.c ../src/ThirdParty/include/ImGui/*.cpp ../src/ThirdParty/include/ImGuiFileDialog/*.cpp ../src/ThirdParty/include/stb/*.cpp   ../src/*.cpp  ../src/Core/Engine/*.cpp ../src/Core/Engine/Buffers/*.cpp ../src/Core/Engine/BuffersGPU/*.cpp ../src/Core/Engine/Events/*.cpp ../src/Core/Engine/FileSystem/*.cpp ../src/Core/Engine/Inputs/*.cpp ../src/Core/Engine/Layer/*.cpp ../src/Core/Engine/Logs/*.cpp ../src/Core/Engine/Memory/*.cpp ../src/Core/Engine/Models/*.cpp ../src/Core/Engine/Parser/*.cpp ../src/Core/Engine/Patterns/*.cpp ../src/Core/Engine/Profiling/*.cpp ../src/Core/Engine/Threads/*.cpp ../src/Core/Engine/Shaders/*.cpp ../src/Core/Engine/Textures/*.cpp ../src/Core/Engine/Timer/*.cpp ../src/Core/Engine/Window/*.cpp ../src/Core/Engine/Layer/*.cpp ../src/Core/Engine/Camera/*.cpp  ../src/Core/Editor/*.cpp  ../src/Core/Editor/EditorLayers/*.cpp ../src/Core/Editor/UI/*.cpp)

add_executable(main ${SOURCES})
target_link_libraries(main glfw -Wall -Ofast)