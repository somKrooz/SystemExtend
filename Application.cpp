#include <Application.h>

void Application::AddEntity(std::unique_ptr<Entity> ent) {
    if (ent) {
        comp.push_back(std::move(ent));  
    }
}

void Application::DrawEntity() {
    for (const auto& en : comp) {  
        en->Draw();
    }
}

void Application::UpdateEntity(float Dt) {
    for (const auto& en : comp) {
        en->Update(Dt);
    }
}
