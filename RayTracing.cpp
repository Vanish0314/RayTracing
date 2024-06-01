
#include "RayTracing.h"

#include "Material.h"
#include "Sphere.h"
#include "Quad.h"
#include "Camera.h"
#include "Scene.h"

Scene* g_Scene;

int main() 
{
    // create materials
    std::shared_ptr<Material> red = std::make_shared<Material>(
        Color(1, 0, 0,1),
        0.5,
        1
    );
    std::shared_ptr<Material> green = std::make_shared<Material>(
        Color(0, 1, 0,1),
        1,
        0.2
    );
    std::shared_ptr<Material> blue = std::make_shared<Material>(
        Color(0, 0, 1,1),
        0,
        1
    );
    std::shared_ptr<Material> white = std::make_shared<Material>(
        Color(1, 1, 1,1),
        0.7,
        0.45
    );
    std::shared_ptr<Material> pink = std::make_shared<Material>(
        Color(1, 0.5, 0.5,1),
        0.2,
        0.7
    );
    std::shared_ptr<Material> BlueLight = std::make_shared<Material>(
        Vector3(0, 0, 1),
        100
    );

    // create objects
    Sphere* sphere1 = new Sphere(
        "Sphere1",
        1,
        Vector3(0.0, 0.0, 0.0),
        red
    );
    Sphere* sphere2 = new Sphere(
        "Sphere2",
        2,
        Vector3(-5.0, 0.0, 0.0),
        BlueLight
    );
    Quad* quad1 = new Quad(
        "Quad1",
        Vector3(-10, -20, 0.0),// bottom left
        Vector3(0, 500, 0),//u
        Vector3(100, 0, 0),//v
        pink
    );

    // create scene
    g_Scene = new Scene();

    // add objects to scene
    g_Scene->AddObject(sphere1);
    g_Scene->AddObject(sphere2);
    g_Scene->AddObject(quad1);

    // set up camera
    Camera* camera = new Camera(
        Vector3(0.0, 0.0, -5 ),
        Vector3(0.0, 0.0, 1.0),
        Vector3(0.0, 1.0, 0.0),
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
}

