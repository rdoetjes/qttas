QT += widgets
requires(qtConfig(filedialog))

HEADERS             = screenshot.h
SOURCES             = main.cpp \
                      screenshot.cpp

# install
target.path = ./screenshot
INSTALLS += target