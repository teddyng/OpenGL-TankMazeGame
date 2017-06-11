TEMPLATE = app

#Executable Name
TARGET = TankAssignment
CONFIG = debug

#Destination
DESTDIR = .
OBJECTS_DIR = ./build/

#Header Files to Include
HEADERS	+= 	../common/Shader.h	    	\	
		../common/Vector.h		        \	
		../common/Matrix.h		        \
		../common/Mesh.h		        \
        ../common/Texture.h             \		
        ../common/SphericalCameraManipulator.h   \

#Sources
SOURCES += 	main.cpp			        \
  		../common/Shader.cpp		    \
		../common/Vector.cpp		    \
		../common/Matrix.cpp		    \
		../common/Mesh.cpp		        \
        ../common/Texture.cpp           \
        ../common/SphericalCameraManipulator.cpp \

#Include Path
INCLUDEPATH += 	./ 				    \
		        ../common/ 			\

		
#Library Libraries
LIBS +=	-L/usr/lib/nvidia-352/                  \
        -lGLEW			    	    	        \
		-lglut			        		        \
        -lGL             	                  	\  
