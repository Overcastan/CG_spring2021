# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Tutorial 1
add_executable(tutorial01_first_window 
	tutorial01_first_window/tutorial01.cpp
)
target_link_libraries(tutorial01_first_window
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial01_first_window PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/")
create_target_launcher(tutorial01_first_window WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/")

# Tutorial 2
add_executable(tutorial02_red_triangle 
	tutorial02_red_triangle/tutorial02.cpp
	common/shader.cpp
	common/shader.hpp
	
	tutorial02_red_triangle/SimpleFragmentShader.fragmentshader
	tutorial02_red_triangle/SimpleVertexShader.vertexshader
)
target_link_libraries(tutorial02_red_triangle
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial02_red_triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_default_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/") # tut 1 is not the default or people would complain that tut 2 doesn't work

# Tutorial 3
add_executable(tutorial03_matrices 
	tutorial03_matrices/tutorial03.cpp
	common/shader.cpp
	common/shader.hpp

	tutorial03_matrices/SimpleTransform.vertexshader
	tutorial03_matrices/SingleColor.fragmentshader
)
#set_target_properties(tutorial03_matrices PROPERTIES RUNTIME_OUTPUT_DIRECTORY /test1)
target_link_libraries(tutorial03_matrices
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial03_matrices PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/")
create_target_launcher(tutorial03_matrices WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/") # Visual

# Tutorial 4
add_executable(tutorial04_colored_cube
	tutorial04_colored_cube/tutorial04.cpp
	common/shader.cpp
	common/shader.hpp
	
	tutorial04_colored_cube/TransformVertexShader.vertexshader
	tutorial04_colored_cube/ColorFragmentShader.fragmentshader
)
target_link_libraries(tutorial04_colored_cube
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial04_colored_cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/")
create_target_launcher(tutorial04_colored_cube WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/")

# HW1 Ex 1 Two triangles
add_executable(two_triangles
		two_triangles/two_triangles.cpp
		common/shader.cpp
		common/shader.hpp
		)
target_link_libraries(two_triangles
		${ALL_LIBS}
		-static-libstdc++ -static-libgcc
		-static
		-lwinpthread
		)
# Xcode and Visual working directories
set_target_properties(two_triangles PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/two_triangles/")
create_target_launcher(two_triangles WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/two_triangles/")

# HW1 Ex 2 3D Figure
add_executable(corona_figure
		corona_figure/corona_figure.cpp
		common/shader.cpp
		common/shader.hpp
		)
target_link_libraries(corona_figure
		${ALL_LIBS}
		-static-libstdc++ -static-libgcc
		-static
		-lwinpthread
		)
# Xcode and Visual working directories
set_target_properties(corona_figure PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/corona_figure/")
create_target_launcher(corona_figure WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/corona_figure/")



SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET tutorial01_first_window POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial01_first_window${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/"
)
add_custom_command(
   TARGET tutorial02_red_triangle POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial02_red_triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/"
)
add_custom_command(
   TARGET tutorial03_matrices POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial03_matrices${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/"
)
add_custom_command(
   TARGET tutorial04_colored_cube POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial04_colored_cube${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/"
)
add_custom_command(
		TARGET two_triangles POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/two_triangles${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/two_triangles/"
)
add_custom_command(
		TARGET corona_figure POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/corona_figure${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/corona_figure/"
)


elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

