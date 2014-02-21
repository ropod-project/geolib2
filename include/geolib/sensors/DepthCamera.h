#ifndef GEOLIB_DEPTHCAMERA_H_
#define GEOLIB_DEPTHCAMERA_H_

#include <opencv2/core/core.hpp>

#include "geolib/Ray.h"

namespace geo {

class Edge {

public:

    Edge(int x1, int y1, float depth1, int x2, int y2, float depth2) {
        if (y1 < y2) {
            Color1 = depth1;
            X1 = x1;
            Y1 = y1;
            Color2 = depth2;
            X2 = x2;
            Y2 = y2;
        } else {
            Color1 = depth2;
            X1 = x2;
            Y1 = y2;
            Color2 = depth1;
            X2 = x1;
            Y2 = y1;
        }
    }

    int X1, X2;
    int Y1, Y2;
    float Color1, Color2;

};

class Span {

public:

    Span(float color1, int x1, float color2, int x2) {
        if(x1 < x2) {
            Color1 = color1;
            X1 = x1;
            Color2 = color2;
            X2 = x2;
        } else {
            Color1 = color2;
            X1 = x2;
            Color2 = color1;
            X2 = x1;
        }
    }

    int X1, X2;
    float Color1, Color2;

};

struct RasterizeResult {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};

typedef std::vector<std::vector<void*> > PointerMap;

static PointerMap EMPTY_POINTER_MAP;

class DepthCamera {

public:

    DepthCamera();

    virtual ~DepthCamera();

    void render(const Shape& shape, const Pose3D& pose, cv::Mat& image);   

    RasterizeResult rasterize(const Shape& shape, const Pose3D& pose, cv::Mat& image,
                              PointerMap& pointer_map = EMPTY_POINTER_MAP,
                              void* pointer = 0) const;

    RasterizeResult rasterize(const Shape& shape, const Pose3D& cam_pose, const Pose3D& obj_pose, cv::Mat& image,
                              PointerMap& pointer_map = EMPTY_POINTER_MAP,
                              void* pointer = 0) const;

    inline cv::Point2d project3Dto2D(const Vector3 p, int width, int height) const {
        return cv::Point2d((fx_ * p.x() + tx_) / -p.z() + cx_, (fy_ * -p.y() + ty_) / -p.z() + cy_);
    }

    inline Vector3 project2Dto3D(int x, int y) const {
        return Vector3((x - cx_ - tx_) / fx_, -(y - cy_ - ty_) / fy_, -1.0);
    }

    void setFocalLengths(double fx, double fy);

    void setOpticalCenter(double cx, double cy);

    void setOpticalTranslation(double tx, double ty);

protected:

    double fx_, fy_;
    double cx_, cy_;
    double tx_, ty_;

    void drawTriangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, cv::Mat& image,
                      PointerMap& pointer_map, void* pointer, RasterizeResult& res) const;

    void drawTriangle(float x1, float y1, float depth1,
                      float x2, float y2, float depth2,
                      float x3, float y3, float depth3, cv::Mat& image,
                      PointerMap& pointer_map, void* pointer, RasterizeResult& res) const;

    void drawSpansBetweenEdges(const Edge& e1, const Edge& e2, cv::Mat& image, PointerMap& pointer_map, void* pointer) const;

    void drawSpan(const Span &span, int y, cv::Mat& image, PointerMap& pointer_map, void* pointer) const;

};

}

#endif
