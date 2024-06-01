#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

#include "RayTracing.h"
#include "Scene.h"
#include "Ray.h"
#include "Color.h"

class Camera
{
public:
	Vector3  m_position;
	Vector3  m_direction;
	Vector3  m_up;

	uint16_t imageWidth, imageHeight;
	float    fov;// 垂直FOV

public:
	Camera(Vector3 position, Vector3 direction, Vector3 up,int imageWidth, int imageHeight, float fov)
		: m_position(position), m_direction(direction), m_up(up), imageWidth(imageWidth), imageHeight(imageHeight), fov(fov) 
		{
			// 计算ViewPort
			double theta = fov * PI / 180.0; // 视角角度
			double viewportHeight = 2.0 * nearZ/std::tan(theta/2); // 视角高度
			double viewportWidth = viewportHeight * (double)imageWidth / imageHeight; // 视角宽度

			//计算摄像机画布坐标系
			w = m_direction; // 视线方向
			u = m_up.Cross(w).Normalized(); // 右侧方向
			v = w.Cross(u); // 下侧方向

			//计算视角坐标系原点
			pixel_00_Location = m_position - u * viewportWidth / 2.0 + v * viewportHeight / 2.0 + w * nearZ; // 视角坐标系原点
			pixelDeltaX = u * viewportWidth / (double)imageWidth; // 视角坐标系每一个像素x方向的大小
			pixelDeltaY = v * viewportHeight / (double)imageHeight; // 视角坐标系每一个像素y方向的大小
		}
	~Camera() {}

public:
	void Render(const Scene& scene,std::ostream& output);

private:
	void WriteFileHeader(std::ostream& output);
	std::shared_ptr<Ray> GetRay(int pixelIndexX,int pixelIndexY);
	Color PerPixelShading(Ray& ray, const Scene& scene);
	void WritePixelColor(std::ostream& output,Color& color);

private:
	Vector3 pixel_00_Location;// 视角坐标系原点，位于画布左上角,00像素的中心位置
	Vector3 u,v,w; // 视角坐标系
	Vector3 pixelDeltaX, pixelDeltaY; // 视角坐标系每一个像素的大小
	double nearZ = 1.0; // 近裁剪面

	Color backgroundColor = Color(1,1,1,1); // 背景颜色
};