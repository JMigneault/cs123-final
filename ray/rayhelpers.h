#ifndef RAYCASTINGHELPERS_H
#define RAYCASTINGHELPERS_H

#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>

#include <glm/gtx/transform.hpp>

#include "glm/gtc/constants.hpp"


class RayHelpers
{

    // Structs
    struct Ray;

    struct Raycast;

    struct IntersectInfo;


public:

    const static int NUMBOUNCES = 2;

    RayHelpers();

    inline static void chooseCast(Raycast &result, const Raycast &cast1, const Raycast &cast2);
    inline static void raycast(Raycast &cast, const Ray &worldRay,
                               const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &inverses);

    inline static float quadraticEquation(float A, float B, float C, bool usePlus);
    inline static glm::vec3 reflectVector(const glm::vec3 &reflect, const glm::vec3 &around);
    inline static unsigned char realToByte(float f);

    inline static void doPlaneCast(Raycast &cast, const Ray &ray, int dim, float bound); // NOTE: must be in one cartesian plan (eg. xy-plane, yz-plane, etc.)
    inline static float isInSquareBounds(glm::vec3 p, int dim);
    inline static void checkCubeSide(Raycast &cast, const Ray &ray, int dim, float bound);
    inline static glm::vec3 getCubeGradient(const glm::vec3 &pos);
    inline static glm::vec2 getCubeUV(const glm::vec3 &pos);
    inline static void rayIntersectCube(Raycast &result, const Ray &ray);

    inline static Raycast checkConeCylinderPoint(const Ray &ray, float t);
    inline static glm::vec3 getConeGradient(const glm::vec3 &pos);
    inline static glm::vec2 getConeUV(const glm::vec3 &pos);
    inline static void rayIntersectCone(Raycast &result, const Ray &ray);

    inline static glm::vec3 getCylinderGradient(const glm::vec3 &pos);
    inline static glm::vec2 getCylinderUV(const glm::vec3 &pos);
    inline static void rayIntersectCylinder(Raycast &result, const Ray &ray);

    inline static glm::vec3 getSphereGradient(const glm::vec3 &pos);
    inline static glm::vec2 getSphereUV(const glm::vec3 &pos);
    inline static void rayIntersectSphere(Raycast &result, const Ray &ray);

    inline static Ray buildWorldRay(int row, int col, float dWidth, float dHeight, const glm::mat4 &cameraToWorld);
    inline static glm::vec3 getPixelColor(const Raycast &cast, const IntersectInfo &info, const Ray &worldRay, const CS123SceneGlobalData &global, const std::vector<CS123SceneLightData> &lights,
                                          const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &inverses,
                                          std::map<QString, QImage> &textureMap);

    inline static void getIntersectInfo(IntersectInfo &info, const Raycast &cast, const Ray &worldRay,
                                      const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &transformations,
                                      const std::vector<glm::mat4> &inverses); // TODO: more args

    static glm::vec3 doRecursiveCastColor(int numBounces, const Ray &ray, const CS123SceneGlobalData &global, const std::vector<CS123SceneLightData> &lights,
                                          const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &transformations,
                                          const std::vector<glm::mat4> &inverses, std::map<QString, QImage> &textureMap);
    static void renderImage(const Camera *camera, int width, int height, Canvas2D *data, const CS123SceneGlobalData &global,
                                 const std::vector<CS123SceneLightData> &lights, const std::vector<CS123ScenePrimitive> &primitives,
                                 const std::vector<glm::mat4> &transformations, const std::vector<glm::mat4> &inverses);
};

#endif // RAYCASTINGHELPERS_H
