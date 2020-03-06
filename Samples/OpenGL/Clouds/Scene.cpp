#include "Scene.h"
#include <fstream>
#include <vector>

using namespace std;

Scene::Scene() : dragging(false)
{
}

LRESULT Scene::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOUSEMOVE:
        if (this->dragging)
        {
            POINT cursor;
            if (GetCursorPos(&cursor))
            {
                auto x = cursor.x - this->cursor.x;
                auto y = cursor.y - this->cursor.y;
                this->cursor = cursor;

                auto& r0 = this->cloud.Rotation[0];
                auto& r1 = this->cloud.Rotation[1];

                r0 += -y * 0.1f;
                r1 +=  x * 0.1f;

                r0 += r0 > 180.f ? -360.f : (r0 < -180.f ? 360.f : 0.f);
                r1 += r1 > 180.f ? -360.f : (r1 < -180.f ? 360.f : 0.f);
            }

            this->Invalidate();
        }
        break;

    case WM_LBUTTONDOWN:
        this->dragging = true;
        SetCapture(this->Handle());
        GetCursorPos(&this->cursor);
        break;

    case WM_LBUTTONUP:
        this->dragging = false;
        ReleaseCapture();
        break;

    case WM_MOUSEWHEEL:
        {
            auto delta = (short)HIWORD(wParam) / WHEEL_DELTA;
            this->scene.Camera().Position(this->scene.Camera().Position() * (1.f + delta * 0.05f));
            this->Invalidate();
        }

        break;

    default:
        break;
    }

    return GLWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}

bool Scene::OnCreated()
{
    if (!GLWindow::OnCreated())
    {
        return false;
    }
    
    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().LookAt(0.f, 0.f, 0.f);
    this->scene.Camera().Position(0.f, 0.f, -2.f);

    this->LoadCloud();
    return true;
}

void Scene::OnDestroy()
{
    this->cloud.Release();
}

void Scene::OnPaint()
{
    this->scene.Begin(this->ClientWidth(), this->ClientHeight());
    this->cloud.Render();
    this->scene.End();

    GLWindow::OnPaint();
}

void Scene::LoadCloud()
{
    ifstream ifs("cloud.txt");

    if (ifs.is_open())
    {
        vector<float> points;
        float value;

        while (!ifs.eof())
        {
            ifs >> value;
            points.push_back(value);
        }

        points.resize((points.size() / 3) * 3);
        if (points.size())
        {
            vector<Vertex> vertices;
            vertices.resize(points.size() / 3);
            memcpy(&vertices[0], (Vertex*)points.data(), vertices.size() * sizeof(vertices[0]));

            while (vertices.size() > 1)
            {
                for (size_t i = 0; i < vertices.size() / 2; i++)
                {
                    auto k = i * 2;
                    vertices[i] = vertices[k] + vertices[k + 1];
                }

                if (vertices.size() & 1)
                {
                    vertices[0] += vertices[vertices.size() - 1];
                }

                vertices.resize(vertices.size() / 2);
            }

            Vertex center = vertices[0] / (float)(points.size() / 3);

            auto& v = (vector<Vertex>&)points;
            for (size_t i = 0; i < v.size(); i++)
            {
                v[i] = v[i] - center;
            }

            this->cloud.Vertices(v.data(), (int)v.size());
        }
    }
}