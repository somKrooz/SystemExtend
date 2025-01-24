bool K_Button() {
    bool anyButtonClicked = false;
    for (auto& ent : GlobalButtons) {
        bool isHovered = CheckCollisionPointRec(GetMousePosition(), ent.Dim); 
        DrawRectangleRec(ent.Dim, isHovered ? Fade(RED, 0.7f) : RED);
    
        Vector2 textSize = MeasureTextEx(GetFontDefault(), ent.Label, 20, 1);
        DrawText(ent.Label, 
            ent.Dim.x + (ent.Dim.width - textSize.x) / 2, 
            ent.Dim.y + (ent.Dim.height - textSize.y) / 2, 
            20, WHITE);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            anyButtonClicked = true;
        }
    }    
    return anyButtonClicked;
}

bool K_Button() {
    bool anyButtonClicked = false;
    for (auto& ent : GlobalButtons) {
        bool isHovered = CheckCollisionPointRec(GetMousePosition(), ent.Dim); 

        DrawRectangleRec(ent.Dim, isHovered ? Fade(RED, 0.9f) : RED);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), ent.Label, 20, 1);
        DrawText(ent.Label, ent.Dim.x + (ent.Dim.width - textSize.x) / 2, ent.Dim.y + (ent.Dim.height - textSize.y) / 2, 20, WHITE);

        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            anyButtonClicked = true;
        }
    }    
    return anyButtonClicked;
}