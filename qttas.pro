QT += widgets
requires(qtConfig(filedialog))

HEADERS             = screenshot.h \
                      gamelogic.h
SOURCES             = main.cpp \
                      screenshot.cpp \
                      gamelogic.cpp

# install
target.path = ./screenshot
INSTALLS += target
@LIBS += -lXtst -lX11@