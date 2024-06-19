/*
 * @Author: Vanish
 * @Date: 2024-06-01 21:38:49
 * @LastEditTime: 2024-06-19 11:08:57
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */


#include "RayTracing.h"

#include "Material.h"
#include "Sphere.h"
#include "Quad.h"
#include "Box.h"
#include "Camera.h"
#include "Scene.h"

Scene* g_Scene;


int main() 
{
    // create materials
    auto red_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(1, 0, 0,1),
        0.9,
        0.5
    );
    auto green_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(0, 1, 0,1),
        1.0,
        0
    );
    auto blue_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(0, 0, 1,1),
        0,
        0.5
    );
    auto yellow_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(1, 1, 0,1),
        0.9,
        0.5
    );
    auto white_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(0.8, 0.8, 0.8,1),
        0.5, 
        0.5
    );
    auto WhiteLight_Lambert = std::make_shared<Material_BlinnPhong>(
        Vector3(0,0,0),
        1000000000
    );

    auto white = std::make_shared<Material_PBM>(
        Color(1, 1, 1,1),
        0.5,
        0.5
    );
    auto grey = std::make_shared<Material_PBM>(
        Color(0.5, 0.5, 0.5,1),
        0.5,
        0.5
    );
    auto red = std::make_shared<Material_PBM>(
        Color(1, 1, 0,1),
        0.5,
        0.5
    );
    auto green = std::make_shared<Material_PBM>(
        Color(0, 1, 1,1),
        0.5,
        0.5
    );
    auto blue = std::make_shared<Material_PBM>(
        Color(0, 0, 1,1),
        0.9,
        0.9
    );
    auto white_Light = std::make_shared<Material_PBM>(
        Vector3(1,1,1),
        600
    );

    // create objects

    //Cornell Box Sides
    Quad* GreenWall = new Quad(
        "GreenWall",
        Vector3(555.0,0.0,0.0),
        Vector3(0,0,555),
        Vector3(0,555,0),
#ifdef RENDERMODE_PBR
        green
#endif
#ifdef RENDERMODE_DEBUG
        green_Lambert
#endif
    );
    Quad* RedWall = new Quad(
        "RedWall",
        Vector3(0.0,0.0,555.0),
        Vector3(0,0,-555),
        Vector3(0,555,0),
#ifdef RENDERMODE_PBR
        red
#endif
#ifdef RENDERMODE_DEBUG
        red_Lambert
#endif
    );
    Quad* WhiteWall = new Quad(
        "WhiteWall",
        Vector3(0.0,555.0,0.0),
        Vector3(555,0,0),
        Vector3(0,0,555),
#ifdef RENDERMODE_PBR
        white
#endif
#ifdef RENDERMODE_DEBUG
        white_Lambert
#endif
    );
    Quad* WhiteWall2 = new Quad(
        "WhiteWall2",
        Vector3(0,0,555),
        Vector3(555,0,0),
        Vector3(0,0,-555),
#ifdef RENDERMODE_PBR
        white
#endif
#ifdef RENDERMODE_DEBUG
        white_Lambert
#endif
    );
    Quad* WhiteWall3 = new Quad(
        "WhiteWall3",
        Vector3(555,0,555),
        Vector3(-555,0,0),
        Vector3(0,555,0),
#ifdef RENDERMODE_PBR
        white
#endif
#ifdef RENDERMODE_DEBUG
        white_Lambert
#endif
    );

    // Box* greyBox = new Box(
    //     "WhiteBox",
    //     Vector3(0,0,0),
    //     Vector3(165,330,165),
    //     white
    // );

    //Sphere
    Sphere* GlassSphere = new Sphere(
        "GlassSphere",
        90,
        Vector3(190,90,290),
#ifdef RENDERMODE_PBR
        blue
#endif
#ifdef RENDERMODE_DEBUG
        blue_Lambert
#endif
    ); 
    Sphere* DiffuseSphere = new Sphere(
        "DiffuseSphere",
        90,
        Vector3(400,90,100),
#ifdef RENDERMODE_PBR
        blue
#endif
#ifdef RENDERMODE_DEBUG
        yellow_Lambert
#endif
    );

    // add lights to scene
    Quad* Light = new Quad(
        "Light",
        Vector3(343+120,554,332 +120),
        Vector3(-380,0,0),
        Vector3(0,0,-380),
#ifdef RENDERMODE_PBR
        white_Light
#endif
#ifdef RENDERMODE_DEBUG
        white_Light
#endif
    );

    // create scene
    g_Scene = new Scene();

    // add objects to scene
    g_Scene->AddObject(GreenWall);
    g_Scene->AddObject(RedWall);
    g_Scene->AddObject(WhiteWall);
    g_Scene->AddObject(WhiteWall2);
    g_Scene->AddObject(WhiteWall3);
    //g_Scene->AddObject(greyBox);
    //g_Scene->AddObject(GlassSphere);
    //g_Scene->AddObject(DiffuseSphere);
    g_Scene->AddObject(Light);

    // set up camera
    Camera* camera = new Camera(
        Vector3(280, 250, -600),
        Vector3(  0,   0,    1),
        Vector3(0.0, 1.0,  0.0),
        1920 /2,
        1080,
        135
        );
    // creat ppm image file
    std::ofstream outputFile("RayTracing.ppm");
    
    // render
    camera->Render(*g_Scene, outputFile);

    // close file
    outputFile.close();

    // 释放内存
    delete GreenWall;
    delete RedWall;
    delete WhiteWall;
    delete WhiteWall2;
    delete WhiteWall3;
    // delete WhiteBox;
    delete GlassSphere;
    delete Light;
    delete camera;
    delete g_Scene;

    return 0;
}

