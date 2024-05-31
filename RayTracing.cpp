
#include "RayTracing.h"

int main() 
{
    // create materials
    std::shared_ptr<Material> red = std::make_shared<Material>(
        Color(1, 0, 0,1)
    );

    // create objects
    Sphere* sphere1 = new Sphere(
        "Sphere1",
        2,
        Vector3(0.0, 0.0, 0.0),
        red
    );

    // create scene
    Scene* conellBoxScene = new Scene();

    // add objects to scene
    conellBoxScene->AddObject(sphere1);

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
    camera->Render(*conellBoxScene, outputFile);

    // close file
    outputFile.close();
}

