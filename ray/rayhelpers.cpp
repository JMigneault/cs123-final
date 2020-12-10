#include "rayhelpers.h"

#include <QFileInfo>

#include <cassert>


RayHelpers::RayHelpers()
{

}

// Structs
struct RayHelpers::Ray {
    glm::vec3 p; // origin
    glm::vec3 d; // direction
};

struct RayHelpers::IntersectInfo { // information about the point of intersection
    glm::vec3 p; // collision point
    glm::vec3 n; // normal vector
    glm::vec2 uv; // uv coordinates
};

struct RayHelpers::Raycast { // vectors in object space
    bool hit;
    float t;
    int index;
};



// General raycasting functions

inline void RayHelpers::doPlaneCast(Raycast &cast, const Ray &ray, int dim, float bound) { // NOTE: must be in one cartesian plan (eg. xy-plane, yz-plane, etc.)
    float t = (bound - ray.p[dim]) / ray.d[dim];

    if (t >= 0) {
        cast = Raycast{true, t, -1 };
    } else {
        cast = Raycast{false, -1, -1 };
    }
}

inline void RayHelpers::chooseCast(Raycast &result, const Raycast &cast1, const Raycast &cast2) {
    if (cast1.hit && cast2.hit) {
        result = cast1.t < cast2.t ? cast1 : cast2;
    } else { if (cast1.hit) {
        result = cast1;
    } else { if (cast2.hit) {
        result = cast2;
    } else {
        result = Raycast{false, -1, -1};
    } } }
}

float inline RayHelpers::quadraticEquation(float A, float B, float C, bool usePlus) {
    float discr = B * B - 4 * A * C;
    if (discr < 0) {
        return -1;
    }
    return usePlus ? (-B + glm::sqrt(discr)) / (2 * A) : (-B - glm::sqrt(discr)) / (2 * A);
}

glm::vec3 RayHelpers::reflectVector(const glm::vec3 &reflect, const glm::vec3 &around) { // note: should be normalized vectors
    return around * 2.0f * glm::dot(around, reflect) - reflect;
}

// Cube functions

inline float RayHelpers::isInSquareBounds(glm::vec3 p, int dim) {
    bool toRet = true;
    for (int i = 0; i < 3; i++) {
        if (i != dim) {
            toRet = toRet && -0.5 <= p[i] && p[i] <= 0.5;
        }
    }
    return toRet;
}

glm::vec3 RayHelpers::getCubeGradient(const glm::vec3 &pos) {
    float eps = .0001;
    if (pos.x >= .5 - eps) {
        return glm::vec3(1, 0, 0);
    } else { if (pos.x <= -.5 + eps) {
        return glm::vec3(-1, 0, 0);
    } else { if (pos.y >= .5 - eps) {
        return glm::vec3(0, 1, 0);
    } else { if (pos.y <= -.5 + eps) {
        return glm::vec3(0, -1, 0);
    } else { if (pos.z >= .5 - eps) {
        return glm::vec3(0, 0, 1);
    } else { if (pos.z <= -.5 + eps) {
        return glm::vec3(0, 0, -1);
    } } } } } }
}

inline glm::vec2 RayHelpers::getCubeUV(const glm::vec3 &pos) {
    float eps = .0001;
    float u = 0, v = 0;
    if (pos.x >= .5 - eps) {
        u = .5 - pos.z; v = .5 - pos.y;  // flip y and z
    } else { if (pos.x <= -.5 + eps) {
        u = .5 + pos.z; v = .5 - pos.y;  // flip y
    } else { if (pos.y >= .5 - eps) {
        u = .5 + pos.x; v = .5 + pos.z;
    } else { if (pos.y <= -.5 + eps) {
        u = .5 + pos.x; v = .5 - pos.z; // flip x, z
    } else { if (pos.z >= .5 - eps) {
        u = .5 + pos.x; v = .5 - pos.y; // flip y
    } else { if (pos.z <= -.5 + eps) {
        u = .5 - pos.x; v = .5 - pos.y; // flip x, y
    } } } } } }
    return glm::vec2(u, v);
}


void RayHelpers::checkCubeSide(Raycast &cast, const Ray &ray, int dim, float bound) {

    doPlaneCast(cast, ray, dim, bound);
    glm::vec3 coll = ray.d * cast.t + ray.p;
    if (cast.hit) {
        cast.hit = isInSquareBounds(coll, dim);
    }

}

inline void RayHelpers::rayIntersectCube(Raycast &result, const Ray &ray) {
    // xy close
    Raycast cast = Raycast{false, -1, -1}, newCast;
    for (int i = 0; i < 3; i++) {
        checkCubeSide(newCast, ray, i, -0.5);
        chooseCast(cast, newCast, cast);
        checkCubeSide(newCast, ray, i, 0.5);
        chooseCast(cast, newCast, cast);
    }

    result = cast;
}

// Cone + Cylinder functions

inline RayHelpers::Raycast RayHelpers::checkConeCylinderPoint(const Ray &ray, float t) {
    glm::vec3 coll = t * ray.d + ray.p; // Note: potential repeated computation here

    if (t >= 0 && -0.5 <= coll.y && coll.y <= 0.5) {
        return Raycast {true, t, -1 };
    }
    return Raycast{false, -1, -1 };
}

// Cone functions

inline glm::vec3 RayHelpers::getConeGradient(const glm::vec3 &pos) {
    float eps = .0001;
    if (pos.y <= -.5 + eps) {
        return glm::vec3(0, -1, 0);
    }

    return glm::normalize(glm::vec3(2 * pos.x, 0.25 - (pos.y / 2.0), 2 * pos.z));
}

inline glm::vec2 RayHelpers::getConeUV(const glm::vec3 &pos) {
    float eps = .0001;
    if (pos.y <= -.5 + eps) {
        return glm::vec2(.5f + pos.x, .5f - pos.z);
    }

    float theta = std::atan2(pos.z, pos.x) / (2 * glm::pi<float>());
    theta = theta >= 0 ? theta : 1 + theta;

    return glm::vec2(1 - theta, .5 - pos.y);
}

inline void RayHelpers::rayIntersectCone(Raycast &result, const Ray &ray) {

    Raycast coneCast, capCast;

    // cone portion
    float A = ray.d.x * ray.d.x + ray.d.z * ray.d.z - .25 * ray.d.y * ray.d.y;
    float B = 2 * ray.p.x * ray.d.x + 2 * ray.p.z * ray.d.z - .5 * ray.p.y * ray.d.y + .25 * ray.d.y;
    float C = ray.p.x * ray.p.x + ray.p.z * ray.p.z - .25 * ray.p.y * ray.p.y + .25 * ray.p.y - (1 / 16.0);

    Raycast cast1 = checkConeCylinderPoint(ray, quadraticEquation(A, B, C, true));
    Raycast cast2 = checkConeCylinderPoint(ray, quadraticEquation(A, B, C, false));
    chooseCast(coneCast, cast1, cast2);


    // cap portion
    doPlaneCast(capCast, ray, 1, -0.5);
    if (capCast.hit) {
        glm::vec3 coll = ray.d * capCast.t + ray.p;
        capCast.hit = coll.x * coll.x + coll.z * coll.z <= 0.25; // check if hit in circle
    }

    chooseCast(result, coneCast, capCast);
}

// Cylinder functions

inline glm::vec3 RayHelpers::getCylinderGradient(const glm::vec3 &pos) {
    float eps = .0001;
    if (pos.y >= .5 - eps) {
        return glm::vec3(0, 1, 0);
    } else { if (pos.y <= -.5 + eps) {
        return glm::vec3(0, -1, 0);
    } }

    return glm::normalize(glm::vec3(2 * pos.x, 0, 2 * pos.z));
}

inline glm::vec2 RayHelpers::getCylinderUV(const glm::vec3 &pos) {
    float eps = .0001;
    if (pos.y >= .5 - eps) {
        return glm::vec2(.5f + pos.x, .5f + pos.z);
    } else {
        if (pos.y <= -.5 + eps) {
            return glm::vec2(.5f + pos.x, .5f - pos.z);
        }
    }

    float theta = std::atan2(pos.z, pos.x) / (2 * glm::pi<float>());
    theta = theta >= 0 ? theta : 1 + theta;

    return glm::vec2(1 - theta, .5 - pos.y);
}


inline void RayHelpers::rayIntersectCylinder(Raycast &result, const Ray &ray) {

    Raycast cylinderCast, capsCast;

    // cylinder portion
    float A = ray.d.x * ray.d.x + ray.d.z * ray.d.z;
    float B = 2 * ray.p.x * ray.d.x + 2 * ray.p.z * ray.d.z;
    float C = ray.p.x * ray.p.x + ray.p.z * ray.p.z - 0.25;

    Raycast cast1 = checkConeCylinderPoint(ray, quadraticEquation(A, B, C, true));
    Raycast cast2 = checkConeCylinderPoint(ray, quadraticEquation(A, B, C, false));
    chooseCast(cylinderCast, cast1, cast2);

    // caps portions
    Raycast topCast;
    doPlaneCast(topCast, ray, 1, 0.5);
    if (topCast.hit) {
        glm::vec3 coll = ray.d * topCast.t + ray.p;
        topCast.hit = coll.x * coll.x + coll.z * coll.z <= 0.25; // check if hit in circle
    }

    Raycast botCast;
    doPlaneCast(botCast, ray, 1, -0.5);
    if (botCast.hit) {
        glm::vec3 coll = ray.d * botCast.t + ray.p;
        botCast.hit = coll.x * coll.x + coll.z * coll.z <= 0.25; // check if hit in circle
    }

    chooseCast(capsCast, topCast, botCast);

    chooseCast(result, cylinderCast, capsCast);

}

// Sphere functions

glm::vec3 RayHelpers::getSphereGradient(const glm::vec3 &pos) {
    return glm::normalize(glm::vec3(2 * pos.x, 2 * pos.y, 2 * pos.z));
}

glm::vec2 RayHelpers::getSphereUV(const glm::vec3 &pos) {
    float theta = std::atan2(pos.z, pos.x) / (2 * glm::pi<float>());
    theta = theta >= 0 ? theta : 1 + theta;
    float phi = glm::acos(2.0 * pos.y);

    return glm::vec2(1 - theta, phi / glm::pi<float>());
}

void RayHelpers::rayIntersectSphere(Raycast &result, const Ray &ray) {

    // cylinder portion
    float A = ray.d.x * ray.d.x + ray.d.y * ray.d.y + ray.d.z * ray.d.z;
    float B = 2 * ray.p.x * ray.d.x + 2 * ray.p.y * ray.d.y + 2 * ray.p.z * ray.d.z;
    float C = ray.p.x * ray.p.x + ray.p.y * ray.p.y + ray.p.z * ray.p.z - 0.25;

    float t1 = quadraticEquation(A, B, C, true);
    float t2 = quadraticEquation(A, B, C, false);

    Raycast cast1 = Raycast{t1 >= 0, t1, -1 };
    Raycast cast2 = Raycast{t2 >= 0, t2, -1 };
    chooseCast(result, cast1, cast2);

}

// Rendering functions

inline RayHelpers::Ray RayHelpers::buildWorldRay(int row, int col, float dWidth, float dHeight, const glm::mat4 &cameraToWorld) {
    glm::vec4 cameraD = glm::vec4(
                dWidth * (col + 0.5) - 1, // camera space x pos
                1 - dHeight * (row + 0.5), // y direction is inverted (row 0 = y -> y = 1)
                -1, 0);
    return Ray{
        (cameraToWorld * glm::vec4(0, 0, 0, 1)).xyz(),
        (glm::normalize(cameraToWorld * cameraD)).xyz()
    };
}

inline unsigned char RayHelpers::realToByte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void RayHelpers::getIntersectInfo(IntersectInfo &info, const Raycast &cast, const Ray &worldRay,
                                  const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &transformations,
                                  const std::vector<glm::mat4> &inverses) { // TODO: more args

    info.p = worldRay.d * cast.t + worldRay.p; // worldIntersect
    glm::vec3 objectIntersect = (inverses[cast.index] * glm::vec4(info.p, 1)).xyz();

    switch(primitives[cast.index].type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            info.n = getCubeGradient(objectIntersect);
            info.uv = getCubeUV(objectIntersect);
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            info.n = getConeGradient(objectIntersect);
            info.uv = getConeUV(objectIntersect);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            info.n = getCylinderGradient(objectIntersect);
            info.uv = getCylinderUV(objectIntersect);
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            info.n = getSphereGradient(objectIntersect);
            info.uv = getSphereUV(objectIntersect);
            break;

    }

    // convert normal to world space
    info.n = glm::normalize(glm::inverse(glm::transpose(glm::mat3(transformations[cast.index]))) * info.n);

}


glm::vec3 RayHelpers::getPixelColor(const Raycast &cast, const IntersectInfo &info, const Ray &worldRay, const CS123SceneGlobalData &global, const std::vector<CS123SceneLightData> &lights,
                                      const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &inverses,
                                      std::map<QString, QImage> &textureMap) {

    // return RGBA(realToByte(cast.uv.x), 0, realToByte(cast.uv.y), 255);

    // return glm::vec3(.5 + info.n.x * .5, .5 + info.n.y * .5, .5 + info.n.z * .5); // normal mapping


    const CS123SceneMaterial &material = primitives[cast.index].material;

    // texture mapping
    glm::vec3 pointColor;
    if (settings.useTextureMapping && material.textureMap.isUsed) {
        QString qfilename(material.textureMap.filename.data());
        QImage *texture;
        if (textureMap.find( qfilename ) == textureMap.end()) {
            textureMap[qfilename] = QImage(material.textureMap.filename.data());
            texture = &textureMap[qfilename];
        } else {
            texture = &textureMap[qfilename];
        }
        int w = texture->width();
        int h = texture->height();
        int s = (static_cast<int>(info.uv.x * w * material.textureMap.repeatU)) % w;
        int t = (static_cast<int>(info.uv.y * h * material.textureMap.repeatV)) % h;
        QRgb textureSample = texture->pixel(s, t);
        glm::vec3 textureColor = glm::vec3(qRed(textureSample) / 255., qGreen(textureSample) / 255., qBlue(textureSample) / 255.);
        pointColor = glm::mix(material.cDiffuse.rgb() * global.kd, textureColor, material.blend);

    } else {
        pointColor = material.cDiffuse.rgb() * global.kd;
    }

    // color computation
    glm::vec3 lightSum = glm::vec3(0);
    for (int i = 0; i < static_cast<int>(lights.size()); i++) {
        glm::vec3 lightNorm;
        float atten = 1.0f;
        if (lights[i].type == LightType::LIGHT_POINT && settings.usePointLights) {
            glm::vec3 dPos = glm::vec3(lights[i].pos) - info.p;
            lightNorm = glm::normalize(dPos);
            float d = glm::length(dPos);
            atten = 1 / (lights[i].function.x + lights[i].function.y * d + lights[i].function.z * d * d);
        } else {
            if (lights[i].type == LightType::LIGHT_DIRECTIONAL && settings.useDirectionalLights) {
                lightNorm = -glm::normalize(lights[i].dir.xyz());
            } else {
                continue; // can't or shouldn't compute light contribution
            }
        }

        // shadow checking
        bool noShadow = true;
        if (settings.useShadows) {
            // need hit and t
            Raycast lightCast;
            Ray ray = Ray{info.p + (lightNorm * .001f), lightNorm};
            raycast(lightCast, ray, primitives, inverses);
            float lightDist = glm::length(glm::vec3(lights[i].pos) - ray.p);
            noShadow = !lightCast.hit || (lights[i].type == LightType::LIGHT_POINT && lightCast.t > lightDist);
        }


        if (noShadow) {

            glm::vec3 reflNorm = reflectVector(-lightNorm, info.n);
            float specDot = global.ks * glm::pow(glm::max(0.0f, glm::dot(reflNorm, worldRay.d)), material.shininess);
            float diffDot = glm::max(0.0f, glm::dot(info.n, lightNorm));

            lightSum.r += atten * lights[i].color.r * (pointColor.r * diffDot + material.cSpecular.r * specDot);
            lightSum.g += atten * lights[i].color.g * (pointColor.g * diffDot + material.cSpecular.g * specDot);
            lightSum.b += atten * lights[i].color.b * (pointColor.b * diffDot + material.cSpecular.b * specDot);
        }

    }
    lightSum.r += global.ka * material.cAmbient.r;
    lightSum.g += global.ka * material.cAmbient.g;
    lightSum.b += global.ka * material.cAmbient.b;

    return lightSum;

}

void RayHelpers::raycast(Raycast &cast, const Ray &worldRay, const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &inverses) {

    Raycast newCast;
    cast = Raycast{false, -1, -1};

    for (int i = 0; i < static_cast<int>(primitives.size()); i++) {
        glm::mat4 worldToObject = inverses[i];

        // convert to object space
        Ray ray = Ray{(worldToObject * glm::vec4(worldRay.p, 1)).xyz(),
                (worldToObject * glm::vec4(worldRay.d, 0)).xyz()};

        switch (primitives[i].type) {
           case PrimitiveType::PRIMITIVE_CUBE:
               rayIntersectCube(newCast, ray);
               break;
           case PrimitiveType::PRIMITIVE_CONE:
               rayIntersectCone(newCast, ray);
               break;
           case PrimitiveType::PRIMITIVE_CYLINDER:
               rayIntersectCylinder(newCast, ray);
               break;
           case PrimitiveType::PRIMITIVE_SPHERE:
               rayIntersectSphere(newCast, ray);
               break;

        }

        newCast.index = i;

        chooseCast(cast, cast, newCast);


    }

}

glm::vec3 RayHelpers::doRecursiveCastColor(int numBounces, const Ray &ray, const CS123SceneGlobalData &global, const std::vector<CS123SceneLightData> &lights,
                                           const std::vector<CS123ScenePrimitive> &primitives, const std::vector<glm::mat4> &transformations,
                                           const std::vector<glm::mat4> &inverses, std::map<QString, QImage> &textureMap) {

    Raycast cast;
    raycast(cast, ray, primitives, inverses);

    if (!cast.hit) {
        return glm::vec3(0, 0, 0);
    }

    // compute world position, world normal, and uv for intersection point
    IntersectInfo info;
    getIntersectInfo(info, cast, ray, primitives, transformations, inverses);

    // return glm::vec3(info.p.x + .5, info.p.y + .5, info.p.z - .5); // debuggin position

    glm::vec3 color = getPixelColor(cast, info, ray, global, lights, primitives, inverses, textureMap);

    if (numBounces == 0) {
        return color;
    } else {
        // reflected raycast and recursive call
        glm::vec3 reflDir = -reflectVector(ray.d, info.n);
        Ray reflectedRay = Ray{info.p + (.001f * reflDir), reflDir};

        glm::vec3 reflectionConstants = glm::vec3(global.ks * primitives[cast.index].material.cReflective.r,
                                                  global.ks * primitives[cast.index].material.cReflective.g,
                                                  global.ks * primitives[cast.index].material.cReflective.b);
        glm::vec3 reflectedColor = doRecursiveCastColor(numBounces - 1, reflectedRay, global, lights, primitives, transformations, inverses, textureMap);

        return glm::vec3(color.r + reflectedColor.r * reflectionConstants.r,
                         color.g + reflectedColor.g * reflectionConstants.g,
                         color.b + reflectedColor.b * reflectionConstants.b);
    }

}

void RayHelpers::renderImage(const Camera *camera, int width, int height, Canvas2D *canvas, const CS123SceneGlobalData &global,
                             const std::vector<CS123SceneLightData> &lights, const std::vector<CS123ScenePrimitive> &primitives,
                             const std::vector<glm::mat4> &transformations, const std::vector<glm::mat4> &inverses) {

    glm::mat4 worldToCamera = camera->getScaleMatrix() * camera->getViewMatrix();
    glm::mat4 cameraToWorld = glm::inverse(worldToCamera);

    std::map<QString, QImage> textureMap;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Ray worldRay = buildWorldRay(row, col, 2.0 / width, 2.0 / height, cameraToWorld);

            int numBounces = settings.useReflection ? NUMBOUNCES : 0;

            glm::vec3 color = doRecursiveCastColor(numBounces, worldRay, global, lights, primitives, transformations, inverses, textureMap);
            canvas->data()[row * width + col] = RGBA(realToByte(color.r), realToByte(color.g), realToByte((color.b)));

        }
        QCoreApplication::processEvents();
        canvas->update();
    }
}

