CXXFLAGS += -I/opt/homebrew/opt/tesseract/include -I/opt/homebrew/opt/leptonica/include -I/opt/homebrew/opt/nlohmann-json/include -std=c++17
LDFLAGS += -L/opt/homebrew/opt/tesseract/lib -L/opt/homebrew/opt/leptonica/lib -ltesseract -lleptonica -lcurl -framework CoreGraphics -framework ImageIO -framework CoreFoundation -framework Cocoa -arch arm64

CXX = g++
OCXX = clang++ # Objective-C++ compiler

SRC_FILES = $(wildcard src/*.cpp) $(wildcard src/*.mm)
OBJ_FILES = $(patsubst src/%.cpp,obj/%.o,$(filter %.cpp,$(SRC_FILES))) $(patsubst src/%.mm,obj/%.o,$(filter %.mm,$(SRC_FILES)))
EXECUTABLE = bin/league_app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p bin
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.mm
	@mkdir -p obj
	$(OCXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj $(EXECUTABLE)

.PHONY: all clean

