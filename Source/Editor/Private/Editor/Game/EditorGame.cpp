#include "PCH.hpp"

#include "Editor.hpp"

#include "Game/ImGuizmo.hpp"
#include "Game/EditorGame.hpp"

#include "GameFramework/Entity.hpp"

#include "Camera/CameraComponent.hpp"

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float *m16)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    m16[0] = temp / temp2;
    m16[1] = 0.0;
    m16[2] = 0.0;
    m16[3] = 0.0;
    m16[4] = 0.0;
    m16[5] = temp / temp3;
    m16[6] = 0.0;
    m16[7] = 0.0;
    m16[8] = (right + left) / temp2;
    m16[9] = (top + bottom) / temp3;
    m16[10] = (-zfar - znear) / temp4;
    m16[11] = -1.0f;
    m16[12] = 0.0;
    m16[13] = 0.0;
    m16[14] = (-temp * zfar) / temp4;
    m16[15] = 0.0;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float *m16)
{
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
    xmax = ymax * aspectRatio;
    Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void Cross(const float* a, const float* b, float* r)
{
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}

float Dot(const float* a, const float* b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Normalize(const float* a, float *r)
{
    float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
    r[0] = a[0] * il;
    r[1] = a[1] * il;
    r[2] = a[2] * il;
}

void LookAt(const float* eye, const float* at, const float* up, float *m16)
{
    float X[3], Y[3], Z[3], tmp[3];

    tmp[0] = eye[0] - at[0];
    tmp[1] = eye[1] - at[1];
    tmp[2] = eye[2] - at[2];
    //Z.normalize(eye - at);
    Normalize(tmp, Z);
    Normalize(up, Y);
    //Y.normalize(up);

    Cross(Y, Z, tmp);
    //tmp.cross(Y, Z);
    Normalize(tmp, X);
    //X.normalize(tmp);

    Cross(Z, X, tmp);
    //tmp.cross(Z, X);
    Normalize(tmp, Y);
    //Y.normalize(tmp);

    m16[0] = X[0];
    m16[1] = Y[0];
    m16[2] = Z[0];
    m16[3] = 0.0f;
    m16[4] = X[1];
    m16[5] = Y[1];
    m16[6] = Z[1];
    m16[7] = 0.0f;
    m16[8] = X[2];
    m16[9] = Y[2];
    m16[10] = Z[2];
    m16[11] = 0.0f;
    m16[12] = -Dot(X, eye);
    m16[13] = -Dot(Y, eye);
    m16[14] = -Dot(Z, eye);
    m16[15] = 1.0f;
}

void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float *m16)
{
    m16[0] = 2 / (r - l);
    m16[1] = 0.0f;
    m16[2] = 0.0f;
    m16[3] = 0.0f;
    m16[4] = 0.0f;
    m16[5] = 2 / (t - b);
    m16[6] = 0.0f;
    m16[7] = 0.0f;
    m16[8] = 0.0f;
    m16[9] = 0.0f;
    m16[10] = 1.0f / (zf - zn);
    m16[11] = 0.0f;
    m16[12] = (l + r) / (l - r);
    m16[13] = (t + b) / (b - t);
    m16[14] = zn / (zn - zf);
    m16[15] = 1.0f;
}

// ============================== [Public Local Methods] ============================== //

void    EditorGame::Init    (Editor* p_editor)  noexcept
{
    m_editor = p_editor;
}

void    EditorGame::Update  ()                  noexcept
{
    static ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE      currentGizmoMode      = ImGuizmo::WORLD;

    static float snap[3] = { 1.0f, 1.0f, 1.0f };

    ImGui::Begin("Game");

    ImVec2 pos  = ImGui::GetWindowPos         ();
    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::Image(const_cast<VkDescriptorSet*>(&RHI::Get().GetCurrentFrame().descriptorSets.scene), size);

    if (m_editor->GetSelectedEntity() && m_editor->GetSelectedEntity()->GetRootComponent())
    {
        CameraComponent* camera = m_editor->GetEditorCamera().GetEditorCamera();

        Matrix4x4 model = m_editor->GetSelectedEntity()->GetWorldTransform().GetMatrix();

        Matrix4x4 view       = camera->GetWorldTransform().GetMatrixInverse       ();
        Matrix4x4 projection = camera->GetCameraView    ().ComputeProjectionMatrix();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist    ();

        if (ImGui::IsKeyPressed(static_cast<int32>(EKey::ONE)))
            currentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(static_cast<int32>(EKey::TWO)))
            currentGizmoOperation = ImGuizmo::SCALE;

        ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

        ImGuizmo::Manipulate(view      .GetValues(),
                             projection.GetValues(),
                             currentGizmoOperation,
                             currentGizmoMode,
                             model.GetValues(),
                             nullptr,
                             snap,
                             nullptr,
                             nullptr);

        float translation[3], rotation[3], scale[3];

        ImGuizmo::DecomposeMatrixToComponents(model.GetValues(), translation, rotation, scale);

        m_editor->GetSelectedEntity()->SetWorldLocation(Vector3(translation[0], translation[1], translation[2]));
        m_editor->GetSelectedEntity()->SetWorldScale   (Vector3(scale      [0], scale      [1], scale      [2]));
    }

    ImGui::End();
}