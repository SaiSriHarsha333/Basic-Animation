run:
	g++ -std=c++11 ./include/stb_image.cpp ./source/Model.cpp ./source/View.cpp ./source/Control.cpp ./source/SceneGraph.cpp ./source/main.cpp -w -lGL -lGLU -lglfw -lGLEW -o out
	mv out bin/
	./bin/out
