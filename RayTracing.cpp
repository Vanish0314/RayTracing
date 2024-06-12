
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
    std::shared_ptr<Material_BlinnPhong> red_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(1, 0, 0,1),
        0.5,
        0.5
    );
    std::shared_ptr<Material_BlinnPhong> green_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(0, 1, 0,1),
        1.0,
        0
    );
    std::shared_ptr<Material_BlinnPhong> blue_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(0, 0, 1,1),
        0.5,
        0.5
    );
    std::shared_ptr<Material_BlinnPhong> white_Lambert = std::make_shared<Material_BlinnPhong>(
        Color(1, 1, 1,1),
        0.5,
        0.5
    );
    std::shared_ptr<Material_BlinnPhong> WhiteLight_Lambert = std::make_shared<Material_BlinnPhong>(
        Vector3(0,0,0),
        1000000000
    );
    std::shared_ptr<Material_DeBug> redDebug = std::make_shared<Material_DeBug>(
        Color(1, 0, 0,1)
    );
    std::shared_ptr<Material_DeBug> whiteDebug = std::make_shared<Material_DeBug>(
        Color(1, 1, 1,1)
    );
    std::shared_ptr<Material_DeBug> blueDebug = std::make_shared<Material_DeBug>(
        Color(0, 0, 1,1)
    );
    std::shared_ptr<Material_DeBug> greenDebug = std::make_shared<Material_DeBug>(
        Color(0, 1, 0,1)
    );

    // create objects

    //Cornell Box Sides
    Quad* GreenWall = new Quad(
        "GreenWall",
        Vector3(555.0,0.0,0.0),
        Vector3(0,0,555),
        Vector3(0,555,0),
        green_Lambert
        //greenDebug
    );
    Quad* RedWall = new Quad(
        "RedWall",
        Vector3(0.0,0.0,555.0),
        Vector3(0,0,-555),
        Vector3(0,555,0),
        red_Lambert
        //redDebug
    );
    Quad* WhiteWall = new Quad(
        "WhiteWall",
        Vector3(0.0,555.0,0.0),
        Vector3(555,0,0),
        Vector3(0,0,555),
        //whiteDebug
        white_Lambert
    );
    Quad* WhiteWall2 = new Quad(
        "WhiteWall2",
        Vector3(0,0,555),
        Vector3(555,0,0),
        Vector3(0,0,-555),
        //whiteDebug
        white_Lambert
    );
    Quad* WhiteWall3 = new Quad(
        "WhiteWall3",
        Vector3(555,0,555),
        Vector3(-555,0,0),
        Vector3(0,555,0),
        //whiteDebug
        white_Lambert
    );

    //Box
    // Box* WhiteBox = new Box(
    //     "WhiteBox",
    //     Vector3(0,0,0),
    //     Vector3(165,330,165),
    //     white
    // );

    //Sphere
    Sphere* GlassSphere = new Sphere(
        "GlassSphere",
        165,
        Vector3(190,90,190),
        //blueDebug
        blue_Lambert
    );

    // add lights to scene
    Quad* Light = new Quad(
        "Light",
        Vector3(343,554,332),
        Vector3(-130,0,0),
        Vector3(0,0,-105),
        //redDebug
        WhiteLight_Lambert
    );

    // create scene
    g_Scene = new Scene();

    // add objects to scene
    g_Scene->AddObject(GreenWall);
    g_Scene->AddObject(RedWall);
    g_Scene->AddObject(WhiteWall);
    g_Scene->AddObject(WhiteWall2);
    g_Scene->AddObject(WhiteWall3);
    // g_Scene->AddObject(WhiteBox);
    g_Scene->AddObject(GlassSphere);
    g_Scene->AddObject(Light);

    // set up camera
    Camera* camera = new Camera(
        Vector3(280, 100, -800),
        Vector3(  0,   0,    1),
        Vector3(0.0, 1.0,  0.0),
        1920,
        1080,
        120
        );

    // creat ppm image file
    std::ofstream outputFile("RayTracing.ppm");
    
    // render
    camera->Render(*g_Scene, outputFile);

    // close file
    outputFile.close();

    return 0;
}

