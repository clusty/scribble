QT += widgets

HEADERS       = mainwindow.h \
                scribblearea.h \
    imageproc.h \
    cv2qt.h \
    graph.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                scribblearea.cpp \
    graph.cpp
QMAKE_CXXFLAGS += -I/usr/local/Cellar/opencv/2.4.12/include/opencv -I/usr/local/Cellar/opencv/2.4.12/include 
LIBS += /usr/local/Cellar/opencv/2.4.12/lib/libopencv_calib3d.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_contrib.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_core.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_features2d.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_flann.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_gpu.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_highgui.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_imgproc.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_legacy.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_ml.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_nonfree.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_objdetect.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_ocl.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_photo.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_stitching.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_superres.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_ts.a /usr/local/Cellar/opencv/2.4.12/lib/libopencv_video.dylib /usr/local/Cellar/opencv/2.4.12/lib/libopencv_videostab.dylib
# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/scribble
INSTALLS += target

