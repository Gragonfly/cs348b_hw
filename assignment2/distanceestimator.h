//
//  distanceestimator.hpp
//
//
//  Created by 付光明 on 2023/4/9.
//
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif


#ifndef distanceestimator_h
#define distanceestimator_h

#include <stdio.h>
#include "shape.h"

namespace pbrt {

// distanceesitimator Declarations
class DistanceEstimator : public Shape {
  public:
    // Sphere Public Methods
    DistanceEstimator(const Transform *ObjectToWorld, const Transform *WorldToObject,
                      bool reverseOrientation, Float radius, int maxIters, Float hitEpsilon,
                      Float rayEpsilonMultiplier, Float normalEpsilon)
        : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
          radius(radius),
          maxIters(maxIters),
          hitEpsilon(hitEpsilon),
          rayEpsilonMultiplier(rayEpsilonMultiplier),
          normalEpsilon(normalEpsilon){}
    
    Vector3f CalculateNormal(const Point3f& pos, float eps, const Vector3f& defaultNormal) const;
    virtual Float Evaluate(const Point3f& p) const = 0;
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    Interaction Sample(const Point2f &u, Float *pdf) const;
//    Interaction Sample(const Interaction &ref, const Point2f &u,
//                       Float *pdf) const;
    Float Area() const;


    // Sphere Private Data
    const Float radius;
    const int maxIters; // Number of steps along the ray until we give up (default 1000)const
    const Float hitEpsilon; // how close to the surface we must be before we say we "hit" it
    const Float rayEpsilonMultiplier; // how much we multiply hitEpsilon by to get pError
    const Float normalEpsilon; // The epsilon we send to CalculateNormal()
};

//std::shared_ptr<Shape> CreateDistanceEstimatorShape(const Transform *o2w,
//                                         const Transform *w2o,
//                                         bool reverseOrientation,
//                                         const ParamSet &params);



}  // namespace pbrt

#endif /* distanceestimator_h */
