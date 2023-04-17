//
//  infinitespheregridde.hpp
//  PBRT-V3
//
//  Created by 付光明 on 2023/4/15.
//
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif


#ifndef infinitespheregridde_h
#define infinitespheregridde_h

#include <stdio.h>
#include "distanceestimator.h"


namespace pbrt {

// distanceesitimator Declarations
class InfiniteSphereGridDE : public DistanceEstimator {
    
  public:
    // Sphere Public Methods
    InfiniteSphereGridDE(const Transform *ObjectToWorld, const Transform *WorldToObject,
                      bool reverseOrientation, Float radius, int maxIters, Float hitEpsilon,
                      Float rayEpsilonMultiplier, Float normalEpsilon, Float cellSize)
        : DistanceEstimator(ObjectToWorld, WorldToObject,
                            reverseOrientation, radius, maxIters, hitEpsilon,
                            rayEpsilonMultiplier, normalEpsilon){}
    
    //Vector3f CalculateNormal(const Point3f& pos, float eps, const Vector3f& defaultNormal) const;
    Float Evaluate(const Point3f& p) const;
    Bounds3f ObjectBound() const;
    //bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect, bool testAlphaTexture) const;
    //Interaction Sample(const Point2f &u, Float *pdf) const;
    Float Area() const;


  private:
};

std::shared_ptr<Shape> CreateInfiniteSphereGridDE(const Transform *o2w,
                                         const Transform *w2o,
                                         bool reverseOrientation,
                                         const ParamSet &params);

}



#endif /* infinitespheregridde_h */
