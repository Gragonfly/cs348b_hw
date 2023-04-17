//
//  distanceestimator.cpp
//  
//
//  Created by 付光明 on 2023/4/9.
//

#include "distanceestimator.h"
#include "sampling.h"
#include "paramset.h"
#include "efloat.h"
#include "stats.h"

namespace pbrt {


Vector3f DistanceEstimator::CalculateNormal(const Point3f& pos, float eps, const Vector3f& defaultNormal) const{
    // 计算法线
    const Vector3f v1 = Vector3f( 1.0,-1.0,-1.0);
    const Vector3f v2 = Vector3f(-1.0,-1.0, 1.0);
    const Vector3f v3 = Vector3f(-1.0, 1.0,-1.0);
    const Vector3f v4 = Vector3f( 1.0, 1.0, 1.0);

    const Vector3f normal = v1 * Evaluate( pos + v1*eps ) +
                 v2 * Evaluate( pos + v2*eps ) +
                 v3 * Evaluate( pos + v3*eps ) +
                 v4 * Evaluate( pos + v4*eps );
    const Float length = normal.Length();

    return length > 0 ? (normal/length) : defaultNormal;
}

bool DistanceEstimator::Intersect(const Ray &r, Float *tHit, SurfaceInteraction *isect,
                       bool testAlphaTexture) const {
    ProfilePhase p(Prof::ShapeIntersect);
    Float phi;
    Point3f pHit;
    // 将光线从世界坐标系转到以球为中心的物体坐标系中，这样可以使球体处在原点，简化交点求解
    Vector3f oErr, dErr;
    Ray ray = (*WorldToObject)(r, &oErr, &dErr);
    
    // 使用作业中提到的距离评估算法来计算交点（或者说接近交点）
    Float t = 0;
    // eps这个参数决定了我们默认点与球多接近时会被认为是交点
    Float epsilon = 0.001;
    Float dis = Evaluate(ray.o);
    Float dis_origin = dis;
    Float length_of_d = std::sqrt(ray.d.x * ray.d.x + ray.d.y * ray.d.y + ray.d.z * ray.d.z);
    bool findflag = 1;
    int count = 0;
    while (dis > epsilon && count <= maxIters) {
        t = t + dis / length_of_d;
        count++;
        pHit = ray.o + t * ray.d;
        dis = Evaluate(pHit);
        //printf("t:%f\n", t);
        // 其实是一种情况，我们找不到交点，当我们的射线与球相离的时候，这个时候t会一直迭代直到超过最大值，但是也可以通过dis超过初始值来判断
        if(t > ray.tMax || dis > dis_origin){
            // 没找到的时候t会超过射线允许最大值，并且findflag置为0
            findflag = 0;
            break;
        }
    }
    if(findflag){
        // 这种情况是找到了交点，我们需要根据作业中的描述记录交点
        Normal3f dndu = Normal3f(0, 0, 0);
        Normal3f dndv = Normal3f(0, 0, 0);
        Float theta = std::acos(Clamp(pHit.z / radius, -1, 1));
        Vector3f normal = CalculateNormal(pHit, epsilon, -ray.d);
        Float zRadius = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
        Float invZRadius = 1 / zRadius;
        Float cosPhi = pHit.x * invZRadius;
        Float sinPhi = pHit.y * invZRadius;
        Vector3f dpdu(-2 * Pi * pHit.y, 2 * Pi * pHit.x, 0);
        Vector3f dpdv = Pi * Vector3f(pHit.z * cosPhi, pHit.z * sinPhi, -radius * std::sin(theta));
        Vector3f error(epsilon*10, epsilon*10, epsilon*10);
        
        // 更新交点信息
        *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, error, Point2f(0, 0),
                                                     -ray.d, dpdu, dpdv, dndu, dndv,
                                                     ray.time, this));
        *tHit = t;
        return true;
    }else{
        return false;
    }

}

Interaction DistanceEstimator::Sample(const Point2f &u, Float *pdf) const {
    LOG(FATAL) << "Cone::Sample not implemented.";
    return Interaction();
}

Bounds3f DistanceEstimator::ObjectBound() const {
    return Bounds3f(Point3f(-radius, -radius, -radius),
                    Point3f(radius, radius, radius));
}

Float DistanceEstimator::Area() const { return 4 * Pi * radius * radius; }

//std::shared_ptr<Shape> CreateDistanceEstimatorShape(const Transform *o2w,
//                                         const Transform *w2o,
//                                         bool reverseOrientation,
//                                         const ParamSet &params) {
//    Float radius = params.FindOneFloat("radius", 1.f);
//    int maxIters = params.FindOneInt("maxIters", 1000); // Number of steps along the ray until we give up (default 1000)
//    Float hitEpsilon = params.FindOneFloat("hitEpsilon", 0.1f); // how close to the surface we must be before we say we "hit" it
//    Float rayEpsilonMultiplier = params.FindOneFloat("rayEpsilonMultiplier", 10.0f); // how much we multiply hitEpsilon by to get pError
//    Float normalEpsilon = params.FindOneFloat("normalEpsilon", 0.1f); // The epsilon we send to CalculateNormal()
//    return std::make_shared<DistanceEstimator>(o2w, w2o, reverseOrientation, radius, maxIters, hitEpsilon, rayEpsilonMultiplier, normalEpsilon);
//}

}


