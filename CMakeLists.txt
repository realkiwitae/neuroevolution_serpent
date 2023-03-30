CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(project)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package( OpenCV 3.2.0 REQUIRED )
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_library(SOIL2 soil2-debug)
FIND_PACKAGE(GLUT REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

SET(SOURCE_FILES 

	main.cpp

	./Engine/Utils/CommonValues.cpp

	./Engine/Window.cpp
	./Engine/Lights/Light.cpp ./Engine/Lights/DirectionalLight.cpp ./Engine/Lights/SpotLight.cpp ./Engine/Lights/PointLight.cpp
	./Engine/Lights/ShadowMap.cpp ./Engine/Lights/OmniShadowMap.cpp

	./Engine/Modeling/Material.cpp ./Engine/Modeling/Mesh.cpp ./Engine/Modeling/Model.cpp
	./Engine/Modeling/Shader.cpp ./Engine/Modeling/Texture.cpp
	./Engine/Modeling/Skybox.cpp

	./Game/Arena.cpp
	./Game/Food.cpp
	./Game/Camera.cpp
	./Game/Snake.cpp

	./kiwinn/DNA.cpp
	./kiwinn/GeneticPool.cpp
	./kiwinn/NeuralNetwork.cpp
	./kiwinn/Stage.cpp
	./kiwinn/utils.cpp
	
	)
	
ADD_EXECUTABLE(project ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(project glfw)
TARGET_LINK_LIBRARIES(project OpenGL::GL GLEW::GLEW glm assimp)
target_link_libraries(project rt ${OpenCV_LIBS} pthread)
TARGET_LINK_LIBRARIES(project ${GLUT_LIBRARIES} ${OPENGL_glu_LIBRARY})