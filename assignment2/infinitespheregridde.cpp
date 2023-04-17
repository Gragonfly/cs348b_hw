//
//  infinitespheregridde.cpp
//  PBRT-V3
//
//  Created by 付光明 on 2023/4/15.
//

#include "infinitespheregridde.h"
#include "sampling.h"
#include "paramset.h"
#include "efloat.h"
#include "stats.h"

namespace pbrt {
    
Float InfiniteSphereGridDE::Evaluate(const Point3f& p) const{
    //这个函数的作用是计算当前点到曲面到最近距离！！
    // 获取p点坐标
    Float px(p.x), py(p.y), pz(p.z);
//    // 计算p点坐标到原点的距离，注意这里默认传入的p点是在物体坐标系下的p点
//    Float distance = std::sqrt(px * px + py * py + pz * pz);
    
    // 在step5中计算的不再是p点到原点的距离，而是p点到最近立方单元格中心到距离
    
    //计算p点当前所处位置的立方单元格中心坐标
    Float distocenterx = remainder(px, radius);
    Float distocentery = remainder(py, radius);
    Float distocenterz = remainder(pz, radius);

    Float distance = std::sqrt(distocenterx * distocenterx + distocentery * distocentery + distocenterz * distocenterz);
    
    
    // 与球面半径做差，得出距离
    Float step_r = distance - 1.0;
//    printf("centerx:%f\n", centerx);
//    printf("centery:%f\n", centery);
//    printf("centerz:%f\n", centerz);
//    printf("remainder(px):%f\n", px);
//    printf("remainder(py):%f\n", py);
//    printf("remainder(pz):%f\n", pz);
//    printf("step_r:%f\n", step_r);
    // 返回距离
    return step_r;
}

Bounds3f InfiniteSphereGridDE::ObjectBound() const {
    return Bounds3f(Point3f(-radius*1000000000, -radius*1000000000, -radius*1000000000),
                    Point3f(radius*1000000000, radius*1000000000, radius*1000000000));
    
}

Float InfiniteSphereGridDE::Area() const { return 4 * Pi * radius * radius * 1000000000; }

std::shared_ptr<Shape> CreateInfiniteSphereGridDE(const Transform *o2w,
                                         const Transform *w2o,
                                         bool reverseOrientation,
                                         const ParamSet &params) {
    Float radius = params.FindOneFloat("radius", 1.f);
    int maxIters = params.FindOneInt("maxIters", 1000); // Number of steps along the ray until we give up (default 1000)
    Float hitEpsilon = params.FindOneFloat("hitEpsilon", 0.1f); // how close to the surface we must be before we say we "hit" it
    Float rayEpsilonMultiplier = params.FindOneFloat("rayEpsilonMultiplier", 10.0f); // how much we multiply hitEpsilon by to get pError
    Float normalEpsilon = params.FindOneFloat("normalEpsilon", 0.1f); // The epsilon we send to CalculateNormal()
    Float cellSize = params.FindOneFloat("cellSize", 6.0f);
    radius = cellSize;
    return std::make_shared<InfiniteSphereGridDE>(o2w, w2o, reverseOrientation, radius, maxIters, hitEpsilon, rayEpsilonMultiplier, normalEpsilon, cellSize);
}


}
